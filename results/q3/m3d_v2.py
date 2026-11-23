"""M3-D v2 (24-Core Parallel Q3 Optimizer):
1. R1 (10 颗西侧目标):
   - 分支 A: 从 v6_p3_r1 出发，完整 30 轮 Joint (Δv, Δt) VC-SCP（不因 stall 提前中断，直接把最终 cb1 送入 Polish！）+ 20 轮 FixedT-Polish；
   - 分支 B: 零先验 3D-CW-VND-DP 重新优化 10 颗西侧目标访问序列（含 b26/b30 顺序等）-> FixedT -> Joint (Δv, Δt) -> FixedT-Polish；
   - 取严格 10/10 认证的更优者！
2. R2 (20 颗东侧目标, e < 0.001, |a - a_GEO| <= 5.90 km):
   - 完整 36 轮 Joint (Δv, Δt, Δx0) VC-SCP（同步优化 40 次脉冲矢量 + 40 次脉冲时刻 + R2 初始六维轨道状态 x0，不提前中断！）
     -> 直接把最终 (cb2, cx2) 送入 20 轮 FixedT+x0 Polish，锁死 20/20 严格认证！
"""
import json
import math
import os
import sys
import time
from collections import defaultdict
from concurrent.futures import ProcessPoolExecutor

import cvxpy as cp
import numpy as np

sys.path.insert(0, os.environ.get('P2LIB_DIR', '/root/autodl-tmp/p2_newdata/batch'))
sys.path.insert(0, '/root/autodl-tmp/m2d')
sys.path.insert(0, '/root/autodl-tmp/m3d')
from p2lib import MU, R1, SUN, TG, kepler, tpos  # noqa: E402
from se_milp_vcscp import AGEO, unit, generate_events, build_r1_frame, phase_in_r1  # noqa: E402
from m3d_joint_scp import elem, positions_x0, certify_x0, compute_jacs, load_plan, save_plan, DUR  # noqa: E402
import m2d_plus_v5 as m2d  # noqa: E402


