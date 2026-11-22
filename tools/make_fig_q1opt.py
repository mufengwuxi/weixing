"""问题一机动方案的最优性检验图（学术样式，纯 SVG）。

  fig7c_q1_t1t2.svg   固定两次机动时刻 (t1, t2)、其余变量重新优化得到的最小速度增量 J*(t1, t2)
  fig7d_q1_dvprof.svg 固定某次脉冲的一个 RTN 分量、其余变量重新优化得到的 J*(δ)
数据：results/q1/landscape/times.json、dv.json（code/p1/landscape.py 生成）
"""
import json
import sys
from pathlib import Path

import numpy as np

sys.path.insert(0, str(Path(__file__).resolve().parent))
import make_fig_analysis as F  # noqa: E402

REPO = Path(__file__).resolve().parents[1]
D = REPO / "results/q1/landscape"
VIR = ["#440154", "#3B528B", "#21908C", "#5DC963", "#FDE725"]


def cmap(u):
    u = min(max(u, 0.0), 1.0) * (len(VIR) - 1)
    i = min(int(u), len(VIR) - 2)
    f = u - i
    a = np.array([int(VIR[i][k:k + 2], 16) for k in (1, 3, 5)])
    b = np.array([int(VIR[i + 1][k:k + 2], 16) for k in (1, 3, 5)])
    c = (a + (b - a) * f).round().astype(int)
    return "#%02X%02X%02X" % tuple(c)


def fig_t1t2():
    d = json.load(open(D / "times.json"))
    opt = d["opt"]
    lo, hi = 5.0, 20.0
    W, H = 640, 440
    L, T, W_, H_ = 80, 20, 440, 360
    X = lambda v: L + v / 38 * W_  # noqa: E731
    Y = lambda v: T + (40 - v) / 40 * H_  # noqa: E731
    body = []
    for g in d["grid"]:
        x0, x1, y0, y1 = X(g["t1"] - 0.5), X(g["t1"] + 0.5), Y(g["t2"] + 0.5), Y(g["t2"] - 0.5)
        if g["J"] is None:
            fill = "#D9D9D9"
        else:
            fill = cmap((np.log(g["J"]) - np.log(lo)) / (np.log(hi) - np.log(lo)))
        body.append(f'<rect x="{x0:.1f}" y="{y0:.1f}" width="{x1 - x0 + 0.3:.1f}" height="{y1 - y0 + 0.3:.1f}" fill="{fill}" stroke="none"/>')
    # t2 = t1 + 1 h 与 t2 = s 两条边界
    body.append(F.line(X(0), Y(1), X(37.4), Y(38.4), "#000000", 0.8, "4,3"))
    body.append(F.line(X(0), Y(opt[8]), X(38), Y(opt[8]), "#000000", 0.8, "4,3"))
    # 最优点
    cx, cy = X(opt[0]), Y(opt[1])
    pts = []
    for k in range(10):
        r = 9 if k % 2 == 0 else 4
        a = np.pi / 2 + k * np.pi / 5
        pts.append((cx + r * np.cos(a), cy - r * np.sin(a)))
    body.append(F.poly(pts, fill="#FFFFFF", stroke="#C0392B", w=1.6))
    F.axes_box(body, L, T, W_, H_, range(0, 39, 6), range(0, 41, 8), X, Y, "第一次机动时刻 t₁ / h", "第二次机动时刻 t₂ / h")
    # 色标
    cbx, cbw = L + W_ + 22, 14
    for k in range(100):
        u0 = k / 100
        y_ = T + H_ * (1 - u0 - 0.01)
        body.append(f'<rect x="{cbx}" y="{y_:.1f}" width="{cbw}" height="{H_ / 100 + 0.5:.1f}" fill="{cmap(u0)}"/>')
    body.append(f'<rect x="{cbx}" y="{T}" width="{cbw}" height="{H_}" fill="none" stroke="#000000" stroke-width="0.8"/>')
    for v in (5, 7, 10, 15, 20):
        yy = T + H_ * (1 - (np.log(v) - np.log(lo)) / (np.log(hi) - np.log(lo)))
        body.append(F.line(cbx + cbw, yy, cbx + cbw + 4, yy, "#000000", 0.8))
        body.append(F.text(cbx + cbw + 7, yy + 4, f"{v}", 12, "start"))
    body.append(F.text(cbx + cbw + 40, T + H_ / 2, "J* / (m/s)", 13, rot=-90))
    F.legend_box(body, L + 8, T + 8, [("star", "#C0392B", None, "本文方案 5.2128 m/s"), ("rect", "#D9D9D9", None, "未找到可行解"),
                                      ("line", "#000000", "4,3", "t₂ = t₁ + 1 h，t₂ = s")])
    F.save("fig7c_q1_t1t2.svg", W, H, body)


