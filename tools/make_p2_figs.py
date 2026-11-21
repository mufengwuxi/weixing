"""根据第二问方案生成论文配图（纯 SVG，可在 Visio 中打开编辑）。

用法：python tools/make_p2_figs.py solutions/q2/p2_claude_30.json
输出：paper/figures/p2_lon_time.svg（巡游经度–时间图）、paper/figures/p2_dv_bar.svg（各目标 Δv 分配图）

只用 line / polyline / circle / rect / text 等基本元素，不用 CSS、clipPath 和嵌入字体，
字体写成“宋体, Times New Roman”，在 Visio 里可以直接改字、改线型。
"""

import json
import sys
from pathlib import Path

import numpy as np

REPO = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO / "tools"))
from verify_p2 import kepler  # noqa: E402

FONT = "SimSun, Times New Roman"


def svg_text(x, y, s, size=12, anchor="middle", rot=None, color="#000000"):
    tr = f' transform="rotate({rot} {x:.1f} {y:.1f})"' if rot is not None else ""
    return f'<text x="{x:.1f}" y="{y:.1f}" font-family="{FONT}" font-size="{size}" text-anchor="{anchor}" fill="{color}"{tr}>{s}</text>'


def phase_track(sol):
    mu = sol["mu_km3_s2"]
    burns = sorted(sol["burns"], key=lambda b: b["t_s"])
    r1 = np.array(sol["r1_km"])
    h = np.cross(r1[:3], r1[3:])
    h /= np.linalg.norm(h)
    X = r1[:3] / np.linalg.norm(r1[:3])
    Y = np.cross(h, X)
    a0 = 1 / (2 / np.linalg.norm(r1[:3]) - r1[3:] @ r1[3:] / mu)
    n0 = np.degrees(np.sqrt(mu / a0 ** 3))

    def ph(p, t):
        return (np.degrees(np.arctan2(p @ Y, p @ X)) - n0 * t + 180) % 360 - 180

    ts = np.arange(0, 30 * 86400 + 1, 1800.0)
    out = []
    r, v, tc, k = r1[:3].copy(), r1[3:].copy(), 0.0, 0
    for T in ts:
        while k < len(burns) and burns[k]["t_s"] <= T:
            R, V = kepler(r, v, burns[k]["t_s"] - tc, mu)
            r, v, tc = R[0], V[0] + np.array(burns[k]["dv_km_s"]), burns[k]["t_s"]
            k += 1
        out.append(ph(kepler(r, v, T - tc, mu)[0][0], T))
    enc = []
    for o in sol["observations"]:
        tg = np.array(sol["targets_km"][o["target"]])
        enc.append((o["target"], o["start_s"] / 86400, ph(kepler(tg[:3], tg[3:], o["start_s"], mu)[0][0], o["start_s"])))
    return ts / 86400, np.array(out), enc


