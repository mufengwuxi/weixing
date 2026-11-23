"""M3-D (Q3 Joint (Δv, Δt, Δx0) VC-SCP, 24-Core Parallel):
在问题三（R1 + R2 双星协同巡视）上应用联合脉冲时刻-矢量-初始轨道序列凸优化：
  - R1 (10 颗西侧目标，初始轨道固定): Joint (Δv, Δt) VC-SCP + 观测窗微滑 + FixedT-Polish
  - R2 (20 颗东侧目标，初始轨道可设计，满足 e < 0.001 与 |a - a_GEO| <= δa):
        Joint (Δv, Δt, Δx0) VC-SCP（在二阶锥规划中同步优化 40 次脉冲矢量 Δv_k、
        40 次脉冲施加时刻 t_k、以及 R2 初始六维状态 x0！）+ FixedT-Polish
全部在 0.05 s 精确二体网格（裕量 md=0.05 km, ma=0.05°, 认证 md=0.01, ma=0.01, e<0.001, |a-a_GEO|<=5.90 km）上严格认证！
"""
import json
import math
import os
import sys
import time
from concurrent.futures import ProcessPoolExecutor

import cvxpy as cp
import numpy as np

sys.path.insert(0, os.environ.get('P2LIB_DIR', '/root/autodl-tmp/p2_newdata/batch'))
sys.path.insert(0, '/root/autodl-tmp/m2d')
from p2lib import MU, R1, SUN, TG, kepler, tpos  # noqa: E402
from se_milp_vcscp import AGEO, unit  # noqa: E402

N_WORKERS = min(24, os.cpu_count() or 24)
DUR = 60.5
_G_CTX = None


def elem(x):
    r, v = x[:3], x[3:]
    rn = float(np.linalg.norm(r))
    a = 1.0 / (2.0 / rn - float(v @ v) / MU)
    ev = ((float(v @ v) - MU / rn) * r - float(r @ v) * v) / MU
    return a, ev


def positions_x0(burns, times, x0):
    r, v, tc = x0[:3].copy(), x0[3:].copy(), 0.0
    out = np.zeros((len(times), 3))
    bi, n = 0, len(burns)
    for i, t in enumerate(times):
        while bi < n and burns[bi][0] <= t:
            R, V = kepler(r, v, burns[bi][0] - tc)
            r, v, tc = R[0], V[0] + burns[bi][1] / 1000.0, burns[bi][0]
            bi += 1
        R, _ = kepler(r, v, t - tc)
        out[i] = R[0]
    return out


def certify_x0(burns, obs, x0, step=0.05, md=0.01, ma=0.01, da_max=None, e_max=0.001):
    # Check orbital elements of x0 if R2
    if da_max is not None:
        a, ev = elem(x0)
        if abs(a - AGEO) > da_max or float(np.linalg.norm(ev)) >= e_max:
            return 0, [f"init_orbit(da={a-AGEO:.3f},e={np.linalg.norm(ev):.6f})"]
    # Check burn rules
    ts = [b[0] for b in burns]
    if any(ts[i + 1] - ts[i] < 3600.0 - 1e-6 for i in range(len(ts) - 1)):
        return 0, ["min_gap<3600"]
    if any(np.linalg.norm(b[1]) > 30.0 for b in burns):
        return 0, ["dv>30"]
    # Check all observations on 0.05 s grid
    ca = math.cos(math.radians(45.0 - ma))
    dmax, dmin = 50.0 - md, 10.0 + md
    bad = []
    # Build sorted dense grid
    n_ok = 0
    for name, s in obs:
        T = np.arange(s, s + DUR + 1e-9, step)
        r = positions_x0(burns, T, x0)
        rT = tpos(name, T)[0]
        rho = r - rT
        d = np.linalg.norm(rho, axis=1)
        if np.max(d) > dmax or np.min(d) < dmin:
            bad.append(name)
            continue
        us = unit(SUN(T) - rT)
        ue = unit(-rT)
        if np.min(np.sum(us * rho, axis=1) / d) < ca or np.min(np.sum(ue * rho, axis=1) / d) < ca:
            bad.append(name)
            continue
        n_ok += 1
    return n_ok, bad


