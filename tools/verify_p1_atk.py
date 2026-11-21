"""用 ATK 独立复核第一问机动方案。

用法（仓库根目录，Windows，已设置 ATK_HOME，已关闭 ATK 界面）：
    python -m pip install pyerfa
    python tools/verify_p1_atk.py solutions/q1/p1_doc.json

两种轨道传播 × 三种太阳位置，交叉检查 60 s 观测段：
    轨道：本脚本自带的开普勒解析传播（纯 Python）；ATK 二体传播
    太阳：ERFA 几何位置；ERFA 视方向（光行时 + 光行差）；ATK 太阳对象（读数不合理时自动弃用）
另外把 Δv 截成 4 位小数（m/s）再用 ATK 算一遍，并在观测段前后扫描，给出不增加 Δv、
只调整观测开始时刻就能得到的最大角度裕量。

输出：results/q1/p1_atk_verify/summary.md、samples.csv、scan.csv
"""

import argparse
import csv
import json
import math
import sys
import warnings
from datetime import datetime, timedelta
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO / "code"))
OUT = REPO / "results" / "q1" / "p1_atk_verify"  # 可用 --out 改
R_E = 6378.137
AU_KM = 149597870.7
C_KM_S = 299792.458


# ---------------- 向量与轨道工具（km、km/s） ----------------
def sub(a, b): return [x - y for x, y in zip(a, b)]
def add(a, b): return [x + y for x, y in zip(a, b)]
def dot(a, b): return sum(x * y for x, y in zip(a, b))
def norm(a): return math.sqrt(dot(a, a))
def cross(a, b): return [a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0]]


def angle_deg(u, v):
    c = dot(u, v) / (norm(u) * norm(v))
    return math.degrees(math.acos(max(-1.0, min(1.0, c))))


def kepler(r0, v0, dt, mu):
    """椭圆轨道 f/g 函数解析传播（偏近点角差形式）。"""
    r0n = norm(r0)
    a = 1.0 / (2.0 / r0n - dot(v0, v0) / mu)
    sa = math.sqrt(a)
    sig0 = dot(r0, v0) / math.sqrt(mu)
    M = math.sqrt(mu / a ** 3) * dt
    x = M
    for _ in range(50):
        f = x - (1 - r0n / a) * math.sin(x) + sig0 / sa * (1 - math.cos(x)) - M
        fp = 1 - (1 - r0n / a) * math.cos(x) + sig0 / sa * math.sin(x)
        dx = f / fp
        x -= dx
        if abs(dx) < 1e-15:
            break
    r = a + (r0n - a) * math.cos(x) + sig0 * sa * math.sin(x)
    F = 1 - a / r0n * (1 - math.cos(x))
    G = dt + math.sqrt(a ** 3 / mu) * (math.sin(x) - x)
    Fd = -math.sqrt(mu * a) / (r * r0n) * math.sin(x)
    Gd = 1 - a / r * (1 - math.cos(x))
    return [F * p + G * q for p, q in zip(r0, v0)], [Fd * p + Gd * q for p, q in zip(r0, v0)]


def elements(r, v, mu):
    rn, vn2 = norm(r), dot(v, v)
    h = cross(r, v)
    a = 1.0 / (2.0 / rn - vn2 / mu)
    evec = [(vn2 / mu - 1 / rn) * x - dot(r, v) / mu * y for x, y in zip(r, v)]
    e = norm(evec)
    return {"a_km": a, "e": e, "i_deg": math.degrees(math.acos(h[2] / norm(h))),
            "raan_deg": math.degrees(math.atan2(h[0], -h[1])) % 360, "rp_km": a * (1 - e),
            "period_h": 2 * math.pi * math.sqrt(a ** 3 / mu) / 3600 if a > 0 else float("nan")}


def geometry(rO, rT, rS):
    """d(km)、光照角 α、侦测角 θ（度，有向角，不取绝对值）。"""
    rho = sub(rO, rT)
    return norm(rho), angle_deg(sub(rS, rT), rho), angle_deg([-x for x in rT], rho)


# ---------------- 时间与太阳 ----------------
class Clock:
    def __init__(self, epoch_iso):
        self.t0 = datetime.fromisoformat(epoch_iso)

    def atk(self, t_s):
        dt = self.t0 + timedelta(seconds=t_s)
        return f"{dt.day} {dt:%b %Y %H:%M:%S}.{dt.microsecond:06d}"

    def iso(self, t_s):
        return (self.t0 + timedelta(seconds=t_s)).isoformat(timespec="milliseconds")


