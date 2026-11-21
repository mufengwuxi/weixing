"""第一问求解器：r1 最多两次脉冲机动，在 48 h 内对 b1 完成连续观测，使累计 Δv 最小。

模型与题面一致：二体引力（μ = 398600.4418 km³/s²），ICRF，太阳位置用 ERFA 几何星历；
观测约束 10 ≤ d ≤ 50 km、光照角 ≤ 45°、侦测角 ≤ 45°（有向角）、连续 ≥ 60 s；
单次 |Δv| ≤ 30 m/s，两次机动间隔 ≥ 3600 s。可选加裕量：距离、角度和观测时长都可以收紧。

求解流程：
1. 多起点：随机取机动时刻和瞄准点，先解“观测中点打到目标附近一点”的两脉冲瞄准问题（SLSQP）；
2. 以瞄准解为初值，在完整连续观测约束下（1 s 网格）用 SLSQP 精修；
3. 在 0.05 s 网格上认证全部约束（含裕量），取认证通过的最小 Δv 解。

用法（依赖 numpy、scipy、pyerfa、openpyxl）：
    python code/p1/solver.py --out solutions/q1/p1_claude_limit.json --margin-d 0.001 --margin-a 0.0001
    python code/p1/solver.py --out solutions/q1/p1_claude_robust.json --margin-d 1 --margin-a 1 --duration 70
"""

import argparse
import json
import time
import warnings
from pathlib import Path

import numpy as np
from scipy.interpolate import CubicSpline
from scipy.optimize import minimize

warnings.filterwarnings("ignore")
MU = 398600.4418
AU = 149597870.7
T_END = 172800.0
EPS = 1e-6
REPO = Path(__file__).resolve().parents[2]


# ---------------- 数据 ----------------
def load_states(xlsx):
    import sys
    sys.path.insert(0, str(REPO / "code"))
    from datafile import read_states  # 自动识别 km 或 m 单位
    R1, TG = read_states(xlsx)
    return R1, TG["b1"]


# ---------------- 动力学 ----------------
def kepler(r0, v0, dt):
    """二体解析传播（f/g 函数），dt 可为数组。"""
    dt = np.atleast_1d(np.asarray(dt, float))
    r0n = np.linalg.norm(r0)
    a = 1 / (2 / r0n - v0 @ v0 / MU)
    sa = np.sqrt(a)
    sig0 = r0 @ v0 / np.sqrt(MU)
    M = np.sqrt(MU / a ** 3) * dt
    x = M.copy()
    for _ in range(30):
        f = x - (1 - r0n / a) * np.sin(x) + sig0 / sa * (1 - np.cos(x)) - M
        fp = 1 - (1 - r0n / a) * np.cos(x) + sig0 / sa * np.sin(x)
        dx = f / fp
        x -= dx
        if np.max(np.abs(dx)) < 1e-14:
            break
    r = a + (r0n - a) * np.cos(x) + sig0 * sa * np.sin(x)
    F = 1 - a / r0n * (1 - np.cos(x))
    G = dt + np.sqrt(a ** 3 / MU) * (np.sin(x) - x)
    Fd = -np.sqrt(MU * a) / (r * r0n) * np.sin(x)
    Gd = 1 - a / r * (1 - np.cos(x))
    return F[:, None] * r0 + G[:, None] * v0, Fd[:, None] * r0 + Gd[:, None] * v0


class Problem:
    def __init__(self, xlsx, md=0.0, ma=0.0, D=60.0):
        self.R1, self.B1 = load_states(xlsx)
        self.md, self.ma, self.D = md, ma, D
        import erfa
        d1, d2 = erfa.dtf2d("UTC", 2027, 1, 1, 0, 0, 0.0)
        a1, a2 = erfa.utctai(d1, d2)
        tt1, tt2 = erfa.taitt(a1, a2)
        ts = np.arange(-3600, T_END + 3601, 600.0)
        self.sun = CubicSpline(ts, np.array([-erfa.epv00(tt1, tt2 + t / 86400)[0]["p"] * AU for t in ts]))

    def traj(self, dvs, tb, t):
        r, v, tc = self.R1[:3].copy(), self.R1[3:].copy(), 0.0
        for b, dv in zip(tb, dvs):
            R, V = kepler(r, v, b - tc)
            r, v, tc = R[0], V[0] + dv, b
        return kepler(r, v, np.asarray(t) - tc)

    def target(self, t):
        return kepler(self.B1[:3], self.B1[3:], t)

    def geom(self, x, t):
        t1, t2, d1, d2, _ = unpack(x)
        rO, _ = self.traj([d1, d2], [t1, t2], t)
        rT, _ = self.target(t)
        rho = rO - rT
        d = np.linalg.norm(rho, axis=1)
        us = self.sun(t) - rT
        us /= np.linalg.norm(us, axis=1)[:, None]
        ue = -rT / np.linalg.norm(rT, axis=1)[:, None]
        return d, np.sum(us * rho, 1) / d, np.sum(ue * rho, 1) / d

    def aim(self, s, L):
        rT = self.target(s)[0][0]
        us = self.sun(s) - rT
        us /= np.linalg.norm(us)
        ue = -rT / np.linalg.norm(rT)
        b = us + ue
        return rT + L * b / np.linalg.norm(b)


