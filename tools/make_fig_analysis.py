"""生成论文第 2 节"问题分析"的配图（纯 SVG，可在 Visio 中打开编辑）。

用法：python tools/make_fig_analysis.py
输出：
  paper/figures/fig1a_geometry.svg   观测几何示意图（平面投影示意）
  paper/figures/fig1b_beta_b1.svg    B1 在 0–48 h 内太阳方向与地心方向的夹角 β(t)
  paper/figures/fig2_targets.svg     初始时刻 30 颗目标的相位差–半长轴偏差分布
  paper/figures/fig3_framework.svg   三问求解框架流程图

只用 line / polyline / polygon / circle / rect / text 等基本元素，不用 CSS、clipPath、path 弧线和嵌入字体，
字体写成"SimSun, Times New Roman"，在 Visio 里可以直接改字、改线型、改颜色。
"""

import json
import sys
from pathlib import Path

import numpy as np

REPO = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO / "code"))
sys.path.insert(0, str(REPO / "code" / "p3" / "v3"))
from datafile import read_states  # noqa: E402
from p2lib import SUN, kepler  # noqa: E402

OUT = REPO / "paper" / "figures"
FONT = "SimSun, Times New Roman"
MU = 398600.4418
A_GEO = (MU * (86164.0905 / (2 * np.pi)) ** 2) ** (1 / 3)

# 配色：蓝（主）、橙（强调）、灰（辅助），与 p2 图保持一致
BLUE, ORANGE, GRAY, LIGHT = "#2B5C8A", "#D9822B", "#7F7F7F", "#DCE6F0"


def text(x, y, s, size=13, anchor="middle", color="#000000", rot=None, italic=False):
    tr = f' transform="rotate({rot} {x:.1f} {y:.1f})"' if rot is not None else ""
    it = ' font-style="italic"' if italic else ""
    return (f'<text x="{x:.1f}" y="{y:.1f}" font-family="{FONT}" font-size="{size}" '
            f'text-anchor="{anchor}" fill="{color}"{it}{tr}>{s}</text>')


def line(x1, y1, x2, y2, color="#000000", w=1.0, dash=None):
    d = f' stroke-dasharray="{dash}"' if dash else ""
    return f'<line x1="{x1:.1f}" y1="{y1:.1f}" x2="{x2:.1f}" y2="{y2:.1f}" stroke="{color}" stroke-width="{w}"{d}/>'


def poly(pts, fill="none", stroke="#000000", w=1.0, closed=True, dash=None, opacity=None):
    tag = "polygon" if closed else "polyline"
    p = " ".join(f"{x:.1f},{y:.1f}" for x, y in pts)
    d = f' stroke-dasharray="{dash}"' if dash else ""
    o = f' fill-opacity="{opacity}"' if opacity is not None else ""
    return f'<{tag} points="{p}" fill="{fill}" stroke="{stroke}" stroke-width="{w}"{d}{o}/>'


def arrow(x1, y1, x2, y2, color="#000000", w=1.4, head=8):
    ang = np.arctan2(y2 - y1, x2 - x1)
    p1 = (x2 - head * np.cos(ang - 0.4), y2 - head * np.sin(ang - 0.4))
    p2 = (x2 - head * np.cos(ang + 0.4), y2 - head * np.sin(ang + 0.4))
    return line(x1, y1, x2, y2, color, w) + poly([(x2, y2), p1, p2], fill=color, stroke=color, w=0.5)


def save(name, W, H, body):
    OUT.mkdir(parents=True, exist_ok=True)
    s = (f'<?xml version="1.0" encoding="UTF-8"?>\n<svg xmlns="http://www.w3.org/2000/svg" '
         f'width="{W}" height="{H}" viewBox="0 0 {W} {H}">\n<rect x="0" y="0" width="{W}" height="{H}" fill="#FFFFFF"/>\n'
         + "\n".join(body) + "\n</svg>\n")
    (OUT / name).write_text(s, encoding="utf-8")
    print("已写入", OUT / name)


