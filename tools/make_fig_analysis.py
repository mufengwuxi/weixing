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
# 参照空间目标观测文献中太阳—目标—观测者的几何示意画法：黑白线条，矢量加符号，夹角用小圆弧标注，不按比例。
def arc(cx, cy, r, a0, a1, n=30):
    return [(cx + r * np.cos(t), cy - r * np.sin(t)) for t in np.linspace(a0, a1, n)]


def satellite(x, y, body_col, panel_col, scale=1.0, rot=0.0):
    """卫星图标：中间本体 + 两侧太阳翼（带分格线），可整体旋转缩放。"""
    g = [f'<g transform="translate({x:.1f} {y:.1f}) rotate({rot:.1f}) scale({scale})">']
    for sx in (-1, 1):  # 两侧太阳翼
        x0 = 10 if sx > 0 else -38
        g.append(f'<rect x="{x0}" y="-6" width="28" height="12" fill="{panel_col}" stroke="#1A1A1A" stroke-width="0.8"/>')
        for k in (1, 2, 3):
            g.append(f'<line x1="{x0 + 7 * k}" y1="-6" x2="{x0 + 7 * k}" y2="6" stroke="#1A1A1A" stroke-width="0.5"/>')
        g.append('<line x1="{0}" y1="-6" x2="{1}" y2="6" stroke="none"/>'.format(x0, x0))
        g.append(f'<line x1="{x0}" y1="0" x2="{x0 + 28}" y2="0" stroke="#1A1A1A" stroke-width="0.5"/>')
        xa, xb = (7, 10) if sx > 0 else (-10, -7)
        g.append(f'<line x1="{xa}" y1="0" x2="{xb}" y2="0" stroke="#1A1A1A" stroke-width="1.2"/>')
    g.append(f'<rect x="-7" y="-8" width="14" height="16" rx="1.5" ry="1.5" fill="{body_col}" stroke="#1A1A1A" stroke-width="0.9"/>')
    g.append(f'<line x1="0" y1="-8" x2="0" y2="-13" stroke="#1A1A1A" stroke-width="0.9"/>')
    g.append(f'<circle cx="0" cy="-14.5" r="2" fill="{body_col}" stroke="#1A1A1A" stroke-width="0.7"/>')
    g.append("</g>")
    return "\n".join(g)


