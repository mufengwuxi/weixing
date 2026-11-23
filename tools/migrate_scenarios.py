"""将问题一、问题二、问题三的最终方案直接迁移写入官方 Scenario1.atk / Scenario2.atk 场景模板中，
保留原场景的全部目标星、约束与界面配置，仅更新 R1（以及问题三新增的 R2）的 Astromaster 机动规划（MCS），
并在 ATK 4.2.0 中执行 RunMCS() 与全观测点独立复核。

用法（每个场景单独起一个进程，避免 ATK 同进程复用限制）：
    python tools/migrate_scenarios.py q1 <Scenario1.atk> <out.atk>
    python tools/migrate_scenarios.py q2 <Scenario2.atk> <out.atk>
    python tools/migrate_scenarios.py q3 <Scenario2.atk> <out.atk>
"""
import argparse
import json
import sys
from datetime import datetime, timedelta
from pathlib import Path

import numpy as np

REPO = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO / "code"))
sys.path.insert(0, str(REPO / "tools"))
from atkbridge import ATK  # noqa: E402
from build_single_r1 import load, vnc  # noqa: E402
from verify_p2 import Sun, geometry, kepler  # noqa: E402


def build_mcs_on_sat(atk, sat, satname, r0, burns, t_end, mu, fmt, step_s=60.0):
    """在已有或新建的卫星对象 sat 上写入 Astromaster 机动规划段序列并 RunMCS()。"""
    m = atk.m
    km = lambda x: [y * 1000.0 for y in x]  # noqa: E731
    conv = (1, 1, False)  # 标准 VNC，任务 14 已标定误差 0.000 m

    def replay(T):
        r, v, tc = r0[:3].copy(), r0[3:].copy(), 0.0
        for tb, dv in burns:
            if tb > T:
                break
            R, V = kepler(r, v, tb - tc, mu)
            r, v, tc = R[0], V[0] + dv, tb
        R, V = kepler(r, v, T - tc, mu)
        return R[0], V[0]

    burns = sorted(burns, key=lambda b: b[0])
    seq_burns = [(tb, dv, replay(tb - 1e-6)) for tb, dv in burns]

    sat.SetPropagatorType(m.ePropagatorAstromaster)
    mcs = sat.GetPropagator()
    try:
        mcs.SetStep(float(step_s))
    except Exception:
        pass
    segs = mcs.GetMainSequence()
    # 清空原有 InitialState 之后的所有非空段
    to_rm = []
    for i in range(1, segs.GetCount()):
        it = segs.Item(i)
        if it is not None:
            try:
                nm = it.GetName()
                if nm and nm != "-":
                    to_rm.append(nm)
            except Exception:
                pass
    for nm in to_rm:
        try:
            segs.Remove(nm)
        except Exception:
            pass

    init = segs.Item(0)
    init.SetOrbitEpoch(fmt(0))
    init.SetElementType(m.eVAElementTypeCartesian)
    el = init.GetElement()
    state0 = r0.copy()
    if seq_burns and seq_burns[0][0] < 1e-3:
        state0[3:] += seq_burns[0][1]
        seq_burns = seq_burns[1:]
        print(f"  [{satname}] 首脉冲在 t=0 历元时刻，已并入初始状态速度", flush=True)
    for f, val in zip(("SetX", "SetY", "SetZ", "SetVx", "SetVy", "SetVz"), km(state0)):
        getattr(el, f)(val)

    def propagate(label, dur):
        p = segs.Insert(m.eVASegmentTypePropagate, label, "-")
        try:
            pr = p.GetPropagator()
            pr.SetMaxDegree(0)
            pr.SetMaxOrder(0)
            pr.SetUseDrag(False)
            pr.SetUseSRP(False)
            for cb in ("Sun", "Moon"):
                try:
                    pr.SetUseTBGravity(cb, False)
                except Exception:
                    pass
            try:
                pr.SetStep(float(step_s))
            except Exception:
                pass
        except Exception:
            pass
        sc = p.GetStoppingConditions().Add("Duration").GetProperties()
        sc.SetTrip(float(dur))
        sc.SetTolerance(1e-6)
        return p

    tc = 0.0
    for k, (tb, dv, (r, v)) in enumerate(seq_burns, 1):
        propagate(f"Coast{k:02d}", tb - tc)
        mv = segs.Insert(m.eVASegmentTypeManeuver, f"Burn{k:02d}", "-")
        mv.SetManeuverType(m.eVAManeuverTypeImpulsive)
        ac = mv.GetManeuver().GetAttitudeControl()
        ac.SetThrustAxesName(f"Satellite/{satname} VNC(Earth)")
        c = vnc(r, v, dv, conv) * 1000.0
        ac.AssignCartesian(float(c[0]), float(c[1]), float(c[2]))
        tc = tb
    propagate("CoastEnd", t_end - tc + 600.0)
    mcs.RunMCS()
    return replay


