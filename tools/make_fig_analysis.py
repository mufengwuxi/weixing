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
        lx, ly = P(r + 22 if r < 100 else r - 22, a_o + np.radians(22))
        body.append(text(lx, ly + 4, lab, 12, "middle"))
    # 相对矢量 ρ 与观测卫星（红色）
    ox, oy = P(140, a_o)
    body.append(arrow(tx, ty, ox + 18 * np.cos(a_o) * -1, oy + 18 * np.sin(a_o), "#000000", 1.5))
    mx, my = P(115, a_o + np.radians(8))
    body.append(text(mx, my, "ρ", 16, "middle", italic=True))
    body.append(satellite(ox, oy, "#D62828", "#8C1C13", 0.9, -15))
    lx, ly = P(140, a_o - np.radians(15))
    body.append(text(lx, ly - 4, "观测卫星 R", 14))
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
    t = np.arange(0, 48 * 3600 + 1, 60.0)
    rb = kepler(b1[:3], b1[3:], t)[0]
    s = SUN(t) - rb
    c = np.sum(s * (-rb), 1) / np.linalg.norm(s, axis=1) / np.linalg.norm(rb, axis=1)
    beta = np.degrees(np.arccos(np.clip(c, -1, 1)))
    h = t / 3600
    # β = 90° 的穿越时刻（线性插值）
    edges = []
    for i in np.where(np.diff(np.sign(beta - 90)) != 0)[0]:
        edges.append(h[i] + (90 - beta[i]) / (beta[i + 1] - beta[i]) * (h[i + 1] - h[i]))
    W, H, L, R, T, B = 700, 380, 72, 24, 46, 78
    X = lambda v: L + v / 48 * (W - L - R)  # noqa: E731
    Y = lambda d: T + (180 - d) / 180 * (H - T - B)  # noqa: E731
    body = []
    # 可观测时段：浅蓝底色
    win = [(edges[k], edges[k + 1]) for k in range(0, len(edges) - 1, 2)]
    for a0, a1 in win:
        body.append(f'<rect x="{X(a0):.1f}" y="{T}" width="{X(a1) - X(a0):.1f}" height="{H - T - B}" fill="#DCEAF7" stroke="none"/>')
    # 网格
    for d in range(0, 181, 30):
        body.append(line(L, Y(d), W - R, Y(d), "#E3E3E3", 0.7))
    for v in range(0, 49, 6):
        body.append(line(X(v), T, X(v), H - B, "#EEEEEE", 0.7))
    # β ≤ 90° 部分的曲线下方填充
    for a0, a1 in win:
        m = (h >= a0) & (h <= a1)
        pts = [(X(a0), Y(90))] + [(X(u), Y(v)) for u, v in zip(h[m], beta[m])] + [(X(a1), Y(90))]
        body.append(poly(pts, fill="#2B5C8A", stroke="none", opacity=0.18))
    # 90° 阈值线
    body.append(line(L, Y(90), W - R, Y(90), "#C0392B", 1.3, "7,4"))
    body.append(text(X(28), Y(90) + 18, "β = 90°", 13, "middle", "#C0392B"))
    # 曲线
    body.append(poly([(X(u), Y(v)) for u, v in zip(h, beta)], stroke="#1F4E79", w=2.2, closed=False))
    # 窗口边界：竖虚线、圆点、横坐标数值
    for e in edges:
        body.append(line(X(e), Y(90), X(e), H - B, "#1F4E79", 0.9, "3,3"))
        body.append(f'<circle cx="{X(e):.1f}" cy="{Y(90):.1f}" r="3.6" fill="#FFFFFF" stroke="#1F4E79" stroke-width="1.5"/>')
        body.append(text(X(e), H - B + 38, f"{e:.2f}", 11.5, "middle", "#1F4E79"))
        body.append(line(X(e), H - B, X(e), H - B + 26, "#1F4E79", 0.7, "2,2"))
    for a0, a1 in win:
        body.append(text((X(a0) + X(a1)) / 2, T + 20, "可能观测时段", 13, "middle", "#1F4E79"))
        body.append(text((X(a0) + X(a1)) / 2, T + 37, f"{a1 - a0:.1f} h", 12, "middle", "#1F4E79"))
    # 坐标框与刻度（刻度朝内）
    body.append(f'<rect x="{L}" y="{T}" width="{W - L - R}" height="{H - T - B}" fill="none" stroke="#000000" stroke-width="1.1"/>')
    for d in range(0, 181, 30):
        body.append(line(L, Y(d), L + 6, Y(d)))
        body.append(line(W - R, Y(d), W - R - 6, Y(d)))
        body.append(text(L - 8, Y(d) + 4, str(d), 12, "end"))
    for v in range(0, 49, 6):
        body.append(line(X(v), H - B, X(v), H - B - 6))
        body.append(line(X(v), T, X(v), T + 6))
        body.append(text(X(v), H - B + 18, str(v), 12))
    body.append(text((L + W - R) / 2, H - 12, "时间 t / h", 13))
    body.append(text(24, (T + H - B) / 2, "β / (°)", 13, rot=-90))
    # 图例
    lx, ly = L + 4, 22
    body.append(line(lx, ly, lx + 28, ly, "#1F4E79", 2.2))
    body.append(text(lx + 34, ly + 4, "B1 处太阳方向与地心方向的夹角 β", 12, "start"))
    body.append(f'<rect x="{lx + 300}" y="{ly - 7}" width="26" height="14" fill="#DCEAF7" stroke="#9DB9D5" stroke-width="0.8"/>')
    body.append(text(lx + 332, ly + 4, "β ≤ 90°（满足必要条件）", 12, "start"))
    save("fig1b_beta_b1.svg", W, H, body)


