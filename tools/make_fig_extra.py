"""第 6、7、8 章补充核心结果图（纯 SVG，与第 5 章学术风格严格统一）。

输出：
  paper/figures/fig10c_q2_ablation.svg   第 6 章：问题二各阶段消融对比
  paper/figures/fig10d_q2_cumdv.svg      第 6 章：问题二 30 天累计速度增量演化
  paper/figures/fig12c_q3_split.svg      第 7 章：问题三边界目标重分配消融
  paper/figures/fig12d_q3_ablation.svg   第 7 章：问题三各阶段模型对比（堆叠）
  paper/figures/fig15a_obs_verify.svg    第 8 章：60 次观测约束边界分布检验
  paper/figures/fig15b_plane_tilt.svg    第 8 章：面外机动占比与轨道面最大倾转角
"""
import json
import sys
from pathlib import Path

import numpy as np

sys.path.insert(0, str(Path(__file__).resolve().parent))
import make_fig_analysis as F  # noqa: E402

REPO = Path(__file__).resolve().parents[1]
C_MAIN = "#1A365D"   # 深藏青
C_ACC = "#9B2C2C"    # 暗砖红
C_SUB = "#4A7EB0"    # 中浅蓝
C_GR = "#666666"     # 中性灰
C_LT = "#D9E2EC"     # 浅灰蓝


def make_q2_ablation():
    """图 (a)：问题二各求解阶段消融对比柱状图。"""
    items = [
        ("M2-A", 210.39, C_SUB),
        ("M2-B 初版", 180.76, C_SUB),
        ("M2-B 完全", 176.31, C_SUB),
        ("M2-D", 159.33, C_MAIN),
        ("本文主模型", 133.03, C_ACC),
    ]
    W, H = 620, 360
    L, T, W_, H_ = 78, 22, 512, 272
    n = len(items)
    bw = W_ / n
    Y = lambda v: T + (240 - v) / 240 * H_  # noqa: E731
    body = []
    for k, (lab, v, col) in enumerate(items):
        x0 = L + k * bw + bw * 0.22
        w_bar = bw * 0.56
        body.append(f'<rect x="{x0:.1f}" y="{Y(v):.1f}" width="{w_bar:.1f}" height="{Y(0) - Y(v):.1f}" fill="{col}"/>')
        xc = L + (k + 0.5) * bw
        body.append(F.text(xc, Y(v) - 7, f"{v:.2f}", 11.5, "middle", col if col == C_ACC else "#222222"))
        body.append(F.text(xc, T + H_ + 20, lab, 12, "middle", "#222222"))
    F.axes_box(body, L, T, W_, H_, [], range(0, 241, 40), lambda v: v, Y, None, "累计速度增量 / (m/s)")
    body.append(F.text(L + W_ / 2, T + H_ + 46, "模型与求解阶段", 13))
    F.legend_box(body, L + W_ - 196, T + 10, [
        ("rect", C_SUB, None, "基线模型"),
        ("rect", C_MAIN, None, "中间模型"),
        ("rect", C_ACC, None, "本文主模型（−36.8%）"),
    ])
    F.save("fig10c_q2_ablation.svg", W, H, body)