def run_q3_scp_v2(burns, obs, x0, opt_time=True, opt_x0=False, da_max=5.895, da_cert=5.90,
                  iters=32, tr_v=0.8, tr_t=900.0, tr_r0=40.0, tr_v0=2e-3, label="Q3-SCP"):
    t_start = time.time()
    nb = len(burns)
    no = len(obs)
    md, ma = 0.05, 0.05
    ca = math.cos(math.radians(45.0 - ma))
    dmax, dmin = 50.0 - md, 10.0 + md

    obs_sorted = sorted(obs, key=lambda o: o[1])
    leg_ids = []
    for b in burns:
        lid = 0
        for j, (_, s) in enumerate(obs_sorted):
            if b[0] <= s:
                lid = j
                break
        leg_ids.append(lid)

    best = None
    n0_ok, _ = certify_x0(burns, obs_sorted, x0, step=0.05, da_max=(da_cert if opt_x0 else None))
    init_dv = sum(np.linalg.norm(b[1]) for b in burns)
    if n0_ok == no:
        best = (init_dv, [[b[0], b[1].copy()] for b in burns], [o[:] for o in obs_sorted], x0.copy())
    print(f"[{label}] 起点: Δv={init_dv:.4f} m/s, 认证={n0_ok}/{no}, nb={nb}, opt_time={opt_time}, opt_x0={opt_x0}", flush=True)

    stall = 0
    for it in range(iters):
        t0 = time.time()
        T_list, idx_list = [], []
        for j, (name, s) in enumerate(obs_sorted):
            for q in np.linspace(0.0, DUR, 5):
                T_list.append(s + q)
                idx_list.append(j)
        order = np.argsort(T_list)
        T = np.array(T_list)[order]
        idx = np.array(idx_list)[order]
        ns = len(T)

        rT = np.array([tpos(obs_sorted[j][0], t)[0][0] for t, j in zip(T, idx)])
        us = unit(SUN(T) - rT)
        ue = unit(-rT)
        ghat_bis = unit(us + ue)

        r0 = positions_x0(burns, T, x0)
        rho0 = r0 - rT

        Jv, Jt, Jx0 = compute_jacs(burns, T, r0, x0, opt_time=opt_time, opt_x0=opt_x0)

        dx = cp.Variable(nb * 3)
        dt = cp.Variable(nb) if opt_time else None
        dx0 = cp.Variable(6) if opt_x0 else None
        sl = cp.Variable(ns, nonneg=True)
        dv0 = np.array([b[1] for b in burns])
        t0_arr = np.array([b[0] for b in burns])

        obj_dv = sum(cp.norm(dv0[k] + dx[3 * k:3 * k + 3]) for k in range(nb))
        cons = [cp.norm_inf(dx) <= tr_v]
        for k in range(nb):
            cons.append(cp.norm(dv0[k] + dx[3 * k:3 * k + 3]) <= 29.9)

        if opt_time:
            cons.append(cp.norm_inf(dt) <= tr_t)
            cons.append(t0_arr[0] + dt[0] >= 0.0)
            for k in range(nb - 1):
                cons.append((t0_arr[k + 1] + dt[k + 1]) - (t0_arr[k] + dt[k]) >= 3601.0)
            for k in range(nb):
                lid = leg_ids[k]
                cons.append(t0_arr[k] + dt[k] <= obs_sorted[lid][1] - 15.0)
                if lid > 0:
                    cons.append(t0_arr[k] + dt[k] >= obs_sorted[lid - 1][1] + DUR + 5.0)

        if opt_x0:
            cons.append(cp.norm_inf(dx0[:3]) <= tr_r0)
            cons.append(cp.norm_inf(dx0[3:]) <= tr_v0)
            a0, e0 = elem(x0)
            Ja = np.zeros(6)
            Je = np.zeros((3, 6))
            for j in range(6):
                ep = 1e-3 if j < 3 else 1e-7
                xp = x0.copy(); xp[j] += ep
                ap, epv = elem(xp)
                Ja[j] = (ap - a0) / ep
                Je[:, j] = (epv - e0) / ep
            cons.append(cp.abs((a0 + Ja @ dx0) - AGEO) <= da_max)
            cons.append(cp.norm(e0 + Je @ dx0) <= 0.00095)

        for i in range(ns):
            rho_i = rho0[i] + Jv[i] @ dx
            if opt_time:
                rho_i = rho_i + Jt[i] @ dt
            if opt_x0:
                rho_i = rho_i + Jx0[i] @ dx0
            rn = float(np.linalg.norm(rho0[i]))
            g_i = (rho0[i] / rn) if (rn > 5.0 and float(np.dot(rho0[i] / rn, ghat_bis[i])) > 0.35) else ghat_bis[i]
            cons.append(cp.norm(rho_i) <= dmax + sl[i])
            cons.append(g_i @ rho_i >= dmin - sl[i])
            cons.append(cp.norm(rho_i) * ca <= us[i] @ rho_i + sl[i])
            cons.append(cp.norm(rho_i) * ca <= ue[i] @ rho_i + sl[i])

        prob = cp.Problem(cp.Minimize(obj_dv + 5000.0 * cp.sum(sl)), cons)
        try:
            prob.solve(solver=cp.CLARABEL, tol_gap_abs=1e-7, tol_gap_rel=1e-7, max_iter=150)
        except Exception:
            tr_v *= 0.5; tr_t *= 0.5; tr_r0 *= 0.5; tr_v0 *= 0.5
            continue
        if dx.value is None:
            tr_v *= 0.5; tr_t *= 0.5; tr_r0 *= 0.5; tr_v0 *= 0.5
            continue

        xval = dx.value.reshape(nb, 3)
        dtval = dt.value if opt_time else np.zeros(nb)
        dx0val = dx0.value if opt_x0 else np.zeros(6)
        cand_b = [[float(b[0] + dtval[k]), b[1] + xval[k]] for k, b in enumerate(burns)]
        cand_x0 = x0 + dx0val
        tot_new = sum(np.linalg.norm(b[1]) for b in cand_b)
        sl_max = float(np.max(sl.value))
        n_ok, bad = certify_x0(cand_b, obs_sorted, cand_x0, step=0.05, da_max=(da_cert if opt_x0 else None))
        n_act = sum(np.linalg.norm(b[1]) > 1e-2 for b in cand_b)
        extra = ""
        if opt_time:
            extra += f", max|dt|={np.max(np.abs(dtval)):.0f}s"
        if opt_x0:
            aa, ee = elem(cand_x0)
            extra += f", da={aa-AGEO:+.2f}km, e={np.linalg.norm(ee):.5f}"
        print(f"  [{label}] it {it:2d}: Δv={tot_new:7.4f} m/s, 有效={n_act:2d}/{nb}, slack={sl_max:.4f}, 认证={n_ok:2d}/{no}, tr={tr_v:.3f}{extra}, {time.time()-t0:.1f}s"
              + ("" if bad else " ✓ALL_OK"), flush=True)

        burns, x0 = cand_b, cand_x0
        if n_ok == no and (best is None or tot_new < best[0] - 1e-5):
            best = (tot_new, [[b[0], b[1].copy()] for b in burns], [o[:] for o in obs_sorted], x0.copy())
            stall = 0
        else:
            stall += 1
        # 关键修复：在 opt_time=True 阶段绝不因 stall 提前退出，走完完整时间微调再进 Polish！
        if (not opt_time) and best is not None and stall >= 6:
            break
        tr_v = max(tr_v * 0.88, 0.005)
        tr_t = max(tr_t * 0.86, 8.0)
        tr_r0 = max(tr_r0 * 0.88, 0.05)
        tr_v0 = max(tr_v0 * 0.88, 1e-6)

    print(f"[{label}] 完成, 耗时 {time.time()-t_start:.1f}s, 最优={best[0] if best else None}", flush=True)
    return best, burns, obs_sorted, x0