# 变量：[t1(h), t2(h), Δv1(m/s)×3, Δv2(m/s)×3, s(h)]
def unpack(x):
    return x[0] * 3600, x[1] * 3600, x[2:5] / 1000, x[5:8] / 1000, x[8] * 3600


def cost(x):
    return np.sqrt(x[2:5] @ x[2:5] + EPS) + np.sqrt(x[5:8] @ x[5:8] + EPS)


def basic_cons(D):
    return {"type": "ineq", "fun": lambda x: np.array([
        x[0], x[1] - x[0] - 1.0, x[8] - x[1], (T_END - D) / 3600 - x[8],
        900 - x[2:5] @ x[2:5], 900 - x[5:8] @ x[5:8]])}


def solve_aim(P, x0, L):
    eq = lambda x: (P.traj([x[2:5] / 1000, x[5:8] / 1000], [x[0] * 3600, x[1] * 3600], [x[8] * 3600])[0][0]  # noqa: E731
                    - P.aim(x[8] * 3600, L)) / 10
    return minimize(cost, x0, method="SLSQP", constraints=[{"type": "eq", "fun": eq}, basic_cons(P.D)],
                    options={"maxiter": 500, "ftol": 1e-10})


def solve_full(P, x0, iters=600):
    ca = np.cos(np.radians(45 - P.ma))
    k = np.arange(0, P.D + 1e-9, 1.0)

    def g(x):
        d, cA, cT = P.geom(x, x[8] * 3600 + k)
        return np.r_[(d - 10 - P.md) / 10, (50 - P.md - d) / 10, (cA - ca) * 10, (cT - ca) * 10]
    return minimize(cost, x0, method="SLSQP", constraints=[{"type": "ineq", "fun": g}, basic_cons(P.D)],
                    options={"maxiter": iters, "ftol": 1e-12})


def certify(P, x, step=0.05):
    t1, t2, d1, d2, s = unpack(x)
    t = np.arange(s, s + P.D + 1e-9, step)
    d, cA, cT = P.geom(x, t)
    al, th = np.degrees(np.arccos(np.clip(cA, -1, 1))), np.degrees(np.arccos(np.clip(cT, -1, 1)))
    tol = 1e-7
    ok = (d.min() >= 10 + P.md - tol and d.max() <= 50 - P.md + tol and al.max() <= 45 - P.ma + tol
          and th.max() <= 45 - P.ma + tol and t1 >= -tol and t2 - t1 >= 3600 - tol and s >= t2 - tol
          and s + P.D <= T_END + tol and np.linalg.norm(d1) <= 0.03 and np.linalg.norm(d2) <= 0.03)
    rr = np.linalg.norm(P.traj([d1, d2], [t1, t2], np.arange(0, s + P.D, 60.0))[0], axis=1)
    return {"ok": bool(ok), "J": float((np.linalg.norm(d1) + np.linalg.norm(d2)) * 1000),
            "d_min": float(d.min()), "d_max": float(d.max()), "alpha_max": float(al.max()),
            "theta_max": float(th.max()), "r_min": float(rr.min())}


def search(P, starts, seed, log=print):
    rng = np.random.default_rng(seed)
    best = None
    t0 = time.time()
    for i in range(starts):
        cross = rng.choice([14.45, 38.43])  # 两个日照窗口内 r1 穿越 b1 轨道面的时刻附近（h）
        L = rng.uniform(10 + P.md, 45 - P.md)
        t1 = rng.uniform(0, cross - 1.2)
        t2 = rng.uniform(t1 + 1.0, cross - 0.05)
        _, v = P.traj([], [], [t1 * 3600])
        tdir = v[0] / np.linalg.norm(v[0])
        x0 = np.r_[t1, t2, tdir * 970 / (3 * (cross - t1) * 3600) * 1000 * rng.uniform(0.3, 1.5) + rng.normal(0, 0.5, 3),
                   rng.normal(0, 0.5, 3), cross]
        a = solve_aim(P, x0, L)
        if not a.success:
            continue
        f = solve_full(P, a.x)
        c = certify(P, f.x)
        if c["ok"] and (best is None or c["J"] < best[1]["J"]):
            best = (f.x, c)
            log(f"  起点 {i + 1}/{starts}：新最优 J = {c['J']:.6f} m/s（{time.time() - t0:.0f} s）")
    return best