# ---------------- 图 2：目标分布 ----------------
# 参照同步带目标统计文献的画法：以相位（经度）为横轴，分别画半长轴偏差和倾角两幅散点图，用实心、空心符号区分分组。
def fig2():
    """同步带扇形图：周向为相对 R1 的相位（真实角度），径向为 a − a_GEO（放大显示）；左下角插图标出放大区域在整个同步带上的位置。"""
    R1, TG = read_states(str(REPO / "data" / "数据.xlsx"))

    def el(x):
        r, v = x[:3], x[3:]
        a = 1 / (2 / np.linalg.norm(r) - v @ v / MU)
        hv = np.cross(r, v)
        inc = np.degrees(np.arccos(hv[2] / np.linalg.norm(hv)))
        return a - A_GEO, inc, np.degrees(np.arctan2(r[1], r[0]))

    da0, i0, l0 = el(np.array(R1))
    west = {"b1", "b2", "b3", "b5", "b8", "b9", "b17", "b18", "b26", "b30"}
    pts = []
    for k, x in TG.items():
        da, inc, lon = el(np.array(x))
        pts.append((k.upper(), (lon - l0 + 180) % 360 - 180, da, inc, k in west))

    W, H = 940, 480
    cx, cy = 455, 1265          # 地心在画面下方很远处
    R0, K = 1100, 12.0          # 同步轨道半径（像素）、半长轴偏差放大系数（像素/km）
    span, lo, hi = 22, -8.0, 7.0

    def P(dl, da):
        th = np.radians(90 - dl)
        r = R0 + K * da
        return cx + r * np.cos(th), cy - r * np.sin(th)

    def ring(da, a0=-span, a1=span, n=160):
        return [P(d, da) for d in np.linspace(a0, a1, n)]

    body = ["""<defs>
<radialGradient id="gEarth2" cx="0.38" cy="0.3" r="0.75">
<stop offset="0" stop-color="#8EC9F2"/><stop offset="0.55" stop-color="#2E6FB7"/><stop offset="1" stop-color="#0D2E5C"/>
</radialGradient>
</defs>"""]
    body.append(poly(ring(lo) + ring(hi)[::-1], fill="#F3F6FA", stroke="none"))
    body.append(poly(ring(lo) + ring(hi)[::-1], stroke="#000000", w=1.0))
    for d in range(-20, 21, 5):
        x1, y1 = P(d, lo)
        x2, y2 = P(d, hi)
        body.append(line(x1, y1, x2, y2, "#D5D5D5", 0.7))
        x2b, y2b = P(d, hi + 0.5)
        body.append(line(x2, y2, x2b, y2b, "#000000", 1.0))
        tx_, ty_ = P(d, hi + 1.5)
        body.append(text(tx_, ty_, (f"{d:+d}°" if d else "0°").replace("-", "−"), 12, rot=d))
    for da in (-5, 0, 5):
        body.append(poly(ring(da), stroke="#000000" if da == 0 else "#8A8A8A", w=1.3 if da == 0 else 0.7,
                         closed=False, dash=None if da == 0 else "4,3"))
        x, y = P(span, da)
        lab = {-5: "−5 km", 5: "+5 km"}.get(da)
        if lab:
            body.append(text(x + 6, y + 4, lab, 11, "start", "#555555"))
        else:
            body.append(f'<text x="{x + 6:.1f}" y="{y + 4:.1f}" font-family="{FONT}" font-size="12" text-anchor="start">a<tspan baseline-shift="sub" font-size="9">GEO</tspan></text>')
    # 运动方向
    pa = [P(d, hi + 3.2) for d in np.linspace(11, 19, 30)]
    body.append(poly(pa, stroke="#000000", w=1.2, closed=False))
    body.append(arrow(*pa[-2], *pa[-1], "#000000", 1.2, 7))
    x, y = P(15, hi + 3.8)
    body.append(text(x, y, "卫星运动方向（向东）", 12, rot=15))
    # 目标
    bins = [(0, 1, "#C6DBEF"), (1, 3, "#6BAED6"), (3, 6, "#2171B5"), (6, 10, "#08306B")]

    def col(inc):
        return next((c for a_, b_, c in bins if a_ <= inc < b_), bins[-1][2])

    for k, dl, da, inc, w in pts:
        x, y = P(dl, da)
        body.append(f'<circle cx="{x:.1f}" cy="{y:.1f}" r="5.5" fill="{col(inc)}" stroke="#1A1A1A" stroke-width="0.8"/>')
    offs = {"B30": (9, 4), "B18": (9, -7), "B21": (9, -6), "B11": (9, 11), "B26": (-9, -7), "B28": (9, 12),
            "B17": (0, 18), "B6": (-8, -8)}
    for k, dl, da, inc, w in pts:
        if k in offs:
            x, y = P(dl, da)
            ox, oy = offs[k]
            body.append(text(x + ox, y + oy, k, 11, "start" if ox > 0 else ("end" if ox < 0 else "middle")))
    x, y = P(12.03, 2.03)
    body.append(line(x, y - 6, x - 6, y - 30, "#000000", 0.7))
    body.append(text(x - 8, y - 33, "B19、B20", 11, "middle"))
    # R1
    x, y = P(0, da0)
    body.append(satellite(x, y + 36, "#D62828", "#8C1C13", 0.55, 0))
    body.append(line(x, y, x, y + 27, "#D62828", 0.8))
    body.append(f'<circle cx="{x:.1f}" cy="{y:.1f}" r="3.5" fill="#D62828" stroke="#1A1A1A" stroke-width="0.6"/>')
    body.append(text(x - 26, y + 40, "R1", 12, "end", "#D62828"))
    # 左下角插图：整个同步带与放大区域
    ix, iy, re, rg = 82, 398, 22, 58
    body.append(f'<circle cx="{ix}" cy="{iy}" r="{rg}" fill="none" stroke="#000000" stroke-width="0.9"/>')
    body.append(f'<circle cx="{ix}" cy="{iy}" r="{re}" fill="url(#gEarth2)" stroke="#0D2E5C" stroke-width="0.6"/>')
    sec = [(ix + rg * np.cos(np.radians(90 - d)), iy - rg * np.sin(np.radians(90 - d))) for d in np.linspace(-19, 19, 20)]
    body.append(poly([(ix, iy)] + sec, fill="#D62828", stroke="#D62828", w=0.8, opacity=0.25))
    body.append(poly(sec, stroke="#D62828", w=2.4, closed=False))
    body.append(text(ix, iy + rg + 18, "同步带与放大区域", 11))
    # 图例（右下）
    lx, ly = W - 150, 352
    body.append(text(lx, ly, "目标倾角", 12, "start"))
    for n, (a_, b_, c) in enumerate(bins):
        yy = ly + 20 + n * 19
        body.append(f'<circle cx="{lx + 7}" cy="{yy - 4}" r="5.5" fill="{c}" stroke="#1A1A1A" stroke-width="0.8"/>')
        body.append(text(lx + 18, yy, f"{a_}°–{b_}°", 12, "start"))
    save("fig2_targets.svg", W, H, body)


# ---------------- 图 3：总体框架 ----------------
# 参照项目申请书和建模论文常见的"总体框架图"：顶部标题条，问题特点一行，三列研究内容（虚线框 + 深色标题 + 浅色要点框 + 底部方法名），
# 底部为支撑层（统一基础模型、模型检验）。只画思路和方法，不出现任何结果数值。
def fig3():
    W, H = 900, 690
    DK, LT, LB = "#1E5B56", "#DCE6EC", "#EEF3F6"
    body = []

    def rect(x, y, w, h, fill, stroke="none", sw=1.0, dash=None, rx=3):
        d = f' stroke-dasharray="{dash}"' if dash else ""
        return f'<rect x="{x:.1f}" y="{y:.1f}" width="{w:.1f}" height="{h:.1f}" rx="{rx}" ry="{rx}" fill="{fill}" stroke="{stroke}" stroke-width="{sw}"{d}/>'

    def down(x, y1, y2, lab=None):
        body.append(arrow(x, y1, x, y2, "#2F6FA8", 2.0, 9))
        if lab:
            body.append(text(x + 12, (y1 + y2) / 2 + 4, lab, 11.5, "start", "#333333"))

    # 标题条
    body.append(rect(20, 14, W - 40, 34, DK))
    body.append(text(W / 2, 37, "高轨卫星抵近观测与机动规划总体框架", 16, color="#FFFFFF"))
    # 问题特点
    body.append(rect(20, 58, W - 40, 50, "#FFFFFF", "#7F7F7F", 1.0))
    feats = ["时变非凸的观测几何", "相位漂移主导的机动代价", "离散排序与连续机动耦合", "第二颗星的轨道待设计"]
    fw = (W - 40 - 50) / 4
    for i, f in enumerate(feats):
        x = 30 + i * (fw + 10)
        body.append(rect(x, 68, fw, 30, LT))
        body.append(text(x + fw / 2, 88, f, 13))
    # 三列研究内容
    cols = [
        ("问题一　单次观测单元", [("决策变量", "两次脉冲时刻 / 速度增量 / 观测开始时刻"),
                               ("约束条件", "60 s 连续观测 / 距离球壳与双角约束"),
                               ("求解策略", "多初值瞄准 / SLSQP 精修 / 网格逐点检验")], "两脉冲非线性规划模型"),
        ("问题二　单星序列巡游", [("离散层", "访问顺序 / 观测日期 / 漂移代价估计"),
                               ("连续层", "全计划脉冲联合优化 / 序列凸近似"),
                               ("外层调整", "顺序与观测时刻调整 / 邻域搜索")], "排序—优化双层模型"),
        ("问题三　双星选轨与分工", [("选轨", "R2 初始轨道设计 / 偏心率与半长轴约束"),
                                 ("分工", "按相位划分目标 / 两星单向扫描"),
                                 ("联合优化", "初始状态与脉冲联合优化 / δa 分析")], "选轨—分工—巡游模型"),
    ]
    cg, cy0, chh = 18, 150, 300
    cw = (W - 40 - 2 * cg) / 3
    for i, (title, items, method) in enumerate(cols):
        x = 20 + i * (cw + cg)
        body.append(rect(x, cy0, cw, chh, "#FFFFFF", "#555555", 1.0, "5,3"))
        body.append(rect(x + 10, cy0 + 10, cw - 20, 32, DK))
        body.append(text(x + cw / 2, cy0 + 31, title, 14, color="#FFFFFF"))
        for j, (k, v) in enumerate(items):
            yy = cy0 + 54 + j * 64
            body.append(rect(x + 10, yy, cw - 20, 54, LT))
            body.append(text(x + 20, yy + 22, k, 12.5, "start", DK))
            body.append(text(x + 20, yy + 42, v, 12, "start", "#333333"))
        body.append(rect(x + 34, cy0 + chh - 46, cw - 68, 30, DK, rx=4))
        body.append(text(x + cw / 2, cy0 + chh - 26, method, 13.5, color="#FFFFFF"))
        if i < 2:
            ax = x + cw + 1
            body.append(poly([(ax + 2, cy0 + 132), (ax + cg - 2, cy0 + 142), (ax + 2, cy0 + 152)], fill="#9FBCD6", stroke="none"))
    # 顶部到三列
    for i in range(3):
        down(20 + i * (cw + cg) + cw / 2, 112, cy0 - 4, "问题驱动" if i == 0 else None)
    # 支撑层
    by0 = cy0 + chh + 44
    bw = (W - 40 - 20) / 2
    blocks = [("统一基础模型", ["二体传播与脉冲状态转移", "时变可观测区域：双圆锥与球壳", "两级字典序目标"]),
              ("模型检验与分析", ["精确传播与 0.05 s 网格认证", "ATK 独立复核", "安全裕量、偏心率与 δa 灵敏度分析"])]
    for i, (title, rows) in enumerate(blocks):
        x = 20 + i * (bw + 20)
        body.append(rect(x, by0, bw, 160, "#FFFFFF", "#555555", 1.0, "5,3"))
        body.append(rect(x + 10, by0 + 10, bw - 20, 30, DK))
        body.append(text(x + bw / 2, by0 + 30, title, 14, color="#FFFFFF"))
        for j, r in enumerate(rows):
            yy = by0 + 50 + j * 35
            body.append(rect(x + 10, yy, bw - 20, 28, LB, "#C9D6DF", 0.6))
            body.append(text(x + 22, yy + 19, r, 12.5, "start"))
    down(20 + cw / 2 + 80, cy0 + chh + 4, by0 - 4, "支撑 / 约束")
    down(W - 20 - cw / 2 - 80, cy0 + chh + 4, by0 - 4, "检验 / 评价")
    save("fig3_framework.svg", W, by0 + 175, body)