def make_q2_cumdv():
    """图 (b)：问题二主方案 30 天累计速度增量阶梯演化（标注三阶段）。"""
    d = json.load(open(REPO / "results/q2/m_uni_q2_best.json", encoding="utf-8"))
    burns = sorted(
        [(b["t_s"] / 86400.0, float(np.linalg.norm(b["dv_km_s"])) * 1000.0) for b in d["burns"]],
        key=lambda z: z[0],
    )
    W, H = 620, 360
    L, T, W_, H_ = 78, 22, 512, 272
    X = lambda v: L + v / 30.0 * W_  # noqa: E731
    Y = lambda v: T + (150 - v) / 150 * H_  # noqa: E731
    body = []
    # 三阶段背景浅区（西行 0-7.8d，折返 7.8-11.6d，东扫 11.6-30d）
    body.append(f'<rect x="{X(7.8):.1f}" y="{T}" width="{X(11.6) - X(7.8):.1f}" height="{H_}" fill="#F3F6FA" stroke="none"/>')
    body.append(F.line(X(7.8), T, X(7.8), T + H_, "#999999", 0.8, "3,3"))
    body.append(F.line(X(11.6), T, X(11.6), T + H_, "#999999", 0.8, "3,3"))
    body.append(F.text(X(3.9), T + 18, "西行去程", 11.5, "middle", C_GR))
    body.append(F.text(X(9.7), T + 18, "折返", 11.5, "middle", C_GR))
    body.append(F.text(X(20.8), T + 18, "东向单程巡游", 11.5, "middle", C_GR))

    # 阶梯累计曲线
    pts = [(X(0.0), Y(0.0))]
    acc = 0.0
    for t_d, dv in burns:
        if dv < 1e-3:
            continue
        pts.append((X(t_d), Y(acc)))
        acc += dv
        pts.append((X(t_d), Y(acc)))
    pts.append((X(30.0), Y(acc)))
    body.append(F.poly(pts, stroke=C_MAIN, w=1.8, closed=False))
    # 标注大脉冲（>10 m/s）
    acc = 0.0
    for t_d, dv in burns:
        if dv < 1e-3:
            continue
        acc += dv
        if dv >= 10.0:
            body.append(f'<circle cx="{X(t_d):.1f}" cy="{Y(acc):.1f}" r="3.6" fill="{C_ACC}" stroke="#FFFFFF" stroke-width="0.9"/>')
    F.axes_box(body, L, T, W_, H_, range(0, 31, 5), range(0, 151, 30), X, Y, "时间 / 天", "累计速度增量 / (m/s)")
    F.legend_box(body, L + 10, T + 34, [
        ("line", C_MAIN, None, "累计速度增量"),
        ("dot", C_ACC, None, "大脉冲节点（≥ 10 m/s）"),
    ])
    F.save("fig10d_q2_cumdv.svg", W, H, body)


def make_q3_split():
    """图 (a)：问题三边界目标重分配消融（10/20 vs 9/21 vs 11/19，R1 侧对比）。"""
    items = [
        ("10 / 20（基准）", 13.39, 11.94, C_MAIN),
        ("9 / 21（B2→R2）", 14.63, None, C_SUB),
        ("11 / 19（B4→R1）", 35.69, None, C_ACC),
    ]
    W, H = 620, 360
    L, T, W_, H_ = 78, 22, 512, 272
    n = len(items)
    bw = W_ / n
    Y = lambda v: T + (42 - v) / 42 * H_  # noqa: E731
    body = []
    # 双星总和参考线（26.07 m/s）
    body.append(F.line(L, Y(26.07), L + W_, Y(26.07), C_GR, 0.9, "4,3"))
    for k, (lab, v1, v_full, col) in enumerate(items):
        xc = L + (k + 0.5) * bw
        w_bar = bw * 0.42
        x0 = xc - w_bar / 2
        body.append(f'<rect x="{x0:.1f}" y="{Y(v1):.1f}" width="{w_bar:.1f}" height="{Y(0) - Y(v1):.1f}" fill="{col}"/>')
        lbl = f">{v1:.2f}（未收敛）" if k == 2 else f"{v1:.2f}"
        body.append(F.text(xc, Y(v1) - 8, lbl, 11.5, "middle", col if k == 2 else "#222222"))
        if v_full is not None:
            body.append(F.line(x0, Y(v_full), x0 + w_bar, Y(v_full), "#FFFFFF", 1.6, "3,2"))
        body.append(F.text(xc, T + H_ + 20, lab, 12, "middle", "#222222"))
    F.axes_box(body, L, T, W_, H_, [], range(0, 43, 10), lambda v: v, Y, None, "R1 速度增量 / (m/s)")
    body.append(F.text(L + W_ / 2, T + H_ + 46, "双星目标划分方案", 13))
    F.legend_box(body, L + 10, T + 10, [
        ("rect", C_MAIN, None, "基准 10/20（精修后 11.94 m/s）"),
        ("rect", C_ACC, None, "折返 11/19（单星超双星总和）"),
        ("line", C_GR, "4,3", "10/20 双星合计（26.07 m/s）"),
    ])
    F.save("fig12c_q3_split.svg", W, H, body)


