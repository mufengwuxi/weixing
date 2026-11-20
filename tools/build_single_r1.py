"""在 ATK 中只用一颗 r1 卫星（机动规划 MCS）搭出整条机动序列，并复核每次观测。

r1 的传播器设为机动规划（Astromaster），段序列为：
    初始状态 → [预报段 → 机动段] × 每次脉冲 → 预报段（到任务结束）
机动段用脉冲机动，速度增量按 VNC(Earth) 坐标给出（由方案中的 ICRF Δv 换算）。
预报段只保留地球中心引力（二体），与题目模型一致。目标星仍是各自独立的二体卫星。

用法（仓库根目录，已设置 ATK_HOME，关闭 ATK 界面）：
    python tools/build_single_r1.py solutions/p1_claude_robust.json --out results/single_r1_p1
    python tools/build_single_r1.py solutions/p2_claude_30.json --out results/single_r1_p2
输出：<out>/summary.md（每一步的诊断、每次观测的检查）、<out>/raw/ 下的场景文件 single_r1.atk（可在 ATK 界面打开）
"""

import argparse
import json
import sys
import traceback
from datetime import datetime, timedelta
from pathlib import Path

import numpy as np

REPO = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO / "code"))
sys.path.insert(0, str(REPO / "tools"))
from verify_p2 import Sun, geometry, kepler  # noqa: E402


def load(path):
    """统一第一问 / 第二问方案格式。"""
    sol = json.loads(Path(path).read_text(encoding="utf-8"))
    if sol.get("problem") == "P2":
        burns = [(b["t_s"], np.array(b["dv_km_s"])) for b in sol["burns"]]
        obs = [(o["target"], o["start_s"], o["duration_s"]) for o in sol["observations"]]
        return sol["epoch_utc"], sol["mu_km3_s2"], np.array(sol["r1_km"]), {k: np.array(v) for k, v in sol["targets_km"].items()}, burns, obs, 2592000.0
    burns = [(m["t_s"], np.array(m["dv_km_s"])) for m in sol["maneuvers"]]
    obs = [("b1", sol["obs_start_s"], sol.get("obs_duration_s", 60.0))]
    return sol["epoch_utc"], sol["mu_km3_s2"], np.array(sol["r1_used_km"]), {"b1": np.array(sol["b1_km"])}, burns, obs, 172800.0


