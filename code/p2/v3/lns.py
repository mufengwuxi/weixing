"""第二问 v3 新模型的组合层：并行大邻域搜索（LNS）。

多个工作进程共享一个"当前最优计划"文件。每个进程循环执行：读当前最优 → 随机选一种邻域动作 →
从第一个被改动的位置起逐段精确求解（opt.chain）→ 如果仍观测满 N 颗、且总 Δv 更小，就加锁写回。
字典序由"观测数不得减少"保证：观测数变少的候选一律拒绝。

邻域动作：
  swap      交换相邻两次观测的先后顺序（各自改到对方附近的观测机会）
  retime    把一次观测改到同一间隙内另一天的观测机会
  relocate  把一次观测移到另一个间隙（在那里找该目标的观测机会）
  seg       把连续 2～4 次观测整体随机重排
得到的计划再交给 scp.py 做全计划联合优化。

用法：python lns.py <共享最优.json> <工作进程编号> <运行秒数>
"""
import fcntl
import json
import os
import random
import sys
import time

sys.path.insert(0, os.environ.get('P2LIB_DIR', '.'))
import p2lib as P  # noqa: E402
from opt import chain, state_after, total, try_dual  # noqa: E402
from p2lib import HZ, NAMES, D, T_END, opportunities  # noqa: E402

for q in NAMES:
    P.OPPS[q] = opportunities(HZ, q)


def ntarg(L):
    s = {x['target'] for x in L}
    return len(s)


def load(path):
    with open(path) as f:
        fcntl.flock(f, fcntl.LOCK_SH)
        L = json.load(f)
        fcntl.flock(f, fcntl.LOCK_UN)
    return L


def save_if_better(path, L):
    with open(path, 'r+') as f:
        fcntl.flock(f, fcntl.LOCK_EX)
        cur = json.load(f)
        better = ntarg(L) > ntarg(cur) or (ntarg(L) == ntarg(cur) and total(L) < total(cur) - 0.01)
        if better:
            f.seek(0)
            f.truncate()
            json.dump(L, f)
        fcntl.flock(f, fcntl.LOCK_UN)
    return better


def gap(L, k):
    lo = L[k - 1]['obs_start'] + D if k > 0 else 0.0
    hi = L[k]['obs_start'] if k < len(L) else T_END - D
    return lo, hi


def rechain(L, k, seq_tail):
    """从第 k 段起按 seq_tail（[(目标, 观测时刻)]）重新逐段求解，并补回伴随观测。"""
    r, v, t0, tmin = state_after(L, k)
    comp = [y['target'] for y in L[k:] if y['t1'] is None]
    out = chain(seq_tail, start=(r, v, t0, tmin), prefix=list(L[:k]), log=lambda *a: None)
    for m in comp:
        if m not in {x['target'] for x in out}:
            d = try_dual(out, m)
            if d:
                out = d
    return out


def main_seq(L, k):
    return [(y['target'], y['obs_start']) for y in L[k:] if y['t1'] is not None]


def move(L, rng):
    n = len(L)
    kind = rng.choice(['swap', 'retime', 'relocate', 'seg'])
    ks = [k for k in range(n) if L[k]['t1'] is not None and (k == 0 or L[k - 1]['t1'] is not None)]
    k = rng.choice(ks)
    seq = main_seq(L, k)
    if kind == 'retime':
        lo, hi = gap(L, k)
        hi2 = L[k + 1]['obs_start'] if k + 1 < n else T_END - D
        c = [e for e in P.OPPS[L[k]['target']] if lo + 2 * 3600 < e < hi2 - 2 * 3600 and abs(e - L[k]['obs_start']) > 3600]
        if not c:
            return None, kind
        seq[0] = (seq[0][0], rng.choice(c))
    elif kind == 'swap' and len(seq) >= 2:
        (a, sa), (b, sb) = seq[0], seq[1]
        ca = [e for e in P.OPPS[a] if e > sb - 1.5 * 86400]
        cb = [e for e in P.OPPS[b] if e < sa + 1.5 * 86400 and e > (L[k - 1]['obs_start'] + D + 2 * 3600 if k > 0 else 2 * 3600)]
        if not ca or not cb:
            return None, kind
        nb = min(cb, key=lambda e: abs(e - sa))
        na = min([e for e in ca if e > nb + 3 * 3600] or [None], key=lambda e: abs(e - sb) if e else 1e18)
        if na is None:
            return None, kind
        seq[0], seq[1] = (b, nb), (a, na)
    elif kind == 'relocate' and len(seq) >= 3:
        a, sa = seq.pop(0)
        p = rng.randrange(1, len(seq))
        lo = seq[p - 1][1] + D + 2 * 3600
        hi = seq[p][1] - 2 * 3600 if p < len(seq) else T_END - D
        c = [e for e in P.OPPS[a] if lo < e < hi]
        if not c:
            return None, kind
        seq.insert(p, (a, rng.choice(c)))
    elif kind == 'seg' and len(seq) >= 3:
        m = rng.randint(2, min(4, len(seq)))
        blk = seq[:m]
        times = sorted(s for _, s in blk)
        names = [a for a, _ in blk]
        rng.shuffle(names)
        new = []
        for a, t in zip(names, times):
            c = [e for e in P.OPPS[a] if abs(e - t) < 1.5 * 86400]
            if not c:
                return None, kind
            new.append((a, min(c, key=lambda e: abs(e - t))))
        new.sort(key=lambda z: z[1])
        if any(new[i + 1][1] - new[i][1] < 3 * 3600 for i in range(len(new) - 1)):
            return None, kind
        seq[:m] = new
    else:
        return None, kind
    seq.sort(key=lambda z: z[1])
    return rechain(L, k, seq), kind


if __name__ == '__main__':
    path, wid, secs = sys.argv[1], int(sys.argv[2]), float(sys.argv[3])
    rng = random.Random(1000 + wid)
    t_end = time.time() + secs
    it = 0
    while time.time() < t_end:
        it += 1
        L = load(path)
        try:
            out, kind = move(L, rng)
        except Exception as e:  # noqa: BLE001
            print('err', e, flush=True)
            continue
        if out is None:
            continue
        ok = ntarg(out) >= ntarg(L)
        if ok and save_if_better(path, out):
            print(f'[{wid}] it {it} {kind}: {ntarg(out)} 颗 {total(out):.3f}（原 {total(L):.3f}）', flush=True)
