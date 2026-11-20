"""用 ATK 独立复核第一问机动方案。

用法（仓库根目录，Windows，已设置 ATK_HOME，已关闭 ATK 界面）：
    python tools/verify_p1_atk.py solutions/p1_best.json

输入 JSON 格式见 solutions/README.md。全部计算分三路互相对照：
    A. 方案文档里声称的数值（JSON 的 claimed 字段）
    B. 本脚本自带的开普勒解析传播（纯 Python，与队友求解代码无关）
    C. ATK 二体传播 + ATK 太阳星历
另外把 Δv 截成文档表格里的 4 位小数（m/s）再算一遍 C，检查文档精度是否足够复现。

输出：results/p1_atk_verify/summary.md、samples_full.csv、scan_full.csv
"""

import argparse
import csv
import json
import math
import sys
from datetime import datetime, timedelta
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO / "code"))
OUT = REPO / "results" / "p1_atk_verify"
COS45 = math.cos(math.radians(45))
R_E = 6378.137


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
    """椭圆轨道 f/g 函数解析传播（Battin 偏近点角差形式）。"""
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
    i = math.degrees(math.acos(h[2] / norm(h)))
    raan = math.degrees(math.atan2(h[0], -h[1])) % 360
    return {"a_km": a, "e": e, "i_deg": i, "raan_deg": raan, "rp_km": a * (1 - e), "ra_km": a * (1 + e),
            "period_h": 2 * math.pi * math.sqrt(a ** 3 / mu) / 3600 if a > 0 else float("nan")}


def geometry(rO, rT, rS):
    """返回 d(km)、光照角 α、侦测角 θ（度，有向角，不取绝对值）。"""
    rho = sub(rO, rT)
    return norm(rho), angle_deg(sub(rS, rT), rho), angle_deg([-x for x in rT], rho)


# ---------------- 时间 ----------------
class Clock:
    def __init__(self, epoch_iso):
        self.t0 = datetime.fromisoformat(epoch_iso)

    def atk(self, t_s):
        dt = self.t0 + timedelta(seconds=t_s)
        return f"{dt.day} {dt:%b %Y %H:%M:%S}.{dt.microsecond:06d}"

    def iso(self, t_s):
        return (self.t0 + timedelta(seconds=t_s)).isoformat(timespec="milliseconds")


