"""生成方案的粗采样查看场景（段结构与 build_single_r1.py 一致，星历步长可调）。

官方 build_single_r1.py 输出的场景是密采样（30 天 × 全部段 × 默认步长），动辄 1.6–2.8 GB，
ATK 界面加载不动。本脚本用同样的段序列（含 t=0 首脉冲并入初始速度、VNC 轴名、二体力模型、
末端 600 s 余量），只把目标星和 MCS 的星历步长调稀，产出几十 MB 的查看版。
二体轨迹与采样密度无关，段名、段数、轨迹与官方输出一致；数值结论一律以官方
verify_*/summary.md 为准，本脚本只用于界面截图和演示。

用法（仓库根目录，已设置 ATK_HOME，关闭 ATK 界面）：
    python tools/make_view_scene.py solutions/q3/v6_p3_r2.json r2 results/q3/single_v6_r2/raw_view/v6_r2_view.atk --step 60
参数：方案文件、卫星名（如 r1 / r2）、输出 .atk 路径、星历步长（秒，默认 60，
目标星步长固定为它的 10 倍）。
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
from verify_p2 import kepler  # noqa: E402


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("solution", help="方案 JSON 路径")
    ap.add_argument("satname", help="机动卫星名（r1 / r2 等）")
    ap.add_argument("out", help="输出 .atk 路径")
    ap.add_argument("--step", type=float, default=60.0, help="MCS 星历步长（秒），默认 60")
    ap.add_argument("--upper", action="store_true",
                    help="卫星名和目标名全部转大写（r1→R1，b12→B12），论文截图用")
    ap.add_argument("--scenario-name", default="SingleR1", help="场景名，默认 SingleR1")
    a = ap.parse_args()

    satname = a.satname.upper() if a.upper else a.satname

    sol = json.loads(Path(a.solution).read_text(encoding="utf-8"))
    mu = sol["mu_km3_s2"]
    if sol.get("problem") == "P2":  # 第二问 / 第三问格式
        r0 = np.array(sol["r1_km"])
        targets = {k: np.array(v) for k, v in sol["targets_km"].items()}
        burns = sorted([(b["t_s"], np.array(b["dv_km_s"])) for b in sol["burns"]], key=lambda b: b[0])
        t_end = 2592000.0
    else:  # 第一问格式（r1_used_km / b1_km / maneuvers）
        r0 = np.array(sol["r1_used_km"])
        targets = {"b1": np.array(sol["b1_km"])}
        burns = sorted([(b["t_s"], np.array(b["dv_km_s"])) for b in sol["maneuvers"]], key=lambda b: b[0])
        t_end = 172800.0
    ep = datetime.fromisoformat(sol["epoch_utc"])
    fmt = lambda x: (lambda dt: f"{dt.day} {dt.strftime('%b %Y')} {dt.strftime('%H:%M:%S')}.{dt.microsecond:06d}")(ep + timedelta(seconds=float(x)))  # noqa: E731

    def replay(T):
        r, v, tc = r0[:3].copy(), r0[3:].copy(), 0.0
        for tb, dv in burns:
            if tb > T:
                break
            R, V = kepler(r, v, tb - tc, mu)
            r, v, tc = R[0], V[0] + dv, tb
        R, V = kepler(r, v, T - tc, mu)
        return R[0], V[0]

    out = Path(a.out).resolve()  # bridge 会把工作目录切到 ATK 安装目录，必须先转绝对路径
    out.parent.mkdir(parents=True, exist_ok=True)
    atk = ATK(out_dir=out.parent / "raw_view")
    m = atk.m
    km = lambda x: [y * 1000 for y in x]  # noqa: E731
    atk.new(a.scenario_name, fmt(0), fmt(t_end))
    for name in sorted(targets):
        tg = targets[name]
        tname = name.upper() if a.upper else name
        atk.add_satellite_cartesian(tname, fmt(0), km(tg[:3]), km(tg[3:]), step_s=a.step * 10)

    sat = atk.scenario().GetChildren().New(m.eSatellite, satname)
    sat.SetPropagatorType(m.ePropagatorAstromaster)
    mcs = sat.GetPropagator()
    try:
        mcs.SetStep(float(a.step))
    except Exception:
        pass
    segs = mcs.GetMainSequence()
    init = segs.Item(0)
    init.SetOrbitEpoch(fmt(0))
    init.SetElementType(m.eVAElementTypeCartesian)
    el = init.GetElement()
    state0 = r0.copy()
    seq = list(zip(burns, [replay(tb - 1e-6) for tb, _ in burns]))
    if seq and seq[0][0][0] < 1e-3:
        state0[3:] += seq[0][0][1]
        seq = seq[1:]
        print("首脉冲在历元时刻，已并入初始状态速度")
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
                pr.SetStep(float(a.step))
            except Exception:
                pass
        except Exception:
            pass
        sc = p.GetStoppingConditions().Add("Duration").GetProperties()
        sc.SetTrip(float(dur))
        sc.SetTolerance(1e-6)

    tc = 0.0
    for k, ((tb, dv), (r, v)) in enumerate(seq, 1):
        propagate(f"Coast{k:02d}", tb - tc)
        mv = segs.Insert(m.eVASegmentTypeManeuver, f"Burn{k:02d}", "-")
        mv.SetManeuverType(m.eVAManeuverTypeImpulsive)
        ac = mv.GetManeuver().GetAttitudeControl()
        ac.SetThrustAxesName(f"Satellite/{satname} VNC(Earth)")
        V = v / np.linalg.norm(v)
        N = np.cross(r, v); N /= np.linalg.norm(N)
        C = np.cross(V, N)
        c = np.array([dv @ V, dv @ N, dv @ C]) * 1000
        ac.AssignCartesian(float(c[0]), float(c[1]), float(c[2]))
        tc = tb
    propagate("CoastEnd", t_end - tc + 600)
    mcs.RunMCS()
    print(f"RunMCS 完成，段数：{segs.GetCount()}")

    atk.save(out)
    print(f"已保存 {out}（{out.stat().st_size} bytes）")
    atk.close()


if __name__ == "__main__":
    main()
