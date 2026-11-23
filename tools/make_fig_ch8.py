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
    q1 = [(0.01, 5.212815), (0.05, 5.213262), (0.10, 5.213820), (0.25, 5.215507), (0.50, 5.218432)]
    q2 = [(0.01, 132.8886), (0.05, 133.0259), (0.10, 133.2254), (0.25, 133.9844), (0.50, 135.6563)]
    q3_r1 = [(0.01, 11.9374), (0.05, 11.9435), (0.10, 11.9517), (0.25, 11.9789)]
    q3_r2 = [(0.01, 14.1182), (0.05, 14.1231), (0.10, 14.1293), (0.25, 14.1489), (0.50, 14.1825)]

    rel = lambda pts: [(m, (v - pts[0][1]) / pts[0][1] * 100.0) for m, v in pts]  # noqa: E731
    r1, r2, r3a, r3b = rel(q1), rel(q2), rel(q3_r1), rel(q3_r2)

    W, H = 620, 360
    L, T, W_, H_ = 78, 20, 512, 280
    X = lambda v: L + v / 0.54 * W_  # noqa: E731
    Y = lambda v: T + (2.4 - v) / 2.4 * H_  # noqa: E731

    body = []
    # 主方案采用裕量 m = 0.05 虚线
    body.append(F.line(X(0.05), T, X(0.05), T + H_, "#999999", 0.8, "3,3"))
    for pts, col, dash in [(r2, C_Q2, None), (r3b, C_Q3, None), (r3a, "#2E86C1", "5,3"), (r1, C_Q1, "2,2")]:
        body.append(F.poly([(X(m), Y(v)) for m, v in pts], stroke=col, w=1.7, dash=dash, closed=False))
        for m, v in pts:
            body.append(f'<circle cx="{X(m):.1f}" cy="{Y(v):.1f}" r="3.2" fill="{col}" stroke="none"/>')

    F.axes_box(
        body, L, T, W_, H_,
        [0.0, 0.1, 0.2, 0.3, 0.4, 0.5],
        [0.0, 0.5, 1.0, 1.5, 2.0],
        X, Y, "安全裕量 m / (km, °)", "相对速度增量增幅 / (%)",
        xfmt=lambda v: f"{v:.1f}",
        yfmt=lambda v: f"{v:.1f}",
    )
    F.legend_box(body, L + 8, T + 8, [
        ("line", C_Q2, None, "问题二 M-Uni（单星 30 目标）"),
        ("line", C_Q3, None, "问题三 M-Uni（R2 东侧 20 目标）"),
        ("line", "#2E86C1", "5,3", "问题三 M-Uni（R1 西侧 10 目标，m ≤ 0.25）"),
        ("line", C_Q1, "2,2", "问题一（单星单目标）"),
    ])
    F.save("fig14a_sens_margin.svg", W, H, body)


def make_sens_emax():
    pts = [
        (0.00002, 14.1450),
        (0.00010, 14.1432),
        (0.00020, 14.1409),
        (0.00050, 14.1340),
        (0.00100, 14.1230),
    ]
    W, H = 620, 360
    L, T, W_, H_ = 100, 20, 490, 280
    X = lambda v: L + (v * 1e4) / 10.6 * W_  # noqa: E731
    Y = lambda v: T + (14.152 - v) / 0.036 * H_  # noqa: E731

    body = []
    # 主方案上限 e_max = 0.001 基准线
    body.append(F.line(L, Y(14.1230), L + W_, Y(14.1230), "#888888", 0.8, "4,3"))
    body.append(F.poly([(X(e), Y(v)) for e, v in pts], stroke=C_Q1, w=1.8, closed=False))
    for e, v in pts:
        col = C_Q2 if abs(e - 0.00100) < 1e-6 else C_Q1
        r = 4.2 if abs(e - 0.00100) < 1e-6 else 3.2
        body.append(f'<circle cx="{X(e):.1f}" cy="{Y(v):.1f}" r="{r}" fill="{col}" stroke="none"/>')

    F.axes_box(
        body, L, T, W_, H_,
        [0.0, 0.0002, 0.0004, 0.0006, 0.0008, 0.0010],
        [14.120, 14.128, 14.136, 14.144, 14.152],
        X, Y, "R2 初始偏心率上限 e_max / (×10⁻⁴)", "R2 累计速度增量 / (m/s)",
        xfmt=lambda v: f"{v * 1e4:.0f}",
        yfmt=lambda v: f"{v:.3f}",
    )
    F.legend_box(body, L + 8, T + 8, [
        ("line", C_Q1, None, "M-Uni 在不同偏心率上限下的 R2 Δv"),
        ("dot", C_Q2, None, "主方案上限 e_max = 10⁻³（14.123 m/s）"),
        ("line", "#888888", "4,3", "主方案基准 14.123 m/s（全区间变幅 +0.16%）"),
    ])
    F.save("fig14b_sens_emax.svg", W, H, body)


if __name__ == "__main__":
    make_sens_margin()
    make_sens_emax()
