"""第 7 节（问题三）配图：抽象图与学术结果图，纯 SVG，复刻第 5、6 节风格。

用法：python tools/make_fig_q3.py
输出：
  paper/figures/fig11_q3abs.svg     问题三的 3D 抽象：经度分组 + R2 初始轨道设计 + 双星单向并行巡游
  paper/figures/fig12a_q3_route.svg 双星相对经度随时间的变化
  paper/figures/fig12b_q3_dv.svg    双星各次观测的速度增量
  paper/figures/fig13_q3_da.svg     半长轴偏离上限 δa 对最优总速度增量 J*(δa) 的影响
"""
import json
import sys
from collections import defaultdict
from pathlib import Path

import numpy as np
from scipy.interpolate import PchipInterpolator

sys.path.insert(0, str(Path(__file__).resolve().parent))
import make_fig_analysis as F  # noqa: E402

REPO = Path(__file__).resolve().parents[1]
PLAN_R1 = REPO / "results/q3/m_uni_q3_r1_best.json"
PLAN_R2 = REPO / "results/q3/m_uni_q3_r2_best.json"
W_E = 2 * np.pi / 86164.0905

C_R1 = "#1F4E79"   # 深蓝（R1）
C_R2 = "#C0392B"   # 砖红（R2）
C_GRAY = "#555555"


def lon(r, t):
    return np.degrees(np.arctan2(r[..., 1], r[..., 0]) - W_E * t)


def track(d, ts, key="r1_km"):
    x = np.array(d[key], float)
    burns = sorted(((b["t_s"], np.array(b["dv_km_s"])) for b in d["burns"]), key=lambda b: b[0])
    r, v, tc, bi, out = x[:3], x[3:], 0.0, 0, []
    for t in ts:
        while bi < len(burns) and burns[bi][0] <= t:
            R, V = F.kepler(r, v, burns[bi][0] - tc)
            r, v, tc = R[0], V[0] + burns[bi][1], burns[bi][0]
            bi += 1
        out.append(F.kepler(r, v, t - tc)[0][0])
    return np.array(out)


