"""对多次观测的方案（第二问、第三问）做约束采样误差检验。

用法：python tools/grid_check_plan.py <方案.json> <输出.json> [观测星键名，默认 r1_km]
对每次观测：在 0.001 s 网格上算 d、α、θ 作为基准；记录 1 s 与 0.05 s 网格（两端点固定，内部点起点平移取最坏）相对基准的偏差，
以及二阶导数上界 M 给出的误差界 h²M/8。
"""
import json
import sys
from pathlib import Path

import numpy as np

REPO = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(REPO / "code/p3/v3"))
from p2lib import SUN, kepler  # noqa: E402

plan = json.load(open(sys.argv[1], encoding="utf-8"))
key = sys.argv[3] if len(sys.argv) > 3 else "r1_km"
x0 = np.array(plan[key], float)
burns = sorted(((b["t_s"], np.array(b["dv_km_s"])) for b in plan["burns"]), key=lambda b: b[0])


def obs_pos(ts):
    r, v, tc, bi, out = x0[:3].copy(), x0[3:].copy(), 0.0, 0, []
    for t in ts:
        while bi < len(burns) and burns[bi][0] <= t:
            R, V = kepler(r, v, burns[bi][0] - tc)
            r, v, tc = R[0], V[0] + burns[bi][1], burns[bi][0]
            bi += 1
        out.append(kepler(r, v, t - tc)[0][0])
    return np.array(out)


def geom(n, ts):
    tg = np.array(plan["targets_km"][n], float)
    rT = kepler(tg[:3], tg[3:], ts)[0]
    rho = obs_pos(ts) - rT
    d = np.linalg.norm(rho, axis=1)
    us = SUN(ts) - rT
    us /= np.linalg.norm(us, axis=1)[:, None]
    ue = -rT / np.linalg.norm(rT, axis=1)[:, None]
    al = np.degrees(np.arccos(np.clip(np.sum(us * rho, 1) / d, -1, 1)))
    th = np.degrees(np.arccos(np.clip(np.sum(ue * rho, 1) / d, -1, 1)))
    return d, al, th


rows = []
for o in plan["observations"]:
    n, s, D = o["target"], o["start_s"], o.get("duration_s", 60.0)
    h0 = 0.001
    tf = np.arange(s, s + D + 1e-9, h0)
    d, al, th = geom(n, tf)
    ref = dict(d_min=d.min(), d_max=d.max(), a_max=al.max(), t_max=th.max())
    M = {k: float(np.abs(np.gradient(np.gradient(f, h0), h0)[5:-5]).max()) for k, f in (("d", d), ("a", al), ("t", th))}
    err = {}
    for h in (1.0, 0.05):
        w = dict(d_min=0.0, d_max=0.0, a_max=0.0, t_max=0.0)
        for off in np.linspace(0, h, 11)[:-1]:
            tt = np.unique(np.r_[s, np.arange(s + off, s + D, h), s + D])  # 端点始终在网格上，内部点随起点平移
            dd, aa, ttt = geom(n, tt)
            w["d_min"] = max(w["d_min"], dd.min() - ref["d_min"])
            w["d_max"] = max(w["d_max"], ref["d_max"] - dd.max())
            w["a_max"] = max(w["a_max"], ref["a_max"] - aa.max())
            w["t_max"] = max(w["t_max"], ref["t_max"] - ttt.max())
        err[str(h)] = w
    rows.append(dict(target=n, ref=ref, M=M, err=err))
    print(n, f"d {ref['d_min']:.4f}-{ref['d_max']:.4f} a {ref['a_max']:.4f} t {ref['t_max']:.4f}",
          "1s err(m,deg): %.3f %.3f %.5f %.5f" % (err['1.0']['d_min'] * 1000, err['1.0']['d_max'] * 1000, err['1.0']['a_max'], err['1.0']['t_max']),
          "0.05s: %.5f %.5f %.7f %.7f" % (err['0.05']['d_min'] * 1000, err['0.05']['d_max'] * 1000, err['0.05']['a_max'], err['0.05']['t_max']), flush=True)
json.dump(rows, open(sys.argv[2], "w"), indent=1, default=float)
