"""第 6 节（问题二）结果图，学术论文常规样式，纯 SVG。

用法：python tools/make_fig_q2.py [方案.json]
输出：paper/figures/fig10a_q2_route.svg（R1 相对经度–时间）、fig10b_q2_dv.svg（各次观测的速度增量）
"""
import json
import sys
from collections import defaultdict
from pathlib import Path

import numpy as np

sys.path.insert(0, str(Path(__file__).resolve().parent))
import make_fig_analysis as F  # noqa: E402

REPO = Path(__file__).resolve().parents[1]
PLAN = Path(sys.argv[1]) if len(sys.argv) > 1 else REPO / "results/q2/m_uni_q2_best.json"
W_E = 2 * np.pi / 86164.0905
C_LINE, C_PT = "#1F4E79", "#C0392B"


def lon(r, t):
    return np.degrees(np.arctan2(r[..., 1], r[..., 0]) - W_E * t)


def r1_track(d, ts):
    x = np.array(d["r1_km"], float)
    burns = sorted(((b["t_s"], np.array(b["dv_km_s"])) for b in d["burns"]), key=lambda b: b[0])
    r, v, tc, bi, out = x[:3], x[3:], 0.0, 0, []
    for t in ts:
        while bi < len(burns) and burns[bi][0] <= t:
            R, V = F.kepler(r, v, burns[bi][0] - tc)
            r, v, tc = R[0], V[0] + burns[bi][1], burns[bi][0]
            bi += 1
        out.append(F.kepler(r, v, t - tc)[0][0])
    return np.array(out)


def main():
    d = json.load(open(PLAN, encoding="utf-8"))
    lam0 = lon(np.array(d["r1_km"][:3]), 0.0)
    wrap = lambda a: (a - lam0 + 180) % 360 - 180  # noqa: E731

    # (a) 相对经度–时间
    ts = np.arange(0, 30 * 86400 + 1, 1800.0)
    lr = wrap(lon(r1_track(d, ts), ts))
    obs = [(o["start_s"], o["target"]) for o in d["observations"]]
    pts = []
    for s, n in obs:
        rT = F.kepler(np.array(d["targets_km"][n][:3]), np.array(d["targets_km"][n][3:]), s)[0][0]
        pts.append((s / 86400, float(wrap(lon(rT, s)))))
    W, H = 620, 380
    L, T, W_, H_ = 80, 20, 510, 300
    X = lambda v: L + v / 30 * W_  # noqa: E731
    Y = lambda v: T + (22 - v) / 44 * H_  # noqa: E731
    body = [F.line(L, Y(0), L + W_, Y(0), "#555555", 0.8, "4,3")]
    body.append(F.poly([(X(t / 86400), Y(v)) for t, v in zip(ts, lr)], stroke=C_LINE, w=1.6, closed=False))
    for t, v in pts:
        body.append(f'<circle cx="{X(t):.1f}" cy="{Y(v):.1f}" r="3.2" fill="{C_PT}" stroke="none"/>')
    F.axes_box(body, L, T, W_, H_, range(0, 31, 5), range(-20, 21, 10), X, Y, "时间 / 天", "相对经度 / (°)",
               yfmt=lambda v: str(v).replace("-", "−"))
    F.legend_box(body, L + 8, T + 8, [("line", C_LINE, None, "R1"), ("dot", C_PT, None, "观测时刻的目标"),
                                      ("line", "#555555", "4,3", "R1 初始经度")])
    F.save("fig10a_q2_route.svg", W, H, body)

    # (b) 各次观测的速度增量
    c = defaultdict(float)
    for b in d["burns"]:
        c[b["for"]] += np.linalg.norm(b["dv_km_s"]) * 1000
    vals = [(n.upper(), c.get(n, 0.0)) for _, n in obs]
    W, H = 620, 380
    L, T, W_, H_ = 70, 20, 530, 290
    n = len(vals)
    bw = W_ / n
    Y = lambda v: T + (22 - v) / 22 * H_  # noqa: E731
    body = []
    med = float(np.median([v for _, v in vals]))
    for k, (name, v) in enumerate(vals):
        x0 = L + k * bw + bw * 0.18
        body.append(f'<rect x="{x0:.1f}" y="{Y(v):.1f}" width="{bw * 0.64:.1f}" height="{Y(0) - Y(v):.1f}" fill="{C_LINE}"/>')
        xc = L + (k + 0.5) * bw
        body.append(F.text(xc + 4, T + H_ + 12, name, 10.5, "end", rot=-60))
    body.append(F.line(L, Y(med), L + W_, Y(med), "#555555", 0.8, "4,3"))
    F.axes_box(body, L, T, W_, H_, [], range(0, 23, 5), lambda v: v, Y, None, "速度增量 / (m/s)")
    body.append(F.text(L + W_ / 2, T + H_ + 58, "观测目标（按观测先后）", 13))
    F.legend_box(body, L + 8, T + 8, [("rect", C_LINE, None, "单次观测的速度增量"),
                                      ("line", "#555555", "4,3", f"中位数 {med:.2f} m/s")])
    F.save("fig10b_q2_dv.svg", W, H + 30, body)




