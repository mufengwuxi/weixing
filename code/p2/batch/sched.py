"""第二问第一层：漂移相位模型上的束搜索，排出访问顺序和每次观测的日期。

模型（论文第 6.2.1 节）：
1. 观测事件：目标穿过 R1 轨道面、且 β ≤ 90° − 2m 的时刻（p2lib.opportunities）。
2. 相位：R1 和目标在 R1 轨道面内的辐角减去参考角速度。要在 s 时刻观测目标，R1 的相位必须追上目标的相位。
3. 代价：漂移率改变 Δω 需要的切向速度增量为 K·|Δω|，K 由 dv_per_rate() 从 R1 的半长轴算出，约 2.84 m/s/(°/天)。
4. 交会时允许的漂移率：切向脉冲把 R1 的拱线放在脉冲方向 ψ 上，R1 在偏离拱线 δ 的方向上的地心距偏差约为
   |Δa|(1 − cos δ)。要求这一偏差不超过 ρ_tol，得到 |ω| ≤ ω_max(δ) = 540 ρ_tol / (a (1 − cos δ))（°/天）。
   拱线方向 ψ* 取 R1 轨道面与赤道面的交线：同步带目标的倾角都很小，它们与 R1 轨道面的交线都靠近这一方向。
   与 R1 近共面的目标没有确定的交线方向，按最不利情形 δ = 90° 处理。

用法：python sched.py <mu> <束宽> <单段最长天数> <巡游角度权重 ct> <巡游速率上限 rate>
可选环境变量：RHO_TOL（km，默认 25）、W_CAP（°/天，默认 4）。
"""
import os
import pickle
import sys
import time

import numpy as np

sys.path.insert(0, '.')
from p2lib import D, MD, MU, NAMES, R1, T_END, TG, opportunities, tpos  # noqa: E402

RHO_TOL = float(os.environ.get('RHO_TOL', 25.0))   # 交会处允许的径向偏差，取瞄准距离区间 [10, 50] km 的中值附近
W_CAP = float(os.environ.get('W_CAP', 4.0))        # 搜索中考虑的最大漂移率（°/天）

# R1 轨道面坐标系：X0 指向 R1 初始位置，Y0 在轨道面内超前 90°
H0 = np.cross(R1[:3], R1[3:]); H0 /= np.linalg.norm(H0)
X0 = R1[:3] / np.linalg.norm(R1[:3]); Y0 = np.cross(H0, X0)
A0 = 1 / (2 / np.linalg.norm(R1[:3]) - R1[3:] @ R1[3:] / MU)
N0 = np.sqrt(MU / A0 ** 3)
DEG_DAY = np.pi / 180 / 86400                      # 1 °/天 对应的 rad/s


def dv_per_rate():
    """漂移率改变 1 °/天 所需的切向速度增量（m/s）：ω = −1.5 n Δa / a，Δv = n Δa / 2。"""
    da = DEG_DAY * A0 / (1.5 * N0)                  # km
    return N0 * da / 2 * 1000


K = dv_per_rate()                                  # 约 2.84 m/s / (°/天)


def inplane_angle(vec):
    """矢量在 R1 轨道面内相对 X0 的辐角，按直线取 [0, 180)。"""
    return np.degrees(np.arctan2(vec @ Y0, vec @ X0)) % 180


PSI = inplane_angle(np.cross([0.0, 0.0, 1.0], H0))  # R1 与赤道面交线方向，即公共拱线方向 ψ*


def node_offset(n):
    """目标 n 与 R1 轨道面的交线方向偏离 ψ* 的角度 δ（°）；近共面目标返回 90。"""
    h = np.cross(TG[n][:3], TG[n][3:]); h /= np.linalg.norm(h)
    nd = np.cross(H0, h)
    if A0 * np.linalg.norm(nd) <= 50 - MD:          # 法向振幅 a·sin(i_rel) 不超过观测距离上限：近共面
        return 90.0
    d = abs(inplane_angle(nd) - PSI)
    return min(d, 180 - d)


def omega_max(delta):
    """交会时允许的最大漂移率（°/天）。"""
    c = 1 - np.cos(np.radians(delta))
    return W_CAP if c < 1e-12 else min(W_CAP, 540.0 * RHO_TOL / (A0 * c))


WMAX = {n: omega_max(node_offset(n)) for n in NAMES}


def phase(p, t):
    """R1 轨道面内的辐角减去参考角速度（°）。"""
    u = np.degrees(np.arctan2(p @ Y0, p @ X0))
    return u - np.degrees(N0) * t


def wrap(x):
    return (x + 180) % 360 - 180


