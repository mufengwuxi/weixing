"""一次抵近观测多颗目标：把可同时观测的目标对合并进同一段机动。

对当前方案中的每一对可同时观测的目标 (a, b)（coobs.json 给出），把 b 的单独一段删去，
让 a 所在的那一段同时观测 a 和 b（两个观测区间可以重叠），再从这一段之后逐段接力重解。
观测数不变且累计速度增量下降时接受，重复直到没有改进。
用法：python merge.py <方案(list).json> <输出.json>
"""
import sys, json, numpy as np
sys.path.insert(0, '/root/autodl-tmp/p2new/batch')
import p2lib as P
from p2lib import Leg, MD, MA, D, NAMES, HZ, opportunities, cost, apply
from opt import chain, state_after, total
from scipy.optimize import minimize
for q in NAMES: P.OPPS[q] = opportunities(HZ, q)
PAIRS = [(r['a'], r['b']) for r in json.load(open('/root/autodl-tmp/p2new/multi/coobs.json')) if r['minutes_coobs'] > 0]

def dual_free(r, v, t0, tmin, main, comp, xm0, sc0):
    """同一段两次脉冲，观测 main（时刻 x[8]）和 comp（时刻 z[9]），两个观测区间可以重叠。"""
    A = Leg(comp, r, v, t0, tmin, MD, MA, D); B = Leg(main, r, v, t0, tmin, MD, MA, D)
    def g(z):
        x = z[:9]; xa = x.copy(); xa[8] = z[9]
        return np.r_[A.cons()[0]['fun'](xa), B.cons()[0]['fun'](x)]
    lin = lambda z: np.r_[B.cons()[1]['fun'](z[:9]), z[9] - z[1], z[8] - z[9]]   # comp 不晚于 main 开始，便于接力
    z0 = np.r_[xm0, sc0 / 3600]
    res = minimize(lambda z: cost(z[:9]), z0, constraints=[{'type': 'ineq', 'fun': g}, {'type': 'ineq', 'fun': lin}],
                   method='SLSQP', options={'maxiter': 800, 'ftol': 1e-12})
    z = res.x; x = z[:9]; xa = x.copy(); xa[8] = z[9]
    oka, ia = A.certify(xa); okb, ib = B.certify(x)
    return oka and okb, z, ia, ib

def try_merge(L, a, b):
    names = [x['target'] for x in L]
    if a not in names or b not in names: return None
    best = None
    for main, comp in ((a, b), (b, a)):
        k = names.index(main)
        x = L[k]
        if x['t1'] is None or (k > 0 and L[k - 1]['t1'] is None): continue
        base = [y for y in L if y['target'] != comp]
        k2 = [y['target'] for y in base].index(main)
        r, v, t0, tmin = state_after(base, k2)
        xm0 = np.r_[x['t1'] / 3600, x['t2'] / 3600, np.array(x['dv1']) * 1000, np.array(x['dv2']) * 1000, x['obs_start'] / 3600]
        for sc in [x['obs_start']] + [e for e in P.OPPS[comp] if abs(e - x['obs_start']) < 1800]:
            ok, z, ia, ib = dual_free(r, v, t0, tmin, main, comp, xm0, sc)
            if not ok: continue
            xx = z[:9]; t1, t2, d1, d2, s = xx[0] * 3600, xx[1] * 3600, xx[2:5] / 1000, xx[5:8] / 1000, xx[8] * 3600; sm = z[9] * 3600
            pre = list(base[:k2]) + [dict(target=comp, t1=None, dv1=None, t2=None, dv2=None, obs_start=sm, **{q: float(w) for q, w in ia.items()}),
                                      dict(target=main, t1=t1, dv1=d1.tolist(), t2=t2, dv2=d2.tolist(), obs_start=s, **{q: float(w) for q, w in ib.items()})]
            end = max(s, sm) + D
            r2, v2 = apply(r, v, t0, t1, d1, t2, d2, end)
            rest = [(y['target'], y['obs_start']) for y in base[k2 + 1:] if y['t1'] is not None]
            comps = [y['target'] for y in base[k2 + 1:] if y['t1'] is None]
            out = chain(rest, start=(r2, v2, end, max(end + 1, t2 + 3601)), prefix=pre, log=lambda *q: None)
            from opt import try_dual
            for m in comps:
                if m not in {y['target'] for y in out}:
                    d = try_dual(out, m)
                    if d: out = d
            if len({y['target'] for y in out}) == len({y['target'] for y in L}) and (best is None or total(out) < total(best)):
                best = out
            print(f'  {main}+{comp} @ {sc/86400:.3f}d -> {len(out)} 颗 {total(out):.2f}', flush=True)
    return best