if __name__ == '__main__':
    T0 = time.time()
    # --- 1A. R1 从 v6 出发: 完整 30 轮 Joint (Δv, Δt) -> 把最终 cb1 送入 Polish ---
    d1, b1, o1, x01 = load_plan('/root/autodl-tmp/m3d/v6_p3_r1.json')
    best1a, cb1, co1, cx1 = run_q3_scp_v2(b1, o1, x01, opt_time=True, opt_x0=False, iters=28, tr_v=0.8, tr_t=1200.0, label="R1A-Joint-dVdt")
    best1a_p, _, _, _ = run_q3_scp_v2(cb1, co1, cx1, opt_time=False, opt_x0=False, iters=18, tr_v=0.4, label="R1A-Polish")
    # 第二轮 Joint -> Polish 进一步压榨
    if best1a_p is not None:
        _, cb1_2, co1_2, cx1_2 = run_q3_scp_v2(best1a_p[1], best1a_p[2], best1a_p[3], opt_time=True, opt_x0=False, iters=20, tr_v=0.4, tr_t=1800.0, label="R1A-Joint2")
        best1a_p2, _, _, _ = run_q3_scp_v2(cb1_2, co1_2, cx1_2, opt_time=False, opt_x0=False, iters=15, tr_v=0.3, label="R1A-Polish2")
    else:
        best1a_p2 = None

    # --- 1B. R1 从零先验 3D-CW-VND-DP 重新优化 10 颗西侧目标 ---
    events, _, _ = generate_events()
    ev_by_tgt = defaultdict(list)
    for ev in events:
        if len(ev['targets']) == 1:
            ev_by_tgt[ev['targets'][0]].append(ev)
    for k in ev_by_tgt:
        ev_by_tgt[k].sort(key=lambda e: e['t'])
    m2d.attach_lvlh_states(ev_by_tgt)
    m2d._G_CW_LUT = m2d.build_cw_lut(7205)
    west10 = [o[0] for o in o1]
    best_w_seq, best_w_cost, best_w_path = m2d.parallel_multistart_cw_vnd([("west10", west10), ("west10-rev", west10[::-1])], ev_by_tgt)
    wb_init, wo_init = m2d.build_clean_2burn_init(best_w_path)
    wb_simple = [[b[0], b[1]] for b in wb_init]
    best1b_f, cb1b, co1b, cx1b = run_q3_scp_v2(wb_simple, wo_init, x01, opt_time=False, opt_x0=False, iters=28, tr_v=2.0, label="R1B-CW-FixedT")
    _, cb1b2, co1b2, cx1b2 = run_q3_scp_v2(best1b_f[1] if best1b_f else cb1b, co1b, cx1b, opt_time=True, opt_x0=False, iters=25, tr_v=0.8, tr_t=1200.0, label="R1B-CW-Joint")
    best1b_p, _, _, _ = run_q3_scp_v2(cb1b2, co1b2, cx1b2, opt_time=False, opt_x0=False, iters=18, tr_v=0.35, label="R1B-CW-Polish")

    r1_cands = [x for x in [best1a, best1a_p, best1a_p2, best1b_f, best1b_p] if x is not None]
    best_r1 = min(r1_cands, key=lambda x: x[0])
    save_plan(d1, best_r1, '/root/autodl-tmp/m3d/m3d_v2_r1.json', 'R1 3D-CW + Joint dV/dt VC-SCP')

    # --- 2. R2 (20 颗东侧目标): 2 轮 Joint (Δv, Δt, Δx0) -> Polish ---
    d2, b2, o2, x02 = load_plan('/root/autodl-tmp/m3d/v6_p3_r2.json')
    best2a, cb2, co2, cx2 = run_q3_scp_v2(b2, o2, x02, opt_time=True, opt_x0=True, da_max=5.895, da_cert=5.90,
                                          iters=32, tr_v=0.8, tr_t=1200.0, tr_r0=40.0, tr_v0=2e-3, label="R2-Joint1")
    best2p1, _, _, _ = run_q3_scp_v2(cb2, co2, cx2, opt_time=False, opt_x0=True, da_max=5.895, da_cert=5.90,
                                     iters=18, tr_v=0.35, tr_r0=5.0, tr_v0=2e-4, label="R2-Polish1")
    if best2p1 is not None:
        _, cb2_2, co2_2, cx2_2 = run_q3_scp_v2(best2p1[1], best2p1[2], best2p1[3], opt_time=True, opt_x0=True, da_max=5.895, da_cert=5.90,
                                               iters=24, tr_v=0.5, tr_t=1800.0, tr_r0=30.0, tr_v0=1.5e-3, label="R2-Joint2")
        best2p2, _, _, _ = run_q3_scp_v2(cb2_2, co2_2, cx2_2, opt_time=False, opt_x0=True, da_max=5.895, da_cert=5.90,
                                         iters=18, tr_v=0.3, tr_r0=3.0, tr_v0=1e-4, label="R2-Polish2")
    else:
        best2p2 = None

    r2_cands = [x for x in [best2a, best2p1, best2p2] if x is not None]
    best_r2 = min(r2_cands, key=lambda x: x[0])
    save_plan(d2, best_r2, '/root/autodl-tmp/m3d/m3d_v2_r2.json', 'R2 Joint dV/dt/x0 VC-SCP')

    a2, e2 = elem(best_r2[3])
    print(f"\n=== M3-D v2 Q3 联合优化完成 (耗时 {time.time()-T0:.1f}s) ===", flush=True)
    print(f"  R1 (10星): 18.1475 -> {best_r1[0]:.4f} m/s", flush=True)
    print(f"  R2 (20星): 15.3121 -> {best_r2[0]:.4f} m/s (da={a2-AGEO:+.4f} km, e={np.linalg.norm(e2):.6f})", flush=True)
    print(f"  总 Δv    : 33.4596 -> {best_r1[0] + best_r2[0]:.4f} m/s (节省 {33.4596 - (best_r1[0]+best_r2[0]):.4f} m/s!)", flush=True)
