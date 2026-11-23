"""图：问题一观测段约束的采样误差（学术样式，纯 SVG）。

数据由 code/p1/solver.py 的 Problem.geom 在 0.001 s 网格上计算（见 dump()），输出：
  paper/figures/fig7a_grid_zoom.svg   距离最小值附近，1 s 采样点与连续曲线
  paper/figures/fig7b_grid_err.svg    最小距离的采样误差随步长变化，与误差界 h²M/8 对比
"""
import json
import sys
from pathlib import Path

import numpy as np

sys.path.insert(0, str(Path(__file__).resolve().parent))
import make_fig_analysis as F  # noqa: E402

REPO = Path(__file__).resolve().parents[1]
DATA = REPO / "results/q1/grid_check.json"
HS = [2, 1, 0.5, 0.2, 0.1, 0.05, 0.02, 0.01]


def dump():
    sys.path.insert(0, str(REPO / "code/p1"))
    import solver as S
    d = json.load(open(REPO / "solutions/q1/p1_claude_v4.json"))
    P = S.Problem(str(REPO / "data/数据.xlsx"), 0.01, 0.01, 60.0)
    m = d["maneuvers"]
    x = np.r_[m[0]["t_s"] / 3600, m[1]["t_s"] / 3600, np.array(m[0]["dv_km_s"]) * 1000, np.array(m[1]["dv_km_s"]) * 1000, d["obs_start_s"] / 3600]
    s = d["obs_start_s"]
    h0 = 0.001
    tf = np.arange(s, s + 60 + 1e-9, h0)
    df, cA, cT = P.geom(x, tf)
    al, th = (np.degrees(np.arccos(np.clip(c, -1, 1))) for c in (cA, cT))
    M = {k: float(np.abs(np.gradient(np.gradient(f, h0), h0)[5:-5]).max()) for k, f in (("d", df), ("alpha", al), ("theta", th))}
    true = float(df.min())
    err = []
    for h in HS:  # 网格起点在 [0, h) 内平移，取最坏情形
        w = 0.0
        for off in np.linspace(0, h, 21)[:-1]:
            dd = P.geom(x, np.arange(s + off, s + 60 + 1e-9, h))[0]
            w = max(w, float(dd.min()) - true)
        err.append(w * 1000)
    out = dict(t=(tf - s)[::10].tolist(), d=df[::10].tolist(), d_min=true, alpha_max=float(al.max()), theta_max=float(th.max()),
               argmin_d=float(tf[df.argmin()] - s), M=M, hs=HS, err_m=err)
    DATA.parent.mkdir(parents=True, exist_ok=True)
    json.dump(out, open(DATA, "w"), indent=1)
    return out


def main():
    D = json.load(open(DATA)) if DATA.exists() and "--recompute" not in sys.argv else dump()
    t, d = np.array(D["t"]), np.array(D["d"])
    C_MAIN, C_ACC, GR = "#1A365D", "#9B2C2C", "#666666"

    # (a) 最小距离附近（缩放到 [41.2, 43.8] s，避免两侧大面积留白）
    lo, hi = 41.2, 43.8
    m = (t >= lo) & (t <= hi)
    yv = (d[m] - 10.0) * 1000
    ys = [(k, (float(np.interp(k, t, d)) - 10) * 1000) for k in range(42, 44)]
    y0, y1 = 279.3, 281.3
    W, H = 620, 360
    L, T, W_, H_ = 80, 20, 510, 280
    X = lambda v: L + (v - lo) / (hi - lo) * W_  # noqa: E731
    Y = lambda v: T + (y1 - v) / (y1 - y0) * H_  # noqa: E731
    body = []
    d_true = (D["d_min"] - 10) * 1000
    d_1s = min(ys, key=lambda q: q[1])[1]
    body.append(F.line(L, Y(d_true), L + W_, Y(d_true), GR, 0.9, "4,3"))
    body.append(F.line(L, Y(d_1s), L + W_, Y(d_1s), C_ACC, 0.9, "2,3"))
    body.append(F.poly([(X(a), Y(b)) for a, b in zip(t[m], yv) if y0 <= b <= y1], stroke=C_MAIN, w=1.8, closed=False))
    for a, b in ys:
        if y0 <= b <= y1:
            body.append(f'<circle cx="{X(a):.1f}" cy="{Y(b):.1f}" r="4.0" fill="{C_ACC}" stroke="#FFFFFF" stroke-width="1.0"/>')
    F.axes_box(body, L, T, W_, H_, [41.5, 42.0, 42.5, 43.0, 43.5], [279.5, 280.0, 280.5, 281.0], X, Y,
               "相对观测开始时刻 / s", "距离 d − 10 km / m", xfmt=lambda v: f"{v:.1f}")
    F.legend_box(body, L + W_ / 2 - 78, T + 12, [
        ("line", C_MAIN, None, "连续曲线（0.001 s）"),
        ("dot", C_ACC, None, "1 s 采样点"),
        ("line", GR, "4,3", "连续最小距离"),
        ("line", C_ACC, "2,3", "1 s 采样最小距离"),
    ])
    F.save("fig7a_grid_zoom.svg", W, H, body)

    # (b) 采样误差随步长（图例置右下角空白区，彻底避免与 10 m 线重叠；三线配色清晰区分）
    hs, err = np.array(D["hs"]), np.array(D["err_m"])
    W, H = 620, 360
    L, T, W_, H_ = 80, 20, 510, 280
    xl, xh, yl, yh = -2.0, np.log10(3), -5.0, 2.0
    X = lambda v: L + (np.log10(v) - xl) / (xh - xl) * W_  # noqa: E731
    Y = lambda v: T + (yh - np.log10(v)) / (yh - yl) * H_  # noqa: E731
    body = []
    hh = np.logspace(xl, xh, 50)
    bound = D["M"]["d"] * hh ** 2 / 8 * 1000
    body.append(F.line(L, Y(10), L + W_, Y(10), GR, 1.0, "3,3"))
    body.append(F.poly([(X(a), Y(b)) for a, b in zip(hh, bound) if yl <= np.log10(b) <= yh], stroke=C_ACC, w=1.4, closed=False, dash="6,3"))
    body.append(F.poly([(X(a), Y(b)) for a, b in zip(hs, err)], stroke=C_MAIN, w=1.8, closed=False))
    for a, b in zip(hs, err):
        body.append(f'<circle cx="{X(a):.1f}" cy="{Y(b):.1f}" r="3.6" fill="#FFFFFF" stroke="{C_MAIN}" stroke-width="1.5"/>')
    sup = {-5: "⁻⁵", -4: "⁻⁴", -3: "⁻³", -2: "⁻²", -1: "⁻¹", 0: "⁰", 1: "¹", 2: "²"}
    minor = [k * 10 ** e for e in range(-5, 2) for k in range(2, 10)]
    F.axes_box(body, L, T, W_, H_, [0.01, 0.1, 1], [10 ** e for e in range(-5, 3)], X, Y, "采样步长 h / s", "最小距离的采样误差 / m",
               yfmt=lambda v: "10" + sup[int(round(np.log10(v)))], xfmt=lambda v: {0.01: "0.01", 0.1: "0.1", 1: "1"}[v], minor_y=minor)
    F.legend_box(body, L + W_ - 166, T + H_ - 76, [
        ("line", C_MAIN, None, "实测最大误差"),
        ("line", C_ACC, "6,3", "理论误差界 h²M/8"),
        ("line", GR, "3,3", "距离裕量 10 m"),
    ])
    F.save("fig7b_grid_err.svg", W, H, body)


if __name__ == "__main__":
    main()
