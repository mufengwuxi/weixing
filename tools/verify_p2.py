"""第二问方案复核：按机动序列重放整条 r1 轨迹，逐个检查观测和全部机动约束。

用法（仓库根目录，Windows，已设置 ATK_HOME，已关闭 ATK 界面）：
    python tools/verify_p2.py solutions/p2_claude_27.json --out results/p2_verify_27
    python tools/verify_p2.py solutions/p2_claude_27.json --out results/p2_verify_27 --no-atk   # 只做 Python 复核

检查内容：
    1. 机动约束：单次 ≤ 30 m/s、任意相邻机动间隔 ≥ 3600 s、累计 ≤ 300 m/s、每次观测最多 2 次机动、观测期间无机动；
    2. 每次观测：0.05 s 网格上距离 10–50 km、光照角和侦测角 ≤ 45°（太阳用 ERFA 几何方向和视方向两种口径）；
    3. 全程最低地心距；
    4. ATK 复核：r1 每段用 ATK 二体卫星接力传播，目标星也用 ATK 传播，再按 0.2 s 网格检查。
输出：<out>/summary.md、observations.csv
"""

import argparse
import csv
import json
import sys
import warnings
from datetime import datetime, timedelta
from pathlib import Path

import numpy as np

REPO = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO / "code"))
AU = 149597870.7
C_KM_S = 299792.458


def kepler(r0, v0, dt, mu):
    dt = np.atleast_1d(np.asarray(dt, float))
    r0 = np.asarray(r0, float)
    v0 = np.asarray(v0, float)
    r0n = np.linalg.norm(r0)
    a = 1 / (2 / r0n - v0 @ v0 / mu)
    sa = np.sqrt(a)
    sig0 = r0 @ v0 / np.sqrt(mu)
    n = np.sqrt(mu / a ** 3)
    M = n * dt
    x = M.copy()
    for _ in range(50):
        f = x - (1 - r0n / a) * np.sin(x) + sig0 / sa * (1 - np.cos(x)) - M
        fp = 1 - (1 - r0n / a) * np.cos(x) + sig0 / sa * np.sin(x)
        dx = f / fp
        x -= dx
        if np.max(np.abs(dx)) < 1e-14:
            break
    r = a + (r0n - a) * np.cos(x) + sig0 * sa * np.sin(x)
    F = 1 - a / r0n * (1 - np.cos(x))
    G = dt + (np.sin(x) - x) / n
    Fd = -np.sqrt(mu * a) / (r * r0n) * np.sin(x)
    Gd = 1 - a / r * (1 - np.cos(x))
    return F[:, None] * r0 + G[:, None] * v0, Fd[:, None] * r0 + Gd[:, None] * v0


class Sun:
    def __init__(self, epoch):
        import erfa
        self.erfa = erfa
        with warnings.catch_warnings():
            warnings.simplefilter("ignore")
            d1, d2 = erfa.dtf2d("UTC", epoch.year, epoch.month, epoch.day, epoch.hour, epoch.minute, float(epoch.second))
            a1, a2 = erfa.utctai(d1, d2)
            self.tt1, self.tt2 = erfa.taitt(a1, a2)

    def geometric(self, t):
        return np.array([-self.erfa.epv00(self.tt1, self.tt2 + x / 86400)[0]["p"] * AU for x in np.atleast_1d(t)])

    def apparent(self, t):
        out = []
        for x in np.atleast_1d(t):
            h, b = self.erfa.epv00(self.tt1, self.tt2 + x / 86400)
            eb = b["p"] * AU
            tau = 499.0
            for _ in range(3):
                h2, b2 = self.erfa.epv00(self.tt1, self.tt2 + (x - tau) / 86400)
                vec = (b2["p"] - h2["p"]) * AU - eb
                tau = np.linalg.norm(vec) / C_KM_S
            u = vec / np.linalg.norm(vec)
            beta = b["v"] * AU / 86400 / C_KM_S
            ua = u + beta - (u @ beta) * u
            out.append(ua / np.linalg.norm(ua) * np.linalg.norm(vec))
        return np.array(out)