def vnc(r, v, dv, conv):
    """把 ICRF 速度增量换成 VNC 分量。conv 选择 N、C 的定义（用于自动标定）。"""
    V = v / np.linalg.norm(v)
    N = np.cross(r, v)
    N /= np.linalg.norm(N)
    C = np.cross(V, N)
    sN, sC, swap = conv
    comps = np.array([dv @ V, sN * (dv @ N), sC * (dv @ C)])
    if swap:
        comps = comps[[0, 2, 1]]
    return comps


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("solution")
    ap.add_argument("--out", default="results/single_r1")
    a = ap.parse_args()
    epoch, mu, r1, targets, burns, obs, t_end = load(a.solution)
    burns.sort(key=lambda b: b[0])
    out = REPO / a.out
    out.mkdir(parents=True, exist_ok=True)
    t0 = datetime.fromisoformat(epoch)
    md = []

    def log(x=""):
        print(x, flush=True)
        md.append(str(x))
        (out / "summary.md").write_text("\n".join(md), encoding="utf-8")

    fmt = lambda x: (lambda dt: f"{dt.day} {dt:%b %Y %H:%M:%S}.{dt.microsecond:06d}")(t0 + timedelta(seconds=float(x)))  # noqa: E731

    # Python 解析重放：每次脉冲前的状态，用于换算 VNC 和对比
    def replay(T):
        r, v, tc = r1[:3].copy(), r1[3:].copy(), 0.0
        for tb, dv in burns:
            if tb > T:
                break
            R, V = kepler(r, v, tb - tc, mu)
            r, v, tc = R[0], V[0] + dv, tb
        R, V = kepler(r, v, T - tc, mu)
        return R[0], V[0]

    pre = []
    for tb, dv in burns:
        r, v = replay(tb - 1e-6)
        pre.append((r, v))

    log("# 单颗 r1 的 ATK 机动规划复核")
    log()
    log(f"- 方案：`{a.solution}`；脉冲 {len(burns)} 次，观测 {len(obs)} 次")

    from atkbridge import ATK
    atk = ATK(out_dir=out / "raw")
    m = atk.m
    km = lambda x: [y * 1000 for y in x]  # noqa: E731

    def build(name, seq_burns, t_stop, conv):
        """建一颗 MCS 卫星；seq_burns 为 [(t, dv_km_s, (r,v))]。"""
        sat = atk.scenario().GetChildren().New(m.eSatellite, name)
        sat.SetPropagatorType(m.ePropagatorAstromaster)
        mcs = sat.GetPropagator()
        segs = mcs.GetMainSequence()
        init = segs.Item(0)
        init.SetOrbitEpoch(fmt(0))
        init.SetElementType(m.eVAElementTypeCartesian)
        el = init.GetElement()
        for f, val in zip(("SetX", "SetY", "SetZ", "SetVx", "SetVy", "SetVz"), km(r1)):
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
            except Exception as e:  # noqa: F841
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
            # 必须显式设为本卫星的 VNC 轴；会打出 "can not choose parent satellite's axes" 的调试信息，但轴名生效（任务 14 实测）
            ac.SetThrustAxesName(f"Satellite/{name} VNC(Earth)")
            c = vnc(r, v, dv, conv) * 1000
            ac.AssignCartesian(float(c[0]), float(c[1]), float(c[2]))
            tc = tb
        propagate("CoastEnd", t_stop - tc + 600)  # 星历末端留余量，查询时刻恰在末端会读不到
        mcs.RunMCS()
        return sat

    # 1. VNC 约定：任务 14 已在 ATK 中标定，(1, 1, False) 与解析解相差 0.000 m，直接采用
    conv = (1, 1, False)
    log()
    log("## 1. 脉冲坐标约定")
    log("- 采用标准 VNC：V 沿速度，N 沿轨道角动量，C = V × N（任务 14 标定误差 0.000 m）")

    # 2. 新建干净的场景：只有目标星和一颗 r1
    log()
    log("## 2. 搭建单颗 r1")
    atk.new("SingleR1", fmt(0), fmt(t_end))
    for name in sorted(set(o[0] for o in obs)):
        tg = targets[name]
        atk.add_satellite_cartesian(name, fmt(0), km(tg[:3]), km(tg[3:]), step_s=10.0)
    try:
        build("r1", [(t, d, p) for (t, d), p in zip(burns, pre)], t_end, conv)
        log(f"- 段序列：初始状态 + {len(burns)} × (预报段 + 机动段) + 结束预报段，RunMCS 完成")
        tchk = burns[0][0] + 3 * 3600
        e = np.linalg.norm(np.array(atk.posvel("r1", fmt(tchk), "CBI")[0]) / 1000 - replay(tchk)[0]) * 1000
        log(f"- 第一次脉冲后 3 h，ATK 与解析重放位置差 {e:.3f} m")
    except Exception:
        log("```")
        log(traceback.format_exc())
        log("```")
        atk.close()
        return

    # 3. 逐次观测检查
    log()
    log("## 3. 逐次观测检查（ATK 单颗 r1 传播，0.2 s 网格，太阳为 ERFA 几何方向）")
    log("| # | 目标 | 与解析重放位置差 (m) | 距离 (km) | 最大光照角 (°) | 最大侦测角 (°) | 满足 |")
    log("|---|---|---|---|---|---|---|")
    sun = Sun(t0)
    n_ok = 0
    for i, (name, s, D) in enumerate(sorted(obs, key=lambda o: o[1]), 1):
        tt = np.arange(s, s + D + 1e-9, 0.2)
        rO = np.array([atk.posvel("r1", fmt(x), "CBI")[0] for x in tt]) / 1000
        rT = np.array([atk.posvel(name, fmt(x), "CBI")[0] for x in tt]) / 1000
        dpos = np.linalg.norm(rO[0] - replay(s)[0]) * 1000
        d, al, th = geometry(rO, rT, sun.geometric(tt))
        ok = d.min() >= 10 and d.max() <= 50 and al.max() <= 45 and th.max() <= 45
        n_ok += ok
        log(f"| {i} | {name} | {dpos:.2f} | {d.min():.3f}–{d.max():.3f} | {al.max():.4f} | {th.max():.4f} | {'是' if ok else '**否**'} |")
    log()
    log(f"## 结论：单颗 r1 在 ATK 中复现方案，{n_ok}/{len(obs)} 次观测满足约束。")
    try:
        atk.save(out / "raw" / "single_r1.atk")
        log(f"场景已保存为 `{(out / 'raw' / 'single_r1.atk').relative_to(REPO)}`，可在 ATK 界面中打开查看 r1 的段序列。")
    except Exception as e:
        log(f"场景保存失败：{e}")
    atk.close()


if __name__ == "__main__":
    main()