def fig1a():
    W, H = 720, 540
    tx, ty = 520, 295                     # 目标
    ex, ey, er = 125, 440, 70             # 地球（示意）
    a_e = np.arctan2(-(ey - ty), ex - tx)  # 地心方向（数学角，y 轴向上）
    a_s = a_e - np.radians(75)             # 太阳方向，与地心方向夹角 β = 75°
    a_o = (a_s + a_e) / 2 + np.radians(4)  # 观测卫星方向

    def P(r, t):
        return tx + r * np.cos(t), ty - r * np.sin(t)

    body = ["""<defs>
<radialGradient id="gEarth" cx="0.35" cy="0.32" r="0.75">
<stop offset="0" stop-color="#8EC9F2"/><stop offset="0.55" stop-color="#2E6FB7"/><stop offset="1" stop-color="#0D2E5C"/>
</radialGradient>
<radialGradient id="gLand" cx="0.35" cy="0.32" r="0.8">
<stop offset="0" stop-color="#A8D08D"/><stop offset="1" stop-color="#3E6B2E"/>
</radialGradient>
<radialGradient id="gSun" cx="0.4" cy="0.38" r="0.7">
<stop offset="0" stop-color="#FFF7C2"/><stop offset="0.5" stop-color="#FFC531"/><stop offset="1" stop-color="#F08A12"/>
</radialGradient>
</defs>"""]
    # 地球：渐变球体 + 示意大陆 + 大气外圈
    body.append(f'<circle cx="{ex}" cy="{ey}" r="{er + 4}" fill="#CFE6FA" stroke="none" fill-opacity="0.6"/>')
    body.append(f'<circle cx="{ex}" cy="{ey}" r="{er}" fill="url(#gEarth)" stroke="#0D2E5C" stroke-width="0.8"/>')
    lands = [
        [(-40, -38), (-18, -50), (2, -42), (8, -25), (-6, -12), (-2, 4), (-18, 12), (-30, -2), (-44, -16)],
        [(14, -10), (34, -20), (52, -6), (48, 14), (30, 30), (20, 18), (24, 4)],
        [(-30, 28), (-10, 24), (-2, 40), (-14, 56), (-30, 50)],
    ]
    def chaikin(pts, n=3):
        for _ in range(n):
            q = []
            for i in range(len(pts)):
                a, b = np.array(pts[i]), np.array(pts[(i + 1) % len(pts)])
                q += [tuple(0.75 * a + 0.25 * b), tuple(0.25 * a + 0.75 * b)]
            pts = q
        return pts

    for ld in lands:
        body.append(poly([(ex + u, ey + v) for u, v in chaikin(ld)], fill="url(#gLand)", stroke="#2F5223", w=0.6))
    body.append(text(ex, ey + er + 24, "地球", 14))
    # 目标指向地心：黑色虚线
    ux, uy = (tx - ex) / np.hypot(tx - ex, ty - ey), (ty - ey) / np.hypot(tx - ex, ty - ey)
    body.append(line(tx, ty, ex + (er + 6) * ux, ey + (er + 6) * uy, "#000000", 1.1, "6,4"))
    # 太阳：渐变球体 + 光芒
    sx, sy = P(265, a_s)
    for k in range(12):
        t = 2 * np.pi * k / 12
        body.append(line(sx + 30 * np.cos(t), sy + 30 * np.sin(t), sx + 40 * np.cos(t), sy + 40 * np.sin(t), "#F2A115", 2.0))
    body.append(f'<circle cx="{sx:.1f}" cy="{sy:.1f}" r="26" fill="url(#gSun)" stroke="#E07B00" stroke-width="0.8"/>')
    body.append(text(sx - 48, sy + 5, "太阳", 14, "end"))
    hx, hy = P(215, a_s)
    body.append(arrow(tx, ty, hx, hy, "#000000", 1.3))
    # 距离约束：两段同心圆弧（黑色虚线）
    for r, lab in ((88, "10 km"), (185, "50 km")):
        body.append(poly(arc(tx, ty, r, a_o - np.radians(32), a_o + np.radians(32)), stroke="#000000", w=1.0, closed=False, dash="4,3"))
        lx, ly = P(r, a_o - np.radians(34))
        body.append(text(lx, ly - 8, lab, 12, "middle"))
    # 相对矢量 ρ 与观测卫星（红色）
    ox, oy = P(140, a_o)
    body.append(arrow(tx, ty, ox + 18 * np.cos(a_o) * -1, oy + 18 * np.sin(a_o), "#000000", 1.5))
    mx, my = P(115, a_o + np.radians(8))
    body.append(text(mx, my, "ρ", 16, "middle", italic=True))
    body.append(satellite(ox, oy, "#D62828", "#8C1C13", 0.9, -15))
    body.append(text(ox - 6, oy - 30, "观测卫星 R", 14))
    # 目标卫星（蓝色）
    body.append(satellite(tx, ty, "#1F5FAD", "#163E73", 0.9, -15))
    body.append(text(tx + 36, ty + 28, "目标卫星 B", 14, "start"))
    # 夹角 α、θ、β
    body.append(poly(arc(tx, ty, 44, a_o, a_s), stroke="#000000", w=1.0, closed=False))
    lx, ly = P(58, (a_o + a_s) / 2)
    body.append(text(lx, ly + 5, "α", 15, italic=True))
    body.append(poly(arc(tx, ty, 44, a_e, a_o), stroke="#000000", w=1.0, closed=False))
    lx, ly = P(58, (a_o + a_e) / 2)
    body.append(text(lx, ly + 5, "θ", 15, italic=True))
    body.append(poly(arc(tx, ty, 215, a_e, a_s, 50), stroke="#000000", w=0.9, closed=False, dash="2,3"))
    lx, ly = P(230, (a_e + a_s) / 2)
    body.append(text(lx, ly + 5, "β", 15, "middle", italic=True))
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
                        f'height="{H - T - B}" fill="#E6E6E6" stroke="none"/>')
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
    body.append(poly([(X(a / 3600), Y(b)) for a, b in zip(t, beta)], stroke="#000000", w=1.5, closed=False))
    # 两次观测机会：本文方案的观测时刻 38.43 h，另一次在 14.45 h 附近（求解器的两族起点）
    sol = json.load(open(REPO / "solutions" / "q1" / "p1_claude_v4.json", encoding="utf-8"))
    for h, lab in ((14.45, "机会 1"), (sol["obs_start_s"] / 3600, "机会 2（本文方案）")):
        body.append(line(X(h), T, X(h), H - B, "#000000", 1.0, "2,3"))
        body.append(text(X(h) + 4, T + 16, lab, 12, "start"))
    body.append(text((L + W - R) / 2, H - 14, "时间 / h", 13))
    body.append(text(20, (T + H - B) / 2, "β / (°)", 13, rot=-90))
    save("fig1b_beta_b1.svg", W, H, body)


