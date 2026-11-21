"""第二问第三种模型 M2-C：时间扩展图上的整数规划（全局访问序列）。

思想：把"每颗目标每天一次的观测机会"作为图的节点，把"从观测完目标 i 到观测目标 j"的转移作为有向弧，
弧的代价用线性化两脉冲瞄准（est4）估计。由于时间单调递增，图是有向无环图。访问序列就是图上从起点出发的一条路径。
"每颗目标至多观测一次"是路径上的附加约束。用混合整数线性规划（HiGHS）一次性求出全局最优路径。

近似：弧代价只依赖于两端的节点，不依赖于更早的历史（马尔可夫近似）。具体做法是把节点 u 处观测星的状态取为
"目标 i 在观测结束时刻的状态 + 指向可观测区的 25 km 偏置"。

字典序：每观测一颗目标奖励 P = 301 m/s。由于总 Δv 受 300 m/s 预算约束，max P·N − J 与"先比 N、再比 J"严格等价。

得到的序列交给 opt.chain 逐段精确求解，再交给 scp.py 做全计划联合优化。

用法：
  python m2c.py arcs  <输出 arcs.npz> <进程数> [最长间隔天数]   # 第一步：并行计算弧代价表
  python m2c.py milp  <arcs.npz> <输出 seq.json> [时限秒]          # 第二步：整数规划求全局序列
  python m2c.py chain <seq.json> <输出 plan.json>                  # 第三步：逐段精确实现
"""
import json
import os
import sys
import time
from multiprocessing import Pool

import numpy as np

sys.path.insert(0, os.environ.get('P2LIB_DIR', '.'))
import p2lib as P  # noqa: E402
from p2lib import D, HZ, NAMES, R1, T_END, aimdir, est4, opportunities, tpos  # noqa: E402

MAXGAP = float(os.environ.get('M2C_MAXGAP_DAYS', 3.0)) * 86400
OFF = 25.0  # km，节点处观测星相对目标的偏置
PRIZE = 301.0  # m/s


def nodes():
    out = []
    for n in NAMES:
        for s in opportunities(HZ, n):
            if 4200 < s < T_END - D:
                out.append((n, float(s)))
    out.sort(key=lambda z: z[1])
    return out


def node_state(n, s):
    te = s + D
    rT, vT = tpos(n, te)
    _, b = aimdir(n, s + D / 2)
    return rT[0] + OFF * b[0], vT[0], te


def arc_job(args):
    u, N = args
    if u < 0:
        r, v, t0 = R1[:3].copy(), R1[3:].copy(), 0.0
    else:
        r, v, t0 = node_state(*N[u])
    res = []
    for w, (m, s) in enumerate(N):
        if u >= 0 and m == N[u][0]:
            continue
        if not (t0 + 4300 < s <= t0 + MAXGAP):
            continue
        try:
            e = est4(r, v, t0, t0 + 1, m, s, L=OFF, top=2)
        except Exception:  # noqa: BLE001
            e = None
        if e is not None:
            res.append((u, w, e[0] * 1000.0))
    return res


def build_arcs(out, procs):
    N = nodes()
    t = time.time()
    with Pool(procs) as pool:
        A = []
        for i, r in enumerate(pool.imap_unordered(arc_job, [(u, N) for u in range(-1, len(N))], chunksize=4)):
            A.extend(r)
            if i % 100 == 0:
                print(f'{i}/{len(N) + 1} 节点，{len(A)} 条弧，{time.time() - t:.0f}s', flush=True)
    A = np.array(A)
    np.savez(out, arcs=A, nodes=np.array([n for n, _ in N]), times=np.array([s for _, s in N]))
    print('完成', len(N), '节点', len(A), '条弧')


def solve_milp(path, out, tlim):
    from scipy.optimize import Bounds, LinearConstraint, milp
    from scipy.sparse import lil_matrix
    Z = np.load(path, allow_pickle=True)
    A, nm, tm = Z['arcs'], Z['nodes'], Z['times']
    nN, nA = len(nm), len(A)
    # 变量：nA 条弧 + nN 条"终止弧"（从节点 u 结束路径）
    nv = nA + nN
    c = np.zeros(nv)
    tg = {n: i for i, n in enumerate(sorted(set(nm)))}
    for k, (u, w, cost) in enumerate(A):
        c[k] = cost - PRIZE  # 进入节点 w 获得奖励
    rows = []
    # 流守恒：起点（u=-1）出流 = 1；每个节点 入流 = 出流 + 终止
    M = lil_matrix((nN + 1 + len(tg) + 1, nv))
    lb, ub = [], []
    r = 0
    for k, (u, w, cost) in enumerate(A):
        if u < 0:
            M[r, k] = 1
    lb.append(1); ub.append(1); r += 1
    for k, (u, w, cost) in enumerate(A):
        M[1 + int(w), k] += 1
        if u >= 0:
            M[1 + int(u), k] -= 1
    for i in range(nN):
        M[1 + i, nA + i] = -1
        lb.append(0); ub.append(0)
    r = 1 + nN
    # 每颗目标至多进入一次
    for k, (u, w, cost) in enumerate(A):
        M[r + tg[nm[int(w)]], k] = 1
    for _ in tg:
        lb.append(0); ub.append(1)
    r += len(tg)
    # 预算
    for k, (u, w, cost) in enumerate(A):
        M[r, k] = cost
    lb.append(0); ub.append(300.0)
    rows = LinearConstraint(M.tocsr(), lb, ub)
    t = time.time()
    res = milp(c, constraints=rows, integrality=np.ones(nv), bounds=Bounds(0, 1),
               options={'time_limit': tlim, 'disp': True, 'mip_rel_gap': 1e-4})
    print('状态', res.status, res.message, f'{time.time() - t:.0f}s')
    x = res.x[:nA] > 0.5
    used = A[x]
    seq, cur = [], -1
    nxt = {int(u): (int(w), cc) for u, w, cc in used}
    tot = 0.0
    while cur in nxt:
        w, cc = nxt[cur]
        seq.append((str(nm[w]), float(tm[w]), float(cc)))
        tot += cc
        cur = w
    print('观测', len(seq), '颗，估计 Δv', round(tot, 3), 'm/s')
    json.dump({'seq': seq, 'est_total': tot, 'status': int(res.status)}, open(out, 'w'), ensure_ascii=False)


def realize(path, out):
    from opt import chain, total
    S = json.load(open(path))['seq']
    for q in NAMES:
        P.OPPS[q] = opportunities(HZ, q)
    L = chain([(n, s) for n, s, _ in S], log=print)
    json.dump(L, open(out, 'w'))
    print('实现', len(L), '颗', round(total(L), 3), 'm/s')


if __name__ == '__main__':
    cmd = sys.argv[1]
    if cmd == 'arcs':
        if len(sys.argv) > 4:
            MAXGAP = float(sys.argv[4]) * 86400
        build_arcs(sys.argv[2], int(sys.argv[3]))
    elif cmd == 'milp':
        solve_milp(sys.argv[2], sys.argv[3], float(sys.argv[4]) if len(sys.argv) > 4 else 1800)
    elif cmd == 'chain':
        realize(sys.argv[2], sys.argv[3])