def _jac_worker(tasks):
    burns, T, r0, x0, opt_time, opt_x0 = _G_CTX
    ns = len(T)
    eps_v = 1e-4
    eps_t = 1.0
    eps_r = 1e-3
    eps_v0 = 1e-7
    out = []
    bs = [[b[0], b[1].copy()] for b in burns]
    for kind, k in tasks:
        if kind == 'burn':
            bt = bs[k][0]
            mask = T > bt
            jv = np.zeros((ns, 3, 3))
            jt = np.zeros((ns, 3)) if opt_time else None
            if np.any(mask):
                Tm = T[mask]
                r0m = r0[mask]
                for c_i in range(3):
                    bs[k][1][c_i] += eps_v
                    rp = positions_x0(bs, Tm, x0)
                    bs[k][1][c_i] -= eps_v
                    jv[mask, :, c_i] = (rp - r0m) / eps_v
                if opt_time:
                    bs[k][0] = bt + eps_t
                    rp = positions_x0(bs, Tm, x0)
                    bs[k][0] = bt
                    jt[mask, :] = (rp - r0m) / eps_t
            out.append(('burn', k, jv, jt))
        elif kind == 'x0':
            e = eps_r if k < 3 else eps_v0
            xp = x0.copy()
            xp[k] += e
            rp = positions_x0(bs, T, xp)
            jx = (rp - r0) / e
            out.append(('x0', k, jx, None))
    return out