def verify_sat_obs(atk, satname, obs, targets, mu, replay, t0, fmt):
    """在已加载的官方场景中按 0.2 s 网格复核所有观测窗口与解析轨迹位置差。"""
    sun = Sun(t0)
    n_ok = 0
    max_dpos = 0.0
    bad = []
    objs = {n for n, _, _ in atk.objects()}
    for name, s, D in sorted(obs, key=lambda o: o[1]):
        tname = name.upper() if name.upper() in objs else name
        tt = np.arange(s, s + D + 1e-9, 0.2)
        rO = np.array([atk.posvel(satname, fmt(x), "CBI")[0] for x in tt]) / 1000.0
        # 优先取 ATK 中目标星位置；若模板目标星未在无界面模式预传播，则用同历元二体位置核对
        p0 = atk.posvel(tname, fmt(tt[0]), "CBI")[0]
        if len(p0) == 3:
            rT = np.array([atk.posvel(tname, fmt(x), "CBI")[0] for x in tt]) / 1000.0
        else:
            tg = targets[name]
            rT = kepler(tg[:3], tg[3:], tt, mu)[0]
        dpos = float(np.linalg.norm(rO[0] - replay(s)[0]) * 1000.0)
        max_dpos = max(max_dpos, dpos)
        d, al, th = geometry(rO, rT, sun.geometric(tt))
        ok = (d.min() >= 10.0) and (d.max() <= 50.0) and (al.max() <= 45.0) and (th.max() <= 45.0)
        if ok:
            n_ok += 1
        else:
            bad.append(f"{tname}(d={d.min():.2f}-{d.max():.2f},al={al.max():.3f},th={th.max():.3f},dpos={dpos:.2f}m)")
    print(f"  [{satname}] ATK 单星 MCS 复核: {n_ok}/{len(obs)} 通过, 最大位置差 = {max_dpos:.3f} m"
          + ("" if not bad else f" | 未过: {bad}"), flush=True)
    return n_ok, len(obs), max_dpos


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("mode", choices=["q1", "q2", "q3"])
    ap.add_argument("template_atk", help="官方场景模板路径 (Scenario1.atk / Scenario2.atk)")
    ap.add_argument("out_atk", help="输出迁移后的 .atk 路径")
    ap.add_argument("--plan1", default=None, help="覆盖默认 R1 方案 JSON 路径")
    ap.add_argument("--plan2", default=None, help="覆盖默认 R2 方案 JSON 路径（仅 q3）")
    ap.add_argument("--step", type=float, default=60.0)
    a = ap.parse_args()

    tpl = Path(a.template_atk).resolve()
    out = Path(a.out_atk).resolve()
    out.parent.mkdir(parents=True, exist_ok=True)

    atk = ATK(out_dir=REPO / "results" / "atk_scenes" / f"work_{a.mode}")
    sc = atk.load(tpl)
    objs = {n for n, _, _ in atk.objects()}
    r1_name = "R1" if "R1" in objs else "r1"
    r1_sat = atk.find(r1_name)

    if a.mode == "q1":
        epoch, mu, r1, targets, burns, obs, t_end = load(REPO / "solutions/q1/p1_claude_v4.json")
        t0 = datetime.fromisoformat(epoch)
        fmt = lambda x: (lambda dt: f"{dt.day} {dt:%b %Y %H:%M:%S}.{dt.microsecond:06d}")(t0 + timedelta(seconds=float(x)))  # noqa: E731
        print(f"[Q1] 载入 {tpl.name} -> 写入 R1 (5.2128 m/s, {len(burns)} 脉冲)", flush=True)
        rep1 = build_mcs_on_sat(atk, r1_sat, r1_name, r1, burns, t_end, mu, fmt, step_s=a.step)
        verify_sat_obs(atk, r1_name, obs, targets, mu, rep1, t0, fmt)

    elif a.mode == "q2":
        p1 = Path(a.plan1) if a.plan1 else REPO / "results/q2/m2d_plus_v5_solution.json"
        epoch, mu, r1, targets, burns, obs, t_end = load(p1)
        t0 = datetime.fromisoformat(epoch)
        fmt = lambda x: (lambda dt: f"{dt.day} {dt:%b %Y %H:%M:%S}.{dt.microsecond:06d}")(t0 + timedelta(seconds=float(x)))  # noqa: E731
        print(f"[Q2] 载入 {tpl.name} -> 写入 R1 ({p1.name}, {len(burns)} 脉冲)", flush=True)
        rep1 = build_mcs_on_sat(atk, r1_sat, r1_name, r1, burns, t_end, mu, fmt, step_s=a.step)
        verify_sat_obs(atk, r1_name, obs, targets, mu, rep1, t0, fmt)

    elif a.mode == "q3":
        p1 = Path(a.plan1) if a.plan1 else REPO / "results/q3/m3d_v2_r1.json"
        p2 = Path(a.plan2) if a.plan2 else REPO / "results/q3/m3d_v2_r2.json"
        epoch, mu, r1, targets1, burns1, obs1, t_end = load(p1)
        _, _, r2, targets2, burns2, obs2, _ = load(p2)
        t0 = datetime.fromisoformat(epoch)
        fmt = lambda x: (lambda dt: f"{dt.day} {dt:%b %Y %H:%M:%S}.{dt.microsecond:06d}")(t0 + timedelta(seconds=float(x)))  # noqa: E731
        print(f"[Q3] 载入 {tpl.name} -> 写入 R1 ({p1.name}) + 新增 R2 ({p2.name})", flush=True)
        rep1 = build_mcs_on_sat(atk, r1_sat, r1_name, r1, burns1, t_end, mu, fmt, step_s=a.step)
        verify_sat_obs(atk, r1_name, obs1, targets1, mu, rep1, t0, fmt)

        r2_name = "R2" if r1_name == "R1" else "r2"
        if r2_name in objs:
            r2_sat = atk.find(r2_name)
        else:
            r2_sat = sc.GetChildren().New(atk.m.eSatellite, r2_name)
        rep2 = build_mcs_on_sat(atk, r2_sat, r2_name, r2, burns2, t_end, mu, fmt, step_s=a.step)
        verify_sat_obs(atk, r2_name, obs2, targets2, mu, rep2, t0, fmt)

    atk.save(out)
    print(f"已保存场景: {out} ({out.stat().st_size} bytes)", flush=True)
    atk.close()


if __name__ == "__main__":
    main()