# ---------------- 图 4：观测单元与全局时间轴 ----------------
DEFS3D = """<defs>
<linearGradient id="gPanel" x1="0" y1="0" x2="1" y2="1"><stop offset="0" stop-color="#5B8FD6"/><stop offset="1" stop-color="#14356B"/></linearGradient>
<linearGradient id="gRedF" x1="0" y1="0" x2="1" y2="1"><stop offset="0" stop-color="#FF6B6B"/><stop offset="1" stop-color="#B01818"/></linearGradient>
<linearGradient id="gRedT" x1="0" y1="0" x2="0" y2="1"><stop offset="0" stop-color="#FFB3B3"/><stop offset="1" stop-color="#E04848"/></linearGradient>
<linearGradient id="gRedS" x1="0" y1="0" x2="1" y2="0"><stop offset="0" stop-color="#8E1010"/><stop offset="1" stop-color="#5E0A0A"/></linearGradient>
<linearGradient id="gBluF" x1="0" y1="0" x2="1" y2="1"><stop offset="0" stop-color="#6FA8E8"/><stop offset="1" stop-color="#1D4E8F"/></linearGradient>
<linearGradient id="gBluT" x1="0" y1="0" x2="0" y2="1"><stop offset="0" stop-color="#CFE3FA"/><stop offset="1" stop-color="#7FB0E6"/></linearGradient>
<linearGradient id="gBluS" x1="0" y1="0" x2="1" y2="0"><stop offset="0" stop-color="#163C70"/><stop offset="1" stop-color="#0E284D"/></linearGradient>
<radialGradient id="gFlame" cx="0.5" cy="0.25" r="0.75"><stop offset="0" stop-color="#FFFBE0"/><stop offset="0.35" stop-color="#FFD23F"/><stop offset="0.75" stop-color="#FF7A1A"/><stop offset="1" stop-color="#D93A0B"/></radialGradient>
<radialGradient id="gBall" cx="0.35" cy="0.3" r="0.75"><stop offset="0" stop-color="#FFFFFF"/><stop offset="0.35" stop-color="#7FD3C6"/><stop offset="1" stop-color="#1E5B56"/></radialGradient>
<linearGradient id="gBeam" x1="0" y1="0" x2="1" y2="0"><stop offset="0" stop-color="#FFD23F" stop-opacity="0.75"/><stop offset="1" stop-color="#FFD23F" stop-opacity="0.08"/></linearGradient>
<linearGradient id="gUnit" x1="0" y1="0" x2="0" y2="1"><stop offset="0" stop-color="#FFFFFF"/><stop offset="1" stop-color="#E8F1F0"/></linearGradient>
<linearGradient id="gObs" x1="0" y1="0" x2="0" y2="1"><stop offset="0" stop-color="#FFE48A"/><stop offset="1" stop-color="#F2A515"/></linearGradient>
<linearGradient id="gArrow" x1="0" y1="0" x2="0" y2="1"><stop offset="0" stop-color="#4A90D9"/><stop offset="1" stop-color="#1F4E79"/></linearGradient>
</defs>"""


def sat3d(x, y, kind="red", s=1.0):
    """立体卫星：等轴测立方体本体（正面、顶面、侧面三种明暗）+ 两侧带分格的太阳翼 + 抛物面天线。"""
    F, T_, S = ("url(#gRedF)", "url(#gRedT)", "url(#gRedS)") if kind == "red" else ("url(#gBluF)", "url(#gBluT)", "url(#gBluS)")
    g = [f'<g transform="translate({x:.1f} {y:.1f}) scale({s})">']
    # 太阳翼（平行四边形，带分格）
    for sx in (-1, 1):
        x0 = 13 if sx > 0 else -47
        pts = [(x0, -3), (x0 + 34, -3), (x0 + 30, 9), (x0 - 4, 9)]
        g.append('<polygon points="' + " ".join(f"{a},{b}" for a, b in pts) + '" fill="url(#gPanel)" stroke="#0B1F3F" stroke-width="0.8"/>')
        for k in (1, 2, 3):
            xa = x0 + 34 * k / 4
            g.append(f'<line x1="{xa:.1f}" y1="-3" x2="{xa - 4:.1f}" y2="9" stroke="#A9C6EE" stroke-width="0.5"/>')
        g.append(f'<line x1="{x0 - 2:.1f}" y1="3" x2="{x0 + 32:.1f}" y2="3" stroke="#A9C6EE" stroke-width="0.5"/>')
        xb = (9, 13) if sx > 0 else (-13, -9)
        g.append(f'<line x1="{xb[0]}" y1="3" x2="{xb[1]}" y2="3" stroke="#333333" stroke-width="1.6"/>')
    # 本体：顶面、侧面、正面
    g.append('<polygon points="-9,-6 -3,-12 15,-12 9,-6" fill="' + T_ + '" stroke="#3A0A0A" stroke-width="0.6"/>')
    g.append('<polygon points="9,-6 15,-12 15,6 9,12" fill="' + S + '" stroke="#3A0A0A" stroke-width="0.6"/>')
    g.append('<rect x="-9" y="-6" width="18" height="18" fill="' + F + '" stroke="#3A0A0A" stroke-width="0.6"/>')
    g.append('<rect x="-6" y="-3" width="5" height="5" fill="#FFFFFF" fill-opacity="0.35"/>')
    # 天线
    g.append('<line x1="3" y1="-12" x2="3" y2="-17" stroke="#333333" stroke-width="1"/>')
    g.append('<polygon points="-3,-18 9,-18 6,-22 0,-22" fill="#D9D9D9" stroke="#555555" stroke-width="0.6"/>')
    g.append("</g>")
    return "\n".join(g)


def flame(x, y, ang, L=26, Wd=11):
    """火焰：朝 ang 方向（度，数学角）喷出的水滴形，渐变填充。"""
    t = np.radians(ang)
    ux, uy = np.cos(t), -np.sin(t)
    px, py = -uy, ux
    pts = []
    for k in np.linspace(0, np.pi, 24):
        r = Wd / 2 * np.sin(k) * (1 - 0.15 * k)
        d = L * (1 - np.cos(k)) / 2
        pts.append((x + ux * d + px * r, y + uy * d + py * r))
    for k in np.linspace(np.pi, 0, 24):
        r = Wd / 2 * np.sin(k) * (1 - 0.15 * k)
        d = L * (1 - np.cos(k)) / 2
        pts.append((x + ux * d - px * r, y + uy * d - py * r))
    return poly(pts, fill="url(#gFlame)", stroke="#D93A0B", w=0.5)