def fig_dv():
    d = json.load(open(D / "dv.json"))
    styles = {("1", "R"): ("#1F4E79", None), ("1", "T"): ("#1F4E79", "7,3"), ("1", "N"): ("#1F4E79", "2,2"),
              ("2", "R"): ("#C0392B", None), ("2", "T"): ("#C0392B", "7,3"), ("2", "N"): ("#C0392B", "2,2")}
    W, H = 620, 400
    L, T, W_, H_ = 80, 20, 510, 320
    X = lambda v: L + (v + 1) / 2 * W_  # noqa: E731
    Y = lambda v: T + (6.3 - v) / (6.3 - 5.1) * H_  # noqa: E731
    body = [F.line(X(0), T, X(0), T + H_, "#555555", 0.8, "4,3"), F.line(L, Y(d["J_opt"]), L + W_, Y(d["J_opt"]), "#555555", 0.8, "4,3")]
    items = []
    for ln in d["lines"]:
        col, dash = styles[(str(ln["burn"]), ln["comp"])]
        pts = [(X(a), Y(min(b, 6.3))) for a, b in ln["pts"] if b is not None]
        body.append(F.poly(pts, stroke=col, w=1.5, closed=False, dash=dash))
        items.append(("line", col, dash, f"Δv{'₁' if ln['burn'] == 1 else '₂'} 的 {ln['comp']} 分量"))
    F.axes_box(body, L, T, W_, H_, [-1, -0.5, 0, 0.5, 1], [5.2, 5.4, 5.6, 5.8, 6.0, 6.2], X, Y, "分量相对本文方案的偏移 δ / (m/s)", "J* / (m/s)",
               xfmt=lambda v: str(v).replace("-", "−"), yfmt=lambda v: f"{v:.1f}")
    F.legend_box(body, L + 8, T + 8, items)
    F.save("fig7d_q1_dvprof.svg", W, H, body)




def fig_ms():
    o = json.load(open(D / "ms_a.json")) + json.load(open(D / "ms_b.json"))
    W, H = 620, 400
    L, T, W_, H_ = 80, 20, 510, 320
    groups = [(14.42, "#C0392B", "第一个观测时段（14.4 h）"), (38.35, "#1F4E79", "第二个观测时段（38.4 h）")]
    n = max(sum(1 for x in o if x["J"] and abs(x["tc"] / 3600 - w) < 0.1) for w, _, _ in groups)
    X = lambda v: L + v / n * W_  # noqa: E731
    lo, hi = np.log10(4), np.log10(30)
    Y = lambda v: T + (hi - np.log10(v)) / (hi - lo) * H_  # noqa: E731
    body = []
    for w, col, _ in groups:
        J = np.sort([x["J"] for x in o if x["J"] and abs(x["tc"] / 3600 - w) < 0.1])
        for k, v in enumerate(J):
            body.append(f'<circle cx="{X(k + 0.5):.1f}" cy="{Y(v):.1f}" r="2.2" fill="{col}"/>')
    body.append(F.line(L, Y(5.212815), L + W_, Y(5.212815), "#555555", 0.8, "4,3"))
    minor = [v for v in (4, 6, 7, 8, 9, 20, 30)]
    F.axes_box(body, L, T, W_, H_, range(0, n + 1, 25), [5, 10, 20], X, Y, "初值序号（按收敛结果从小到大排列）", "收敛后的 J / (m/s)", minor_y=minor)
    F.legend_box(body, L + 8, T + 8, [("dot", groups[0][1], None, groups[0][2]), ("dot", groups[1][1], None, groups[1][2]),
                                      ("line", "#555555", "4,3", "本文方案 5.2128 m/s")])
    F.save("fig7e_q1_multistart.svg", W, H, body)


if __name__ == "__main__":
    fig_t1t2()
    fig_dv()
    fig_ms()
