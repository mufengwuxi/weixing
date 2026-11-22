"""第一问多起点搜索的局部最优分布：记录每个起点精修并认证后的 J（以及所属候选时刻）。
用法：python code/p1/multistart_dist.py <输出.json> <起点数> <种子>"""
import json, sys
import numpy as np
import solver as S
from pathlib import Path
REPO = Path(__file__).resolve().parents[2]
P = S.Problem(str(REPO / "data/数据.xlsx"), 0.01, 0.01, 60.0)
n, seed = int(sys.argv[2]), int(sys.argv[3])
rng = np.random.default_rng(seed)
cands = S.observation_candidates(P)
out = []
for i in range(n):
    tc, dl = cands[rng.integers(len(cands))]
    L = rng.uniform(10 + P.md, 50 - P.md)
    x0 = S.initial_guess(P, rng, tc, dl)
    a = S.solve_aim(P, x0, L)
    if not a.success:
        out.append(dict(tc=tc, J=None)); continue
    f = S.solve_full(P, a.x)
    c = S.certify(P, f.x)
    out.append(dict(tc=tc, L=L, J=c["J"] if c["ok"] else None, x=f.x.tolist()))
json.dump(out, open(sys.argv[1], "w"))
print("done", sum(1 for o in out if o["J"]), "feasible")