# ---------------- 图 1(a)：观测几何示意 ----------------
def fig1a():
    W, H = 560, 470
    cx, cy = 260, 250           # 目标位置
    r_in, r_out = 45, 170        # 10 km 与 50 km 球面（示意比例）
    th_sun, th_earth = np.radians(25), np.radians(-45)   # 两条轴线方向（屏幕坐标，y 向下取负）
    hw = np.radians(45)

    def P(r, th):
        return cx + r * np.cos(th), cy - r * np.sin(th)

    body = []
    # 两个圆锥（平面上是两个扇形），淡色填充
    for th, col in ((th_sun, ORANGE), (th_earth, BLUE)):
        pts = [(cx, cy)] + [P(r_out + 22, t) for t in np.linspace(th - hw, th + hw, 40)]
        body.append(poly(pts, fill=col, stroke=col, w=0.8, opacity=0.10))
    # 可观测区域：两扇形交集与环形区域的交
    lo, hi = max(th_sun, th_earth) - hw, min(th_sun, th_earth) + hw
    ts = np.linspace(lo, hi, 60)
    pts = [P(r_out, t) for t in ts] + [P(r_in, t) for t in ts[::-1]]
    body.append(poly(pts, fill=BLUE, stroke=BLUE, w=1.2, opacity=0.45))
    # 球壳
    for r, lab in ((r_in, "10 km"), (r_out, "50 km")):
        body.append(poly([P(r, t) for t in np.linspace(0, 2 * np.pi, 120)], stroke=GRAY, w=1.0, dash="5,4"))
    body.append(text(*P(r_in + 4, np.radians(200)), "10 km", 12, "end", GRAY))
    body.append(text(*P(r_out + 6, np.radians(200)), "50 km", 12, "end", GRAY))
    # 轴线
    body.append(arrow(cx, cy, *P(r_out + 70, th_sun), ORANGE, 1.6))
    body.append(arrow(cx, cy, *P(r_out + 70, th_earth), BLUE, 1.6))
    body.append(text(*P(r_out + 88, th_sun), "指向太阳", 13, "middle", ORANGE))
    body.append(text(*P(r_out + 92, th_earth), "指向地心", 13, "middle", BLUE))
    # 两个 45° 半顶角
    for th, col in ((th_sun, ORANGE), (th_earth, BLUE)):
        body.append(line(cx, cy, *P(r_out + 22, th + hw), col, 0.8, "3,3"))
        body.append(line(cx, cy, *P(r_out + 22, th - hw), col, 0.8, "3,3"))
    body.append(text(*P(105, th_sun + np.radians(25)), "45°", 12, "middle", ORANGE))
    body.append(text(*P(105, th_earth - np.radians(25)), "45°", 12, "middle", BLUE))
    # 目标与示意观测卫星
    body.append(f'<circle cx="{cx}" cy="{cy}" r="5" fill="#000000"/>')
    body.append(text(cx - 10, cy + 20, "目标", 13, "end"))
    ox, oy = P(105, (lo + hi) / 2)
    body.append(f'<circle cx="{ox:.1f}" cy="{oy:.1f}" r="4.5" fill="#FFFFFF" stroke="#000000" stroke-width="1.4"/>')
    body.append(line(ox + 4, oy + 3, ox + 40, oy + 60, "#000000", 0.8))
    body.append(text(ox + 42, oy + 74, "观测卫星", 13, "start"))
    body.append(text(W / 2, 24, "深色区域为瞬时可观测区域", 13))
    save("fig1a_geometry.svg", W, H, body)