def make_route_and_dv():
    d1 = json.load(open(PLAN_R1, encoding="utf-8"))
    d2 = json.load(open(PLAN_R2, encoding="utf-8"))
    lam0 = lon(np.array(d1["r1_km"][:3]), 0.0)
    wrap = lambda a: (a - lam0 + 180) % 360 - 180  # noqa: E731

    # (a) 双星相对经度–时间
    ts1 = np.arange(0, 30 * 86400 + 1, 1800.0)
    ts2 = np.arange(0, 30 * 86400 + 1, 1800.0)
    lr1 = wrap(lon(track(d1, ts1, "r1_km"), ts1))
    lr2 = wrap(lon(track(d2, ts2, "r1_km" if "r1_km" in d2 else "r2_km"), ts2))

    obs1 = sorted([(o["start_s"], o["target"]) for o in d1["observations"]])
    obs2 = sorted([(o["start_s"], o["target"]) for o in d2["observations"]])
    pts1, pts2 = [], []
    for s, n in obs1:
        rT = F.kepler(np.array(d1["targets_km"][n][:3]), np.array(d1["targets_km"][n][3:]), s)[0][0]
        pts1.append((s / 86400, float(wrap(lon(rT, s)))))
    for s, n in obs2:
        rT = F.kepler(np.array(d2["targets_km"][n][:3]), np.array(d2["targets_km"][n][3:]), s)[0][0]
        pts2.append((s / 86400, float(wrap(lon(rT, s)))))

    W, H = 620, 380
    L, T, W_, H_ = 80, 20, 510, 300
    X = lambda v: L + v / 30 * W_  # noqa: E731
    Y = lambda v: T + (22 - v) / 44 * H_  # noqa: E731
    body = [F.line(L, Y(0), L + W_, Y(0), C_GRAY, 0.8, "4,3")]
    body.append(F.poly([(X(t / 86400), Y(v)) for t, v in zip(ts1, lr1)], stroke=C_R1, w=1.6, closed=False))
    body.append(F.poly([(X(t / 86400), Y(v)) for t, v in zip(ts2, lr2)], stroke=C_R2, w=1.6, closed=False))
    for t, v in pts1:
        body.append(f'<circle cx="{X(t):.1f}" cy="{Y(v):.1f}" r="3.2" fill="{C_R1}" stroke="none"/>')
    for t, v in pts2:
        body.append(f'<circle cx="{X(t):.1f}" cy="{Y(v):.1f}" r="3.2" fill="{C_R2}" stroke="none"/>')
    F.axes_box(body, L, T, W_, H_, range(0, 31, 5), range(-20, 21, 10), X, Y, "时间 / 天", "相对经度 / (°)",
               yfmt=lambda v: str(v).replace("-", "−"))
    F.legend_box(body, L + 8, T + 8, [
        ("line", C_R1, None, "R1（西侧 10 颗）"),
        ("line", C_R2, None, "R2（东侧 20 颗）"),
        ("line", C_GRAY, "4,3", "R1 初始经度"),
    ])
    F.save("fig12a_q3_route.svg", W, H, body)

    # (b) 双星各次观测的速度增量
    c1, c2 = defaultdict(float), defaultdict(float)
    for b in d1["burns"]:
        c1[b["for"]] += np.linalg.norm(b["dv_km_s"]) * 1000
    for b in d2["burns"]:
        c2[b["for"]] += np.linalg.norm(b["dv_km_s"]) * 1000
    vals = [(n.upper(), c1.get(n, 0.0), C_R1) for _, n in obs1] + [(n.upper(), c2.get(n, 0.0), C_R2) for _, n in obs2]

    W, H = 620, 380
    L, T, W_, H_ = 70, 20, 530, 290
    n = len(vals)
    bw = W_ / n
    Y = lambda v: T + (6.0 - v) / 6.0 * H_  # noqa: E731
    body = []
    med = float(np.median([v for _, v, _ in vals]))
    x_sep = L + len(obs1) * bw
    body.append(F.line(x_sep, T, x_sep, T + H_, "#888888", 0.9, "3,3"))
    for k, (name, v, col) in enumerate(vals):
        x0 = L + k * bw + bw * 0.18
        body.append(f'<rect x="{x0:.1f}" y="{Y(v):.1f}" width="{bw * 0.64:.1f}" height="{Y(0) - Y(v):.1f}" fill="{col}"/>')
        xc = L + (k + 0.5) * bw
        body.append(F.text(xc + 4, T + H_ + 12, name, 10.5, "end", rot=-60))
    body.append(F.line(L, Y(med), L + W_, Y(med), C_GRAY, 0.8, "4,3"))
    F.axes_box(body, L, T, W_, H_, [], range(0, 7, 1), lambda v: v, Y, None, "速度增量 / (m/s)")
    body.append(F.text(L + W_ / 2, T + H_ + 58, "观测目标（左 10 颗 R1，右 20 颗 R2）", 13))
    F.legend_box(body, L + 8, T + 8, [
        ("rect", C_R1, None, "R1 各次观测（合计 11.94 m/s）"),
        ("rect", C_R2, None, "R2 各次观测（合计 14.12 m/s）"),
        ("line", C_GRAY, "4,3", f"全部 30 颗中位数 {med:.2f} m/s"),
    ])
    F.save("fig12b_q3_dv.svg", W, H + 30, body)