def fig_lon_time(sol, path):
    t, p, enc = phase_track(sol)
    W, H, L, R, T, B = 760, 460, 70, 30, 30, 60
    x = lambda d: L + d / 30 * (W - L - R)  # noqa: E731
    y = lambda g: T + (22 - g) / 44 * (H - T - B)  # noqa: E731
    e = [f'<svg xmlns="http://www.w3.org/2000/svg" width="{W}" height="{H}" viewBox="0 0 {W} {H}">',
         f'<rect x="0" y="0" width="{W}" height="{H}" fill="#ffffff" stroke="none"/>']
    for g in range(-20, 21, 5):
        e.append(f'<line x1="{L}" y1="{y(g):.1f}" x2="{W - R}" y2="{y(g):.1f}" stroke="#e0e0e0" stroke-width="0.8"/>')
        e.append(svg_text(L - 8, y(g) + 4, f"{g}", 11, "end"))
    for d in range(0, 31, 5):
        e.append(f'<line x1="{x(d):.1f}" y1="{T}" x2="{x(d):.1f}" y2="{H - B}" stroke="#e0e0e0" stroke-width="0.8"/>')
        e.append(svg_text(x(d), H - B + 18, f"{d}", 11))
    e.append(f'<rect x="{L}" y="{T}" width="{W - L - R}" height="{H - T - B}" fill="none" stroke="#000000" stroke-width="1"/>')
    pts = " ".join(f"{x(a):.1f},{y(b):.1f}" for a, b in zip(t, p))
    e.append(f'<polyline points="{pts}" fill="none" stroke="#1f4e79" stroke-width="1.8"/>')
    for name, d, g in enc:
        e.append(f'<circle cx="{x(d):.1f}" cy="{y(g):.1f}" r="3.6" fill="#c00000" stroke="#ffffff" stroke-width="0.8"/>')
        e.append(svg_text(x(d) + 5, y(g) - 5, name, 9, "start", color="#404040"))
    e.append(svg_text((L + W - R) / 2, H - 18, "时间 / 天", 13))
    e.append(svg_text(20, (T + H - B) / 2, "相对经度 / (°)", 13, rot=-90))
    lx = W - R - 200
    ly = H - B - 70
    e.append(f'<line x1="{lx}" y1="{ly + 18}" x2="{lx + 28}" y2="{ly + 18}" stroke="#1f4e79" stroke-width="1.8"/>')
    e.append(svg_text(lx + 34, ly + 22, "r1 漂移轨迹", 11, "start"))
    e.append(f'<circle cx="{lx + 14}" cy="{ly + 38}" r="3.6" fill="#c00000"/>')
    e.append(svg_text(lx + 34, ly + 42, "观测事件（目标编号）", 11, "start"))
    e.append("</svg>")
    path.write_text("\n".join(e), encoding="utf-8")


def fig_dv_bar(sol, path):
    by = {}
    for b in sol["burns"]:
        by[b["for"]] = by.get(b["for"], 0.0) + float(np.linalg.norm(b["dv_km_s"])) * 1000
    obs = sorted(sol["observations"], key=lambda o: o["start_s"])
    vals = [(o["target"], by.get(o["target"], 0.0)) for o in obs]
    W, H, L, R, T, B = 760, 360, 60, 20, 30, 70
    top = max(30, np.ceil(max(v for _, v in vals) / 5) * 5)
    bw = (W - L - R) / len(vals)
    y = lambda v: T + (top - v) / top * (H - T - B)  # noqa: E731
    e = [f'<svg xmlns="http://www.w3.org/2000/svg" width="{W}" height="{H}" viewBox="0 0 {W} {H}">',
         f'<rect x="0" y="0" width="{W}" height="{H}" fill="#ffffff" stroke="none"/>']
    for g in np.arange(0, top + 1, 5):
        e.append(f'<line x1="{L}" y1="{y(g):.1f}" x2="{W - R}" y2="{y(g):.1f}" stroke="#e0e0e0" stroke-width="0.8"/>')
        e.append(svg_text(L - 8, y(g) + 4, f"{g:.0f}", 11, "end"))
    for i, (n, v) in enumerate(vals):
        x0 = L + i * bw + bw * 0.15
        e.append(f'<rect x="{x0:.1f}" y="{y(v):.1f}" width="{bw * 0.7:.1f}" height="{y(0) - y(v):.1f}" fill="#5b9bd5" stroke="#1f4e79" stroke-width="0.6"/>')
        e.append(svg_text(x0 + bw * 0.35, H - B + 14, n, 9, rot=-60))
    e.append(f'<rect x="{L}" y="{T}" width="{W - L - R}" height="{H - T - B}" fill="none" stroke="#000000" stroke-width="1"/>')
    e.append(svg_text(18, (T + H - B) / 2, "该次观测机动量 / (m/s)", 12, rot=-90))
    e.append(svg_text((L + W - R) / 2, H - 8, "目标（按观测时间排序）", 12))
    e.append("</svg>")
    path.write_text("\n".join(e), encoding="utf-8")


if __name__ == "__main__":
    sol = json.loads(Path(sys.argv[1]).read_text(encoding="utf-8"))
    d = REPO / "paper" / "figures"
    d.mkdir(parents=True, exist_ok=True)
    fig_lon_time(sol, d / "p2_lon_time.svg")
    fig_dv_bar(sol, d / "p2_dv_bar.svg")
    print("已生成 paper/figures/p2_lon_time.svg、paper/figures/p2_dv_bar.svg")