# ---------------- 图 1(b)：B1 的 β(t) ----------------
def fig1b():
    _, TG = read_states(str(REPO / "data" / "数据.xlsx"))
    b1 = np.array(TG["b1"])
    t = np.arange(0, 48 * 3600 + 1, 300.0)
    rb = kepler(b1[:3], b1[3:], t)[0]
    s = SUN(t) - rb
    c = np.sum(s * (-rb), 1) / np.linalg.norm(s, axis=1) / np.linalg.norm(rb, axis=1)
    beta = np.degrees(np.arccos(np.clip(c, -1, 1)))
    W, H, L, R, T, B = 640, 340, 70, 25, 25, 60
    X = lambda h: L + h / 48 * (W - L - R)  # noqa: E731
    Y = lambda d: T + (180 - d) / 180 * (H - T - B)  # noqa: E731
    body = []
    # β ≤ 90° 的时段加阴影
    ok = beta <= 90
    i = 0
    while i < len(t):
        if ok[i]:
            j = i
            while j + 1 < len(t) and ok[j + 1]:
                j += 1
            body.append(f'<rect x="{X(t[i] / 3600):.1f}" y="{T}" width="{X(t[j] / 3600) - X(t[i] / 3600):.1f}" '
                        f'height="{H - T - B}" fill="{LIGHT}" stroke="none"/>')
            i = j + 1
        else:
            i += 1
    # 坐标轴与网格
    for d in range(0, 181, 45):
        body.append(line(L, Y(d), W - R, Y(d), "#E0E0E0", 0.8))
        body.append(text(L - 8, Y(d) + 4, str(d), 12, "end"))
    for h in range(0, 49, 6):
        body.append(line(X(h), H - B, X(h), H - B + 5))
        body.append(text(X(h), H - B + 20, str(h), 12))
    body.append(line(L, H - B, W - R, H - B))
    body.append(line(L, T, L, H - B))
    body.append(line(L, Y(90), W - R, Y(90), ORANGE, 1.2, "6,4"))
    body.append(text(L + 6, Y(90) - 6, "β = 90°", 12, "start", ORANGE))
    body.append(poly([(X(a / 3600), Y(b)) for a, b in zip(t, beta)], stroke=BLUE, w=1.8, closed=False))
    # 两次观测机会：本文方案的观测时刻 38.43 h，另一次在 14.45 h 附近（求解器的两族起点）
    sol = json.load(open(REPO / "solutions" / "q1" / "p1_claude_v4.json", encoding="utf-8"))
    for h, lab in ((14.45, "机会 1"), (sol["obs_start_s"] / 3600, "机会 2（本文方案）")):
        body.append(line(X(h), T, X(h), H - B, "#000000", 1.0, "2,3"))
        body.append(text(X(h) + 4, T + 16, lab, 12, "start"))
    body.append(text((L + W - R) / 2, H - 14, "时间 / h", 13))
    body.append(text(20, (T + H - B) / 2, "β / (°)", 13, rot=-90))
    save("fig1b_beta_b1.svg", W, H, body)


# ---------------- 图 2：目标分布 ----------------
def fig2():
    R1, TG = read_states(str(REPO / "data" / "数据.xlsx"))

    def el(x):
        r, v = x[:3], x[3:]
        a = 1 / (2 / np.linalg.norm(r) - v @ v / MU)
        h = np.cross(r, v)
        inc = np.degrees(np.arccos(h[2] / np.linalg.norm(h)))
        return a - A_GEO, inc, np.degrees(np.arctan2(r[1], r[0]))

    da0, i0, l0 = el(np.array(R1))
    pts = []
    for k, x in TG.items():
        da, inc, lon = el(np.array(x))
        pts.append((k.upper(), (lon - l0 + 180) % 360 - 180, da, inc))
    W, H, L, R, T, B = 700, 420, 75, 150, 25, 60
    X = lambda d: L + (d + 21) / 42 * (W - L - R)  # noqa: E731
    Y = lambda d: T + (6 - d) / 13 * (H - T - B)  # noqa: E731
    body = []
    for d in range(-6, 7, 2):
        body.append(line(L, Y(d), W - R, Y(d), "#E8E8E8", 0.8))
        body.append(text(L - 8, Y(d) + 4, str(d), 12, "end"))
    for d in range(-20, 21, 5):
        body.append(line(X(d), H - B, X(d), H - B + 5))
        body.append(text(X(d), H - B + 20, str(d), 12))
    body.append(line(L, H - B, W - R, H - B))
    body.append(line(L, T, L, H - B))
    # 第三问东西分界（B2 与 B4 之间）
    xs = X(1.6)
    body.append(line(xs, T, xs, H - B, GRAY, 1.0, "6,4"))
    body.append(text(xs - 6, T + 14, "R1 负责（西侧 10 颗）", 12, "end", GRAY))
    body.append(text(xs + 6, T + 14, "R2 负责（东侧 20 颗）", 12, "start", GRAY))
    # 颜色表示倾角：4 档离散色
    bins = [(0, 1, "#C6DBEF"), (1, 3, "#6BAED6"), (3, 6, "#2171B5"), (6, 10, "#08306B")]

    def col(inc):
        for a, b, c in bins:
            if a <= inc < b:
                return c
        return bins[-1][2]

    for k, dl, da, inc in pts:
        body.append(f'<circle cx="{X(dl):.1f}" cy="{Y(da):.1f}" r="5" fill="{col(inc)}" stroke="#333333" stroke-width="0.6"/>')
    for k in ("B30", "B21", "B19", "B1", "B28", "B18"):
        _, dl, da, _ = next(p for p in pts if p[0] == k)
        body.append(text(X(dl) + 7, Y(da) - 6, k, 11, "start"))
    # R1
    rx, ry = X(0), Y(da0)
    body.append(poly([(rx, ry - 8), (rx + 7, ry + 5), (rx - 7, ry + 5)], fill=ORANGE, stroke="#000000", w=0.8))
    body.append(text(rx + 9, ry + 16, "R1", 12, "start", ORANGE))
    # 图例
    lx, ly = W - R + 18, T + 30
    body.append(text(lx, ly - 10, "目标倾角", 12, "start"))
    for n, (a, b, c) in enumerate(bins):
        y = ly + 8 + n * 22
        body.append(f'<circle cx="{lx + 6}" cy="{y}" r="5" fill="{c}" stroke="#333333" stroke-width="0.6"/>')
        body.append(text(lx + 18, y + 4, f"{a}°–{b}°", 12, "start"))
    y = ly + 8 + 4 * 22 + 6
    body.append(poly([(lx + 6, y - 7), (lx + 12, y + 4), (lx, y + 4)], fill=ORANGE, stroke="#000000", w=0.8))
    body.append(text(lx + 18, y + 4, f"R1（{i0:.2f}°）", 12, "start"))
    body.append(text((L + W - R) / 2, H - 14, "初始时刻相对 R1 的相位差 / (°)", 13))
    body.append(f'<text x="22" y="{(T + H - B) / 2:.1f}" font-family="{FONT}" font-size="13" text-anchor="middle" transform="rotate(-90 22 {(T + H - B) / 2:.1f})">a − a<tspan baseline-shift="sub" font-size="10">GEO</tspan> / km</text>')
    save("fig2_targets.svg", W, H, body)