def fat_arrow(x1, y1, x2, y2, w=7, head=14, fill="url(#gArrow)"):
    ang = np.arctan2(y2 - y1, x2 - x1)
    ux, uy = np.cos(ang), np.sin(ang)
    px, py = -uy, ux
    L = np.hypot(x2 - x1, y2 - y1) - head
    bx, by = x1 + ux * L, y1 + uy * L
    pts = [(x1 + px * w / 2, y1 + py * w / 2), (bx + px * w / 2, by + py * w / 2), (bx + px * w * 1.3, by + py * w * 1.3),
           (x2, y2), (bx - px * w * 1.3, by - py * w * 1.3), (bx - px * w / 2, by - py * w / 2), (x1 - px * w / 2, y1 - py * w / 2)]
    return poly(pts, fill=fill, stroke="#163A5C", w=0.6)


def fig4():
    W, H = 1000, 470
    body = [DEFS3D]
    DK = "#1E5B56"
    ya = 300                      # 时间轴
    # 两个观测单元的背景卡片（带阴影）
    units = [(30, 470, "观测单元 k"), (520, 975, "观测单元 k+1")]
    for x0, x1, lab in units:
        body.append(f'<rect x="{x0 + 5}" y="{48 + 5}" width="{x1 - x0}" height="{ya - 18}" rx="10" ry="10" fill="#000000" fill-opacity="0.10"/>')
        body.append(f'<rect x="{x0}" y="48" width="{x1 - x0}" height="{ya - 18}" rx="10" ry="10" fill="url(#gUnit)" stroke="#9DB7B4" stroke-width="1"/>')
        body.append(f'<rect x="{x0}" y="48" width="{x1 - x0}" height="30" rx="10" ry="10" fill="{DK}"/>')
        body.append(f'<rect x="{x0}" y="66" width="{x1 - x0}" height="12" fill="{DK}"/>')
        body.append(text((x0 + x1) / 2, 69, lab, 15, color="#FFFFFF"))
    # 时间轴
    body.append(line(20, ya, W - 30, ya, "#333333", 2.2))
    body.append(poly([(W - 30, ya - 6), (W - 16, ya), (W - 30, ya + 6)], fill="#333333", stroke="#333333", w=1))
    body.append(text(W - 14, ya + 24, "t", 15, "middle", italic=True))

    def tick(x, lab):
        body.append(line(x, ya - 6, x, ya + 6, "#333333", 1.6))
        body.append(f'<text x="{x:.1f}" y="{ya + 24:.1f}" font-family="{FONT}" font-size="14" text-anchor="middle">{lab}</text>')

    def burn(x, y, lab, ang=200):
        body.append(line(x, y + 16, x, ya - 6, "#7F7F7F", 1.0, "3,3"))
        body.append(flame(x - 6, y + 10, 215, L=34, Wd=13))
        body.append(sat3d(x, y, "red", 1.0))
        body.append(fat_arrow(x + 4, y - 26, x + 52, y - 50))
        body.append(text(x + 58, y - 52, lab, 14, "start"))

    def observe(x0, x1, ys, tgt, lab, sc=0.8):
        body.append(f'<rect x="{x0 + 2}" y="{ya - 11 + 2}" width="{x1 - x0}" height="22" rx="4" ry="4" fill="#000000" fill-opacity="0.15"/>')
        body.append(f'<rect x="{x0}" y="{ya - 11}" width="{x1 - x0}" height="22" rx="4" ry="4" fill="url(#gObs)" stroke="#B5770A" stroke-width="0.8"/>')
        xm = (x0 + x1) / 2
        body.append(text(xm, ya + 4.5, "τ", 13, italic=True))
        # 观测卫星 R 与目标 B，光束
        rx_, ry_ = xm - 24, ys + 52
        bx, by = xm + 30, ys
        body.append(poly([(rx_ + 6, ry_ - 8), (bx - 6, by + 2), (bx + 8, by + 12), (rx_ + 10, ry_ - 2)], fill="url(#gBeam)", stroke="none"))
        body.append(sat3d(rx_, ry_, "red", sc))
        body.append(sat3d(bx, by, "blue", sc))
        body.append(text(bx + 4, by - 22 * sc - 6, tgt, 13, "middle", "#1D4E8F"))
        body.append(line(xm, ry_ + 16, xm, ya - 11, "#B5770A", 1.0, "3,3"))
        body.append(text(xm, ya + 46, lab, 12.5, "middle", "#8A5A00"))

    # 单元 k
    tick(62, "t<tspan baseline-shift=\"sub\" font-size=\"10\">0</tspan>")
    body.append(f'<circle cx="62" cy="{ya}" r="11" fill="url(#gBall)" stroke="{DK}" stroke-width="0.8"/>')
    body.append(f'<text x="62" y="{ya - 18}" font-family="{FONT}" font-size="14" text-anchor="middle" font-style="italic">x<tspan baseline-shift="sub" font-size="10">0</tspan></text>')
    burn(140, 190, "Δv<tspan baseline-shift=\"sub\" font-size=\"10\">1</tspan>")
    tick(140, "t<tspan baseline-shift=\"sub\" font-size=\"10\">1</tspan>")
    burn(275, 190, "Δv<tspan baseline-shift=\"sub\" font-size=\"10\">2</tspan>")
    tick(275, "t<tspan baseline-shift=\"sub\" font-size=\"10\">2</tspan>")
    # 间隔标注
    yb = 248
    body.append(line(140, yb, 275, yb, DK, 1.1))
    for xx in (140, 275):
        body.append(line(xx, yb - 5, xx, yb + 5, DK, 1.1))
    body.append(f'<rect x="{207 - 42}" y="{yb - 10}" width="84" height="20" rx="3" ry="3" fill="#FFFFFF"/>')
    body.append(text(207, yb + 5, "≥ 3600 s", 13, color=DK))
    # 自由飞行
    body.append(text(338, ya - 14, "自由飞行", 12.5, color="#555555"))
    observe(375, 445, 118, "B", "[s, s+τ]", 0.75)
    # 交接箭头：单元 k 的观测结束状态 → 单元 k+1 的 x0
    pts = [(445, ya - 12)] + [(445 + (565 - 445) * u, ya - 12 - 70 * np.sin(np.pi * u)) for u in np.linspace(0, 1, 30)][1:]
    body.append(poly(pts, stroke="#1F4E79", w=2.2, closed=False, dash="7,4"))
    body.append(poly([(565, ya - 12), (553, ya - 26), (568, ya - 26)], fill="#1F4E79", stroke="#1F4E79", w=0.8))
    body.append(f'<circle cx="565" cy="{ya}" r="11" fill="url(#gBall)" stroke="{DK}" stroke-width="0.8"/>')
    body.append(f'<rect x="{505 - 70}" y="{ya - 110}" width="140" height="40" rx="5" ry="5" fill="#FFFFFF" stroke="#1F4E79" stroke-width="0.8"/>')
    body.append(text(505, ya - 94, "观测结束状态", 12.5, color="#1F4E79"))
    body.append(f'<text x="505" y="{ya - 77}" font-family="{FONT}" font-size="12.5" text-anchor="middle" fill="#1F4E79">作为下一单元的 x<tspan baseline-shift="sub" font-size="9">0</tspan></text>')
    # 单元 k+1：两次脉冲后，同一次飞越连续观测两颗目标
    burn(605, 190, "Δv<tspan baseline-shift=\"sub\" font-size=\"10\">3</tspan>")
    tick(605, "t<tspan baseline-shift=\"sub\" font-size=\"10\">3</tspan>")
    burn(712, 190, "Δv<tspan baseline-shift=\"sub\" font-size=\"10\">4</tspan>")
    tick(712, "t<tspan baseline-shift=\"sub\" font-size=\"10\">4</tspan>")
    observe(790, 850, 118, "B′", "观测 B′", 0.75)
    observe(892, 952, 118, "B″", "观测 B″", 0.75)
    body.append(f'<rect x="{871 - 90}" y="{ya - 60}" width="180" height="22" rx="4" ry="4" fill="#FFFFFF" stroke="#B5770A" stroke-width="0.8"/>')
    body.append(text(871, ya - 44, "同一次飞越，中间不再机动", 12, color="#8A5A00"))
    # 底部：全局时间轴
    yg = 410
    body.append(text(W / 2, yg - 24, "全部脉冲排在同一条时间轴上：相邻脉冲间隔 ≥ 3600 s，累计 Δv ≤ 300 m/s，全部观测在任务时长 T 内完成", 13))
    body.append(line(40, yg, W - 40, yg, DK, 1.6))
    for xx in (40, W - 40):
        body.append(line(xx, yg - 7, xx, yg + 7, DK, 1.6))
    body.append(text(40, yg + 22, "0", 13))
    body.append(text(W - 40, yg + 22, "T", 13, italic=True))
    for xx in np.r_[np.linspace(90, 920, 16)]:
        body.append(poly([(xx, yg - 2), (xx - 5, yg - 12), (xx + 5, yg - 12)], fill="#E04848", stroke="#8E1010", w=0.5))
    for xx in np.linspace(130, 940, 10):
        body.append(f'<rect x="{xx - 6:.1f}" y="{yg + 3}" width="12" height="7" fill="url(#gObs)" stroke="#B5770A" stroke-width="0.5"/>')
    lx = W - 300
    body.append(poly([(lx, yg + 38), (lx - 5, yg + 28), (lx + 5, yg + 28)], fill="#E04848", stroke="#8E1010", w=0.5))
    body.append(text(lx + 12, yg + 38, "脉冲", 12, "start"))
    body.append(f'<rect x="{lx + 60}" y="{yg + 28}" width="14" height="9" fill="url(#gObs)" stroke="#B5770A" stroke-width="0.5"/>')
    body.append(text(lx + 80, yg + 38, "观测", 12, "start"))
    save("fig4_unit.svg", W, H, body)


