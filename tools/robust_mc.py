"""蒙特卡洛鲁棒性检验：在脉冲幅值、方向、时刻和初始位置上加随机误差，统计观测成功率与约束裕量。

用法：python tools/robust_mc.py <方案.json（第二问格式）> <试验次数> [输出.json] [进程数]
误差模型（1σ，可用环境变量修改）：
  MC_MAG   脉冲幅值相对误差，默认 0.01（1%）
  MC_DIR   脉冲方向误差，默认 0.1°
  MC_TIME  脉冲时刻误差，默认 1 s
  MC_POS   初始位置误差（每轴），默认 0.1 km
每次试验对全部观测在 0.1 s 网格上检查距离 10–50 km、光照角与侦测角 ≤ 45°、连续 60 s（观测时长按方案给定）。
"""
import json
import os
import sys
from multiprocessing import Pool

import numpy as np

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(REPO, 'code', 'p3', 'v3'))
from p2lib import SUN, kepler  # noqa: E402

SIG_MAG = float(os.environ.get('MC_MAG', 0.01))
SIG_DIR = np.radians(float(os.environ.get('MC_DIR', 0.1)))
SIG_T = float(os.environ.get('MC_TIME', 1.0))
SIG_POS = float(os.environ.get('MC_POS', 0.1))


def load(path):
    d = json.load(open(path, encoding='utf-8'))
    x0 = np.array(d['r1_km'])
    burns = sorted([(b['t_s'], np.array(b['dv_km_s'])) for b in d['burns']], key=lambda z: z[0])
    obs = [(o['target'], o['start_s'], o.get('duration_s', 60.0)) for o in d['observations']]
    tg = {k: np.array(v) for k, v in d['targets_km'].items()}
    return x0, burns, obs, tg


def rotate_small(v, ang, rng):
    if np.linalg.norm(v) == 0 or ang == 0:
        return v
    u = v / np.linalg.norm(v)
    a = rng.normal(size=3)
    a -= a @ u * u
    a /= np.linalg.norm(a)
    th = rng.normal(0, ang)
    return np.linalg.norm(v) * (np.cos(th) * u + np.sin(th) * a)


def positions(x0, burns, times):
    r, v, tc = x0[:3].copy(), x0[3:].copy(), 0.0
    out = np.zeros((len(times), 3))
    bi = 0
    for i, t in enumerate(times):
        while bi < len(burns) and burns[bi][0] <= t:
            R, V = kepler(r, v, burns[bi][0] - tc)
            r, v, tc = R[0], V[0] + burns[bi][1], burns[bi][0]
            bi += 1
        R, _ = kepler(r, v, t - tc)
        out[i] = R[0]
    return out


def check(x0, burns, obs, tg, step=0.1):
    res = []
    for n, s, dur in obs:
        tt = np.arange(s, s + dur + 1e-9, step)
        rO = positions(x0, burns, tt)
        rT, _ = kepler(tg[n][:3], tg[n][3:], tt)
        rho = rO - rT
        d = np.linalg.norm(rho, axis=1)
        S = SUN(tt)
        us = S - rT
        us /= np.linalg.norm(us, axis=1)[:, None]
        ue = -rT / np.linalg.norm(rT, axis=1)[:, None]
        al = np.degrees(np.arccos(np.clip(np.sum(us * rho, 1) / d, -1, 1)))
        th = np.degrees(np.arccos(np.clip(np.sum(ue * rho, 1) / d, -1, 1)))
        ok = (d >= 10) & (d <= 50) & (al <= 45) & (th <= 45)
        # 允许观测窗口在原定时段附近整体平移：找最长连续满足段
        best, cur = 0, 0
        for f in ok:
            cur = cur + 1 if f else 0
            best = max(best, cur)
        res.append(dict(target=n, ok=bool(best * step >= 60.0), d_margin=float(min(d.min() - 10, 50 - d.max())),
                        ang_margin=float(45 - max(al.max(), th.max()))))
    return res


def trial(args):
    path, seed = args
    rng = np.random.default_rng(seed)
    x0, burns, obs, tg = load(path)
    x0 = x0.copy()
    x0[:3] += rng.normal(0, SIG_POS, 3)
    nb = []
    for t, dv in burns:
        dv2 = rotate_small(dv * (1 + rng.normal(0, SIG_MAG)), SIG_DIR, rng)
        nb.append((t + rng.normal(0, SIG_T), dv2))
    nb.sort(key=lambda z: z[0])
    r = check(x0, nb, obs, tg)
    return dict(n_ok=sum(z['ok'] for z in r), n=len(r), min_d_margin=min(z['d_margin'] for z in r),
                min_ang_margin=min(z['ang_margin'] for z in r), failed=[z['target'] for z in r if not z['ok']])


if __name__ == '__main__':
    path, N = sys.argv[1], int(sys.argv[2])
    out = sys.argv[3] if len(sys.argv) > 3 else None
    procs = int(sys.argv[4]) if len(sys.argv) > 4 else 4
    with Pool(procs) as p:
        R = p.map(trial, [(path, s) for s in range(N)])
    n = R[0]['n']
    full = sum(r['n_ok'] == n for r in R)
    from collections import Counter
    fails = Counter(t for r in R for t in r['failed'])
    summ = dict(plan=path, trials=N, sigma=dict(mag=SIG_MAG, dir_deg=float(np.degrees(SIG_DIR)), time_s=SIG_T, pos_km=SIG_POS),
                full_success_rate=full / N, mean_observed=float(np.mean([r['n_ok'] for r in R])),
                worst_observed=min(r['n_ok'] for r in R), fail_counts=dict(fails.most_common()))
    print(json.dumps(summ, ensure_ascii=False, indent=1))
    if out:
        json.dump(dict(summary=summ, trials=R), open(out, 'w'), ensure_ascii=False)