# ---------------- 图 3：求解框架 ----------------
def fig3():
    W, H = 760, 300
    body = []

    def box(x, y, w, h, title, lines, color):
        body.append(f'<rect x="{x}" y="{y}" width="{w}" height="{h}" rx="6" ry="6" fill="#FFFFFF" stroke="{color}" stroke-width="1.6"/>')
        body.append(f'<rect x="{x}" y="{y}" width="{w}" height="30" rx="6" ry="6" fill="{color}" stroke="{color}" stroke-width="1.6"/>')
        body.append(text(x + w / 2, y + 20, title, 14, color="#FFFFFF"))
        for n, s in enumerate(lines):
            body.append(text(x + w / 2, y + 54 + n * 22, s, 13))

    bw, bh, y0 = 200, 125, 30
    xs = [20, 280, 540]
    box(xs[0], y0, bw, bh, "问题一  单次观测单元", ["两脉冲非线性规划", "多初值搜索", "0.05 s 网格逐点检验"], BLUE)
    box(xs[1], y0, bw, bh, "问题二  单星序列巡游", ["漂移代价 + 束搜索定顺序", "全计划序列凸近似", "外层调整顺序与时刻"], BLUE)
    box(xs[2], y0, bw, bh, "问题三  双星选轨与分工", ["R2 初始轨道设计", "东西分区、单向扫描", "δa 灵敏度分析"], BLUE)
    for i in range(2):
        body.append(arrow(xs[i] + bw + 4, y0 + bh / 2, xs[i + 1] - 4, y0 + bh / 2, "#000000", 1.6))
    body.append(text(xs[0] + bw + 30, y0 + bh / 2 - 10, "串成序列", 12))
    body.append(text(xs[1] + bw + 30, y0 + bh / 2 - 10, "加一颗星", 12))
    # 底部：统一模型与复核
    yb = y0 + bh + 45
    body.append(f'<rect x="{xs[0]}" y="{yb}" width="{xs[2] + bw - xs[0]}" height="40" rx="6" ry="6" fill="{LIGHT}" stroke="{GRAY}" stroke-width="1.2"/>')
    body.append(text((xs[0] + xs[2] + bw) / 2, yb + 25, "统一基础模型：二体传播、时变可观测区域、两级字典序目标", 13))
    for x in xs:
        body.append(line(x + bw / 2, y0 + bh, x + bw / 2, yb, GRAY, 1.2, "4,3"))
    yv = yb + 60
    body.append(f'<rect x="{xs[0]}" y="{yv}" width="{xs[2] + bw - xs[0]}" height="34" rx="6" ry="6" fill="#FFFFFF" stroke="{ORANGE}" stroke-width="1.4"/>')
    body.append(text((xs[0] + xs[2] + bw) / 2, yv + 22, "全部方案经 Python 精确传播认证与 ATK 独立复核", 13, color=ORANGE))
    save("fig3_framework.svg", W, H, body)


if __name__ == "__main__":
    fig1a()
    fig1b()
    fig2()
    fig3()