def make_da_curve():
    """δa 参数曲线 J*(δa)（M-Uni 主模型），学术常规样式，图例左上角。"""
    pts = [
        (3.0, 26.142),
        (5.0, 26.069),
        (5.90, 26.036),
        (8.0, 25.960),
        (15.0, 25.709),
        (20.0, 25.529),
    ]
    W, H = 620, 360
    L, T, W_, H_ = 84, 20, 506, 280
    X = lambda v: L + (v - 2.0) / 19.5 * W_  # noqa: E731
    Y = lambda v: T + (26.35 - v) / 0.95 * H_  # noqa: E731
    body = []
    # 主方案 δa = 5.90 km 投影线
    body.append(F.line(X(5.90), Y(26.036), X(5.90), T + H_, C_R2, 0.9, "3,3"))
    body.append(F.line(L, Y(26.036), X(5.90), Y(26.036), C_R2, 0.9, "3,3"))
    # 曲线与数据点
    body.append(F.poly([(X(a), Y(j)) for a, j in pts], stroke=C_R1, w=1.8, closed=False))
    for a, j in pts:
        if abs(a - 5.90) < 0.01:
            body.append(f'<circle cx="{X(a):.1f}" cy="{Y(j):.1f}" r="4.6" fill="{C_R2}" stroke="white" stroke-width="1.2"/>')
        else:
            body.append(f'<circle cx="{X(a):.1f}" cy="{Y(j):.1f}" r="3.4" fill="{C_R1}" stroke="none"/>')

    F.axes_box(
        body, L, T, W_, H_,
        [3, 6, 9, 12, 15, 18, 21],
        [25.4, 25.6, 25.8, 26.0, 26.2],
        X, Y, "半长轴偏离上限 δa / km", "两星合计速度增量 J*(δa) / (m/s)",
        yfmt=lambda v: f"{v:.1f}",
    )
    F.legend_box(body, L + 8, T + 8, [
        ("line", C_R1, None, "M-Uni 联合优化曲线 J*(δa)"),
        ("dot", C_R2, None, "主方案边界 δa = 5.90 km（26.04 m/s）"),
    ])
    F.save("fig13_q3_da.svg", W, H, body)