# ---------------- 图 2：目标分布 ----------------
# 参照同步带目标统计文献的画法：以相位（经度）为横轴，分别画半长轴偏差和倾角两幅散点图，用实心、空心符号区分分组。
def fig2():
    R1, TG = read_states(str(REPO / "data" / "数据.xlsx"))

    def el(x):
        r, v = x[:3], x[3:]
        a = 1 / (2 / np.linalg.norm(r) - v @ v / MU)
        h = np.cross(r, v)
        inc = np.degrees(np.arccos(h[2] / np.linalg.norm(h)))
        return a - A_GEO, inc, np.degrees(np.arctan2(r[1], r[0]))

    da0, i0, l0 = el(np.array(R1))
    west = {"b1", "b2", "b3", "b5", "b8", "b9", "b17", "b18", "b26", "b30"}
    pts = []
    for k, x in TG.items():
        da, inc, lon = el(np.array(x))
        pts.append((k.upper(), (lon - l0 + 180) % 360 - 180, da, inc, k in west))
    W, H = 720, 520
    L, R = 80, 20
    X = lambda d: L + (d + 21) / 42 * (W - L - R)  # noqa: E731
    panels = [  # (上边界, 下边界, 取值函数, 纵轴范围, 刻度, 纵轴名称, 子图名, R1 取值, 标注目标)
        (30, 225, lambda p: p[2], (-7, 6), range(-6, 7, 3), "a − a<tspan baseline-shift=\"sub\" font-size=\"10\">GEO</tspan> / km", "(a)", da0,
         ("B30", "B21", "B11")),
        (285, 470, lambda p: p[3], (0, 10), range(0, 11, 2), "倾角 / (°)", "(b)", i0, ("B18", "B30", "B21")),
    ]
    body = []
    for top, bot, f, (lo, hi), ticks, ylab, tag, r1v, lab in panels:
        Y = lambda v, top=top, bot=bot, lo=lo, hi=hi: bot - (v - lo) / (hi - lo) * (bot - top)  # noqa: E731
        body.append(f'<rect x="{L}" y="{top}" width="{W - L - R}" height="{bot - top}" fill="none" stroke="#000000" stroke-width="1"/>')
        for v in ticks:
            body.append(line(L, Y(v), L + 5, Y(v)))
            body.append(line(W - R, Y(v), W - R - 5, Y(v)))
            body.append(text(L - 7, Y(v) + 4, str(v).replace("-", "−"), 12, "end"))
        for d in range(-20, 21, 5):
            body.append(line(X(d), bot, X(d), bot - 5))
            body.append(line(X(d), top, X(d), top + 5))
            body.append(text(X(d), bot + 18, str(d).replace("-", "−"), 12))
        body.append(line(X(1.6), top, X(1.6), bot, GRAY, 0.9, "5,4"))
        for k, dl, da, inc, w in pts:
            v = f((k, dl, da, inc))
            if w:
                body.append(f'<circle cx="{X(dl):.1f}" cy="{Y(v):.1f}" r="4.2" fill="#FFFFFF" stroke="#000000" stroke-width="1.1"/>')
            else:
                body.append(f'<circle cx="{X(dl):.1f}" cy="{Y(v):.1f}" r="4.2" fill="#000000" stroke="#000000" stroke-width="1.1"/>')
            if k in lab:
                body.append(text(X(dl) + 7, Y(v) - 6, k, 11, "start"))
        rx, ry = X(0), Y(r1v)
        body.append(poly([(rx, ry - 7), (rx + 6.5, ry + 4.5), (rx - 6.5, ry + 4.5)], fill=ORANGE, stroke="#000000", w=0.8))
        body.append(text(rx - 9, ry + 4, "R1", 11, "end", ORANGE))
        cy = (top + bot) / 2
        body.append(f'<text x="28" y="{cy:.1f}" font-family="{FONT}" font-size="13" text-anchor="middle" transform="rotate(-90 28 {cy:.1f})">{ylab}</text>')
        body.append(text(L + 8, top + 18, tag, 13, "start"))
    body.append(text((L + W - R) / 2, H - 16, "初始时刻相对 R1 的相位差 / (°)", 13))
    # 图例放在 (a) 子图右下角
    lx, ly = W - R - 200, 195
    body.append(f'<rect x="{lx - 8}" y="{ly - 16}" width="196" height="24" fill="#FFFFFF" stroke="none"/>')
    body.append(f'<circle cx="{lx}" cy="{ly - 4}" r="4.2" fill="#FFFFFF" stroke="#000000" stroke-width="1.1"/>')
    body.append(text(lx + 9, ly, "西侧 10 颗", 12, "start"))
    body.append(f'<circle cx="{lx + 90}" cy="{ly - 4}" r="4.2" fill="#000000"/>')
    body.append(text(lx + 99, ly, "东侧 20 颗", 12, "start"))
    save("fig2_targets.svg", W, H, body)


