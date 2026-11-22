"""两颗目标能否在同一 60 s 内被同时观测：可观测集合 C_a(t) 与 C_b(t) 的交集是否非空。"""
import sys, json, numpy as np
sys.path.insert(0, '/root/autodl-tmp/p2new/batch')
from p2lib import TG, NAMES, kepler, SUN
rng = np.random.default_rng(0)
ts = np.arange(0, 30 * 86400, 60.0)
P = {n: kepler(TG[n][:3], TG[n][3:], ts)[0] for n in NAMES}
S = SUN(ts)
def axes(n):
    r = P[n]; us = S - r; us /= np.linalg.norm(us, axis=1)[:, None]; ue = -r / np.linalg.norm(r, axis=1)[:, None]; return us, ue
AX = {n: axes(n) for n in NAMES}
c45 = np.cos(np.radians(45))
# 在 C_a 内均匀抽样：方向在单位球上随机，距离 10–50
M = 4000
dirs = rng.normal(size=(M, 3)); dirs /= np.linalg.norm(dirs, axis=1)[:, None]
dist = rng.uniform(10, 50, M)
out = []
for i, a in enumerate(NAMES):
    for b in NAMES[i + 1:]:
        dab = np.linalg.norm(P[a] - P[b], axis=1)
        idx = np.where(dab < 100)[0]
        ok_t = []
        for k in idx:
            usa, uea = AX[a][0][k], AX[a][1][k]
            usb, ueb = AX[b][0][k], AX[b][1][k]
            pa = dirs[(dirs @ usa >= c45) & (dirs @ uea >= c45)]
            if len(pa) == 0: continue
            dd = dist[:len(pa)]
            rho_a = pa * dd[:, None]
            rho_b = rho_a + (P[a][k] - P[b][k])
            nb = np.linalg.norm(rho_b, axis=1)
            m = (nb >= 10) & (nb <= 50) & (rho_b @ usb >= c45 * nb) & (rho_b @ ueb >= c45 * nb)
            if m.any(): ok_t.append(float(ts[k]))
        if idx.size:
            out.append(dict(a=a, b=b, dmin=float(dab.min()), minutes_close=int(idx.size), minutes_coobs=len(ok_t), first=ok_t[:5], days=sorted(set(int(t // 86400) for t in ok_t))))
out.sort(key=lambda r: -r['minutes_coobs'])
json.dump(out, open('/root/autodl-tmp/p2new/multi/coobs.json', 'w'), indent=1)
for r in out:
    print(r['a'], r['b'], 'dmin %.1f' % r['dmin'], 'close %d min' % r['minutes_close'], 'coobs %d min' % r['minutes_coobs'], 'days', r['days'][:12])