def warm(P, path):
    """从已有方案出发精修（用于裕量延拓）。先按给定裕量精修，认证不过就把优化裕量略微加大再试。"""
    d = json.load(open(path, encoding="utf-8"))
    m = d["maneuvers"]
    x0 = np.r_[m[0]["t_s"] / 3600, m[1]["t_s"] / 3600, np.array(m[0]["dv_km_s"]) * 1000,
               np.array(m[1]["dv_km_s"]) * 1000, d["obs_start_s"] / 3600]
    md, ma = P.md, P.ma
    for extra in (0.0, 0.005, 0.02, 0.05):
        P.md, P.ma = md + extra, ma + extra
        f = solve_full(P, x0)
        P.md, P.ma = md, ma
        c = certify(P, f.x)
        print(f"  热启动（优化裕量 +{extra}）：J = {c['J']:.6f} m/s，认证{'通过' if c['ok'] else '未通过'}")
        if c["ok"]:
            return f.x, c
    return None


def to_json(P, x, c, note):
    t1, t2, d1, d2, s = unpack(x)
    return {
        "epoch_utc": "2027-01-01T00:00:00", "mu_km3_s2": MU,
        "r1_original_km": P.R1.tolist(), "r1_used_km": P.R1.tolist(), "b1_km": P.B1.tolist(),
        "maneuvers": [{"t_s": float(t1), "dv_km_s": d1.tolist()}, {"t_s": float(t2), "dv_km_s": d2.tolist()}],
        "obs_start_s": float(s), "obs_duration_s": P.D,
        "claimed": {"total_dv_m_s": c["J"], "d_min_km": c["d_min"], "d_max_km": c["d_max"],
                    "alpha_max_deg": c["alpha_max"], "theta_max_deg": c["theta_max"]},
        "margins": {"distance_km": P.md, "angle_deg": P.ma, "duration_s": P.D},
        "note": note,
    }


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--data", default=str(REPO / "data" / "数据.xlsx"))
    ap.add_argument("--out", required=True)
    ap.add_argument("--margin-d", type=float, default=0.0, help="距离裕量 km")
    ap.add_argument("--margin-a", type=float, default=0.0, help="角度裕量 度")
    ap.add_argument("--duration", type=float, default=60.0, help="要求的连续观测时长 s")
    ap.add_argument("--starts", type=int, default=200)
    ap.add_argument("--seed", type=int, default=1)
    ap.add_argument("--init", default=None, help="热启动：以已有方案（本脚本输出的 JSON）为初值，直接精修并认证")
    a = ap.parse_args()

    P = Problem(a.data, a.margin_d, a.margin_a, a.duration)
    print(f"裕量：距离 {a.margin_d} km，角度 {a.margin_a}°，时长 {a.duration} s；起点 {a.starts} 个")
    best = warm(P, a.init) if a.init else search(P, a.starts, a.seed)
    if best is None:
        raise SystemExit("没有找到通过认证的解，请增加 --starts")
    x, c = best
    t1, t2, d1, d2, s = unpack(x)
    note = (f"code/p1/solver.py 求解（seed={a.seed}, starts={a.starts}），裕量 距离 {a.margin_d} km、"
            f"角度 {a.margin_a}°、时长 {a.duration} s；0.05 s 网格认证通过")
    out = Path(a.out)
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(json.dumps(to_json(P, x, c, note), ensure_ascii=False, indent=2), encoding="utf-8")
    print(f"最优 J = {c['J']:.6f} m/s；t1 = {t1:.3f} s，t2 = {t2:.3f} s，观测开始 {s:.3f} s")
    print(f"|Δv1| = {np.linalg.norm(d1) * 1000:.6f}，|Δv2| = {np.linalg.norm(d2) * 1000:.6f} m/s")
    print(f"距离 {c['d_min']:.4f}–{c['d_max']:.4f} km，最大光照角 {c['alpha_max']:.6f}°，最大侦测角 {c['theta_max']:.6f}°，"
          f"最小地心距 {c['r_min']:.1f} km")
    print(f"已写入 {out}")


if __name__ == "__main__":
    main()
