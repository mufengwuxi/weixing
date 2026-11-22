"""第一问最优性检验：固定部分决策变量、其余变量重新优化，得到最小速度增量的剖面。

两个实验：
  times  固定两次机动时刻 (t1, t2)，在网格上逐点优化 Δv1、Δv2、s，得到 J*(t1, t2)
  dv     固定某一脉冲在 RTN 坐标系下的一个分量（相对最优值偏移 δ），其余变量重新优化，得到 J*(δ)
每个点用多个初值（最优解平移、相邻网格点的解、瞄准问题解），取 0.05 s 认证通过的最小值。

用法：python code/p1/landscape.py times <输出.json> [进程数]
      python code/p1/landscape.py dv <输出.json> [进程数]
"""
import json
import sys
from multiprocessing import Pool
from pathlib import Path

import numpy as np
from scipy.optimize import minimize

sys.path.insert(0, str(Path(__file__).resolve().parent))
import solver as S  # noqa: E402

REPO = Path(__file__).resolve().parents[2]
P = S.Problem(str(REPO / "data/数据.xlsx"), 0.01, 0.01, 60.0)
_d = json.load(open(REPO / "solutions/q1/p1_claude_v4.json"))
_m = _d["maneuvers"]
XOPT = np.r_[_m[0]["t_s"] / 3600, _m[1]["t_s"] / 3600, np.array(_m[0]["dv_km_s"]) * 1000,
             np.array(_m[1]["dv_km_s"]) * 1000, _d["obs_start_s"] / 3600]
JOPT = S.certify(P, XOPT)["J"]


def cons_full(extra=None):
    ca = np.cos(np.radians(45 - P.ma))
    k = np.arange(0, P.D + 1e-9, 1.0)

    def g(x):
        d, cA, cT = P.geom(x, x[8] * 3600 + k)
        return np.r_[(d - 10 - P.md) / 10, (50 - P.md - d) / 10, (cA - ca) * 10, (cT - ca) * 10]
    c = [{"type": "ineq", "fun": g}, S.basic_cons(P.D)]
    if extra:
        c.append(extra)
    return c


def solve(x0, bounds=None, extra=None):
    r = minimize(S.cost, x0, method="SLSQP", constraints=cons_full(extra), bounds=bounds,
                 options={"maxiter": 400, "ftol": 1e-10})
    c = S.certify(P, r.x)
    return (c["J"], r.x) if c["ok"] else (None, r.x)


# ---------- 实验一：固定机动时刻 ----------
def times_point(args):
    t1, t2, seeds = args
    b = [(t1, t1), (t2, t2)] + [(None, None)] * 7
    best = None
    rng = np.random.default_rng(int(t1 * 100 + t2))
    inits = []
    for x in [XOPT] + seeds:
        y = np.array(x, float).copy()
        y[0], y[1] = t1, t2
        inits.append(y)
    # 瞄准问题初值：两种瞄准距离
    tc, dl = S.observation_candidates(P)[-1]
    for L in (15.0, 30.0):
        x0 = S.initial_guess(P, rng, tc, dl)
        x0[0], x0[1], x0[8] = t1, t2, max(XOPT[8], t2 + 0.01)
        a = minimize(S.cost, x0, method="SLSQP", bounds=b,
                     constraints=[{"type": "eq", "fun": lambda x, L=L: (P.traj([x[2:5] / 1000, x[5:8] / 1000], [x[0] * 3600, x[1] * 3600], [x[8] * 3600])[0][0] - P.aim(x[8] * 3600, L)) / 10},
                                  S.basic_cons(P.D)], options={"maxiter": 300, "ftol": 1e-10})
        inits.append(a.x)
    for x0 in inits:
        J, x = solve(x0, bounds=b)
        if J is not None and (best is None or J < best[0]):
            best = (J, x.tolist())
    return t1, t2, best


