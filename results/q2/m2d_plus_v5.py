"""M2-D+ v5 (24-Core Parallel, Exact 3D CW Energy-Integral 2-Burn DP + 3-Stage VC-SCP):
  核心创新（零数据先验、不剪解、纯轨道动力学）：
    1. Stage 1: 24 核并行多起点全范围 3D-CW-VND-SE-DP
       —— 采用 Hill/Clohessy-Wiltshire 能量积分漂移速度 v_y = -2 n x - (1/3) y_dot_sec
          与两脉冲（t1 与 t1+0.53*dt）4 方程（三维位置 + 末端漂移率）最小范数解析解作为精确边权，
          彻底解决整周期奇异性与短时转移 (4~6h) 虚低失配！
    2. Stage 2A: 24 核并行固定时刻 VC-SCP（先稳健收敛到 slack=0 与 30/30 严格可行域）
    3. Stage 2B: 24 核并行温和联合 (Δv, Δt) VC-SCP（可行域内小步长 tr_t=360s 联合微调时刻与矢量 + 价值函数保护）
    4. Stage 2C: 24 核并行固定时刻精修 VC-SCP（锁死最优时刻，0.05 s 网格严格 30/30 认证）
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
from p2lib import MU, R1, SUN, TG, kepler, tpos  # noqa: E402
from se_milp_vcscp import (
    T_END, AGEO, N_GEO, KAPPA, W_CAP, RHO_TOL,
    unit, build_r1_frame, phase_in_r1, generate_events,
    stm_fd, positions, certify_all,
)
from se_dp_vcscp_v2 import slide_obs_windows

N_WORKERS = min(24, os.cpu_count() or 24)
W_LEVELS = [-2.5, -1.8, -1.2, -0.6, 0.0, 0.6, 1.2, 1.8, 2.5]
_DEG_DAY_TO_RAD_S = math.pi / (180.0 * 86400.0)

_G_EV_BY_TGT = None
_G_JAC_CTX = None
_G_CW_LUT = None


def build_cw_lut(max_min=7205):
    """预计算 1 分钟步长的 3D-CW 状态转移与伪逆查找表 (~0.15s)，24 核 fork 零拷贝共享，提速 ~15x！"""
    t0 = time.time()
    lut = [None] * (max_min + 1)
    I4 = 1e-4 * np.eye(4)
    for m in range(1, max_min + 1):
        dt = m * 60.0
        Prr, Prv, Pvr, Pvv = _cw_stm(dt)
        _, Prv2, _, Pvv2 = _cw_stm(0.47 * dt)
        A = np.empty((4, 6))
        A[:3, :3] = Prv
        A[:3, 3:] = Prv2
        A[3, :3] = Pvv[1, :] * 3000.0
        A[3, 3:] = Pvv2[1, :] * 3000.0
        Ap = A.T @ np.linalg.inv(A @ A.T + I4)
        lut[m] = (Prr, Prv, Pvr[1, :], Pvv[1, :], Ap)
    print(f"[CW-LUT] 预计算 {max_min} 档三维 CW 解析查表完成 ({time.time()-t0:.2f}s)", flush=True)
    return lut


def attach_lvlh_states(ev_by_tgt):
    h1, e1, e2, _ = build_r1_frame()
    for tname, evs in ev_by_tgt.items():
        for ev in evs:
            t = ev['t']
            rt = tpos(tname, t)[0][0]
            sc = SUN(t)
            us = unit(sc - rt)
            ue = unit(-rt)
            g = unit(us + ue)
            r_aim = rt + 25.0 * g
            c_th = math.cos(N_GEO * t)
            s_th = math.sin(N_GEO * t)
            u_r = c_th * e1 + s_th * e2
            u_t = -s_th * e1 + c_th * e2
            r_norm_xy = math.hypot(float(np.dot(r_aim, u_r)), float(np.dot(r_aim, u_t)))
            x_lvlh = r_norm_xy - AGEO
            phi_rad = math.atan2(float(np.dot(r_aim, u_t)), float(np.dot(r_aim, u_r)))
            y_lvlh = AGEO * phi_rad
            z_lvlh = float(np.dot(r_aim, h1))
            ev['lvlh'] = np.array([x_lvlh, y_lvlh, z_lvlh], dtype=float)


def _cw_stm(dt):
    n = N_GEO
    nt = n * dt
    s, c = math.sin(nt), math.cos(nt)
    Prr = np.array([[4.0 - 3.0 * c, 0.0, 0.0], [6.0 * (s - nt), 1.0, 0.0], [0.0, 0.0, c]])
    Prv = np.array([[s / n, 2.0 * (1.0 - c) / n, 0.0], [-2.0 * (1.0 - c) / n, (4.0 * s - 3.0 * nt) / n, 0.0], [0.0, 0.0, s / n]])
    Pvr = np.array([[3.0 * n * s, 0.0, 0.0], [6.0 * n * (c - 1.0), 0.0, 0.0], [0.0, 0.0, -n * s]])
    Pvv = np.array([[c, 2.0 * s, 0.0], [-2.0 * s, 4.0 * c - 3.0, 0.0], [0.0, 0.0, c]])
    return Prr, Prv, Pvr, Pvv


def _cw_drift_v(r_lvlh, w_deg_day):
    """Hill/CW 精确能量积分漂移速度：y_dot_sec = -6 n x - 3 v_y => v_y = -2 n x - y_dot_sec / 3。"""
    ydot_sec = w_deg_day * _DEG_DAY_TO_RAD_S * AGEO
    vy = -2.0 * N_GEO * r_lvlh[0] - ydot_sec / 3.0
    return np.array([0.0, vy, 0.0])


def cw_2burn_dv(r1, v1, r2, v2, dt):
    """两脉冲（t1 与 t1 + 0.53*dt）精确三维 CW 最小范数解析转移代价 (m/s)。"""
    Prr, Prv, Pvr, Pvv = _cw_stm(dt)
    _, Prv2, _, Pvv2 = _cw_stm(0.47 * dt)
    rf = Prr @ r1 + Prv @ v1
    vf = Pvr @ r1 + Pvv @ v1
    A = np.empty((4, 6))
    A[:3, :3] = Prv
    A[:3, 3:] = Prv2
    A[3, :3] = Pvv[1, :] * 3000.0
    A[3, 3:] = Pvv2[1, :] * 3000.0
    b = np.empty(4)
    b[:3] = r2 - rf
    b[3] = (v2[1] - vf[1]) * 3000.0
    u = A.T @ np.linalg.solve(A @ A.T + 1e-4 * np.eye(4), b)
    dv1 = math.sqrt(u[0]*u[0] + u[1]*u[1] + u[2]*u[2]) * 1000.0
    dv2 = math.sqrt(u[3]*u[3] + u[4]*u[4] + u[5]*u[5]) * 1000.0
    return dv1, dv2


def eval_seq_cw_dp(seq_names, ev_by_tgt=None):
    if ev_by_tgt is None:
        ev_by_tgt = _G_EV_BY_TGT
    n_st = len(seq_names)
    dp = [{} for _ in range(n_st)]

    r0_lvlh = np.zeros(3)
    v0_lvlh = np.zeros(3)
    evs0 = ev_by_tgt[seq_names[0]]
    for ei, ev in enumerate(evs0):
        t = ev['t']
        dt_d = t / 86400.0
        if dt_d < 0.25 or dt_d > 4.2:
            continue
        rc = ev['lvlh']
        for wi, wq in enumerate(W_LEVELS):
            if abs(wq) > ev['wmax'] + 0.05:
                continue
            vc = _cw_drift_v(rc, wq)
            dv1, dv2 = cw_2burn_dv(r0_lvlh, v0_lvlh, rc, vc, t)
            if dv1 <= 29.0 and dv2 <= 29.0:
                dp[0][(ei, wi)] = (dv1 + dv2, -1, -1)
    if not dp[0]:
        return 1e9, None

    for k in range(1, n_st):
        evs_prev = ev_by_tgt[seq_names[k - 1]]
        evs_cur = ev_by_tgt[seq_names[k]]
        min_t = (k - 1) * 0.12 * 86400.0
        max_t = T_END - (n_st - 1 - k) * 0.12 * 86400.0
        for ei_c, ev_c in enumerate(evs_cur):
            tc = ev_c['t']
            if tc < min_t or tc > max_t:
                continue
            rc = ev_c['lvlh']
            valid_wi = [wi for wi, wq in enumerate(W_LEVELS) if abs(wq) <= ev_c['wmax'] + 0.05] or [4]
            vc_map = {wi: _cw_drift_v(rc, W_LEVELS[wi]) for wi in valid_wi}
            for (ei_p, wi_p), (c_prev, _, _) in dp[k - 1].items():
                ev_p = evs_prev[ei_p]
                dt_s = tc - ev_p['t']
                dt_d = dt_s / 86400.0
                if dt_d < 0.003 or dt_d > 5.0:
                    continue
                rp = ev_p['lvlh']
                vp = _cw_drift_v(rp, W_LEVELS[wi_p])
                # 精确三维 CW 自由漂移伴随飞越判据（无先验）
                if dt_d < 0.02:
                    Prr, Prv, _, _ = _cw_stm(dt_s)
                    r_coast = Prr @ rp + Prv @ vp
                    if float(np.linalg.norm(rc - r_coast)) < 42.0:
                        for wi_c in valid_wi:
                            if wi_c == wi_p:
                                key = (ei_c, wi_c)
                                if key not in dp[k] or c_prev < dp[k][key][0]:
                                    dp[k][key] = (c_prev, ei_p, wi_p)
                    continue
                if dt_d < 0.14:
                    continue
                w_bar = (ev_c['phi'] - ev_p['phi']) / dt_d
                if abs(w_bar) > 5.0 or abs(w_bar - W_LEVELS[wi_p]) > 5.5:
                    continue
                # 直接 O(1) 查表取预计算 CW 伪逆矩阵（零矩阵求逆开销！）
                m_idx = int(dt_s / 60.0 + 0.5)
                if m_idx < 1 or m_idx >= len(_G_CW_LUT):
                    continue
                Prr, Prv, Pvr1, Pvv1, Ap = _G_CW_LUT[m_idx]
                rf = Prr @ rp + Prv @ vp
                vfy = float(Pvr1 @ rp + Pvv1 @ vp)
                dr = rc - rf
                # 预计算 Ap[:, :3] @ dr，循环内只需加 1 列！
                u_base = Ap[:, :3] @ dr
                ap3 = Ap[:, 3]
                for wi_c in valid_wi:
                    b3 = (vc_map[wi_c][1] - vfy) * 3000.0
                    u = u_base + ap3 * b3
                    dv1 = math.sqrt(u[0]*u[0] + u[1]*u[1] + u[2]*u[2]) * 1000.0
                    if dv1 > 29.0:
                        continue
                    dv2 = math.sqrt(u[3]*u[3] + u[4]*u[4] + u[5]*u[5]) * 1000.0
                    if dv2 > 29.0:
                        continue
                    tot = c_prev + dv1 + dv2
                    key = (ei_c, wi_c)
                    if key not in dp[k] or tot < dp[k][key][0]:
                        dp[k][key] = (tot, ei_p, wi_p)
        if not dp[k]:
            return 1e9, None

    best_key = min(dp[-1], key=lambda k: dp[-1][k][0])
    best_cost = dp[-1][best_key][0]
    path = []
    cur = best_key
    for k in range(n_st - 1, -1, -1):
        ei, wi = cur
        ev = ev_by_tgt[seq_names[k]][ei]
        _, pei, pwi = dp[k][cur]
        path.append((ev, W_LEVELS[wi]))
        cur = (pei, pwi)
    path.reverse()
    return best_cost, path


def _eval_batch_seqs(seqs):
    best_c = 1e9
    best_s = None
    for s in seqs:
        c, _ = eval_seq_cw_dp(s)
        if c < best_c:
            best_c = c
            best_s = s
    return best_c, best_s


def parallel_multistart_cw_vnd(starts, ev_by_tgt):
    global _G_EV_BY_TGT
    _G_EV_BY_TGT = ev_by_tgt
    t0 = time.time()
    overall_best = (1e9, None, None)

    with ProcessPoolExecutor(max_workers=N_WORKERS) as ex:
        def eval_cands(cands):
            if not cands:
                return 1e9, None
            chunk = max(1, (len(cands) + N_WORKERS - 1) // N_WORKERS)
            batches = [cands[i:i + chunk] for i in range(0, len(cands), chunk)]
            res = list(ex.map(_eval_batch_seqs, batches))
            return min(res, key=lambda x: x[0])

        for sid, (sname, init_seq) in enumerate(starts):
            cur_seq = list(init_seq)
            cur_cost, cur_path = eval_seq_cw_dp(cur_seq, ev_by_tgt)
            n = len(cur_seq)
            ts = time.time()
            print(f"[CW-VND-DP Start {sid+1}/{len(starts)} ({sname})] 初始={cur_cost:.2f} m/s", flush=True)

            improved = True
            rnd = 0
            while improved:
                improved = False
                rnd += 1
                # 1. Full-Range Or-opt
                cands = []
                for i in range(n):
                    item = cur_seq[i]
                    rem = cur_seq[:i] + cur_seq[i + 1:]
                    for j in range(n):
                        if j != i:
                            cands.append(rem[:j] + [item] + rem[j:])
                bc, bs = eval_cands(cands)
                if bc < cur_cost - 1e-4:
                    cur_seq = bs
                    cur_cost, cur_path = eval_seq_cw_dp(cur_seq, ev_by_tgt)
                    print(f"    轮 {rnd:2d} Or-opt: {cur_cost:.2f} m/s ({time.time()-ts:.1f}s)", flush=True)
                    improved = True
                    continue

                # 2. All-Pairs Swap
                cands = []
                for i in range(n - 1):
                    for j in range(i + 1, n):
                        cand = list(cur_seq)
                        cand[i], cand[j] = cand[j], cand[i]
                        cands.append(cand)
                bc, bs = eval_cands(cands)
                if bc < cur_cost - 1e-4:
                    cur_seq = bs
                    cur_cost, cur_path = eval_seq_cw_dp(cur_seq, ev_by_tgt)
                    print(f"    轮 {rnd:2d} Swap  : {cur_cost:.2f} m/s ({time.time()-ts:.1f}s)", flush=True)
                    improved = True
                    continue

                # 3. 2-opt
                cands = []
                for i in range(n - 1):
                    for j in range(i + 2, min(n + 1, i + 12)):
                        cands.append(cur_seq[:i] + cur_seq[i:j][::-1] + cur_seq[j:])
                bc, bs = eval_cands(cands)
                if bc < cur_cost - 1e-4:
                    cur_seq = bs
                    cur_cost, cur_path = eval_seq_cw_dp(cur_seq, ev_by_tgt)
                    print(f"    轮 {rnd:2d} 2-opt : {cur_cost:.2f} m/s ({time.time()-ts:.1f}s)", flush=True)
                    improved = True

            print(f"  -> Start {sid+1} 收敛 ({rnd} 轮, {time.time()-ts:.1f}s): 3D-CW DP Δv = {cur_cost:.2f} m/s", flush=True)
            if cur_cost < overall_best[0]:
                overall_best = (cur_cost, cur_seq, cur_path)

    print(f"[Stage 1 完成] 3D-CW 多起点全局最优 DP Δv = {overall_best[0]:.2f} m/s, 总耗时 {time.time()-t0:.2f}s", flush=True)
    print("  最优序列:", " -> ".join(overall_best[1]), flush=True)
    return overall_best[1], overall_best[0], overall_best[2]


def build_clean_2burn_init(path):
    burns = []
    obs = []
    r, v, tc = R1[:3].copy(), R1[3:].copy(), 0.0
    prev_end = 0.0
    prev_burn_t = -3600.0

    for idx, (ev, _) in enumerate(path):
        tname = ev['targets'][0]
        t_obs = ev['t'] - 30.0
        t_aim = ev['t']

        t_lo = 0.0 if idx == 0 else max(prev_end + 10.0, prev_burn_t + 3605.0)
        avail = (t_obs - 10.0) - t_lo

        if avail < 1800.0:
            print(f"  [Init] 段 {idx:2d} ({tname}): 3D 伴随飞越 (0 脉冲)", flush=True)
            obs.append([tname, t_obs])
            prev_end = max(prev_end, t_obs + 60.5)
            continue

        if avail < 7210.0:
            t_grid = [t_lo]
        else:
            t2 = t_lo + max(3605.0, min(avail - 3600.0, 0.53 * avail))
            t_grid = [t_lo, t2]

        R_lo, V_lo = kepler(r, v, t_grid[0] - tc)
        r1_, v1_ = R_lo[0], V_lo[0]
        rt = tpos(tname, t_aim)[0][0]
        sc = SUN(t_aim)
        us = (sc - rt) / np.linalg.norm(sc - rt)
        ue = -rt / np.linalg.norm(rt)
        g = us + ue
        g /= np.linalg.norm(g)
        r_aim = rt + 25.0 * g

        dvs = [np.zeros(3) for _ in t_grid]
        if len(t_grid) == 1:
            for _ in range(3):
                rf, Phi1 = stm_fd(r1_, v1_ + dvs[0], t_aim - t_grid[0])
                delta = Phi1.T @ np.linalg.solve(Phi1 @ Phi1.T + 1e-4 * np.eye(3), r_aim - rf)
                dvs[0] += delta
                if np.linalg.norm(dvs[0]) > 0.028:
                    dvs[0] *= 0.028 / np.linalg.norm(dvs[0])
        else:
            t1, t2 = t_grid[0], t_grid[1]
            for _ in range(3):
                R2_, V2_ = kepler(r1_, v1_ + dvs[0], t2 - t1)
                rf, Phi2 = stm_fd(R2_[0], V2_[0] + dvs[1], t_aim - t2)
                _, Phi1 = stm_fd(r1_, v1_ + dvs[0], t_aim - t1)
                J = np.hstack([Phi1, Phi2])
                delta = J.T @ np.linalg.solve(J @ J.T + 1e-4 * np.eye(3), r_aim - rf)
                dvs[0] += delta[:3]
                dvs[1] += delta[3:]
                if np.linalg.norm(dvs[0]) > 0.028:
                    dvs[0] *= 0.028 / np.linalg.norm(dvs[0])
                if np.linalg.norm(dvs[1]) > 0.028:
                    dvs[1] *= 0.028 / np.linalg.norm(dvs[1])

        for tk, dvk in zip(t_grid, dvs):
            burns.append([float(tk), dvk * 1000.0, idx])
            Rk, Vk = kepler(r, v, tk - tc)
            r, v, tc = Rk[0], Vk[0] + dvk, tk
        prev_burn_t = t_grid[-1]
        obs.append([tname, t_obs])
        prev_end = t_obs + 60.5

    tot_init = sum(np.linalg.norm(b[1]) for b in burns)
    print(f"[Init] 2-脉冲/段 STM 初值: {len(burns)} 脉冲 (30 观测), 初值 Δv = {tot_init:.2f} m/s", flush=True)
    return burns, obs


def _jac_worker(k_list):
    b_simple, T, r0, opt_time = _G_JAC_CTX
    ns = len(T)
    eps_v = 1e-4
    eps_t = 1.0
    out = []
    bs = [[b[0], b[1].copy()] for b in b_simple]
    for k in k_list:
        bt = bs[k][0]
        mask = T > bt
        jv = np.zeros((ns, 3, 3))
        jt = np.zeros((ns, 3)) if opt_time else None
        if np.any(mask):
            Tm = T[mask]
            r0m = r0[mask]
            for c_i in range(3):
                bs[k][1][c_i] += eps_v
                rp = positions(bs, Tm)
                bs[k][1][c_i] -= eps_v
                jv[mask, :, c_i] = (rp - r0m) / eps_v
            if opt_time:
                bs[k][0] = bt + eps_t
                rp = positions(bs, Tm)
                bs[k][0] = bt
                jt[mask, :] = (rp - r0m) / eps_t
        out.append((k, jv, jt))
    return out


def compute_jacobians_parallel(b_simple, T, r0, opt_time=False):
    global _G_JAC_CTX
    _G_JAC_CTX = (b_simple, T, r0, opt_time)
    nb = len(b_simple)
    ns = len(T)
    Jv = np.zeros((ns, 3, nb * 3))
    Jt = np.zeros((ns, 3, nb)) if opt_time else None

    chunk = max(1, (nb + N_WORKERS - 1) // N_WORKERS)
    tasks = [list(range(i, min(nb, i + chunk))) for i in range(0, nb, chunk)]
    with ProcessPoolExecutor(max_workers=N_WORKERS) as ex:
        for res in ex.map(_jac_worker, tasks):
            for k, jv, jt in res:
                Jv[:, :, 3 * k:3 * k + 3] = jv
                if opt_time:
                    Jt[:, :, k] = jt
    return Jv, Jt


def run_vc_scp(burns, obs, iters=30, md=0.05, ma=0.05, dur=60.5, slide_until=8,
               opt_time=False, tr_init=3.0, tr_t_init=360.0, merit_guard=False, label="SCP"):
    t_start = time.time()
    nb = len(burns)
    no = len(obs)
    ref_centers = [s + 30.0 for _, s in obs]
    ca = math.cos(math.radians(45.0 - ma))
    dmax, dmin = 50.0 - md, 10.0 + md

    tr = tr_init
    tr_t = tr_t_init
    best = None
    stall = 0

    for it in range(iters):
        t0 = time.time()
        if it <= slide_until and (it <= 3 or it % 2 == 0):
            obs = slide_obs_windows([[b[0], b[1]] for b in burns], obs, ref_centers, dur=dur)

        T_list, idx_list = [], []
        for j, (name, s) in enumerate(obs):
            for q in np.linspace(0.0, dur, 5):
                T_list.append(s + q)
                idx_list.append(j)
        order = np.argsort(T_list)
        T = np.array(T_list)[order]
        idx = np.array(idx_list)[order]
        ns = len(T)

        rT = np.array([tpos(obs[j][0], t)[0][0] for t, j in zip(T, idx)])
        us = unit(SUN(T) - rT)
        ue = unit(-rT)
        ghat_bis = unit(us + ue)

        b_simple = [[b[0], b[1]] for b in burns]
        r0 = positions(b_simple, T)
        rho0 = r0 - rT

        Jv, Jt = compute_jacobians_parallel(b_simple, T, r0, opt_time=opt_time)

        dx = cp.Variable(nb * 3)
        dt = cp.Variable(nb) if opt_time else None
        sl = cp.Variable(ns, nonneg=True)
        dv0 = np.array([b[1] for b in burns])
        t0_arr = np.array([b[0] for b in burns])

        obj_dv = sum(cp.norm(dv0[k] + dx[3 * k:3 * k + 3]) for k in range(nb))
        pen_w = 800.0 if (it < 4 and not merit_guard) else 5000.0
        cons = [cp.norm_inf(dx) <= tr]
        for k in range(nb):
            cons.append(cp.norm(dv0[k] + dx[3 * k:3 * k + 3]) <= 29.9)

        if opt_time:
            cons.append(cp.norm_inf(dt) <= tr_t)
            cons.append(t0_arr[0] + dt[0] >= 0.0)
            for k in range(nb - 1):
                cons.append((t0_arr[k + 1] + dt[k + 1]) - (t0_arr[k] + dt[k]) >= 3601.0)
            for k in range(nb):
                lid = burns[k][2]
                cons.append(t0_arr[k] + dt[k] <= obs[lid][1] - 15.0)
                if lid > 0:
                    cons.append(t0_arr[k] + dt[k] >= obs[lid - 1][1] + dur + 5.0)

        for i in range(ns):
            rho_i = rho0[i] + Jv[i] @ dx + (Jt[i] @ dt if opt_time else 0)
            rn = float(np.linalg.norm(rho0[i]))
            g_i = (rho0[i] / rn) if (rn > 5.0 and float(np.dot(rho0[i] / rn, ghat_bis[i])) > 0.35) else ghat_bis[i]
            cons.append(cp.norm(rho_i) <= dmax + sl[i])
            cons.append(g_i @ rho_i >= dmin - sl[i])
            cons.append(cp.norm(rho_i) * ca <= us[i] @ rho_i + sl[i])
            cons.append(cp.norm(rho_i) * ca <= ue[i] @ rho_i + sl[i])

        prob = cp.Problem(cp.Minimize(obj_dv + pen_w * cp.sum(sl)), cons)
        try:
            prob.solve(solver=cp.CLARABEL, tol_gap_abs=1e-6, tol_gap_rel=1e-6, max_iter=150)
        except Exception:
            tr *= 0.5
            tr_t *= 0.5
            continue
        if dx.value is None:
            tr *= 0.5
            tr_t *= 0.5
            continue

        xval = dx.value.reshape(nb, 3)
        dtval = dt.value if opt_time else np.zeros(nb)
        cand = [[float(b[0] + dtval[k]), b[1] + xval[k], b[2]] for k, b in enumerate(burns)]
        tot_new = sum(np.linalg.norm(b[1]) for b in cand)
        sl_max = float(np.max(sl.value))
        n_ok, bad = certify_all([[b[0], b[1]] for b in cand], obs, step=0.05)
        n_active = sum(np.linalg.norm(b[1]) > 1e-2 for b in cand)
        extra = f", max|dt|={np.max(np.abs(dtval)):.0f}s" if opt_time else ""

        if merit_guard and best is not None and n_ok < no - 3 and sl_max > 0.5:
            print(f"  [{label}] it {it:2d}: 拒绝大步(认证={n_ok}, slack={sl_max:.2f}), 收缩信任域", flush=True)
            tr *= 0.5
            tr_t *= 0.5
            continue

        print(f"  [{label}] it {it:2d}: Δv={tot_new:7.3f} m/s, 有效={n_active:2d}/{nb}, slack={sl_max:7.4f}, 认证={n_ok:2d}/{no}, tr={tr:.3f}{extra}, {time.time()-t0:.1f}s"
              + ("" if bad else " ✓ALL_30_OK"), flush=True)

        burns = cand
        if n_ok == no and (best is None or tot_new < best[0] - 1e-4):
            best = (tot_new, [[b[0], b[1].copy(), b[2]] for b in burns], [o[:] for o in obs])
            stall = 0
        else:
            stall += 1
        if best is not None and stall >= 6:
            break
        tr = max(tr * (0.86 if it < slide_until + 2 else 0.84), 0.02)
        tr_t = max(tr_t * 0.86, 10.0)

    print(f"[{label}] 完成, 耗时 {time.time()-t_start:.1f}s, 最优={best[0] if best else None}", flush=True)
    return best, burns, obs


if __name__ == '__main__':
    T_ALL = time.time()
    events, super_ev, tg_delta = generate_events()
    ev_by_tgt = defaultdict(list)
    for ev in events:
        if len(ev['targets']) == 1:
            ev_by_tgt[ev['targets'][0]].append(ev)
    for k in ev_by_tgt:
        ev_by_tgt[k].sort(key=lambda e: e['t'])
    attach_lvlh_states(ev_by_tgt)
    _G_CW_LUT = build_cw_lut(7205)

    h1, e1, e2, _ = build_r1_frame()
    phi0 = {name: float(phase_in_r1(x[:3], 0.0, e1, e2)) for name, x in TG.items()}
    all_names = sorted(TG.keys())
    west = sorted([k for k in all_names if phi0[k] < 0], key=lambda k: -phi0[k])
    east = sorted([k for k in all_names if phi0[k] >= 0], key=lambda k: phi0[k])
    s_alt = west[::2] + west[1::2][::-1] + east

    starts = [
        ("west+east", west + east),
        ("alt-sweep", s_alt),
    ]

    # Stage 1: 24 核并行 3D-CW 能量积分两脉冲精确动态规划
    best_seq, best_dp_cost, best_path = parallel_multistart_cw_vnd(starts, ev_by_tgt)

    # Stage 2A: 先用固定时刻 VC-SCP 稳健收敛到 slack=0 与 30/30 严格可行域！
    init_burns, init_obs = build_clean_2burn_init(best_path)
    best_2a, cur_burns, cur_obs = run_vc_scp(
        init_burns, init_obs, iters=30, md=0.05, ma=0.05, slide_until=8,
        opt_time=False, tr_init=3.0, merit_guard=False, label="Stage2A-FixedT"
    )

    # Stage 2B: 从 30/30 可行解出发，温和联合优化 (Δv, Δt)
    src_burns = best_2a[1] if best_2a else cur_burns
    src_obs = best_2a[2] if best_2a else cur_obs
    best_2b, cur_burns2, cur_obs2 = run_vc_scp(
        src_burns, src_obs, iters=22, md=0.05, ma=0.05, slide_until=0,
        opt_time=True, tr_init=1.0, tr_t_init=360.0, merit_guard=True, label="Stage2B-Joint-dVdt"
    )

    # Stage 2C: 锁定最优脉冲时刻 t_k，做最终固定时刻精修，确保 30/30 严格认证！
    src_burns3 = best_2b[1] if best_2b else cur_burns2
    src_obs3 = best_2b[2] if best_2b else cur_obs2
    best_final, _, _ = run_vc_scp(
        src_burns3, src_obs3, iters=15, md=0.05, ma=0.05, slide_until=0,
        opt_time=False, tr_init=0.4, merit_guard=False, label="Stage2C-Polish"
    )

    cands = [x for x in [best_2a, best_2b, best_final] if x is not None]
    best_all = min(cands, key=lambda x: x[0]) if cands else None

    print(f"=== M2-D+ v5 (3D-CW-VND-DP + FixedT + Gentle Joint dV/dt VC-SCP) 端到端总耗时: {time.time()-T_ALL:.1f}s ===", flush=True)
    if best_all is not None:
        tot_dv, b_fin, o_fin = best_all
        n_act = sum(np.linalg.norm(b[1]) > 1e-2 for b in b_fin)
        print(f"=== 最终 0.05s 全认证 30/30 最优解: Δv = {tot_dv:.4f} m/s, 有效脉冲 = {n_act}/{len(b_fin)} ===", flush=True)
        out_path = '/root/autodl-tmp/m2d/m2d_plus_v5_solution.json'
        sol_burns = [{"t_s": float(b[0]), "dv_km_s": (b[1] / 1000.0).tolist(), "for": best_seq[b[2]]} for b in b_fin]
        sol_obs = [{"target": n, "start_s": float(s), "duration_s": 60.5} for n, s in o_fin]
        json.dump({
            "problem": "P2", "model": "M2-D+ v5 (3D-CW-VND-DP + FixedT + Joint dV/dt VC-SCP)",
            "epoch_utc": "2027-01-01T00:00:00", "mu_km3_s2": MU,
            "r1_km": R1.tolist(), "targets_km": {k: v.tolist() for k, v in TG.items()},
            "burns": sol_burns, "observations": sol_obs,
            "summary": {"n_targets": len({o["target"] for o in sol_obs}),
                        "total_dv_m_s": float(tot_dv),
                        "n_active_burns": int(n_act),
                        "elapsed_s": float(time.time() - T_ALL)}
        }, open(out_path, 'w'), indent=1)
        print("Saved:", out_path)