def fig9():
    """问题二的抽象：时间–相位平面上的观测事件网格，R1 依次穿过选中事件处的可观测管道截面。"""
    W, H = 1100, 540
    O = np.array([110.0, 450.0])
    et, ep, ez = np.array([100.0, 0.0]), np.array([40.0, -50.0]), np.array([0.0, -1.0])
    P = lambda t, p, z=0.0: tuple(O + t * et + p * ep + z * ez)  # noqa: E731
    nl, nd = 5, 7
    body = [F.DEFS3D, """<defs>
<linearGradient id="gSec" x1="0" y1="0" x2="0" y2="1"><stop offset="0" stop-color="#BFE3F7" stop-opacity="0.85"/><stop offset="1" stop-color="#2E6FB7" stop-opacity="0.6"/></linearGradient>
<linearGradient id="gSecHi" x1="0" y1="0" x2="0" y2="1"><stop offset="0" stop-color="#FFE9A0"/><stop offset="1" stop-color="#F2A515"/></linearGradient>
<linearGradient id="gFloor" x1="0" y1="1" x2="0.4" y2="0"><stop offset="0" stop-color="#F5F8FC"/><stop offset="1" stop-color="#E1EAF4"/></linearGradient>
</defs>"""]
    p0, p1, t0, t1 = -0.7, nl - 0.3, -0.3, nd + 0.2
    body.append(F.poly([P(t0, p0), P(t1, p0), P(t1, p1), P(t0, p1)], fill="url(#gFloor)", stroke="#9FB3CC", w=0.8))
    body.append(F.arrow(*P(t0, p0), *P(t1 + 0.5, p0), "#333333", 1.3))
    body.append(F.text(*np.add(P(t1 + 0.55, p0), (6, 5)), "时间 t", 14, "start"))
    body.append(F.arrow(*P(t0, p0), *P(t0, p1 + 0.6), "#333333", 1.3))
    body.append(F.text(*np.add(P(t0, p1 + 0.65), (-6, -4)), "相位 φ", 14, "end"))
    for d in range(1, nd + 1):
        x, y = P(d - 0.5, p0)
        body.append(F.line(x, y, x, y + 5, "#333333", 1))
        body.append(F.text(x, y + 20, f"第 {d} 天", 12))
    rnd = np.random.default_rng(7)
    jit = {(l, d): 0.16 * rnd.standard_normal() for l in range(nl) for d in range(nd)}
    picks = [(1, 1), (0, 2), (2, 4), (3, 5), (4, 6)]
    sel = set(picks)

    def section(t, p, hi):
        ang = np.linspace(np.radians(20), np.radians(160), 22)
        outer = [P(t + 0.26 * np.cos(a), p, 40 * np.sin(a) - 2) for a in ang]
        inner = [P(t + 0.10 * np.cos(a), p, 15 * np.sin(a) - 2) for a in ang[::-1]]
        return F.poly(outer + inner, fill="url(#gSecHi)" if hi else "url(#gSec)", stroke="#B5770A" if hi else "#2E6FB7", w=1.1 if hi else 0.6)

    knots = [(0.0, 2.0)] + [(d + 0.5 + jit[(l, d)], l) for l, d in picks]
    from scipy.interpolate import PchipInterpolator
    tt = np.linspace(knots[0][0], knots[-1][0] + 0.4, 500)
    pp = PchipInterpolator([k[0] for k in knots] + [knots[-1][0] + 0.4], [k[1] for k in knots] + [knots[-1][1]])(tt)
    # 由远及近：每条目标带先画带线和截面，再画位于该带之后的路径片段
    for l in range(nl - 1, -1, -1):
        body.append(F.line(*P(t0 + 0.1, l), *P(t1 - 0.05, l), "#7F98B8", 0.8, "4,3"))
        body.append(F.text(*np.add(P(t1, l), (8, 4)), f"目标 {chr(0x2160 + l)}", 12, "start", "#1D4E8F"))
        for d in range(nd):
            body.append(section(d + 0.5 + jit[(l, d)], l, (l, d) in sel))
    body.append(F.poly([P(a, b, 26) for a, b in zip(tt, pp)], stroke="#B01818", w=2.6, closed=False))
    for a, b in zip(knots[:-1], knots[1:]):
        for f in (0.28, 0.7):
            t = a[0] + f * (b[0] - a[0])
            x, y = P(t, float(np.interp(t, tt, pp)), 26)
            body.append(F.flame(x, y, 205, L=18, Wd=8))
    body.append(F.sat3d(*P(0.0, 2.0, 48), "red", 0.75))
    x, y = P(0.0, 2.0, 48)
    body.append(F.text(x - 52, y + 6, "R1", 14, "end", "#B01818"))

    def tag(x, y, lx, ly, s1, s2, col, anchor="start"):
        body.append(F.line(x, y, lx, ly, "#666666", 0.8))
        dx = 5 if anchor == "start" else -5
        body.append(F.text(lx + dx, ly - 6, s1, 13.5, anchor, col))
        body.append(F.text(lx + dx, ly + 12, s2, 12, anchor, "#444444"))

    x, y = P(2.5 + jit[(4, 2)], 4, 44)
    tag(x, y, x - 40, 50, "观测事件", "目标约每天一次靠近 R1 轨道面，此时的管道截面", "#1F4E79", "end")
    l, d = picks[-1]
    x, y = P(d + 0.5 + jit[(l, d)], l, 44)
    tag(x, y, x + 30, 50, "选中的事件", "R1 依次穿过，完成观测", "#B5770A")
    t = 0.5 * (knots[2][0] + knots[3][0])
    x, y = P(t, float(np.interp(t, tt, pp)), 26)
    tag(x, y, x + 260, 528, "第 k 段：至多两次脉冲", "上一次观测的终端状态是这一段的初值", "#B01818")
    F.save("fig9_q2abs.svg", W, H + 20, body)


if __name__ == "__main__":
    main()
    fig9()