def run_times(out, nproc):
    t1s = np.arange(0.5, 37.6, 1.0)
    t2s = np.arange(1.5, 38.6, 1.0)
    pts = [(a, b) for a in t1s for b in t2s if b - a >= 1.0 and b <= XOPT[8] - 0.02]
    pts += [(XOPT[0], XOPT[1])]
    with Pool(nproc) as pool:
        res = pool.map(times_point, [(a, b, []) for a, b in pts], chunksize=1)
    json.dump(dict(opt=XOPT.tolist(), J_opt=JOPT, grid=[dict(t1=a, t2=b, J=(r[0] if r else None), x=(r[1] if r else None)) for a, b, r in res]),
              open(out, "w"), indent=0)


# ---------- 实验二：固定脉冲的一个 RTN 分量 ----------
def rtn(t):
    r, v = P.traj([XOPT[2:5] / 1000], [XOPT[0] * 3600], [t]) if t > XOPT[0] * 3600 + 1 else P.traj([], [], [t])
    r, v = r[0], v[0]
    R = r / np.linalg.norm(r)
    N = np.cross(r, v); N /= np.linalg.norm(N)
    T = np.cross(N, R)
    return np.array([R, T, N])


def dv_line(args):
    j, k, deltas = args          # j = 0/1 第几次脉冲，k = 0/1/2 R/T/N
    E = rtn(XOPT[j] * 3600)
    e = E[k]
    sl = slice(2, 5) if j == 0 else slice(5, 8)
    c0 = e @ XOPT[sl]
    out = []
    for sign in (1, -1):
        x = XOPT.copy()
        for dd in [d for d in deltas if d * sign >= 0 and (d != 0 or sign == 1)][:: 1 if sign > 0 else -1]:
            dd_sorted = dd
            extra = {"type": "eq", "fun": lambda y, c=c0 + dd_sorted: np.array([(e @ y[sl] - c) * 10])}
            y0 = x.copy()
            y0[sl] = y0[sl] + (c0 + dd - e @ y0[sl]) * e
            J, y = solve(y0, extra=extra)
            if J is None:
                J2, y2 = solve(XOPT + 0, extra=extra)
                J, y = (J2, y2) if J2 is not None else (None, y)
            if J is not None:
                x = y
            out.append((float(dd), J))
    out = sorted(set(out), key=lambda q: q[0])
    return j, k, c0, out


def run_dv(out, nproc):
    deltas = np.round(np.arange(-1.0, 1.0001, 0.05), 3)
    with Pool(nproc) as pool:
        res = pool.map(dv_line, [(j, k, deltas) for j in (0, 1) for k in (0, 1, 2)], chunksize=1)
    json.dump(dict(J_opt=JOPT, lines=[dict(burn=j + 1, comp="RTN"[k], value=c0, pts=p) for j, k, c0, p in res]), open(out, "w"), indent=0)


def refine_times(path, nproc):
    """对未找到可行解或明显高于邻居的网格点，用相邻可行点的解作初值重算。"""
    d = json.load(open(path))
    G = d["grid"]
    ok = [g for g in G if g["J"] is not None]
    jobs = []
    for g in G:
        nb = sorted(ok, key=lambda h: (h["t1"] - g["t1"]) ** 2 + (h["t2"] - g["t2"]) ** 2)[1:9]
        nbJ = min([h["J"] for h in nb]) if nb else None
        if g["J"] is None or (nbJ is not None and g["J"] > 1.3 * nbJ):
            jobs.append((g["t1"], g["t2"], [h["x"] for h in nb]))
    print("重算", len(jobs), "个点", flush=True)
    with Pool(nproc) as pool:
        res = pool.map(times_point, jobs, chunksize=1)
    key = {(g["t1"], g["t2"]): g for g in G}
    for a, b, r in res:
        g = key[(a, b)]
        if r is not None and (g["J"] is None or r[0] < g["J"]):
            g["J"], g["x"] = r[0], r[1]
    json.dump(d, open(path, "w"), indent=0)


if __name__ == "__main__":
    mode, out = sys.argv[1], sys.argv[2]
    n = int(sys.argv[3]) if len(sys.argv) > 3 else 4
    print("J_opt =", JOPT)
    {"times": run_times, "dv": run_dv, "refine": refine_times}[mode](out, n)
    print("done")