# ---------------- 主流程 ----------------
def chain_python(sol, dvs):
    """纯 Python 复现：r1 初始 → 机动 → 机动 → 观测段初始状态。"""
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
    a = ap.parse_args()

    sol = json.loads(Path(a.solution).read_text(encoding="utf-8"))
    mu = sol["mu_km3_s2"]
    clk = Clock(sol["epoch_utc"])
    s, dur = sol["obs_start_s"], sol.get("obs_duration_s", 60.0)
    dvs_full = [m["dv_km_s"] for m in sol["maneuvers"]]
    dvs_round = [[round(x * 1000, 4) / 1000 for x in dv] for dv in dvs_full]  # 文档表格精度：0.1 mm/s
    OUT.mkdir(parents=True, exist_ok=True)
    md = []

    def log(x=""):
        print(x, flush=True)
        md.append(str(x))
        (OUT / "summary.md").write_text("\n".join(md), encoding="utf-8")

    log("# 第一问方案 ATK 独立复核")
    log()
    log(f"- 方案文件：`{a.solution}`，历元 {sol['epoch_utc']} UTC，μ = {mu} km³/s²")
    log(f"- 观测段：{clk.iso(s)} 起 {dur} s，检查网格 {a.grid} s，ATK 星历步长 {a.step} s")
    log()

    # 1. 数据检查：原始 r1 与修正 r1
    log("## 1. 初始数据检查（纯 Python）")
    log("| 对象 | a (km) | e | i (°) | RAAN (°) | 近地点 (km) | 周期 (h) |")
    log("|---|---|---|---|---|---|---|")
    for key in ("r1_original_km", "r1_used_km", "b1_km"):
        if key in sol:
            el = elements(sol[key][:3], sol[key][3:], mu)
            log(f"| {key} | {el['a_km']:.3f} | {el['e']:.6f} | {el['i_deg']:.4f} | {el['raan_deg']:.3f} | "
                f"{el['rp_km']:.3f} | {el['period_h']:.4f} |")
    if "r1_original_km" in sol:
        o, u = sol["r1_original_km"], sol["r1_used_km"]
        flips = [n for n, x, y in zip(["X", "Y", "Z", "Vx", "Vy", "Vz"], o, u) if x != 0 and abs(x + y) < 1e-12 * max(1, abs(x))]
        log(f"\n原始与使用值之间取反的分量：{flips or '无'}")
        ro, vo = o[:3], o[3:]
        rmin = min(norm(kepler(ro, vo, t, mu)[0]) for t in range(0, 172801, 60))
        log(f"原始 r1 不机动 48 h 内最小地心距：{rmin:.3f} km（地球半径 {R_E} km）")
        rT, vT = sol["b1_km"][:3], sol["b1_km"][3:]
        dmin = min(norm(sub(kepler(ro, vo, t, mu)[0], kepler(rT, vT, t, mu)[0])) for t in range(0, 172801, 60))
        log(f"原始 r1 不机动 48 h 内与 b1 最小距离：{dmin:.3f} km")
    log()

    # 2. Python 复现方案
    log("## 2. 纯 Python 开普勒复现")
    t2, r2p, v2p, mstates = chain_python(sol, dvs_full)
    for (tm, r, v), dv in zip(mstates, dvs_full):
        log(f"- 机动 {clk.iso(tm)}：|Δv| = {norm(dv) * 1000:.6f} m/s，机动前 |r| = {norm(r):.3f} km")
    log(f"- 累计 Δv = {sum(norm(dv) for dv in dvs_full) * 1000:.6f} m/s")
    for i, (r, v) in enumerate([(r2p, v2p)]):
        el = elements(r, v, mu)
        log(f"- 末次机动后：a = {el['a_km']:.3f} km，e = {el['e']:.6f}，i = {el['i_deg']:.4f}°")
    log()

    # 3. ATK
    log("## 3. ATK 复核")
    from atkbridge import ATK
    atk = ATK(out_dir=OUT / "raw")
    atk.new("P1Verify", clk.atk(0), clk.atk(172800))
    km = lambda x: [y * 1000 for y in x]  # noqa: E731
    atk.add_satellite_cartesian("b1", clk.atk(0), km(sol["b1_km"][:3]), km(sol["b1_km"][3:]), step_s=a.step)
    atk.add_satellite_cartesian("r1_arc0", clk.atk(0), km(sol["r1_used_km"][:3]), km(sol["r1_used_km"][3:]), step_s=a.step)
    try:
        atk.add_planet("Sun", "Sun")
        sun_ok = True
    except Exception as e:
        sun_ok = False
        log(f"- 太阳对象创建失败：{e}")

    # 3.1 判断 ATK 读出的哪个坐标系等于赋值时的 ICRF
    assigned = km(sol["r1_used_km"][:3])
    frame, best = "CBI", None
    for fr in ("CBI", "J2000"):
        try:
            p, _ = atk.posvel("r1_arc0", clk.atk(0), fr)
            dif = norm(sub(p, assigned))
            log(f"- 历元时刻 {fr} 读数与 ICRF 赋值相差 {dif:.6f} m")
            if best is None or dif < best:
                frame, best = fr, dif
        except Exception as e:
            log(f"- 读取 {fr} 失败：{e}")
    log(f"- 后续统一使用 {frame} 读数")

    # 3.2 ATK 传播精度：与 Python 开普勒在非网格时刻对比（反映 μ 差异与星历插值误差）
    for tt in (3600.5, 86400.25, s + 0.025):
        p, _ = atk.posvel("r1_arc0", clk.atk(tt), frame)
        pk, _ = kepler(sol["r1_used_km"][:3], sol["r1_used_km"][3:], tt, mu)
        log(f"- t = {tt} s：ATK 与 Python 开普勒位置差 {norm(sub(p, km(pk))):.4f} m")

    def run_variant(tag, dvs):
        prev, t_prev = "r1_arc0", 0.0
        for k, (m, dv) in enumerate(zip(sol["maneuvers"], dvs), 1):
            p, v = atk.posvel(prev, clk.atk(m["t_s"]), frame)
            name = f"r1_{tag}_arc{k}"
            atk.add_satellite_cartesian(name, clk.atk(m["t_s"]), p, add(v, km(dv)), frame="ICRF", step_s=a.step)
            prev = name
        return prev

    def check(arc, tag):
        rows = []
        n = int(round(dur / a.grid))
        for k in range(n + 1):
            t = s + k * a.grid
            ts = clk.atk(t)
            rO = [x / 1000 for x in atk.posvel(arc, ts, frame)[0]]
            rT = [x / 1000 for x in atk.posvel("b1", ts, frame)[0]]
            rS = [x / 1000 for x in atk.posvel("Sun", ts, frame)[0]] if sun_ok else None
            d, al, th = geometry(rO, rT, rS) if rS else (norm(sub(rO, rT)), float("nan"), angle_deg([-x for x in rT], sub(rO, rT)))
            rows.append({"t_s": round(t, 4), "utc": clk.iso(t), "d_km": d, "alpha_deg": al, "theta_deg": th})
        with open(OUT / f"samples_{tag}.csv", "w", encoding="utf-8-sig", newline="") as f:
            w = csv.DictWriter(f, fieldnames=list(rows[0]))
            w.writeheader()
            w.writerows(rows)
        dd = [r["d_km"] for r in rows]
        al = [r["alpha_deg"] for r in rows]
        th = [r["theta_deg"] for r in rows]
        ok = min(dd) >= 10 and max(dd) <= 50 and max(al) <= 45 and max(th) <= 45
        return {"d_min": min(dd), "d_max": max(dd), "alpha_max": max(al), "theta_max": max(th), "ok": ok,
                "first": rows[0], "last": rows[-1]}

    def scan(arc, tag, before=300, after=360, step=0.5):
        """在观测段前后扫描，找出四个条件同时满足的最长连续区间。"""
        best_len, cur, best_rng = 0.0, None, None
        rows = []
        k = 0
        while True:
            t = s - before + k * step
            if t > s + after:
                break
            ts = clk.atk(t)
            rO = [x / 1000 for x in atk.posvel(arc, ts, frame)[0]]
            rT = [x / 1000 for x in atk.posvel("b1", ts, frame)[0]]
            rS = [x / 1000 for x in atk.posvel("Sun", ts, frame)[0]] if sun_ok else rT
            d, al, th = geometry(rO, rT, rS)
            good = 10 <= d <= 50 and al <= 45 and th <= 45
            rows.append({"t_s": t, "d_km": d, "alpha_deg": al, "theta_deg": th, "ok": int(good)})
            if good:
                cur = t if cur is None else cur
                if t - cur > best_len:
                    best_len, best_rng = t - cur, (cur, t)
            else:
                cur = None
            k += 1
        with open(OUT / f"scan_{tag}.csv", "w", encoding="utf-8-sig", newline="") as f:
            w = csv.DictWriter(f, fieldnames=list(rows[0]))
            w.writeheader()
            w.writerows(rows)
        return best_len, best_rng

    results = {}
    for tag, dvs in (("full", dvs_full), ("round4", dvs_round)):
        arc = run_variant(tag, dvs)
        p, v = atk.posvel(arc, clk.atk(s), frame)
        tp, rp, vp, _ = chain_python(sol, dvs)
        pk, _ = kepler(rp, vp, s - tp, mu)
        results[tag] = check(arc, tag)
        results[tag]["atk_vs_py_m"] = norm(sub(p, km(pk)))
        results[tag]["scan"] = scan(arc, tag) if tag == "full" else None
    # Python 版本同网格检查（用 ATK 太阳，避免自带太阳模型）
    py_rows = []
    n = int(round(dur / a.grid))
    for k in range(n + 1):
        t = s + k * a.grid
        rO, _ = kepler(r2p, v2p, t - t2, mu)
        rT, _ = kepler(sol["b1_km"][:3], sol["b1_km"][3:], t, mu)
        rS = [x / 1000 for x in atk.posvel("Sun", clk.atk(t), frame)[0]] if sun_ok else rT
        py_rows.append(geometry(rO, rT, rS))
    atk.close()

    log()
    log("### 观测段 60 s 检查结果")
    c = sol.get("claimed", {})
    py = {"d_min": min(r[0] for r in py_rows), "d_max": max(r[0] for r in py_rows),
          "alpha_max": max(r[1] for r in py_rows), "theta_max": max(r[2] for r in py_rows)}
    log("| 项目 | 方案声称 | Python 开普勒 + ATK 太阳 | ATK 全精度 Δv | ATK 4 位小数 Δv | 要求 |")
    log("|---|---|---|---|---|---|")
    f, r4 = results["full"], results["round4"]
    log(f"| 最小距离 (km) | {c.get('d_min_km', '')} | {py['d_min']:.4f} | {f['d_min']:.4f} | {r4['d_min']:.4f} | ≥ 10 |")
    log(f"| 最大距离 (km) | {c.get('d_max_km', '')} | {py['d_max']:.4f} | {f['d_max']:.4f} | {r4['d_max']:.4f} | ≤ 50 |")
    log(f"| 最大光照角 (°) | {c.get('alpha_max_deg', '')} | {py['alpha_max']:.6f} | {f['alpha_max']:.6f} | {r4['alpha_max']:.6f} | ≤ 45 |")
    log(f"| 最大侦测角 (°) | {c.get('theta_max_deg', '')} | {py['theta_max']:.6f} | {f['theta_max']:.6f} | {r4['theta_max']:.6f} | ≤ 45 |")
    log(f"| 60 s 全程满足 | 是 | {'是' if py['d_min'] >= 10 and py['d_max'] <= 50 and py['alpha_max'] <= 45 and py['theta_max'] <= 45 else '否'} | "
        f"{'是' if f['ok'] else '否'} | {'是' if r4['ok'] else '否'} | |")
    log(f"| 观测开始时 ATK 与 Python 位置差 (m) | | | {f['atk_vs_py_m']:.3f} | {r4['atk_vs_py_m']:.3f} | |")
    log()
    ln, rng = f["scan"]
    if rng:
        log(f"在观测段前 300 s 到后 360 s 内按 0.5 s 扫描，四个条件同时满足的最长连续区间为 "
            f"{clk.iso(rng[0])} 至 {clk.iso(rng[1])}，约 {ln:.1f} s。")
    else:
        log("在观测段前后扫描，没有找到四个条件同时满足的时刻。")
    log()
    log("明细：samples_full.csv、samples_round4.csv（0.05 s 网格），scan_full.csv（0.5 s 扫描）。")


if __name__ == "__main__":
    main()
