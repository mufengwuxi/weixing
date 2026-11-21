"""第三问 v4（高精度新数据，M3-B：M3-A + 全计划 SCP）：把两颗星的结果合成方案文件（裕量只为数值安全保留：优化 0.05 km / 0.05°，认证 0.01 km / 0.01°，观测 60.5 s）。
用法（仓库根目录）：python code/p3/v4/export_v4.py
输出：solutions/q3/v4_p3_claude.json（两星合并）、solutions/q3/v4_p3_r1.json、solutions/q3/v4_p3_r2.json（与第二问同格式，可直接用 verify_p2.py / build_single_r1.py 复核）"""
import json, os, sys
import numpy as np
H = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, H)
from p2lib import R1, TG, MU  # 不设 P2_INIT/P2_TARGETS 时为 r1 与全部 30 颗目标

AGEO = 42164.1696
DA_MIN = 7.81   # 本方案 r2 初始半长轴（首脉冲并入后）= GEO + 7.80 km，要求 δa ≥ 7.81 km


def elements(s):
    r, v = np.array(s[:3]), np.array(s[3:])
    rn, vn = np.linalg.norm(r), np.linalg.norm(v)
    a = 1 / (2 / rn - vn * vn / MU)
    e = np.linalg.norm(((vn * vn - MU / rn) * r - r @ v * v) / MU)
    h = np.cross(r, v)
    return dict(a_km=a, e=e, i_deg=float(np.degrees(np.arccos(h[2] / np.linalg.norm(h)))),
                raan_deg=float(np.degrees(np.arctan2(h[0], -h[1])) % 360))


def one(L, state, name):
    burns = []
    for x in L:
        if x.get('t1') is None:
            continue
        for t, d in ((x['t1'], x['dv1']), (x['t2'], x['dv2'])):
            if np.linalg.norm(d) > 0:     # r2 的首脉冲已并入初始轨道设计
                burns.append({"t_s": t, "dv_km_s": d, "for": x['target']})
    burns.sort(key=lambda b: b['t_s'])
    obs = [{"target": x['target'], "start_s": x['obs_start'], "duration_s": 60.5,
            "claimed": {k: x[k] for k in ('d_min', 'd_max', 'alpha_max', 'theta_max')}} for x in L]
    tot = sum(np.linalg.norm(b['dv_km_s']) for b in burns) * 1000
    tg = sorted({o['target'] for o in obs}, key=lambda k: int(k[1:]))
    return {"problem": "P2", "satellite": name, "epoch_utc": "2027-01-01T00:00:00", "mu_km3_s2": MU,
            "r1_km": list(state), "targets_km": {k: TG[k].tolist() for k in tg}, "burns": burns,
            "observations": obs, "summary": {"n_targets": len(obs), "total_dv_m_s": tot},
            "note": f"第三问 v4 {name} 的分工部分，格式同第二问；r1_km 字段存放该星初始状态"}


L1 = json.load(open(f'{H}/final/r1_west.json'))
L2 = json.load(open(f'{H}/final/r2_east.json'))
s2 = json.load(open(f'{H}/final/r2_init.json'))['state']
A, B = one(L1, R1.tolist(), 'r1'), one(L2, s2, 'r2')
el = elements(s2)
assert el['e'] < 1e-3 and abs(el['a_km'] - AGEO) <= DA_MIN + 1e-3   # elem2rv 的浮点误差约 2e-5 km
assert not set(A['targets_km']) & set(B['targets_km'])
json.dump(A, open('solutions/q3/v4_p3_r1.json', 'w', encoding='utf-8'), ensure_ascii=False, indent=1)
json.dump(B, open('solutions/q3/v4_p3_r2.json', 'w', encoding='utf-8'), ensure_ascii=False, indent=1)
tot = A['summary']['total_dv_m_s'] + B['summary']['total_dv_m_s']
P = {"problem": "P3", "epoch_utc": "2027-01-01T00:00:00", "mu_km3_s2": MU, "delta_a_km_required_min": DA_MIN, "version": "v4", "model": "M3-B", "data": "data/数据.xlsx 高精度版（2026-11-20，PR #22）", "margins": {"optimize": {"distance_km": 0.05, "angle_deg": 0.05, "note": "SCP 先用 0.1 再用 0.05 热启动"}, "certify": {"distance_km": 0.01, "angle_deg": 0.01}, "duration_s": 60.5},
     "r2_design": {"state_km": s2, **el, "a_minus_GEO_km": el['a_km'] - AGEO},
     "assignment": {"r1": list(A['targets_km']), "r2": list(B['targets_km'])},
     "summary": {"n_targets": len(A['targets_km']) + len(B['targets_km']),
                 "dv_r1_m_s": A['summary']['total_dv_m_s'], "dv_r2_m_s": B['summary']['total_dv_m_s'], "total_dv_m_s": tot},
     "files": {"r1": "solutions/q3/v4_p3_r1.json", "r2": "solutions/q3/v4_p3_r2.json"}}
json.dump(P, open('solutions/q3/v4_p3_claude.json', 'w', encoding='utf-8'), ensure_ascii=False, indent=1)
print(P['summary'], {k: round(v, 5) for k, v in el.items()})