# ---------------- 图 5：问题一观测过程 ----------------
def q1_series(t):
    R1, TG = read_states(str(REPO / "data" / "数据.xlsx"))
    x, b = np.array(R1), np.array(TG["b1"])
    sol = json.load(open(REPO / "solutions" / "q1" / "p1_claude_v4.json", encoding="utf-8"))
    out = []
    for tt in t:
        r, v, tc = x[:3], x[3:], 0.0
        for m in sol["maneuvers"]:
            if m["t_s"] <= tt:
                R_, V_ = kepler(r, v, m["t_s"] - tc)
                r, v, tc = R_[0], V_[0] + np.array(m["dv_km_s"]), m["t_s"]
        out.append(kepler(r, v, tt - tc)[0][0])
    ro = np.array(out)
    rb = kepler(b[:3], b[3:], t)[0]
    rho = ro - rb
    d = np.linalg.norm(rho, axis=1)
    us = SUN(t) - rb
    us /= np.linalg.norm(us, axis=1)[:, None]
    ue = -rb / np.linalg.norm(rb, axis=1)[:, None]
    al = np.degrees(np.arccos(np.clip(np.sum(us * rho, 1) / d, -1, 1)))
    th = np.degrees(np.arccos(np.clip(np.sum(ue * rho, 1) / d, -1, 1)))
    return d, al, th, sol["obs_start_s"]


def frame(body, L, T, W_, H_, xt, yt, X, Y, xlab, ylab, yfmt=str):
    body.append(f'<rect x="{L}" y="{T}" width="{W_}" height="{H_}" fill="none" stroke="#000000" stroke-width="1.1"/>')
    for v in yt:
        body.append(line(L, Y(v), L + W_, Y(v), "#E6E6E6", 0.7))
        body.append(line(L, Y(v), L + 6, Y(v)))
        body.append(text(L - 7, Y(v) + 4, yfmt(v), 12, "end"))
    for v in xt:
        body.append(line(X(v), T + H_, X(v), T + H_ - 6))
        body.append(text(X(v), T + H_ + 18, str(v).replace("-", "−"), 12))
    body.append(text(L + W_ / 2, T + H_ + 40, xlab, 13))
    body.append(text(L - 44, T + H_ / 2, ylab, 13, rot=-90))


def fig5():
    # (a) 观测前后 ±15 min 的相对距离（对数坐标）
    _, _, _, s0 = q1_series(np.array([0.0]))
    ta = np.arange(-900, 901, 5.0)
    d, al, th, s0 = q1_series(s0 + ta)
    W, H = 640, 360
    L, T, W_, H_ = 80, 40, 520, 250
    lo, hi = np.log10(5), np.log10(300)
    X = lambda v: L + (v + 15) / 30 * W_  # noqa: E731  横轴单位 min
    Y = lambda v: T + (hi - np.log10(v)) / (hi - lo) * H_  # noqa: E731
    body = []
    body.append(f'<rect x="{L}" y="{Y(50):.1f}" width="{W_}" height="{Y(10) - Y(50):.1f}" fill="#DCEAF7"/>')
    body.append(text(L + 8, Y(50) + 16, "10 ~ 50 km", 12, "start", "#1F4E79"))
    body.append(f'<rect x="{X(0):.1f}" y="{T}" width="{X(1) - X(0):.1f}" height="{H_}" fill="#F2A515" fill-opacity="0.35"/>')
    frame(body, L, T, W_, H_, range(-15, 16, 5), [5, 10, 20, 50, 100, 200], X, Y, "相对观测开始时刻 / min", "距离 d / km")
    m = (d >= 5) & (d <= 300)
    body.append(poly([(X(u / 60), Y(v)) for u, v, k in zip(ta, d, m) if k], stroke="#1F4E79", w=2.2, closed=False))
    body.append(text(X(0.5), T - 8, "观测 60 s", 12, "middle", "#B5770A"))
    body.append(arrow(X(0.5), T - 4, X(0.5), T + 10, "#B5770A", 1.0, 6))
    save("fig5a_q1_range.svg", W, H, body)

    # (b) 观测前后 ±3 min 的光照角、侦测角与距离
    tb = np.arange(-180, 241, 1.0)
    d, al, th, _ = q1_series(s0 + tb)
    L, T, W_, H_ = 80, 40, 500, 250
    X = lambda v: L + (v + 180) / 420 * W_  # noqa: E731
    Y = lambda v: T + (120 - v) / 120 * H_  # noqa: E731  左轴 0°–120°
    Y2 = lambda v: T + (60 - v) / 60 * H_  # noqa: E731  右轴 0–60 km
    body = []
    body.append(f'<rect x="{X(0):.1f}" y="{T}" width="{X(60) - X(0):.1f}" height="{H_}" fill="#F2A515" fill-opacity="0.30"/>')
    frame(body, L, T, W_, H_, range(-180, 241, 60), range(0, 121, 30), X, Y, "相对观测开始时刻 / s", "角度 / (°)")
    for v in range(0, 61, 15):
        body.append(line(L + W_, Y2(v), L + W_ - 6, Y2(v)))
        body.append(text(L + W_ + 7, Y2(v) + 4, str(v), 12, "start"))
    body.append(text(L + W_ + 44, T + H_ / 2, "距离 d / km", 13, rot=90))
    body.append(line(L, Y(45), L + W_, Y(45), "#C0392B", 1.2, "7,4"))
    body.append(text(L + 6, Y(45) - 6, "45°", 12, "start", "#C0392B"))
    clip = lambda arr, lo_, hi_: [(X(u), Y(min(max(v, lo_), hi_))) for u, v in zip(tb, arr)]  # noqa: E731
    body.append(poly(clip(al, 0, 120), stroke="#D9822B", w=2.0, closed=False))
    body.append(poly(clip(th, 0, 120), stroke="#2E7D32", w=2.0, closed=False, dash="8,3"))
    body.append(poly([(X(u), Y2(min(v, 60))) for u, v in zip(tb, d)], stroke="#1F4E79", w=2.0, closed=False))
    body.append(line(L, Y2(10), L + W_, Y2(10), "#1F4E79", 0.9, "3,3"))
    body.append(text(L + W_ - 6, Y2(10) - 5, "10 km", 11, "end", "#1F4E79"))
    lx, ly = L + 20, T + H_ + 62
    for k, (lab, col, dash) in enumerate((("光照角 α", "#D9822B", None), ("侦测角 θ", "#2E7D32", "8,3"), ("距离 d", "#1F4E79", None))):
        xx = lx + k * 150
        body.append(line(xx, ly, xx + 30, ly, col, 2.0, dash))
        body.append(text(xx + 36, ly + 4, lab, 12, "start"))
    body.append(f'<rect x="{lx + 450}" y="{ly - 7}" width="24" height="14" fill="#F2A515" fill-opacity="0.30"/>')
    body.append(text(lx + 480, ly + 4, "观测时段", 12, "start"))
    save("fig5b_q1_angles.svg", 660, 380, body)