def make_q3_ablation():
    """图 (b)：问题三各阶段模型对比（R1 + R2 堆叠柱状图）。"""
    items = [
        ("M3-A", 18.94, 16.91),
        ("M3-B", 18.148, 15.956),
        ("M3-C", 18.148, 15.312),
        ("M3-D", 13.499, 14.615),
        ("本文主模型", 11.9435, 14.1231),
    ]
    W, H = 620, 360
    L, T, W_, H_ = 78, 22, 512, 272
    n = len(items)
    bw = W_ / n
    Y = lambda v: T + (42 - v) / 42 * H_  # noqa: E731
    body = []
    for k, (lab, r1, r2) in enumerate(items):
        tot = r1 + r2
        x0 = L + k * bw + bw * 0.22
        w_bar = bw * 0.56
        body.append(f'<rect x="{x0:.1f}" y="{Y(r1):.1f}" width="{w_bar:.1f}" height="{Y(0) - Y(r1):.1f}" fill="{C_MAIN}"/>')
        body.append(f'<rect x="{x0:.1f}" y="{Y(tot):.1f}" width="{w_bar:.1f}" height="{Y(r1) - Y(tot):.1f}" fill="{C_ACC}"/>')
        xc = L + (k + 0.5) * bw
        body.append(F.text(xc, Y(tot) - 7, f"{tot:.2f}", 11.5, "middle", C_ACC if k == n - 1 else "#222222"))
        body.append(F.text(xc, T + H_ + 20, lab, 12, "middle", "#222222"))
    F.axes_box(body, L, T, W_, H_, [], range(0, 43, 10), lambda v: v, Y, None, "两星累计速度增量 / (m/s)")
    body.append(F.text(L + W_ / 2, T + H_ + 46, "模型与求解阶段", 13))
    F.legend_box(body, L + W_ - 150, T + 10, [
        ("rect", C_MAIN, None, "R1（西侧 10 颗）"),
        ("rect", C_ACC, None, "R2（东侧 20 颗）"),
    ])
    F.save("fig12d_q3_ablation.svg", W, H, body)