# ---------------- 图 3：技术路线 ----------------
# 参照数学建模论文常见的"技术路线图"：按问题分行，按"数据—模型—算法—结果"分列，黑色细线框，灰色表头。
def fig3():
    W, H = 820, 400
    cols = [("输入与预处理", 120), ("模型", 205), ("求解算法", 215), ("结果", 150)]
    x0, y0, hh, rh = 110, 20, 32, 88
    body = []
    xs = [x0]
    for _, w in cols:
        xs.append(xs[-1] + w)
    # 表头
    for (name, w), x in zip(cols, xs):
        body.append(f'<rect x="{x}" y="{y0}" width="{w}" height="{hh}" fill="#D9D9D9" stroke="#000000" stroke-width="1"/>')
        body.append(text(x + w / 2, y0 + 21, name, 13))
    rows = [
        ("问题一", ["R1、B1 初始状态", "β ≤ 90° 时段"], ["两脉冲非线性规划", "60 s 连续观测约束"], ["多初值瞄准 + SLSQP 精修", "0.05 s 网格逐点认证"], ["2 次机动", "5.2128 m/s"]),
        ("问题二", ["30 颗目标状态", "每日观测机会"], ["访问序列 + 全局脉冲时间轴", "两级字典序目标"], ["束搜索定顺序", "全计划序列凸近似 + 邻域搜索"], ["30/30 颗", "176.31 m/s"]),
        ("问题三", ["目标相位分布", "R2 初始轨道约束"], ["R2 选轨 + 东西分工", "δa、e 初始约束"], ["初始状态与脉冲联合优化", "δa 灵敏度扫描"], ["30/30 颗", "33.460 m/s"]),
    ]
    for r, (name, *cells) in enumerate(rows):
        y = y0 + hh + r * rh
        body.append(f'<rect x="20" y="{y}" width="{x0 - 20}" height="{rh}" fill="#F2F2F2" stroke="#000000" stroke-width="1"/>')
        body.append(text((20 + x0) / 2, y + rh / 2 + 5, name, 14))
        for c, (lines, x, (_, w)) in enumerate(zip(cells, xs, cols)):
            body.append(f'<rect x="{x}" y="{y}" width="{w}" height="{rh}" fill="#FFFFFF" stroke="#000000" stroke-width="1"/>')
            for n, s in enumerate(lines):
                body.append(text(x + w / 2, y + rh / 2 - 4 + (n - (len(lines) - 1) / 2) * 22 + 4, s, 12.5, color=(BLUE if c == 3 else "#000000")))
    yb = y0 + hh + 3 * rh + 16
    body.append(f'<rect x="20" y="{yb}" width="{xs[-1] - 20}" height="34" fill="#FFFFFF" stroke="#000000" stroke-width="1" stroke-dasharray="5,3"/>')
    body.append(text((20 + xs[-1]) / 2, yb + 22, "模型检验：Python 精确传播认证、ATK 独立复核、安全裕量与 δa、e 的灵敏度分析", 12.5))
    save("fig3_framework.svg", W, yb + 50, body)


if __name__ == "__main__":
    fig1a()
    fig1b()
    fig2()
    fig3()