class ErfaSun:
    """ERFA epv00 地心太阳位置（ICRS 轴向，km）。"""

    def __init__(self, epoch):
        import erfa
        self.erfa = erfa
        with warnings.catch_warnings():
            warnings.simplefilter("ignore")
            d1, d2 = erfa.dtf2d("UTC", epoch.year, epoch.month, epoch.day, epoch.hour, epoch.minute,
                                epoch.second + epoch.microsecond / 1e6)
            a1, a2 = erfa.utctai(d1, d2)
            self.tt1, self.tt2 = erfa.taitt(a1, a2)

    def _pv(self, t_s):
        h, b = self.erfa.epv00(self.tt1, self.tt2 + t_s / 86400.0)
        return [x * AU_KM for x in h["p"]], [x * AU_KM for x in b["p"]]

    def geometric(self, t_s):
        h, _ = self._pv(t_s)
        return [-x for x in h]

    def apparent(self, t_s):
        """光行时 + 周年光行差：目标星随地球运动时看到的太阳光入射方向。"""
        u = self.light_time(t_s)
        _, b = self.erfa.epv00(self.tt1, self.tt2 + t_s / 86400.0)
        v = [x * AU_KM / 86400.0 / C_KM_S for x in b["v"]]  # v/c
        n = norm(u)
        uh = [x / n for x in u]
        k = dot(uh, v)
        ua = [x + y - k * x for x, y in zip(uh, v)]
        m = norm(ua)
        return [x / m * n for x in ua]

    def light_time(self, t_s):
        _, eb = self._pv(t_s)
        tau = 499.0
        for _ in range(3):
            h, b = self._pv(t_s - tau)
            sun_b = sub(b, h)
            vec = sub(sun_b, eb)
            tau = norm(vec) / C_KM_S
        return vec


