"""第二问 v3（M2-B）方案导出：python code/p2/v3/export_v3.py <计划.json> <输出.json> <说明>"""
import json
import os
import sys

import numpy as np

H = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.join(H, '..', '..', 'p3', 'v3'))  # 新数据的 states.json 与 p2lib
from p2lib import R1, TG, MU  # noqa: E402

L = json.load(open(sys.argv[1]))
burns = []
for x in L:
    if x.get('t1') is None:
        continue
    for t, d in ((x['t1'], x['dv1']), (x['t2'], x['dv2'])):
        burns.append({"t_s": t, "dv_km_s": d, "for": x['target']})
burns.sort(key=lambda b: b['t_s'])
obs = [{"target": x['target'], "start_s": x['obs_start'], "duration_s": 60.5} for x in L]
tot = sum(np.linalg.norm(b['dv_km_s']) for b in burns) * 1000
sol = {"problem": "P2", "epoch_utc": "2027-01-01T00:00:00", "mu_km3_s2": MU, "r1_km": R1.tolist(),
       "targets_km": {k: v.tolist() for k, v in TG.items()}, "burns": burns, "observations": obs,
       "summary": {"n_targets": len({o['target'] for o in obs}), "total_dv_m_s": tot,
                   "margins": {"certify": {"distance_km": 0.01, "angle_deg": 0.01}, "duration_s": 60.5}},
       "data": "data/数据.xlsx 高精度版", "note": sys.argv[3] if len(sys.argv) > 3 else ""}
json.dump(sol, open(sys.argv[2], 'w', encoding='utf-8'), ensure_ascii=False, indent=1)
print(sol['summary']['n_targets'], 'targets', round(tot, 4), 'm/s')