# ---------------- 图 6：候选观测时刻（轨道面穿越） ----------------
def q1_plane_series(t):
    import sys as _s
    _s.path.insert(0, str(REPO / "code" / "p1"))
    import solver as S
    P = S.Problem(str(REPO / "data" / "数据.xlsx"))
    rR, _ = P.traj([], [], t)
    rB, _ = P.target(t)
    hB = np.cross(P.B1[:3], P.B1[3:])
    hB /= np.linalg.norm(hB)
    g = rR @ hB
    us = P.sun(t) - rB
    us /= np.linalg.norm(us, axis=1)[:, None]
    ue = -rB / np.linalg.norm(rB, axis=1)[:, None]
    beta = np.degrees(np.arccos(np.clip(np.sum(us * ue, 1), -1, 1)))
    return g, beta


def fig6():
    # (a) 示意图：两个轨道面、交线、两个交点，太阳在右侧
    W, H = 880, 420
    cx, cy, rx = 450, 215, 255
    body = [DEFS3D, """<defs>
<radialGradient id="gEarth6" cx="0.38" cy="0.3" r="0.75">
<stop offset="0" stop-color="#8EC9F2"/><stop offset="0.55" stop-color="#2E6FB7"/><stop offset="1" stop-color="#0D2E5C"/>
</radialGradient>
<radialGradient id="gSun6" cx="0.4" cy="0.38" r="0.7">
<stop offset="0" stop-color="#FFF7C2"/><stop offset="0.5" stop-color="#FFC531"/><stop offset="1" stop-color="#F08A12"/>
</radialGradient>
<linearGradient id="gPlaneR" x1="0" y1="0" x2="0" y2="1"><stop offset="0" stop-color="#F4B6B6" stop-opacity="0.45"/><stop offset="1" stop-color="#E06060" stop-opacity="0.18"/></linearGradient>
<linearGradient id="gPlaneB" x1="0" y1="0" x2="0" y2="1"><stop offset="0" stop-color="#B8D4F2" stop-opacity="0.55"/><stop offset="1" stop-color="#5B8FD6" stop-opacity="0.20"/></linearGradient>
</defs>"""]

    def ell(ry, a0, a1, n=120):
        return [(cx + rx * np.cos(t), cy + ry * np.sin(t)) for t in np.linspace(a0, a1, n)]

    ryB, ryR = 50, 108
    # 轨道面（半透明填充）
    body.append(poly(ell(ryB, 0, 2 * np.pi), fill="url(#gPlaneB)", stroke="none"))
    body.append(poly(ell(ryR, 0, 2 * np.pi), fill="url(#gPlaneR)", stroke="none"))
    # 后半圈（被地球挡住的一侧）虚线，前半圈实线
    for ry, col in ((ryB, "#1D4E8F"), (ryR, "#B01818")):
        body.append(poly(ell(ry, np.pi, 2 * np.pi), stroke=col, w=1.3, closed=False, dash="5,4"))
    # 地球
    body.append(f'<circle cx="{cx}" cy="{cy}" r="58" fill="url(#gEarth6)" stroke="#0D2E5C" stroke-width="0.8"/>')
    for ry, col in ((ryB, "#1D4E8F"), (ryR, "#B01818")):
        body.append(poly(ell(ry, 0, np.pi), stroke=col, w=2.0, closed=False))
    # 交线
    body.append(line(cx - rx - 30, cy, cx - 62, cy, "#000000", 1.1, "7,4"))
    body.append(line(cx + 62, cy, cx + rx + 30, cy, "#000000", 1.1, "7,4"))
    body.append(text(cx + rx + 34, cy - 6, "交线", 13, "start"))
    # 太阳在右侧
    sx, sy = W - 40, 70
    for k in range(12):
        t = 2 * np.pi * k / 12
        body.append(line(sx + 27 * np.cos(t), sy + 27 * np.sin(t), sx + 36 * np.cos(t), sy + 36 * np.sin(t), "#F2A115", 2.0))
    body.append(f'<circle cx="{sx}" cy="{sy}" r="22" fill="url(#gSun6)" stroke="#E07B00" stroke-width="0.8"/>')
    body.append(text(sx, sy + 56, "太阳", 13))
    body.append(arrow(sx - 44, sy + 10, cx + rx - 10, cy - 110, "#C9A227", 1.4))
    # 轨道面标注
    body.append(text(cx - 150, cy - ryR - 10, "R1 轨道面", 13, "middle", "#B01818"))
    body.append(text(cx - 40, cy + ryB + 34, "目标轨道面", 13, "middle", "#1D4E8F"))
    # 左交点（背阳侧，候选）：R1 即将穿越，画出法向距离 g
    nx = cx - rx
    tR = np.pi - 0.42                       # R1 在前半圈、离交点不远处
    rxp, ryp = cx + rx * np.cos(tR), cy + ryR * np.sin(tR)
    yB = cy + ryB * np.sin(tR)                # 同一 x 处目标轨道面上的点
    body.append(line(rxp, ryp - 2, rxp, yB, "#000000", 1.4))
    for yy in (ryp - 2, yB):
        body.append(line(rxp - 5, yy, rxp + 5, yy, "#000000", 1.4))
    body.append(f'<text x="{rxp - 10:.1f}" y="{(ryp + yB) / 2 + 5:.1f}" font-family="{FONT}" font-size="14" font-style="italic" text-anchor="end">g(t)</text>')
    body.append(sat3d(rxp + 4, ryp + 22, "red", 0.75))
    body.append(text(rxp + 44, ryp + 30, "R1", 13, "start", "#B01818"))
    tB = np.pi - 0.16
    bxp, byp = cx + rx * np.cos(tB), cy + ryB * np.sin(tB)
    body.append(sat3d(bxp + 2, byp - 22, "blue", 0.75))
    body.append(text(bxp + 2, byp - 44, "目标", 13, "middle", "#1D4E8F"))
    body.append(f'<circle cx="{nx}" cy="{cy}" r="5" fill="#2E7D32" stroke="#FFFFFF" stroke-width="1.2"/>')
    body.append(line(nx, cy, nx - 60, cy - 70, "#2E7D32", 0.9))
    body.append(f'<rect x="{nx - 175}" y="{cy - 120}" width="196" height="46" rx="5" ry="5" fill="#FFFFFF" stroke="#2E7D32" stroke-width="1"/>')
    body.append(text(nx - 77, cy - 101, "背阳侧交点：β ≤ 90°", 12.5, color="#2E7D32"))
    body.append(f'<text x="{nx - 77:.1f}" y="{cy - 82:.1f}" font-family="{FONT}" font-size="12.5" text-anchor="middle" fill="#2E7D32">候选观测时刻 t<tspan baseline-shift="sub" font-size="9">c</tspan></text>')
    # 右交点（向阳侧）
    px = cx + rx
    body.append(f'<circle cx="{px}" cy="{cy}" r="5" fill="#9E9E9E" stroke="#FFFFFF" stroke-width="1.2"/>')
    body.append(line(px, cy, px + 40, cy + 70, "#9E9E9E", 0.9))
    body.append(f'<rect x="{px - 30}" y="{cy + 72}" width="150" height="46" rx="5" ry="5" fill="#FFFFFF" stroke="#9E9E9E" stroke-width="1"/>')
    body.append(text(px + 45, cy + 91, "向阳侧交点", 12.5, color="#616161"))
    body.append(text(px + 45, cy + 110, "β > 90°，不可观测", 12.5, color="#616161"))
    save("fig6a_crossing.svg", W, H, body)

    # (b) 48 h 内 g(t)，阴影为 β ≤ 90°；(c) 候选时刻 ±10 min 放大
    t = np.arange(0, 48 * 3600 + 1, 60.0)
    g, beta = q1_plane_series(t)
    h = t / 3600
    W, H = 640, 330
    L, T, W_, H_ = 80, 30, 520, 230
    X = lambda v: L + v / 48 * W_  # noqa: E731
    Y = lambda v: T + (3500 - v) / 7000 * H_  # noqa: E731
    body = []
    ok = beta <= 90
    edges = np.where(np.diff(ok.astype(int)) != 0)[0]
    segs, st = [], (0 if ok[0] else None)
    for e in edges:
        if st is None:
            st = e + 1
        else:
            segs.append((st, e))
            st = None
    if st is not None:
        segs.append((st, len(t) - 1))
    for a0, a1 in segs:
        body.append(f'<rect x="{X(h[a0]):.1f}" y="{T}" width="{X(h[a1]) - X(h[a0]):.1f}" height="{H_}" fill="#DCEAF7"/>')
    frame(body, L, T, W_, H_, range(0, 49, 6), range(-3000, 3001, 1500), X, Y, "时间 t / h", "g(t) / km",
          yfmt=lambda v: str(v).replace("-", "−"))
    body.append(line(L, Y(0), L + W_, Y(0), "#555555", 0.8))
    body.append(poly([(X(u), Y(v)) for u, v in zip(h, g)], stroke="#B01818", w=2.0, closed=False))
    for i in np.where(np.sign(g[:-1]) != np.sign(g[1:]))[0]:
        tc = h[i] - g[i] * (h[i + 1] - h[i]) / (g[i + 1] - g[i])
        good = beta[i] <= 90
        col = "#2E7D32" if good else "#9E9E9E"
        body.append(f'<circle cx="{X(tc):.1f}" cy="{Y(0):.1f}" r="5.5" fill="{col}" stroke="#FFFFFF" stroke-width="1.2"/>')
        dx = -34 if good else 34                # 标注放在曲线不经过的那一侧
        body.append(text(X(tc) + dx, Y(0) - 12, f"{tc:.2f} h", 11.5, "middle", col))
    lx, ly = L + 6, T + H_ + 62
    body.append(f'<circle cx="{lx + 6}" cy="{ly - 4}" r="5.5" fill="#2E7D32"/>')
    body.append(text(lx + 16, ly, "候选观测时刻（β ≤ 90°）", 12, "start"))
    body.append(f'<circle cx="{lx + 216}" cy="{ly - 4}" r="5.5" fill="#9E9E9E"/>')
    body.append(text(lx + 226, ly, "β > 90°，舍去", 12, "start"))
    body.append(f'<rect x="{lx + 356}" y="{ly - 11}" width="24" height="14" fill="#DCEAF7"/>')
    body.append(text(lx + 386, ly, "β ≤ 90° 时段", 12, "start"))
    save("fig6b_g48h.svg", W, H + 40, body)

    # (c) 放大 38.35 h 附近 ±10 min：红线 g(t) 落在蓝带 |g| ≤ 50 km 内的时段（橙色）才可能观测
    tc = 38.353 * 3600
    tz = np.arange(tc - 600, tc + 601, 2.0)
    gz, _ = q1_plane_series(tz)
    i0 = np.argmin(np.abs(gz))
    m = (tz - tz[i0]) / 60
    W, H = 560, 400
    L, T, W_, H_ = 80, 40, 440, 260
    X = lambda v: L + (v + 10) / 20 * W_  # noqa: E731
    Y = lambda v: T + (150 - v) / 300 * H_  # noqa: E731
    inb = np.abs(gz) <= 50
    x0, x1 = m[inb].min(), m[inb].max()
    body = [DEFS3D]
    # 蓝带：距离上限给出的必要条件
    body.append(f'<rect x="{L}" y="{Y(50):.1f}" width="{W_}" height="{Y(-50) - Y(50):.1f}" fill="#DCEAF7"/>')
    # 橙色：可能观测时间窗
    body.append(f'<rect x="{X(x0):.1f}" y="{T}" width="{X(x1) - X(x0):.1f}" height="{H_}" fill="#F2A515" fill-opacity="0.22"/>')
    # 交汇区：蓝带与时间窗重叠的矩形加深
    body.append(f'<rect x="{X(x0):.1f}" y="{Y(50):.1f}" width="{X(x1) - X(x0):.1f}" height="{Y(-50) - Y(50):.1f}" fill="#F2A515" fill-opacity="0.35" stroke="#B5770A" stroke-width="1.1" stroke-dasharray="4,3"/>')
    frame(body, L, T, W_, H_, range(-10, 11, 5), range(-150, 151, 50), X, Y, "相对穿越时刻 / min", "R1 到目标轨道面的距离 g / km",
          yfmt=lambda v: str(v).replace("-", "−"))
    body.append(line(L, Y(0), L + W_, Y(0), "#888888", 0.8, "2,3"))
    # 红线：带外细、带内加粗
    body.append(poly([(X(u), Y(max(min(v, 150), -150))) for u, v in zip(m, gz)], stroke="#B01818", w=1.6, closed=False))
    body.append(poly([(X(u), Y(v)) for u, v, k in zip(m, gz, inb) if k], stroke="#B01818", w=4.0, closed=False))
    body.append(text(X(4.6), Y(98), "g(t)", 14, "middle", "#B01818", italic=True))
    # 时间窗边界与宽度标注
    for xx, lab in ((x0, f"−{abs(x0) * 60:.0f} s"), (x1, f"+{x1 * 60:.0f} s")):
        body.append(line(X(xx), T, X(xx), T + H_, "#B5770A", 1.0, "4,3"))
        side = 1 if xx < 0 else -1               # 标注放在时间窗内侧
        body.append(text(X(xx) + side * 6, Y(-118), lab, 11.5, "start" if side > 0 else "end", "#8A5A00"))
    ya = T + 22
    body.append(arrow(X(0), ya, X(x0) + 2, ya, "#8A5A00", 1.1, 7))
    body.append(arrow(X(0), ya, X(x1) - 2, ya, "#8A5A00", 1.1, 7))
    body.append(text(X(0), ya - 10, f"可能观测时间窗 {(x1 - x0):.1f} min", 12, "middle", "#8A5A00"))
    # 穿越点与 R1 图标
    body.append(f'<circle cx="{X(0):.1f}" cy="{Y(0):.1f}" r="5.5" fill="#2E7D32" stroke="#FFFFFF" stroke-width="1.3"/>')
    body.append(line(X(0) + 4, Y(0) + 4, X(2.6), Y(-95), "#2E7D32", 0.9))
    body.append(f'<text x="{X(2.8):.1f}" y="{Y(-100):.1f}" font-family="{FONT}" font-size="12.5" fill="#2E7D32">穿越时刻 t<tspan baseline-shift="sub" font-size="9">c</tspan>（g = 0）</text>')
    # 两侧小图标：穿越前在目标轨道面一侧，穿越后在另一侧
    body.append(sat3d(X(-5.7), Y(-133), "red", 0.55))
    body.append(text(X(-7.1), Y(-133) + 4, "穿越前", 11, "end", "#555555"))
    body.append(sat3d(X(5.7), Y(133), "red", 0.55))
    body.append(text(X(7.1), Y(133) + 4, "穿越后", 11, "start", "#555555"))
    # 带的标注（放在带内左侧，不压线）
    body.append(text(L + 8, Y(50) + 16, "|g| ≤ 50 km", 12, "start", "#1F4E79"))
    # 图例
    lx, ly = L, T + H_ + 62
    body.append(line(lx, ly - 4, lx + 26, ly - 4, "#B01818", 4.0))
    body.append(text(lx + 32, ly, "g(t)，带内加粗", 12, "start"))
    body.append(f'<rect x="{lx + 150}" y="{ly - 11}" width="24" height="14" fill="#DCEAF7"/>')
    body.append(text(lx + 180, ly, "距离上限 |g| ≤ 50 km", 12, "start"))
    body.append(f'<rect x="{lx + 330}" y="{ly - 11}" width="24" height="14" fill="#F2A515" fill-opacity="0.35" stroke="#B5770A" stroke-width="0.8" stroke-dasharray="3,2"/>')
    body.append(text(lx + 360, ly, "可能观测区", 12, "start"))
    save("fig6c_gzoom.svg", W, H + 40, body)


