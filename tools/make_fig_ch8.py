"""第 8 节（模型检验与灵敏度分析）配图，学术常规样式，纯 SVG。

用法：python tools/make_fig_ch8.py
输出：
  paper/figures/fig14a_sens_margin.svg  安全裕量 m 对三问速度增量相对增幅的影响
  paper/figures/fig14b_sens_emax.svg    R2 初始偏心率上限 e_max 对 R2 速度增量的影响
"""
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import make_fig_analysis as F  # noqa: E402

C_Q1 = "#1F4E79"   # 深蓝
C_Q2 = "#C0392B"   # 砖红
C_Q3 = "#1E8449"   # 深绿
C_GRAY = "#555555"


def make_sens_margin():
    q1 = [(0.01, 5.212815), (0.1, 5.213820), (0.25, 5.215507), (0.35, 5.216641),
          (0.5, 5.218432), (0.75, 5.222823), (1.0, 5.227627)]
    q2 = [(0.01, 176.312), (0.1, 177.594), (0.15, 178.345), (0.2, 179.119),
          (0.25, 179.912), (0.35, 181.549), (0.5, 184.144)]
    q3 = [(0.01, 33.460), (0.1, 33.483), (0.25, 33.522), (0.5, 33.586), (1.0, 33.714)]

    rel = lambda pts: [(m, (v - pts[0][1]) / pts[0][1] * 100.0) for m, v in pts]  # noqa: E731
    r1, r2, r3 = rel(q1), rel(q2), rel(q3)

    W, H = 620, 360
    L, T, W_, H_ = 78, 20, 512, 280
    X = lambda v: L + v / 1.05 * W_  # noqa: E731
    Y = lambda v: T + (5.2 - v) / 5.2 * H_  # noqa: E731

    body = []
    # 问题二固定序列可行上限 m = 0.5 虚线
    body.append(F.line(X(0.5), T, X(0.5), T + H_, "#999999", 0.8, "3,3"))
    for pts, col, dash in [(r1, C_Q1, None), (r2, C_Q2, None), (r3, C_Q3, "5,3")]:
        body.append(F.poly([(X(m), Y(v)) for m, v in pts], stroke=col, w=1.7, dash=dash, closed=False))
        for m, v in pts:
            body.append(f'<circle cx="{X(m):.1f}" cy="{Y(v):.1f}" r="3.2" fill="{col}" stroke="none"/>')

    F.axes_box(
        body, L, T, W_, H_,
        [0.0, 0.2, 0.4, 0.6, 0.8, 1.0],
        [0, 1, 2, 3, 4, 5],
        X, Y, "安全裕量 m / (km, °)", "相对速度增量增幅 / (%)",
        xfmt=lambda v: f"{v:.1f}",
    )
    F.legend_box(body, L + 8, T + 8, [
        ("line", C_Q2, None, "问题二（单星 30 目标，m ≤ 0.5）"),
        ("line", C_Q3, "5,3", "问题三（双星合计）"),
        ("line", C_Q1, None, "问题一（单星单目标）"),
    ])
    F.save("fig14a_sens_margin.svg", W, H, body)


def make_sens_emax():
    pts = [
        (0.00002, 15.3149),
        (0.00008, 15.3149),
        (0.00014, 15.3149),
        (0.00020, 15.3130),
        (0.00026, 15.3123),
        (0.00033, 15.3119),
        (0.00040, 15.3118),
        (0.00045, 15.3118),
    ]
    W, H = 620, 360
    L, T, W_, H_ = 100, 20, 490, 280
    X = lambda v: L + (v * 1e4) / 4.8 * W_  # noqa: E731
    Y = lambda v: T + (15.320 - v) / 0.012 * H_  # noqa: E731

    body = []
    # 自由解基准线
    body.append(F.line(L, Y(15.3118), L + W_, Y(15.3118), "#888888", 0.8, "4,3"))
    body.append(F.poly([(X(e), Y(v)) for e, v in pts], stroke=C_Q1, w=1.8, closed=False))
    for e, v in pts:
        col = C_Q2 if abs(e - 0.00045) < 1e-6 else C_Q1
        r = 4.2 if abs(e - 0.00045) < 1e-6 else 3.2
        body.append(f'<circle cx="{X(e):.1f}" cy="{Y(v):.1f}" r="{r}" fill="{col}" stroke="none"/>')

    F.axes_box(
        body, L, T, W_, H_,
        [0.0, 0.0001, 0.0002, 0.0003, 0.0004],
        [15.308, 15.311, 15.314, 15.317, 15.320],
        X, Y, "R2 初始偏心率上限 e_max / (×10⁻⁴)", "R2 累计速度增量 / (m/s)",
        xfmt=lambda v: f"{v * 1e4:.1f}",
        yfmt=lambda v: f"{v:.3f}",
    )
    F.legend_box(body, L + 8, T + 8, [
        ("line", C_Q1, None, "不同偏心率上限下的 R2 最优 Δv"),
        ("dot", C_Q2, None, "主方案自由解（e = 4.5×10⁻⁴）"),
        ("line", "#888888", "4,3", "自由解基准 15.312 m/s"),
    ])
    F.save("fig14b_sens_emax.svg", W, H, body)


if __name__ == "__main__":
    make_sens_margin()
    make_sens_emax()