# ---------------- 主流程 ----------------
def chain_python(sol, dvs):
    mu = sol["mu_km3_s2"]
    r, v, t = sol["r1_used_km"][:3], sol["r1_used_km"][3:], 0.0
    states = []
    for m, dv in zip(sol["maneuvers"], dvs):
        r, v = kepler(r, v, m["t_s"] - t, mu)
        states.append((m["t_s"], r[:], v[:]))
        v = add(v, dv)
        t = m["t_s"]
    return t, r, v, states


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("solution")
    ap.add_argument("--step", type=float, default=1.0, help="ATK 二体星历步长（秒）")
    ap.add_argument("--grid", type=float, default=0.05, help="观测段检查网格（秒）")
    ap.add_argument("--out", help="输出目录，默认 results/q1/p1_atk_verify")
    a = ap.parse_args()
    global OUT
    if a.out:
        OUT = (REPO / a.out) if not Path(a.out).is_absolute() else Path(a.out)

    sol = json.loads(Path(a.solution).read_text(encoding="utf-8"))
    mu = sol["mu_km3_s2"]
    clk = Clock(sol["epoch_utc"])
    s, dur = sol["obs_start_s"], sol.get("obs_duration_s", 60.0)
    dvs_full = [m["dv_km_s"] for m in sol["maneuvers"]]
    dvs_round = [[round(x * 1000, 4) / 1000 for x in dv] for dv in dvs_full]
    OUT.mkdir(parents=True, exist_ok=True)
    for old in OUT.glob("*.csv"):
        old.unlink()
    md = []

    def log(x=""):
        print(x, flush=True)
        md.append(str(x))
        (OUT / "summary.md").write_text("\n".join(md), encoding="utf-8")

    log("# 第一问方案 ATK 独立复核")
    log()
    log(f"- 方案文件：`{a.solution}`，历元 {sol['epoch_utc']} UTC，μ = {mu} km³/s²")
    log(f"- 观测段：{clk.iso(s)} 起 {dur} s，检查网格 {a.grid} s，ATK 星历步长 {a.step} s")
    if sol.get("note"):
        log(f"- 方案说明：{sol['note']}")
    log()

    # 1. 数据
    log("## 1. 初始数据检查")
    log("| 对象 | a (km) | e | i (°) | RAAN (°) | 近地点地心距 (km) | 周期 (h) |")
    log("|---|---|---|---|---|---|---|")
    for key in ("r1_original_km", "r1_used_km", "b1_km"):
        if key in sol:
            el = elements(sol[key][:3], sol[key][3:], mu)
            log(f"| {key} | {el['a_km']:.3f} | {el['e']:.6f} | {el['i_deg']:.4f} | {el['raan_deg']:.3f} | "
                f"{el['rp_km']:.3f} | {el['period_h']:.4f} |")
    log()

    # 2. 太阳
    log("## 2. 太阳位置来源")
    sun = ErfaSun(clk.t0)
    sources = {"ERFA 几何": sun.geometric, "ERFA 视方向": sun.apparent}
    g, ap = sun.geometric(s), sun.apparent(s)
    log(f"- 观测开始时 ERFA 地心太阳距离 {norm(g):.0f} km；几何方向与视方向（含光行时和光行差）相差 {angle_deg(g, ap) * 3600:.2f} 角秒")

    from atkbridge import ATK
    atk = ATK(out_dir=OUT / "raw")
    atk.new("P1Verify", clk.atk(0), clk.atk(172800))
    km = lambda x: [y * 1000 for y in x]  # noqa: E731
    atk.add_satellite_cartesian("b1", clk.atk(0), km(sol["b1_km"][:3]), km(sol["b1_km"][3:]), step_s=a.step)
    atk.add_satellite_cartesian("r1_arc0", clk.atk(0), km(sol["r1_used_km"][:3]), km(sol["r1_used_km"][3:]), step_s=a.step)

    frame, best = "CBI", None
    for fr in ("CBI", "J2000"):
        try:
            p, _ = atk.posvel("r1_arc0", clk.atk(0), fr)
            dif = norm(sub(p, km(sol["r1_used_km"][:3])))
            if best is None or dif < best:
                frame, best = fr, dif
        except Exception:
            pass

    def atk_pos(name, t):
        return [x / 1000 for x in atk.posvel(name, clk.atk(t), frame)[0]]

    try:
        atk.add_planet("Sun", "Sun")
        rs = atk_pos("Sun", s)
        ang = angle_deg(rs, g)
        log(f"- ATK 太阳对象：{frame} 读数长度 {norm(rs):.0f} km，与 ERFA 几何方向相差 {ang * 3600:.2f} 角秒")
        if 1.4e8 < norm(rs) < 1.6e8 and ang < 0.1:
            sources["ATK 太阳"] = lambda t: atk_pos("Sun", t)
        else:
            log("- ATK 太阳读数不是地心太阳位置，弃用（上一版脚本直接用了这个读数，光照角结果作废）")
    except Exception as e:
        log(f"- ATK 太阳对象创建失败，弃用：{e}")
    log()

    # 3. 传播一致性
    log("## 3. ATK 与解析传播一致性")
    log(f"- 坐标系：使用 {frame} 读数（历元时刻与 ICRF 赋值相差 {best:.6f} m）")
    for tt in (3600.5, 86400.25, s + 0.025):
        pk, _ = kepler(sol["r1_used_km"][:3], sol["r1_used_km"][3:], tt, mu)
        log(f"- t = {tt:.3f} s：ATK 与开普勒位置差 {norm(sub(atk_pos('r1_arc0', tt), pk)) * 1000:.4f} m")

    def atk_arc(tag, dvs):
        prev = "r1_arc0"
        for k, (m, dv) in enumerate(zip(sol["maneuvers"], dvs), 1):
            p, v = atk.posvel(prev, clk.atk(m["t_s"]), frame)
            name = f"r1_{tag}_arc{k}"
            atk.add_satellite_cartesian(name, clk.atk(m["t_s"]), p, add(v, km(dv)), frame="ICRF", step_s=a.step)
            prev = name
        return prev

    arc_full, arc_round = atk_arc("full", dvs_full), atk_arc("round4", dvs_round)
    t2, r2p, v2p, _ = chain_python(sol, dvs_full)
    log(f"- 累计 Δv = {sum(norm(dv) for dv in dvs_full) * 1000:.6f} m/s")
    log()

    def propagate(kind, t):
        rT = atk_pos("b1", t) if kind != "py" else kepler(sol["b1_km"][:3], sol["b1_km"][3:], t, mu)[0]
        if kind == "py":
            rO = kepler(r2p, v2p, t - t2, mu)[0]
        else:
            rO = atk_pos(arc_full if kind == "atk" else arc_round, t)
        return rO, rT

    # 4. 观测段检查
    log("## 4. 观测段 60 s 检查")
    n = int(round(dur / a.grid))
    times = [s + k * a.grid for k in range(n + 1)]
    sun_cache = {name: [f(t) for t in times] for name, f in sources.items()}
    combos = [("开普勒", "py"), ("ATK", "atk"), ("ATK，Δv 4 位小数", "round")]
    rows, table = [], []
    for label, kind in combos:
        pos = [propagate(kind, t) for t in times]
        for sname, svals in sun_cache.items():
            geo = [geometry(rO, rT, rS) for (rO, rT), rS in zip(pos, svals)]
            dd, al, th = [x[0] for x in geo], [x[1] for x in geo], [x[2] for x in geo]
            ok = min(dd) >= 10 and max(dd) <= 50 and max(al) <= 45 and max(th) <= 45
            table.append((label, sname, min(dd), max(dd), max(al), max(th), ok))
            for t, x in zip(times, geo):
                rows.append({"orbit": label, "sun": sname, "t_s": round(t, 4), "d_km": x[0], "alpha_deg": x[1], "theta_deg": x[2]})
    with open(OUT / "samples.csv", "w", encoding="utf-8-sig", newline="") as f:
        w = csv.DictWriter(f, fieldnames=list(rows[0]))
        w.writeheader()
        w.writerows(rows)
    c = sol.get("claimed", {})
    log("| 轨道 | 太阳 | 距离 (km) | 最大光照角 (°) | 最大侦测角 (°) | 60 s 全程满足 |")
    log("|---|---|---|---|---|---|")
    log(f"| 方案声称 | | {c.get('d_min_km', '')}–{c.get('d_max_km', '')} | {c.get('alpha_max_deg', '')} | {c.get('theta_max_deg', '')} | 是 |")
    for label, sname, d0, d1, al, th, ok in table:
        log(f"| {label} | {sname} | {d0:.4f}–{d1:.4f} | {al:.6f} | {th:.6f} | {'是' if ok else '**否**'} |")
    log()

    # 5. 扫描：不改 Δv，只调观测开始时刻
    log("## 5. 观测开始时刻的调整空间（ATK 轨道）")
    step, before, after = 0.5, 300, 360
    st = [s - before + k * step for k in range(int((before + after) / step) + 1)]
    pos = [propagate("atk", t) for t in st]
    scan_rows = []
    for sname, f in sources.items():
        geo = [geometry(rO, rT, f(t)) for (rO, rT), t in zip(pos, st)]
        okv = [10 <= d <= 50 and al <= 45 and th <= 45 for d, al, th in geo]
        for t, x, o in zip(st, geo, okv):
            scan_rows.append({"sun": sname, "t_s": t, "d_km": x[0], "alpha_deg": x[1], "theta_deg": x[2], "ok": int(o)})
        # 最长连续满足区间
        best_len, cur, rng = 0, None, None
        for t, o in zip(st, okv):
            if o:
                cur = t if cur is None else cur
                if t - cur > best_len:
                    best_len, rng = t - cur, (cur, t)
            else:
                cur = None
        if not rng:
            log(f"- {sname}：观测段前后都找不到四个条件同时满足的时刻")
            continue
        # 区间内每个可选开始时刻的最小角度裕量
        win = int(round(dur / step))
        cand = []
        for i, t in enumerate(st):
            if rng[0] <= t and t + dur <= rng[1] + 1e-9:
                seg = geo[i:i + win + 1]
                cand.append((min(45 - max(x[1], x[2]) for x in seg), min(min(x[0] - 10, 50 - x[0]) for x in seg), t))
        orig = next((x for x in cand if abs(x[2] - s) < step / 2), None)
        bestc = max(cand) if cand else None
        log(f"- {sname}：连续满足区间 {clk.iso(rng[0])} 至 {clk.iso(rng[1])}，约 {best_len:.1f} s")
        if orig:
            log(f"  - 方案原定开始时刻：角度裕量 {orig[0]:.4f}°，距离裕量 {orig[1]:.3f} km")
        if bestc:
            log(f"  - 开始时刻改为 {clk.iso(bestc[2])}（后移 {bestc[2] - s:.1f} s）：角度裕量 {bestc[0]:.4f}°，"
                f"距离裕量 {bestc[1]:.3f} km，Δv 不变")
    with open(OUT / "scan.csv", "w", encoding="utf-8-sig", newline="") as f:
        w = csv.DictWriter(f, fieldnames=list(scan_rows[0]))
        w.writeheader()
        w.writerows(scan_rows)
    scen = OUT / "P1Verify.atk"
    try:
        atk.save(scen)
        log()
        log(f"场景已保存到 `{scen.relative_to(REPO)}`，可以在 ATK 界面里打开查看轨道（b1、r1_full_arc2 等）。")
    except Exception as e:
        log(f"场景保存失败：{e}")
    atk.close()
    log()
    log("明细：samples.csv（0.05 s 网格，全部组合），scan.csv（0.5 s 扫描）。")


if __name__ == "__main__":
    main()