# 观测事件：(开始时刻, 目标, 目标相位)
EV = []
for n in NAMES:
    for s in opportunities(H0, n):
        if s > T_END - D - 60:
            continue
        p = tpos(n, s + D / 2)[0][0]
        EV.append((s, n, phase(p, s + D / 2)))
EV.sort()
PHT = {n: [(s_, p) for s_, m, p in EV if m == n] for n in NAMES}


def ph_at(n, t):
    L = PHT[n]
    i = min(range(len(L)), key=lambda k: abs(L[k][0] - t))
    return L[i][1]


def leg_cost(om, need, T, wmax):
    """当前漂移率 om，要在 T 天内追回相位 need，交会时漂移率不超过 wmax。返回 (代价, 交会时漂移率)。"""
    opts = []
    wa = om + need / T                              # 一次脉冲
    if abs(wa) <= wmax:
        opts.append((K * abs(wa - om), wa))
    T2 = min(1.0, T / 2)                            # 两次脉冲：交会前最后一天降到允许的漂移率
    if T - T2 > 0.05:
        for wb in (0.0, np.sign(wa) * min(abs(wa), wmax)):
            wa2 = om + (need - (wb - om) * T2) / (T - T2)
            if abs(wa2) <= W_CAP:
                opts.append((K * (abs(wa2 - om) + abs(wb - wa2)), wb))
    return min(opts) if opts else None


def run(width=3000, mu=0.3, min_gap=1.5 * 3600, lead=1.2 * 3600, maxdays=4.0, ct=0.0, rate=2.5, cmax=30.0):
    """束搜索。评价值 = 累计代价 + mu·已用天数 + ct·剩余巡游角度（+ 罚项：所需巡游速率超过 rate）。"""
    beam = [(0.0, 0.0, 0.0, 0.0, frozenset(), ())]  # (代价, 时刻, R1 相位, 漂移率, 已访问, 序列)
    best_by_count = {}
    for depth in range(len(NAMES)):
        cand = {}
        for cost, t, phi, om, vis, seq in beam:
            for s, n, ph in EV:
                if s < t + min_gap:
                    continue
                if s > t + maxdays * 86400:
                    break
                if n in vis:
                    continue
                tm = s + D / 2
                need = wrap(ph - (phi + om * (tm - t) / 86400))
                t1 = t + lead * 0.5 if depth > 0 else 0.0
                r = leg_cost(om, need, (tm - t1) / 86400, WMAX[n])
                if r is None or r[0] > cmax:          # 单次脉冲不超过 30 m/s
                    continue
                c, wnew = r
                nc = cost + c
                key = vis | {n}
                unv = [m for m in NAMES if m not in key]
                sc = nc + mu * s / 86400
                if unv and ct > 0:
                    ps = np.array([wrap(ph_at(m, s) - ph) for m in unv])
                    lo, hi = min(ps.min(), 0), max(ps.max(), 0)
                    sweep = (hi - lo) + min(-lo, hi)
                    rem = max((T_END - s) / 86400, 0.1)
                    sc += ct * sweep + (100 if sweep / rem > rate else 0)
                k2 = (key, round(s / 21600))
                if k2 not in cand or sc < cand[k2][0]:
                    cand[k2] = (sc, (nc, s + D, ph, wnew, key, seq + ((n, s, c),)))
        if not cand:
            break
        beam = [c for _, c in sorted(cand.values(), key=lambda x: x[0])[:width]]
        best_by_count[depth + 1] = beam[0]
        print('depth', depth + 1, 'cost %.1f t=%.2f' % (beam[0][0], beam[0][1] / 86400), flush=True)
    return best_by_count


if __name__ == '__main__':
    mu, W, md, ct, rt = float(sys.argv[1]), int(sys.argv[2]), float(sys.argv[3]), float(sys.argv[4]), float(sys.argv[5])
    print(f'K = {K:.3f} m/s/(°/天)，ψ* = {PSI:.2f}°，ρ_tol = {RHO_TOL} km，ω_cap = {W_CAP} °/天，事件 {len(EV)} 个')
    print(' '.join(f'{n}:{node_offset(n):.1f}°/{WMAX[n]:.2f}' for n in NAMES))
    t = time.time()
    res = run(W, mu, maxdays=md, ct=ct, rate=rt)
    k = max(res); b = res[k]
    print('RESULT mu', mu, 'count', k, 'cost %.1f' % b[0], 'end %.2f d' % (b[1] / 86400), '%.0fs' % (time.time() - t))
    print(' '.join(f'{n}@{s / 86400:.2f}:{c:.1f}' for n, s, c in b[5]))
    pickle.dump(res, open(f'sched_{mu}_{W}_{md}_{ct}_{rt}_rho{RHO_TOL}.pkl', 'wb'))