def make_q3_abstract():
    """问题三的 3D 抽象图：时间–相位平面上经度分组 + R2 初始轨道设计 + 双星并行单向巡游。"""
    W, H = 1100, 560
    O = np.array([110.0, 465.0])
    et, ep, ez = np.array([100.0, 0.0]), np.array([38.0, -48.0]), np.array([0.0, -1.0])
    P = lambda t, p, z=0.0: tuple(O + t * et + p * ep + z * ez)  # noqa: E731
    nl, nd = 6, 7  # 0,1 为西侧(R1)，2,3,4,5 为东侧(R2)

    body = [F.DEFS3D, """<defs>
<linearGradient id="gSec" x1="0" y1="0" x2="0" y2="1"><stop offset="0" stop-color="#BFE3F7" stop-opacity="0.82"/><stop offset="1" stop-color="#2E6FB7" stop-opacity="0.55"/></linearGradient>
<linearGradient id="gSecR1" x1="0" y1="0" x2="0" y2="1"><stop offset="0" stop-color="#FFD4A3"/><stop offset="1" stop-color="#E67E22"/></linearGradient>
<linearGradient id="gSecR2" x1="0" y1="0" x2="0" y2="1"><stop offset="0" stop-color="#FFE9A0"/><stop offset="1" stop-color="#F2A515"/></linearGradient>
<linearGradient id="gFloor" x1="0" y1="1" x2="0.4" y2="0"><stop offset="0" stop-color="#F5F8FC"/><stop offset="1" stop-color="#E1EAF4"/></linearGradient>
<linearGradient id="gGrnF" x1="0" y1="0" x2="0" y2="1"><stop offset="0" stop-color="#27AE60"/><stop offset="1" stop-color="#196F3D"/></linearGradient>
<linearGradient id="gGrnT" x1="0" y1="0" x2="1" y2="1"><stop offset="0" stop-color="#82E0AA"/><stop offset="1" stop-color="#27AE60"/></linearGradient>
<linearGradient id="gGrnS" x1="0" y1="0" x2="1" y2="0"><stop offset="0" stop-color="#1E8449"/><stop offset="1" stop-color="#145A32"/></linearGradient>
</defs>"""]

    def sat3d_green(x, y, s=0.75):
        """复刻 make_fig_analysis.sat3d 的等轴测立体卫星（绿色，用于 R2 与红色 R1 区分）。"""
        return F.sat3d(x, y, "red", s).replace("url(#gRedF)", "url(#gGrnF)").replace("url(#gRedT)", "url(#gGrnT)").replace("url(#gRedS)", "url(#gGrnS)").replace("#7A1111", "#145A32")

    p0, p1, t0, t1 = -0.6, nl - 0.35, -0.3, nd + 0.2
    # 地板（西侧与东侧分界线在 p = 1.65）
    body.append(F.poly([P(t0, p0), P(t1, p0), P(t1, p1), P(t0, p1)], fill="url(#gFloor)", stroke="#9FB3CC", w=0.8))
    body.append(F.line(*P(t0, 1.65), *P(t1, 1.65), "#5D6D7E", 1.3, "6,4"))
    body.append(F.text(*np.add(P(t1, 1.65), (8, 4)), "经度分界", 12, "start", "#5D6D7E"))

    # 坐标轴
    body.append(F.arrow(*P(t0, p0), *P(t1 + 0.45, p0), "#333333", 1.3))
    body.append(F.text(*np.add(P(t1 + 0.5, p0), (6, 5)), "时间 t", 14, "start"))
    body.append(F.arrow(*P(t0, p0), *P(t0, p1 + 0.55), "#333333", 1.3))
    body.append(F.text(*np.add(P(t0, p1 + 0.6), (-6, -4)), "相位 φ", 14, "end"))
    for d in range(1, nd + 1):
        x, y = P(d - 0.5, p0)
        body.append(F.line(x, y, x, y + 5, "#333333", 1))
        body.append(F.text(x, y + 20, f"第 {d} 天", 12))

    # R2 初始轨道设计盒（t=0，东侧起点附近）
    bx = [P(-0.22, 1.9, 6), P(0.22, 1.9, 6), P(0.22, 2.75, 6), P(-0.22, 2.75, 6)]
    bx_top = [P(-0.22, 1.9, 54), P(0.22, 1.9, 54), P(0.22, 2.75, 54), P(-0.22, 2.75, 54)]
    body.append('<polygon points="' + " ".join(f"{a:.1f},{b:.1f}" for a, b in bx) + '" fill="#D5F5E3" fill-opacity="0.45" stroke="#1E8449" stroke-width="1.0" stroke-dasharray="4,3"/>')
    body.append('<polygon points="' + " ".join(f"{a:.1f},{b:.1f}" for a, b in bx_top) + '" fill="none" stroke="#1E8449" stroke-width="1.0" stroke-dasharray="4,3"/>')
    for a, b in zip(bx, bx_top):
        body.append(F.line(*a, *b, "#1E8449", 0.8, "3,3"))

    rnd = np.random.default_rng(11)
    jit = {(l, d): 0.14 * rnd.standard_normal() for l in range(nl) for d in range(nd)}
    # R1（西侧，由 1.45 单向向西到 0）
    picks1 = [(1, 1), (0, 3)]
    # R2（东侧，由 2.2 单向向东到 5）
    picks2 = [(2, 1), (3, 2), (4, 4), (5, 6)]
    sel1, sel2 = set(picks1), set(picks2)

    def section(t, p, mode):
        ang = np.linspace(np.radians(20), np.radians(160), 22)
        outer = [P(t + 0.24 * np.cos(a), p, 36 * np.sin(a) - 2) for a in ang]
        inner = [P(t + 0.09 * np.cos(a), p, 14 * np.sin(a) - 2) for a in ang[::-1]]
        if mode == 1:
            return F.poly(outer + inner, fill="url(#gSecR1)", stroke="#A04000", w=1.1)
        if mode == 2:
            return F.poly(outer + inner, fill="url(#gSecR2)", stroke="#B5770A", w=1.1)
        return F.poly(outer + inner, fill="url(#gSec)", stroke="#2E6FB7", w=0.55)

    # 先画背景目标线与截面（由远及近）
    labels = ["西侧目标 Ⅰ", "西侧目标 Ⅱ", "东侧目标 Ⅰ", "东侧目标 Ⅱ", "东侧目标 Ⅲ", "东侧目标 Ⅳ"]
    for l in range(nl - 1, -1, -1):
        body.append(F.line(*P(t0 + 0.1, l), *P(t1 - 0.05, l), "#7F98B8", 0.8, "4,3"))
        body.append(F.text(*np.add(P(t1, l), (8, 4)), labels[l], 11.5, "start", "#1D4E8F"))
        for d in range(nd):
            mode = 1 if (l, d) in sel1 else (2 if (l, d) in sel2 else 0)
            body.append(section(d + 0.5 + jit[(l, d)], l, mode))

    # R1 轨迹（红，单向向西）
    k1 = [(0.0, 1.45)] + [(d + 0.5 + jit[(l, d)], l) for l, d in picks1]
    tt1 = np.linspace(k1[0][0], k1[-1][0] + 0.5, 300)
    pp1 = PchipInterpolator([k[0] for k in k1] + [k1[-1][0] + 0.5], [k[1] for k in k1] + [k1[-1][1] - 0.15])(tt1)
    body.append(F.poly([P(a, b, 24) for a, b in zip(tt1, pp1)], stroke="#B01818", w=2.5, closed=False))
    for a, b in zip(k1[:-1], k1[1:]):
        for f in (0.3, 0.72):
            t = a[0] + f * (b[0] - a[0])
            x, y = P(t, float(np.interp(t, tt1, pp1)), 24)
            body.append(F.flame(x, y, 200, L=16, Wd=7))

    # R2 轨迹（绿，单向向东）
    k2 = [(0.0, 2.2)] + [(d + 0.5 + jit[(l, d)], l) for l, d in picks2]
    tt2 = np.linspace(k2[0][0], k2[-1][0] + 0.35, 400)
    pp2 = PchipInterpolator([k[0] for k in k2] + [k2[-1][0] + 0.35], [k[1] for k in k2] + [k2[-1][1] + 0.1])(tt2)
    body.append(F.poly([P(a, b, 24) for a, b in zip(tt2, pp2)], stroke="#1E8449", w=2.5, closed=False))
    for a, b in zip(k2[:-1], k2[1:]):
        for f in (0.3, 0.72):
            t = a[0] + f * (b[0] - a[0])
            x, y = P(t, float(np.interp(t, tt2, pp2)), 24)
            body.append(F.flame(x, y, 200, L=16, Wd=7))

    # 两颗立体卫星图标
    body.append(F.sat3d(*P(0.0, 1.45, 44), "red", 0.72))
    x, y = P(0.0, 1.45, 44)
    body.append(F.text(x - 48, y + 6, "R1", 14, "end", "#B01818"))

    body.append(sat3d_green(*P(0.0, 2.2, 44), 0.72))
    x, y = P(0.0, 2.2, 44)
    body.append(F.text(x - 48, y + 6, "R2", 14, "end", "#1E8449"))

    def tag(x, y, lx, ly, s1, s2, col, anchor="start"):
        body.append(F.line(x, y, lx, ly, "#666666", 0.8))
        dx = 5 if anchor == "start" else -5
        body.append(F.text(lx + dx, ly - 6, s1, 13, anchor, col))
        body.append(F.text(lx + dx, ly + 12, s2, 11.5, anchor, "#444444"))

    x, y = P(0.0, 2.55, 54)
    tag(x, y, x + 35, 115, "R2 初始轨道设计区", "|a − aGEO| ≤ δa，e &lt; 0.001", "#1E8449", "start")
    x, y = P(picks2[-1][1] + 0.5 + jit[picks2[-1]], 5, 38)
    tag(x, y, x - 35, 95, "R2 单向向东巡游", "东侧 20 颗，无需折返", "#1E8449", "end")
    x, y = P(picks1[-1][1] + 0.5 + jit[picks1[-1]], 0, 22)
    tag(x, y, x + 150, 550, "R1 单向向西巡游", "西侧 10 颗，无需折返", "#B01818", "start")

    F.save("fig11_q3abs.svg", W, H + 26, body)


if __name__ == "__main__":
    make_route_and_dv()
    make_da_curve()
    make_q3_abstract()