# ---------------- 图 8：问题一的抽象——穿过时变可观测管道 ----------------
def fig_tube():
    W, H = 1000, 600
    # 斜二测投影：世界坐标 (t, x, y)，t 为时间轴（向右上），x 为相对位置的水平分量（向右下），y 为竖直分量
    O = np.array([170.0, 400.0])
    et, ex, ey = np.array([1.0, -0.26]), np.array([0.55, 0.42]), np.array([0.0, -1.0])

    def P(t, x, y):
        q = O + t * et + x * ex + y * ey
        return float(q[0]), float(q[1])

    body = [DEFS3D, """<defs>
<linearGradient id="gTube" x1="0" y1="0" x2="0" y2="1"><stop offset="0" stop-color="#9FD3F0" stop-opacity="0.55"/><stop offset="1" stop-color="#2E6FB7" stop-opacity="0.35"/></linearGradient>
<linearGradient id="gTubeHi" x1="0" y1="0" x2="0" y2="1"><stop offset="0" stop-color="#FFE48A" stop-opacity="0.85"/><stop offset="1" stop-color="#F2A515" stop-opacity="0.65"/></linearGradient>
<radialGradient id="gStar" cx="0.4" cy="0.35" r="0.7"><stop offset="0" stop-color="#FFFFFF"/><stop offset="0.4" stop-color="#7BE07B"/><stop offset="1" stop-color="#1B7A1B"/></radialGradient>
</defs>"""]
    r_in, r_out, half = 32.0, 105.0, np.radians(40)
    phi = lambda t: np.radians(120) + 0.0010 * (t - 380)  # noqa: E731  管道截面朝向随时间缓慢转动

    def section(t):
        ang = np.linspace(phi(t) - half, phi(t) + half, 28)
        outer = [P(t, r_out * np.cos(a), r_out * np.sin(a)) for a in ang]
        inner = [P(t, r_in * np.cos(a), r_in * np.sin(a)) for a in ang[::-1]]
        return outer + inner

    t0, t1, tc0, tc1 = 0.0, 620.0, 345.0, 415.0
    ts = np.r_[0, 110, 220, 345, 380, 415, 520, 620]
    # 时间轴（目标所在的轴线）
    body.append(line(*P(-40, 0, 0), *P(t1 + 90, 0, 0), "#333333", 1.4))
    body.append(poly([P(t1 + 90, 0, 0), (P(t1 + 90, 0, 0)[0] - 12, P(t1 + 90, 0, 0)[1] - 1), (P(t1 + 90, 0, 0)[0] - 10, P(t1 + 90, 0, 0)[1] + 7)], fill="#333333", stroke="#333333", w=1))
    tx, ty = P(t1 + 100, 0, 0)
    body.append(text(tx + 4, ty + 4, "t", 15, "start", italic=True))
    # 管道四条棱
    for a_k, r_k in ((-1, r_in), (-1, r_out), (1, r_in), (1, r_out)):
        pts = [P(t, r_k * np.cos(phi(t) + a_k * half), r_k * np.sin(phi(t) + a_k * half)) for t in np.linspace(t0, t1, 40)]
        body.append(poly(pts, stroke="#2E6FB7", w=0.9, closed=False, dash="5,3"))
    # 截面（远端先画）
    for t in ts:
        hi = tc0 <= t <= tc1
        body.append(poly(section(t), fill="url(#gTubeHi)" if hi else "url(#gTube)", stroke="#B5770A" if hi else "#2E6FB7", w=1.2 if hi else 0.7))
    # 目标卫星位于轴线起点附近
    body.append(sat3d(*P(-10, 0, 0), "blue", 0.8))
    bx, by = P(-10, 0, 0)
    body.append(text(bx, by + 34, "目标 B1（原点）", 12.5, "middle", "#1D4E8F"))
    # R1 相对轨迹：从远处斜穿管道
    tt = np.linspace(120, 620, 300)
    pm = phi(380)
    rho0 = np.array([68 * np.cos(pm), 68 * np.sin(pm)])          # 观测中点：截面内的瞄准点
    vdir = np.array([np.cos(pm - np.pi / 2), np.sin(pm - np.pi / 2)])  # 相对速度方向：横穿截面

    def rel(t):
        # 观测段附近相对运动近似匀速直线（飞越），远处叠加脉冲前的较大偏离
        u = (t - 380) / 35
        q = rho0 + vdir * 34 * u + rho0 / np.linalg.norm(rho0) * max(0.0, -u - 1.2) ** 2 * 3.2
        return float(q[0]), float(q[1])
    pts = [P(t, *rel(t)) for t in tt]
    inside = [(r_in <= np.hypot(*rel(t)) <= r_out) and abs(np.arctan2(rel(t)[1], rel(t)[0]) - phi(t)) <= half for t in tt]
    body.append(poly(pts, stroke="#B01818", w=2.2, closed=False))
    seg = [p for p, k in zip(pts, inside) if k]
    if seg:
        body.append(poly(seg, stroke="#B01818", w=5.0, closed=False))
    # 定推力：两次脉冲（火焰 + 卫星）
    for tb, lab in ((150, "Δv₁"), (255, "Δv₂")):
        x_, y_ = P(tb, *rel(tb))
        body.append(flame(x_ - 6, y_ + 10, 215, L=30, Wd=12))
        body.append(sat3d(x_, y_, "red", 0.7))
        body.append(text(x_ - 6, y_ - 24, lab, 13, "end", "#B01818"))
    # 定点：候选时刻截面上的瞄准点（角平分线方向）
    tm = 380
    ax_, ay_ = P(tm, *rel(tm))
    star = []
    for k in range(10):
        rr = 9 if k % 2 == 0 else 4
        aa = np.pi / 2 + k * np.pi / 5
        star.append((ax_ + rr * np.cos(aa), ay_ - rr * np.sin(aa)))
    body.append(poly(star, fill="url(#gStar)", stroke="#1B5E20", w=0.8))
    # 候选时段在时间轴上的标记
    body.append(line(*P(tc0, 0, 0), *P(tc1, 0, 0), "#F2A515", 6))
    # 三层标注（统一放在图右侧和上方的空白处，用引线连接）
    def tag(xp, yp, lx, ly, title, sub, col):
        body.append(line(xp, yp, lx, ly, col, 0.9))
        body.append(f'<rect x="{lx - 4}" y="{ly - 30}" width="232" height="44" rx="6" ry="6" fill="#FFFFFF" stroke="{col}" stroke-width="1.2"/>')
        if title.endswith("t_c"):
            body.append(f'<text x="{lx + 6}" y="{ly - 11}" font-family="{FONT}" font-size="13.5" fill="{col}">{title[:-3]}t<tspan baseline-shift="sub" font-size="10">c</tspan></text>')
        else:
            body.append(text(lx + 6, ly - 11, title, 13.5, "start", col))
        body.append(text(lx + 6, ly + 8, sub, 11.5, "start", "#444444"))
    cxa, cya = P((tc0 + tc1) / 2, 0, 0)
    tag(cxa, cya, 640, 540, "定时：候选观测时刻 t_c", "轨道面交线附近且 β ≤ 90°", "#B5770A")
    tag(ax_ + 8, ay_ - 4, 700, 80, "定点：瞄准点", "截面角平分线方向，离边界最远", "#1B7A1B")
    x2, y2 = P(255, *rel(255))
    tag(x2 + 10, y2 + 10, 330, 560, "定推力：两次脉冲", "相位漂移估计初值，非线性规划精修", "#B01818")
    # 截面说明
    top = min((P(t1, r_out * np.cos(a), r_out * np.sin(a)) for a in np.linspace(phi(t1) - half, phi(t1) + half, 30)), key=lambda q: q[1])
    body.append(text(top[0] + 40, top[1] - 14, "管道截面 C(t)：双圆锥 ∩ 球壳", 12.5, "middle", "#1F4E79"))
    # 图例
    lx, ly = 40, 520
    body.append(f'<rect x="{lx}" y="{ly - 10}" width="26" height="14" fill="url(#gTube)" stroke="#2E6FB7" stroke-width="0.7"/>')
    body.append(text(lx + 32, ly + 2, "时变可观测管道", 12, "start"))
    body.append(f'<rect x="{lx}" y="{ly + 14}" width="26" height="14" fill="url(#gTubeHi)" stroke="#B5770A" stroke-width="0.8"/>')
    body.append(text(lx + 32, ly + 26, "候选时段内的截面", 12, "start"))
    body.append(line(lx, ly + 45, lx + 26, ly + 45, "#B01818", 2.2))
    body.append(text(lx + 32, ly + 50, "R1 相对轨迹（管道内加粗）", 12, "start"))
    save("fig8_tube.svg", W, H, body)


if __name__ == "__main__":
    fig1a()
    fig1b()
    fig2()
    fig3()
    fig4()
    fig5()
    fig6()
    fig_tube()