def geometry(rO, rT, rS):
    rho = rO - rT
    d = np.linalg.norm(rho, axis=1)
    us = rS - rT
    us /= np.linalg.norm(us, axis=1)[:, None]
    ue = -rT / np.linalg.norm(rT, axis=1)[:, None]
    al = np.degrees(np.arccos(np.clip(np.sum(us * rho, 1) / d, -1, 1)))
    th = np.degrees(np.arccos(np.clip(np.sum(ue * rho, 1) / d, -1, 1)))
    return d, al, th


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("solution")
    ap.add_argument("--out", default="results/p2_verify")
    ap.add_argument("--no-atk", action="store_true")
    ap.add_argument("--atk-grid", type=float, default=0.2)
    a = ap.parse_args()
    sol = json.loads(Path(a.solution).read_text(encoding="utf-8"))
    out = REPO / a.out
    out.mkdir(parents=True, exist_ok=True)
    mu = sol["mu_km3_s2"]
    t0 = datetime.fromisoformat(sol["epoch_utc"])
    md = []

    def log(x=""):
        print(x, flush=True)
        md.append(str(x))
        (out / "summary.md").write_text("\n".join(md), encoding="utf-8")

    burns = sorted(sol["burns"], key=lambda b: b["t_s"])
    obs = sorted(sol["observations"], key=lambda o: o["start_s"])
    log("# 第二问方案复核")
    log()
    log(f"- 方案文件：`{a.solution}`；目标 {len(obs)} 颗（去重后 {len(set(o['target'] for o in obs))} 颗），机动 {len(burns)} 次")

    # 1. 机动约束
    log()
    log("## 1. 机动约束")
    dvs = np.array([np.linalg.norm(b["dv_km_s"]) * 1000 for b in burns])
    ts = np.array([b["t_s"] for b in burns])
    gaps = np.diff(ts)
    eff = dvs > 1e-3
    log(f"- 累计 Δv：{dvs.sum():.4f} m/s（上限 300）")
    log(f"- 单次最大：{dvs.max():.4f} m/s（上限 30）；有效机动（> 1 mm/s）{eff.sum()} 次，其余 {(~eff).sum()} 次小于 1 mm/s")
    log(f"- 相邻机动最小间隔：{gaps.min():.6f} s（要求 ≥ 3600）")
    per = {}
    for b in burns:
        per[b["for"]] = per.get(b["for"], 0) + 1
    log(f"- 每次观测对应的机动次数最大值：{max(per.values())}（要求 ≤ 2）")
    clash = [(o["target"], b["t_s"]) for o in obs for b in burns if o["start_s"] < b["t_s"] < o["start_s"] + o["duration_s"]]
    log(f"- 观测期间发生机动：{'无' if not clash else clash}")
    last_end = max(o["start_s"] + o["duration_s"] for o in obs)
    log(f"- 最后一次观测结束：{last_end / 86400:.4f} 天（要求 ≤ 30）")
    ok_burn = dvs.max() <= 30 and gaps.min() >= 3600 - 1e-6 and dvs.sum() <= 300 and max(per.values()) <= 2 and not clash and last_end <= 2592000

    # 2. Python 重放
    r1 = np.array(sol["r1_km"])

    def state_at(T):
        r, v, tc = r1[:3].copy(), r1[3:].copy(), 0.0
        for b in burns:
            if b["t_s"] > T:
                break
            R, V = kepler(r, v, b["t_s"] - tc, mu)
            r, v, tc = R[0], V[0] + np.array(b["dv_km_s"]), b["t_s"]
        return r, v, tc

    sun = Sun(t0)
    log()
    log("## 2. 逐次观测检查（Python 解析重放，0.05 s 网格）")
    log("| # | 目标 | 开始（UTC） | 距离 (km) | 最大光照角 几何/视方向 (°) | 最大侦测角 (°) | 满足 |")
    log("|---|---|---|---|---|---|---|")
    rows = []
    n_ok = 0
    for i, o in enumerate(obs, 1):
        s, D = o["start_s"], o["duration_s"]
        tt = np.arange(s, s + D + 1e-9, 0.05)
        r, v, tc = state_at(s)
        rO, _ = kepler(r, v, tt - tc, mu)
        tg = np.array(sol["targets_km"][o["target"]])
        rT, _ = kepler(tg[:3], tg[3:], tt, mu)
        tsun = tt[::20]
        sg = np.array([np.interp(tt, tsun, c) for c in sun.geometric(tsun).T]).T
        sa = np.array([np.interp(tt, tsun, c) for c in sun.apparent(tsun).T]).T
        d, al, th = geometry(rO, rT, sg)
        _, al2, _ = geometry(rO, rT, sa)
        ok = d.min() >= 10 and d.max() <= 50 and max(al.max(), al2.max()) <= 45 and th.max() <= 45
        n_ok += ok
        utc = (t0 + timedelta(seconds=s)).strftime("%m-%d %H:%M:%S")
        log(f"| {i} | {o['target']} | {utc} | {d.min():.3f}–{d.max():.3f} | {al.max():.4f} / {al2.max():.4f} | {th.max():.4f} | {'是' if ok else '**否**'} |")
        rows.append({"target": o["target"], "start_s": s, "d_min": d.min(), "d_max": d.max(),
                     "alpha_max_geo": al.max(), "alpha_max_app": al2.max(), "theta_max": th.max(), "ok_python": int(ok)})
    log()
    log(f"Python 复核：{n_ok}/{len(obs)} 次观测满足全部约束。")
    grid = np.arange(0, last_end, 600.0)
    rmin = min(np.linalg.norm(kepler(*state_at(T)[:2], T - state_at(T)[2], mu)[0][0]) for T in grid[::6])
    log(f"全程最低地心距（1 h 采样）：{rmin:.1f} km")

    # 3. ATK
    if not a.no_atk:
        log()
        log("## 3. ATK 复核（r1 分段接力传播，0.2 s 网格，太阳用 ERFA 几何方向）")
        from atkbridge import ATK
        atk = ATK(out_dir=out / "raw")
        fmt = lambda x: (lambda dt: f"{dt.day} {dt:%b %Y %H:%M:%S}.{dt.microsecond:06d}")(t0 + timedelta(seconds=x))  # noqa: E731
        atk.new("P2Verify", fmt(0), fmt(2592000))
        km = lambda x: [y * 1000 for y in x]  # noqa: E731
        atk.add_satellite_cartesian("r1_0", fmt(0), km(r1[:3]), km(r1[3:]), step_s=10.0)
        for name in sorted(set(o["target"] for o in obs)):
            tg = sol["targets_km"][name]
            atk.add_satellite_cartesian(name, fmt(0), km(tg[:3]), km(tg[3:]), step_s=10.0)
        arcs = [(0.0, "r1_0")]
        for k, b in enumerate(burns, 1):
            p, vv = atk.posvel(arcs[-1][1], fmt(b["t_s"]), "CBI")
            nm = f"r1_{k}"
            atk.add_satellite_cartesian(nm, fmt(b["t_s"]), p, [x + y * 1000 for x, y in zip(vv, b["dv_km_s"])], step_s=10.0)
            arcs.append((b["t_s"], nm))
        n_atk = 0
        for row, o in zip(rows, obs):
            s, D = o["start_s"], o["duration_s"]
            arc = [nm for tb, nm in arcs if tb <= s][-1]
            tt = np.arange(s, s + D + 1e-9, a.atk_grid)
            rO = np.array([atk.posvel(arc, fmt(x), "CBI")[0] for x in tt]) / 1000
            rT = np.array([atk.posvel(o["target"], fmt(x), "CBI")[0] for x in tt]) / 1000
            d, al, th = geometry(rO, rT, sun.geometric(tt))
            ok = d.min() >= 10 and d.max() <= 50 and al.max() <= 45 and th.max() <= 45
            n_atk += ok
            row.update({"d_min_atk": d.min(), "d_max_atk": d.max(), "alpha_max_atk": al.max(), "theta_max_atk": th.max(), "ok_atk": int(ok)})
            log(f"- {o['target']}：距离 {d.min():.3f}–{d.max():.3f} km，光照角 {al.max():.4f}°，侦测角 {th.max():.4f}°，{'满足' if ok else '**不满足**'}")
        atk.close()
        log()
        log(f"ATK 复核：{n_atk}/{len(obs)} 次观测满足全部约束。")

    with open(out / "observations.csv", "w", encoding="utf-8-sig", newline="") as f:
        w = csv.DictWriter(f, fieldnames=list(rows[-1]))
        w.writeheader()
        w.writerows(rows)
    log()
    log(f"## 结论：机动约束{'全部满足' if ok_burn else '**有不满足项**'}；成功观测 {n_ok} 颗不同目标，累计 Δv {dvs.sum():.4f} m/s。")


if __name__ == "__main__":
    main()