def make_obs_verify():
    """图 (a)：问题二/三全部 60 次观测的最小距离与最大观测角分布（均严格落在可行域内）。"""
    # 从三个主方案 JSON 直接算每个观测窗 5 采样点的 (d_min, d_max, max_ang)
    # 为了速度与精确性，采用 11 点等分 + erfa 太阳方向
    import erfa
    from datetime import datetime, timezone

    def sun_dir(t_s):
        t0 = datetime(2027, 1, 1, 0, 0, 0, tzinfo=timezone.utc)
        jd1, jd2 = erfa.dtf2d("UTC", t0.year, t0.month, t0.day, t0.hour, t0.minute, t0.second)
        jd2_t = jd2 + t_s / 86400.0
        pvh, _ = erfa.epv00(jd1, jd2_t)
        r_sun = -pvh[0] * 149597870.7
        return r_sun

    def eval_plan(path):
        d = json.load(open(path, encoding="utf-8"))
        burns = sorted(((b["t_s"], np.array(b["dv_km_s"], float)) for b in d["burns"]), key=lambda z: z[0])
        x = np.array(d["r1_km"], float)
        pts = []
        for o in d["observations"]:
            s0, dur, name = o["start_s"], o["duration_s"], o["target"]
            ts = np.linspace(s0, s0 + dur, 13)
            # 传播观测星到 s0
            r, v, tc = x[:3].copy(), x[3:].copy(), 0.0
            for bt, bdv in burns:
                if bt <= s0:
                    R, V = F.kepler(r, v, bt - tc)
                    r, v, tc = R[0], V[0] + bdv, bt
            R0, V0 = F.kepler(r, v, s0 - tc)
            r, v = R0[0], V0[0]
            rt0 = np.array(d["targets_km"][name][:3], float)
            vt0 = np.array(d["targets_km"][name][3:], float)
            dmin, dmax, amax = 1e9, 0.0, 0.0
            for ti in ts:
                ro = F.kepler(r, v, ti - s0)[0][0]
                rt = F.kepler(rt0, vt0, ti)[0][0]
                rho = ro - rt
                dist = float(np.linalg.norm(rho))
                rs = sun_dir(ti)
                u_s = (rs - rt) / np.linalg.norm(rs - rt)
                u_e = -rt / np.linalg.norm(rt)
                al = float(np.degrees(np.arccos(np.clip(np.dot(u_s, rho) / dist, -1, 1))))
                th = float(np.degrees(np.arccos(np.clip(np.dot(u_e, rho) / dist, -1, 1))))
                dmin = min(dmin, dist)
                dmax = max(dmax, dist)
                amax = max(amax, al, th)
            pts.append((dmin, dmax, amax))
        return pts

    p_q2 = eval_plan(REPO / "results/q2/m_uni_q2_best.json")
    p_r1 = eval_plan(REPO / "results/q3/m_uni_q3_r1_best.json")
    p_r2 = eval_plan(REPO / "results/q3/m_uni_q3_r2_best.json")
    p_q3 = [(a, b, C_SUB) for a, b, _ in p_r1] + [(a, b, C_ACC) for a, b, _ in p_r2]

    W, H = 620, 360
    L, T, W_, H_ = 78, 22, 512, 272
    X = lambda k: L + (k + 0.5) / 30.0 * W_  # noqa: E731
    Y = lambda v: T + (56.0 - v) / 50.0 * H_  # noqa: E731
    body = []
    # 可行距离带 [10, 50] km 浅灰参考线
    body.append(F.line(L, Y(10), L + W_, Y(10), C_GR, 0.9, "4,3"))
    body.append(F.line(L, Y(50), L + W_, Y(50), C_GR, 0.9, "4,3"))
    for k in range(30):
        # 左偏一点：问题二
        x_q2 = X(k) - 2.6
        d0, d1, _ = p_q2[k]
        body.append(F.line(x_q2, Y(d0), x_q2, Y(d1), C_MAIN, 2.0))
        body.append(f'<circle cx="{x_q2:.1f}" cy="{Y(d0):.1f}" r="2.0" fill="{C_MAIN}"/>')
        # 右偏一点：问题三（左 10 颗 R1，右 20 颗 R2）
        x_q3 = X(k) + 2.6
        e0, e1, col = p_q3[k]
        body.append(F.line(x_q3, Y(e0), x_q3, Y(e1), col, 2.0))
        body.append(f'<circle cx="{x_q3:.1f}" cy="{Y(e0):.1f}" r="2.0" fill="{col}"/>')
    F.axes_box(body, L, T, W_, H_, [1, 5, 10, 15, 20, 25, 30], [10, 20, 30, 40, 50],
               lambda k: X(k - 1), Y, "观测序号", "观测段距离区间 / km")
    F.legend_box(body, L + W_ / 2 - 72, T + H_ - 62, [
        ("line", C_MAIN, None, "问题二（30 次）"),
        ("line", C_SUB, None, "问题三 R1（10 次）"),
        ("line", C_ACC, None, "问题三 R2（20 次）"),
    ])
    F.save("fig15a_obs_verify.svg", W, H, body)


