"""第三问 v5：r2 初始轨道与全计划机动联合序列凸优化（M3-C）。

在 scp.py（M2-B 连续层）的基础上，把 r2 在 t0 时刻的初始状态也作为决策变量，并加入题面对"提前发射"的 R2 的约束：
    t0 时刻偏心率 e < 0.001，半长轴 |a - a_GEO| ≤ δa（δa 由环境变量 SCP_DA 给出）。
这两条在每轮都在当前点线性化：偏心率矢量 e(x0+δx) ≈ e0 + J_e δx，用二阶锥 ‖e0 + J_e δx‖ ≤ e_max；半长轴同理用线性约束。
认证时用精确公式重新检查这两条。

以下为原 scp.py 说明：

给定访问序列、观测时刻和脉冲时刻，对整个 30 天计划的全部脉冲联合优化：
    min Σ‖Δv_k‖
    s.t. 每次观测的起点、中点、终点：相对位置 ρ 落在凸集 C 内
         C = {光照锥 ∩ 侦测锥 ∩ 半径 50 km 的球 ∩ 半空间 ĝ·ρ ≥ 10}
         ‖Δv_k‖ ≤ 30 m/s；信赖域 ‖δΔv_k‖∞ ≤ Δ
动力学在名义轨迹附近线性化，雅可比用有限差分（精确二体传播）求得；每轮解完都用精确二体模型重新传播，
并在 0.05 s 网格上认证，认证通过才接受，然后重新线性化。

用法：python scp.py <计划.json> <输出.json> [迭代次数]
计划格式与 opt.py 的输出相同：每段 {target, t1, dv1, t2, dv2, obs_start, ...}，t1 为 None 表示同次飞越的伴随观测。
裕量通过环境变量设置：P2_D（观测时长）、P2_MD/P2_MA（优化裕量）、P2_CERT_MD/P2_CERT_MA（认证裕量）。
"""
import json
import os
import sys
import time

import cvxpy as cp
import numpy as np

sys.path.insert(0, os.environ.get('P2LIB_DIR', '.'))
from p2lib import MU, R1, SUN, kepler, tpos  # noqa: E402

D = float(os.environ.get('P2_D', 60.5))
MD = float(os.environ.get('P2_MD', 0.05))
MA = float(os.environ.get('P2_MA', 0.05))
CMD = float(os.environ.get('P2_CERT_MD', 0.01))
CMA = float(os.environ.get('P2_CERT_MA', 0.01))
X0 = R1.copy()
AGEO = 42164.1696
DA = float(os.environ.get('SCP_DA', 8.0))
EMAX = float(os.environ.get('SCP_EMAX', 0.00095))


def elem(x):
    r, v = x[:3], x[3:]
    rn = np.linalg.norm(r)
    a = 1 / (2 / rn - v @ v / MU)
    ev = ((v @ v - MU / rn) * r - (r @ v) * v) / MU
    return a, ev


def unpack(L):
    burns, obs = [], []
    for x in L:
        if x.get('t1') is not None:
            burns.append([x['t1'], np.array(x['dv1']) * 1000.0])
            burns.append([x['t2'], np.array(x['dv2']) * 1000.0])
        obs.append([x['target'], x['obs_start']])
    burns.sort(key=lambda b: b[0])
    return burns, obs


def repack(L, burns):
    """把优化后的脉冲写回原计划结构（脉冲时刻不变，按时刻对应）。"""
    m = {round(t, 6): dv for t, dv in burns}
    out = []
    for x in L:
        y = dict(x)
        if x.get('t1') is not None:
            y['dv1'] = (m[round(x['t1'], 6)] / 1000.0).tolist()
            y['dv2'] = (m[round(x['t2'], 6)] / 1000.0).tolist()
        out.append(y)
    return out


def positions(burns, times, x0=None):
    """精确二体：给定初始状态和脉冲序列（m/s），返回各采样时刻的观测星位置（km）。times 需升序。"""
    x0 = X0 if x0 is None else x0
    r, v, tc = x0[:3].copy(), x0[3:].copy(), 0.0
    out = np.zeros((len(times), 3))
    bi, n = 0, len(burns)
    for i, t in enumerate(times):
        while bi < n and burns[bi][0] <= t:
            R, V = kepler(r, v, burns[bi][0] - tc)
            r, v, tc = R[0], V[0] + burns[bi][1] / 1000.0, burns[bi][0]
            bi += 1
        R, _ = kepler(r, v, t - tc)
        out[i] = R[0]
    return out


