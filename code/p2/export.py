"""把 realize.py 的输出转成 solutions/ 下的方案文件。用法：python code/p2/export.py <realized.json> <out.json>"""
import json, sys, os
import numpy as np
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from eng import R1, TG, MU
L = json.load(open(sys.argv[1]))
burns = []
for x in L:
    burns.append({"t_s": x['t1'], "dv_km_s": x['dv1'], "for": x['target']})
    burns.append({"t_s": x['t2'], "dv_km_s": x['dv2'], "for": x['target']})
burns.sort(key=lambda b: b['t_s'])
obs = [{"target": x['target'], "start_s": x['obs_start'], "duration_s": 62.0,
        "claimed": {k: x[k] for k in ('d_min', 'd_max', 'alpha_max', 'theta_max')}} for x in L]
tot = sum(np.linalg.norm(b['dv_km_s']) for b in burns) * 1000
sol = {"problem": "P2", "epoch_utc": "2027-01-01T00:00:00", "mu_km3_s2": MU, "r1_km": R1.tolist(),
       "targets_km": {k: v.tolist() for k, v in TG.items()}, "burns": burns, "observations": obs,
       "summary": {"n_targets": len(obs), "total_dv_m_s": tot, "margins": {"distance_km": 0.5, "angle_deg": 0.5, "duration_s": 62}},
       "note": "code/p2 求解：解析漂移模型排日程（sched.py）+ 逐段真实二体动力学 SLSQP（realize.py）；每次观测最多 2 次脉冲；太阳为 ERFA 几何星历"}
json.dump(sol, open(sys.argv[2], 'w', encoding='utf-8'), ensure_ascii=False, indent=1)
print(len(obs), 'targets', round(tot, 4), 'm/s')