def make_plane_tilt():
    """图 (b)：面外机动占比与 30 天轨道面最大倾转角对比。"""
    exp = json.load(open(REPO / "results/reviewer_exps/reviewer_exps_out.json", encoding="utf-8"))["exp2"]
    items = [
        ("问题二 R1", exp["Q2-MUni"]["ratio"] * 100, exp["Q2-MUni"]["max_tilt_deg"]),
        ("问题三 R1", exp["Q3-R1-MUni"]["ratio"] * 100, exp["Q3-R1-MUni"]["max_tilt_deg"]),
        ("问题三 R2", exp["Q3-R2-MUni"]["ratio"] * 100, exp["Q3-R2-MUni"]["max_tilt_deg"]),
    ]
    W, H = 620, 360
    L, T, W_, H_ = 78, 22, 460, 272
    n = len(items)
    bw = W_ / n
    Y1 = lambda v: T + (8.0 - v) / 8.0 * H_     # noqa: E731 (左轴：占比 0-8%)
    Y2 = lambda v: T + (0.08 - v) / 0.08 * H_   # noqa: E731 (右轴：倾转角 0-0.08°)
    body = []
    for k, (lab, rat, tilt) in enumerate(items):
        xc = L + (k + 0.5) * bw
        w_bar = bw * 0.28
        # 左柱：面外占比
        body.append(f'<rect x="{xc - w_bar - 3:.1f}" y="{Y1(rat):.1f}" width="{w_bar:.1f}" height="{Y1(0) - Y1(rat):.1f}" fill="{C_MAIN}"/>')
        body.append(F.text(xc - w_bar / 2 - 3, Y1(rat) - 6, f"{rat:.2f}%", 10.5, "middle", C_MAIN))
        # 右柱：最大倾转角
        body.append(f'<rect x="{xc + 3:.1f}" y="{Y2(tilt):.1f}" width="{w_bar:.1f}" height="{Y2(0) - Y2(tilt):.1f}" fill="{C_ACC}"/>')
        body.append(F.text(xc + w_bar / 2 + 3, Y2(tilt) - 6, f"{tilt:.4f}°", 10.5, "middle", C_ACC))
        body.append(F.text(xc, T + H_ + 20, lab, 12, "middle", "#222222"))
    F.axes_box(body, L, T, W_, H_, [], [0, 2, 4, 6, 8], lambda v: v, Y1, None, "面外速度增量占比 / (%)")
    # 右轴刻度
    for tv in [0.00, 0.02, 0.04, 0.06, 0.08]:
        yy = Y2(tv)
        body.append(F.line(L + W_ - 5, yy, L + W_, yy, "#000000", 1.0))
        body.append(F.text(L + W_ + 6, yy + 4, f"{tv:.2f}", 11, "start", C_ACC))
    body.append(F.text(L + W_ + 58, T + H_ / 2, "最大轨道面倾转角 / (°)", 12.5, "middle", C_ACC, rot=-90))
    body.append(F.text(L + W_ / 2, T + H_ + 46, "任务方案", 13))
    F.legend_box(body, L + W_ - 180, T + 10, [
        ("rect", C_MAIN, None, "面外机动占比（左轴）"),
        ("rect", C_ACC, None, "最大轨道面倾转角（右轴）"),
    ])
    F.save("fig15b_plane_tilt.svg", W, H, body)