def compute_jacs(burns, T, r0, x0, opt_time=True, opt_x0=False):
    global _G_CTX
    _G_CTX = (burns, T, r0, x0, opt_time, opt_x0)
    nb = len(burns)
    ns = len(T)
    Jv = np.zeros((ns, 3, nb * 3))
    Jt = np.zeros((ns, 3, nb)) if opt_time else None
    Jx0 = np.zeros((ns, 3, 6)) if opt_x0 else None

    all_tasks = [('burn', k) for k in range(nb)]
    if opt_x0:
        all_tasks += [('x0', j) for j in range(6)]
    chunk = max(1, (len(all_tasks) + N_WORKERS - 1) // N_WORKERS)
    batches = [all_tasks[i:i + chunk] for i in range(0, len(all_tasks), chunk)]
    with ProcessPoolExecutor(max_workers=N_WORKERS) as ex:
        for res in ex.map(_jac_worker, batches):
            for kind, k, j1, j2 in res:
                if kind == 'burn':
                    Jv[:, :, 3 * k:3 * k + 3] = j1
                    if opt_time:
                        Jt[:, :, k] = j2
                else:
                    Jx0[:, :, k] = j1
    return Jv, Jt, Jx0


def run_q3_scp(burns, obs, x0, opt_time=True, opt_x0=False, da_max=5.895, da_cert=5.90,
               iters=35, tr_v=0.8, tr_t=600.0, tr_r0=20.0, tr_v0=1e-3, label="Q3-SCP"):
    t_start = time.time()
    nb = len(burns)
    no = len(obs)
    md, ma = 0.05, 0.05
    ca = math.cos(math.radians(45.0 - ma))
    dmax, dmin = 50.0 - md, 10.0 + md

    # Assign leg_id to each burn based on obs order
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
            # Linearized orbital elements constraints: |a - AGEO| <= da_max, ||e|| <= 0.00095
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
        if best is not None and stall >= 7:
            break
        tr_v = max(tr_v * 0.86, 0.005)
        tr_t = max(tr_t * 0.86, 5.0)
        tr_r0 = max(tr_r0 * 0.86, 0.05)
        tr_v0 = max(tr_v0 * 0.86, 1e-6)

    print(f"[{label}] 完成, 耗时 {time.time()-t_start:.1f}s, 最优={best[0] if best else None}", flush=True)
    return best, burns, obs_sorted, x0


def load_plan(path):
    d = json.load(open(path, encoding='utf-8'))
    burns = [[float(b['t_s']), np.array(b['dv_km_s'], float) * 1000.0] for b in d['burns']]
    burns.sort(key=lambda x: x[0])
    obs = [[o['target'], float(o['start_s'])] for o in sorted(d['observations'], key=lambda x: x['start_s'])]
    x0 = np.array(d['r1_km'], float)
    return d, burns, obs, x0


def save_plan(orig_d, best, out_path, tag):
    tot_dv, b_fin, o_fin, x0_fin = best
    d = dict(orig_d)
    d['r1_km'] = x0_fin.tolist()
    d['model'] = f"M3-D ({tag})"
    # Map each burn to its leg target
    sol_burns = []
    for bt, bdv in b_fin:
        tgt = o_fin[-1][0]
        for name, s in o_fin:
            if bt <= s:
                tgt = name
                break
        sol_burns.append({"t_s": float(bt), "dv_km_s": (bdv / 1000.0).tolist(), "for": tgt})
    d['burns'] = sol_burns
    d['observations'] = [{"target": n, "start_s": float(s), "duration_s": DUR} for n, s in o_fin]
    d['summary'] = {"n_targets": len({o[0] for o in o_fin}), "total_dv_m_s": float(tot_dv)}
    json.dump(d, open(out_path, 'w'), indent=1)
    print("Saved:", out_path, f"(Δv={tot_dv:.4f} m/s)")


if __name__ == '__main__':
    T0 = time.time()
    # 1. Optimize R1 (10 west targets): Joint (Δv, Δt) -> FixedT Polish
    d1, b1, o1, x01 = load_plan('/root/autodl-tmp/m3d/v6_p3_r1.json')
    best1a, cb1, co1, cx1 = run_q3_scp(b1, o1, x01, opt_time=True, opt_x0=False, iters=32, tr_v=0.8, tr_t=900.0, label="R1-Joint-dVdt")
    src_b1 = best1a[1] if best1a else cb1
    best1b, _, _, _ = run_q3_scp(src_b1, co1, cx1, opt_time=False, opt_x0=False, iters=18, tr_v=0.25, label="R1-Polish")
    best_r1 = min([x for x in [best1a, best1b] if x is not None], key=lambda x: x[0])
    save_plan(d1, best_r1, '/root/autodl-tmp/m3d/m3d_r1.json', 'R1 Joint dV/dt VC-SCP')

    # 2. Optimize R2 (20 east targets): Joint (Δv, Δt, Δx0) -> FixedT+x0 Polish (da <= 5.90 km, e < 0.001)
    d2, b2, o2, x02 = load_plan('/root/autodl-tmp/m3d/v6_p3_r2.json')
    best2a, cb2, co2, cx2 = run_q3_scp(b2, o2, x02, opt_time=True, opt_x0=True, da_max=5.895, da_cert=5.90,
                                       iters=36, tr_v=0.8, tr_t=900.0, tr_r0=15.0, tr_v0=8e-4, label="R2-Joint-dVdtx0")
    src_b2 = best2a[1] if best2a else cb2
    src_x2 = best2a[3] if best2a else cx2
    best2b, _, _, _ = run_q3_scp(src_b2, co2, src_x2, opt_time=False, opt_x0=True, da_max=5.895, da_cert=5.90,
                                 iters=18, tr_v=0.25, tr_r0=2.0, tr_v0=1e-4, label="R2-Polish")
    best_r2 = min([x for x in [best2a, best2b] if x is not None], key=lambda x: x[0])
    save_plan(d2, best_r2, '/root/autodl-tmp/m3d/m3d_r2.json', 'R2 Joint dV/dt/x0 VC-SCP')

    a2, e2 = elem(best_r2[3])
    print(f"\n=== M3-D Q3 联合优化完成 (耗时 {time.time()-T0:.1f}s) ===", flush=True)
    print(f"  R1 (10星): 18.1475 -> {best_r1[0]:.4f} m/s", flush=True)
    print(f"  R2 (20星): 15.3121 -> {best_r2[0]:.4f} m/s (da={a2-AGEO:+.4f} km, e={np.linalg.norm(e2):.6f})", flush=True)
    print(f"  总 Δv    : 33.4596 -> {best_r1[0] + best_r2[0]:.4f} m/s (节省 {33.4596 - (best_r1[0]+best_r2[0]):.4f} m/s!)", flush=True)
