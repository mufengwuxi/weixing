"""在 v3/lns.py 的四种邻域动作之外，增加"合并"动作：把一对可同时观测的目标放进同一段机动（merge_lib.try_merge）。
用法同 lns.py：python lns_m.py <共享最优.json> <工作进程编号> <运行秒数>"""
import sys, os, json, random, time
sys.path.insert(0, '/root/autodl-tmp/p2new/batch'); sys.path.insert(0, '/root/autodl-tmp/p2new/v3')
import lns as N
import importlib.util
spec = importlib.util.spec_from_file_location('mg', '/root/autodl-tmp/p2new/multi/merge_lib.py'); mg = importlib.util.module_from_spec(spec); spec.loader.exec_module(mg)
from opt import total

def move_m(L, rng):
    if rng.random() < 0.25:
        names = {x['target'] for x in L}
        pairs = [p for p in mg.PAIRS if p[0] in names and p[1] in names]
        a, b = rng.choice(pairs)
        import builtins
        _p = builtins.print; builtins.print = lambda *q, **k: None
        try:
            out = mg.try_merge(L, a, b)
        finally:
            builtins.print = _p
        return out, f'merge {a}+{b}'
    return N.move(L, rng)

if __name__ == '__main__':
    path, wid, secs = sys.argv[1], int(sys.argv[2]), float(sys.argv[3])
    rng = random.Random(2000 + wid)
    t_end = time.time() + secs
    it = 0
    while time.time() < t_end:
        it += 1
        L = N.load(path)
        try:
            out, kind = move_m(L, rng)
        except Exception as e:
            print('err', e, flush=True); continue
        if out is None: continue
        if N.ntarg(out) >= N.ntarg(L) and N.save_if_better(path, out):
            print(f'[{wid}] it {it} {kind}: {N.ntarg(out)} 颗 {total(out):.3f}（原 {total(L):.3f}）', flush=True)