def samples(obs, k=5):
    T, idx = [], []
    for j, (n, s) in enumerate(obs):
        for q in np.linspace(0, D, k):
            T.append(s + q)
            idx.append(j)
    order = np.argsort(T)
    return np.array(T)[order], np.array(idx)[order]


def geometry(obs, T, idx):
    """各采样时刻的目标位置、太阳方向、地心方向。"""
    rT = np.array([tpos(obs[j][0], t)[0][0] for t, j in zip(T, idx)])
    S = SUN(T)
    us = S - rT
    us /= np.linalg.norm(us, axis=1)[:, None]
    ue = -rT / np.linalg.norm(rT, axis=1)[:, None]
    return rT, us, ue


def certify(burns, obs, step=0.05, x0=None):
    """严格认证：0.05 s 网格上的全部观测约束，以及观测期间不机动。返回 (是否通过, 最差信息)。"""
    worst = []
    if x0 is not None:
        a, ev = elem(x0)
        if np.linalg.norm(ev) >= 0.001 or abs(a - AGEO) > DA:
            return False, ('init', a - AGEO, np.linalg.norm(ev))
    for n, s in obs:
        if any(s < b[0] < s + D for b in burns):
            return False, (n, 'burn in obs')
        tt = np.arange(s, s + D + 1e-9, step)
        rO = positions(burns, tt, x0)
        rT = tpos(n, tt)[0]
        rho = rO - rT
        d = np.linalg.norm(rho, axis=1)
        S = SUN(tt)
        us = S - rT
        us /= np.linalg.norm(us, axis=1)[:, None]
        ue = -rT / np.linalg.norm(rT, axis=1)[:, None]
        al = np.degrees(np.arccos(np.clip(np.sum(us * rho, 1) / d, -1, 1)))
        th = np.degrees(np.arccos(np.clip(np.sum(ue * rho, 1) / d, -1, 1)))
        ok = d.min() >= 10 + CMD and d.max() <= 50 - CMD and al.max() <= 45 - CMA and th.max() <= 45 - CMA
        if not ok:
            return False, (n, d.min(), d.max(), al.max(), th.max())
        worst.append((d.min(), d.max(), al.max(), th.max()))
    return True, worst


def total(burns):
    return sum(np.linalg.norm(dv) for _, dv in burns)