def make_q2_baseline_figs():
    """第 6.2.1 节图形化机理图（单色深蓝系，左右双区）：
    左：拱线偏角 δ 引起交会高度偏差 |Δa|(1−cos δ) 的轨道几何示意；
    右：漂移相位束搜索的树状扩展与剪枝示意。
    """
    W, H = 1000, 410
    DK, MD, LT, BG, BD = "#1A365D", "#3D72A4", "#EBF2F8", "#F7FAFC", "#9BB7D4"
    ACC = "#9B2C2C"
    GR = "#8A99AD"
    body = []

    # 左右分区柔和背景框
    body.append(f'<rect x="14" y="14" width="470" height="382" rx="6" ry="6" fill="{BG}" stroke="{BD}" stroke-width="0.9"/>')
    body.append(f'<rect x="506" y="14" width="480" height="382" rx="6" ry="6" fill="{BG}" stroke="{BD}" stroke-width="0.9"/>')
    body.append(F.text(249, 42, "(a) 交线偏角 δ 与交会高度偏差几何关系", 14, "middle", DK))
    body.append(F.text(746, 42, "(b) 漂移相位束搜索的逐层扩展与剪枝", 14, "middle", DK))

    # ==================== 左图：轨道拱线与高度偏差几何 ====================
    cx, cy = 204.0, 222.0
    r_geo = 90.0
    # 中心地球
    body.append(f'<circle cx="{cx}" cy="{cy}" r="18" fill="{LT}" stroke="{DK}" stroke-width="1.2"/>')
    body.append(F.text(cx, cy + 4, "地球", 10.5, "middle", DK))

    # 名义 GEO 圆轨道（虚线）
    ang = np.linspace(0, 2 * np.pi, 180)
    geo_pts = [(cx + r_geo * np.cos(a), cy - r_geo * np.sin(a)) for a in ang]
    body.append(F.poly(geo_pts, stroke=GR, w=1.4, dash="5,4", closed=True))

    # 切向脉冲后的椭圆漂移轨道（在右侧 θ=0 处与 GEO 相切，近地点在 θ=0，远地点在 θ=π）
    da = 24.0
    ell_pts = [(cx + (r_geo + da * (1.0 - np.cos(a))) * np.cos(a),
                cy - (r_geo + da * (1.0 - np.cos(a))) * np.sin(a)) for a in ang]
    body.append(F.poly(ell_pts, stroke=DK, w=2.0, closed=True))

    # 公共拱线（水平轴，过脉冲点 θ=0）
    body.append(F.line(cx - 148, cy, cx + 140, cy, MD, 1.1, "4,3"))
    body.append(F.text(cx + 144, cy + 4, "公共拱线", 11.5, "start", MD))

    # 脉冲点（切点 θ=0）
    px, py = cx + r_geo, cy
    body.append(f'<circle cx="{px:.1f}" cy="{py:.1f}" r="4.5" fill="{DK}" stroke="#FFFFFF" stroke-width="1.0"/>')
    body.append(F.text(px + 8, py + 18, "切向脉冲点", 11.5, "start", DK))

    # 情形 1：小偏角交线（δ = 20°）
    a1 = np.radians(20.0)
    r1_e = r_geo + da * (1.0 - np.cos(a1))
    body.append(F.line(cx, cy, cx + (r1_e + 22) * np.cos(a1), cy - (r1_e + 22) * np.sin(a1), MD, 1.0, "3,2"))
    body.append(f'<circle cx="{cx + r_geo * np.cos(a1):.1f}" cy="{cy - r_geo * np.sin(a1):.1f}" r="3.5" fill="{MD}"/>')
    body.append(F.text(cx + (r1_e + 26) * np.cos(a1), cy - (r1_e + 26) * np.sin(a1) - 2, "小偏角交线", 11.5, "start", MD))
    body.append(F.text(cx + (r1_e + 26) * np.cos(a1), cy - (r1_e + 26) * np.sin(a1) + 14, "偏差小，可快漂", 10.5, "start", "#4A5568"))

    # 情形 2：大偏角交线（δ = 70°）
    a2 = np.radians(70.0)
    r2_e = r_geo + da * (1.0 - np.cos(a2))
    g2x, g2y = cx + r_geo * np.cos(a2), cy - r_geo * np.sin(a2)
    e2x, e2y = cx + r2_e * np.cos(a2), cy - r2_e * np.sin(a2)
    body.append(F.line(cx, cy, cx + (r2_e + 16) * np.cos(a2), cy - (r2_e + 16) * np.sin(a2), ACC, 1.1, "3,2"))
    # 红色加粗线段突出径向高度偏差 |Δa|(1-cos δ)
    body.append(F.line(g2x, g2y, e2x, e2y, ACC, 3.2))
    body.append(f'<circle cx="{g2x:.1f}" cy="{g2y:.1f}" r="3.5" fill="{GR}"/>')
    body.append(f'<circle cx="{e2x:.1f}" cy="{e2y:.1f}" r="4.0" fill="{ACC}"/>')
    body.append(F.text(e2x + 36, e2y - 6, "大偏角交线（δ 较大）", 11.5, "start", ACC))
    body.append(F.text(e2x + 36, e2y + 11, "高度偏差 ≈ |Δa|(1−cos δ)", 11, "start", ACC))
    body.append(F.text(e2x + 36, e2y + 27, "需降低漂移率 ω 慢速接近", 10.5, "start", "#4A5568"))
    body.append(F.line(e2x + 5, e2y, e2x + 32, e2y + 4, ACC, 0.8))

    # 角度弧 δ 标注
    arc_a = np.linspace(0, a2, 24)
    body.append(F.poly([(cx + 38 * np.cos(t), cy - 38 * np.sin(t)) for t in arc_a], stroke=ACC, w=1.2, closed=False))
    body.append(F.text(cx + 46 * np.cos(a2 / 2), cy - 46 * np.sin(a2 / 2) + 4, "δ", 12, "middle", ACC))

    # 左图底部图例条
    body.append(F.line(36, 372, 62, 372, GR, 1.4, "5,4"))
    body.append(F.text(68, 376, "名义 GEO 轨道", 11, "start", "#4A5568"))
    body.append(F.line(176, 372, 202, 372, DK, 2.0))
    body.append(F.text(208, 376, "脉冲后漂移轨道", 11, "start", DK))
    body.append(F.line(322, 372, 348, 372, ACC, 3.0))
    body.append(F.text(354, 376, "交会高度偏差", 11, "start", ACC))

    # ==================== 右图：束搜索逐层扩展与剪枝树 ====================
    xs = [556, 672, 796, 922]
    # 层标题
    for x, lab in zip(xs, ["起点 R1", "第 1 步", "第 2 步", "第 3 步"]):
        body.append(F.text(x, 76, lab, 12, "middle", DK))
        body.append(F.line(x, 86, x, 336, "#E2E8F0", 0.9, "2,3"))

    # 节点坐标：(x, y, kept, label)
    root = (xs[0], 210)
    l1 = [(xs[1], 124, True, "e1"), (xs[1], 196, True, "e2"), (xs[1], 266, False, "×"), (xs[1], 320, False, "×")]
    l2 = [
        (xs[2], 108, True, "e3", 0), (xs[2], 152, False, "×", 0),
        (xs[2], 204, True, "e4", 1), (xs[2], 254, False, "×", 1),
    ]
    l3 = [
        (xs[3], 108, True, "优选", 0), (xs[3], 150, False, "×", 0),
        (xs[3], 204, True, "保留", 2), (xs[3], 254, False, "×", 2),
    ]

    # 画连线：先画剪枝虚线，再画保留实线
    for nx, ny, kept, _ in l1:
        body.append(F.line(root[0], root[1], nx, ny, DK if kept else "#B8C4D0", 1.8 if kept else 1.0, None if kept else "4,3"))
    for nx, ny, kept, _, pid in l2:
        px0, py0 = l1[pid][0], l1[pid][1]
        body.append(F.line(px0, py0, nx, ny, DK if kept else "#B8C4D0", 1.8 if kept else 1.0, None if kept else "4,3"))
    for nx, ny, kept, _, pid in l3:
        px0, py0 = l2[pid][0], l2[pid][1]
        body.append(F.line(px0, py0, nx, ny, DK if kept else "#B8C4D0", 1.8 if kept else 1.0, None if kept else "4,3"))

    # 画节点
    body.append(f'<circle cx="{root[0]}" cy="{root[1]}" r="12" fill="{DK}"/>')
    body.append(F.text(root[0], root[1] + 4, "R1", 11, "middle", "#FFFFFF"))

    def draw_node(x, y, kept, lab):
        if kept:
            rr = 14 if len(lab) >= 2 and not lab.startswith("e") else 11
            body.append(f'<circle cx="{x}" cy="{y}" r="{rr}" fill="{LT}" stroke="{DK}" stroke-width="1.6"/>')
            body.append(F.text(x, y + 4, lab, 10.5, "middle", DK))
        else:
            body.append(f'<circle cx="{x}" cy="{y}" r="9" fill="#FFFFFF" stroke="#B8C4D0" stroke-width="1.1" stroke-dasharray="2,2"/>')
            body.append(F.text(x, y + 4, "×", 11, "middle", "#9AA5B1"))

    for nx, ny, kept, lab in l1:
        draw_node(nx, ny, kept, lab)
    for nx, ny, kept, lab, _ in l2:
        draw_node(nx, ny, kept, lab)
    for nx, ny, kept, lab, _ in l3:
        draw_node(nx, ny, kept, lab)

    # 束宽 W 说明与评价值提示框
    body.append(f'<rect x="536" y="346" width="420" height="36" rx="4" ry="4" fill="#FFFFFF" stroke="{BD}" stroke-width="0.9"/>')
    body.append(F.text(746, 369, "每层按评价值 f 保留前 W 条低代价路径，剪去高代价与超 30 m/s 分支", 11.5, "middle", DK))

    F.save("fig10e_q2_mechanism.svg", W, H, body)


if __name__ == "__main__":
    make_q2_ablation()
    make_q2_cumdv()
    make_q3_split()
    make_q3_ablation()
    make_obs_verify()
    make_plane_tilt()
    make_q2_baseline_figs()