def scp(L, iters=30, tr=1.0, log=print):
    burns, obs = unpack(L)
    x0 = X0.copy()
    ok, _ = certify(burns, obs, x0=x0)
    if not ok:
        log('初始计划未通过认证', _)
    T, idx = samples(obs)
    rT, us, ue = geometry(obs, T, idx)
    nb = len(burns)
    tb = np.array([b[0] for b in burns])
    ca = np.cos(np.radians(45 - MA))
    best = (total(burns), [b[:] for b in burns], x0.copy()) if ok else None
    h = 1e-3  # m/s，有限差分步长
    stall = 0
    for it in range(iters):
        t0 = time.time()
        P0 = positions(burns, T, x0)
        J = np.zeros((len(T), 3, nb, 3))
        # 初始状态的雅可比（位置步长 1e-3 km，速度步长 1e-6 km/s）
        JX = np.zeros((len(T), 3, 6))
        steps = [1e-3] * 3 + [1e-6] * 3
        for a in range(6):
            xx = x0.copy()
            xx[a] += steps[a]
            JX[:, :, a] = (positions(burns, T, xx) - P0) / steps[a]
        a0, e0 = elem(x0)
        Ja, Je = np.zeros(6), np.zeros((3, 6))
        for a in range(6):
            xx = x0.copy()
            xx[a] += steps[a]
            a1, e1 = elem(xx)
            Ja[a] = (a1 - a0) / steps[a]
            Je[:, a] = (e1 - e0) / steps[a]
        for k in range(nb):
            mask = T > tb[k]
            if not mask.any():
                continue
            for a in range(3):
                bb = [b[:] for b in burns]
                bb[k] = [bb[k][0], bb[k][1] + h * np.eye(3)[a]]
                J[mask, :, k, a] = (positions(bb, T[mask], x0) - P0[mask]) / h
        dv0 = np.array([b[1] for b in burns])
        xv = cp.Variable(nb * 3)
        X = cp.reshape(xv, (nb, 3), order='C')
        dx = cp.Variable(6)  # 初始状态修正：位置 km，速度 km/s
        cons = [cp.norm(dv0 + X, 2, axis=1) <= 30.0, cp.abs(xv) <= tr,
                cp.abs(dx[:3]) <= float(os.environ.get('SCP_INIT_R', 20.0)) * tr, cp.abs(dx[3:]) <= float(os.environ.get('SCP_INIT_V', 1e-3)) * tr,
                cp.norm(e0 + Je @ dx) <= EMAX,
                a0 + Ja @ dx <= AGEO + DA - 0.01, a0 + Ja @ dx >= AGEO - DA + 0.01]
        # 距离下限的半空间：同一次观测的全部采样点共用一个法向（取各点相对位置的平均方向），
        # 这样沿近似直线的相对运动，ĝ·ρ 是线性函数，端点满足就保证整段满足
        G = {}
        for j in set(idx.tolist()):
            m = (P0 - rT)[idx == j].mean(axis=0)
            G[j] = m / np.linalg.norm(m)
        for i in range(len(T)):
            r0 = P0[i] - rT[i]
            rho = r0 + J[i].reshape(3, nb * 3) @ xv + JX[i] @ dx
            g = G[idx[i]]
            cons += [ca * cp.norm(rho) <= us[i] @ rho, ca * cp.norm(rho) <= ue[i] @ rho,
                     cp.norm(rho) <= 50 - MD, g @ rho >= 10 + MD]
        prob = cp.Problem(cp.Minimize(cp.sum(cp.norm(dv0 + X, 2, axis=1))), cons)
        try:
            prob.solve(solver=cp.CLARABEL)
        except Exception as e:  # noqa: BLE001
            log('求解失败', e)
            tr *= 0.5
            continue
        if xv.value is None:
            log(f'第 {it} 轮无解，信赖域 {tr:.3f} → {tr / 2:.3f}')
            tr *= 0.5
            continue
        xval = xv.value.reshape(nb, 3)
        cand = [[b[0], dv0[k] + xval[k]] for k, b in enumerate(burns)]
        x0c = x0 + dx.value
        ok, info = certify(cand, obs, x0=x0c)
        c_new, c_old = total(cand), total(burns)
        log(f'第 {it} 轮：预测 {prob.value:.4f}，实际 {c_new:.4f}（原 {c_old:.4f}），认证 {"通过" if ok else "未通过 " + str(info)}，信赖域 {tr:.3f}，{time.time() - t0:.0f}s')
        # 序列凸化：无论是否通过认证都把候选作为新的线性化点（线性化误差在下一轮被修正），
        # 只把通过认证且更省的解记为最优；连续多轮没有改进就收敛
        burns, x0 = cand, x0c
        if ok and (best is None or c_new < best[0] - 1e-4):
            best = (c_new, [b[:] for b in burns], x0.copy())
            stall = 0
        else:
            stall += 1
        if stall >= 15:
            break
        tr = max(tr * float(os.environ.get('SCP_DECAY', 0.9)), 0.02)
    return best


if __name__ == '__main__':
    L = json.load(open(sys.argv[1]))
    it = int(sys.argv[3]) if len(sys.argv) > 3 else 30
    res = scp(L, iters=it, log=lambda *a: print(*a, flush=True))
    if res:
        out = repack(L, res[1])
        json.dump(out, open(sys.argv[2], 'w'))
        a, ev = elem(res[2])
        json.dump({'state': res[2].tolist(), 'a_minus_GEO_km': a - AGEO, 'e': float(np.linalg.norm(ev)), 'delta_a_km': DA},
                  open(sys.argv[2].replace('.json', '_init.json'), 'w'))
        print('FINAL %.4f  a-aGEO %.3f km  e %.6f' % (res[0], a - AGEO, np.linalg.norm(ev)))
