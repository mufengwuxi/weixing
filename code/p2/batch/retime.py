"""局部改进：第 k 段换观测日期或重度多起点重解，之后接力。用法：python retime.py base.json k out.json"""
import sys,json; sys.path.insert(0,'.')
from heavy import solve_heavy
from opt import *
import p2lib as P
base=json.load(open(sys.argv[1])); k=int(sys.argv[2])
for q in NAMES: P.OPPS[q]=opportunities(HZ,q)
x=base[k]
if x['t1'] is None: sys.exit()
if k>0 and base[k-1]['t1'] is None: sys.exit()  # 第 k 段同时服务前面的伴随观测，不能单独重解
rng=np.random.default_rng(100+k)
lo=base[k-1]['obs_start']+D if k>0 else 0
hi=base[k+1]['obs_start'] if k+1<len(base) else T_END-D
if k+1<len(base) and base[k+1]['t1'] is None: cand=[x['obs_start']]
else: cand=sorted(set([e for e in P.OPPS[x['target']] if lo+2*3600<e<hi-2*3600]+[x['obs_start']]))
best=None; T0=total(base)
for e in cand:
    r,v,t0,tmin=state_after(base,k)
    b=solve_heavy(r,v,t0,tmin,x['target'],e,rng)
    if b is None: print('k',k,'e %.2f fail'%(e/86400),flush=True); continue
    c,xx,info=b; t1,t2,d1,d2,sx=xx[0]*3600,xx[1]*3600,xx[2:5]/1000,xx[5:8]/1000,xx[8]*3600
    pre=list(base[:k])+[dict(target=x['target'],t1=t1,dv1=d1.tolist(),t2=t2,dv2=d2.tolist(),obs_start=sx,**{q:float(w) for q,w in info.items()})]
    r2,v2=apply(r,v,t0,t1,d1,t2,d2,sx+D)
    rest=base[k+1:]
    comp=[y['target'] for y in rest if y['t1'] is None]
    out=chain([(y['target'],y['obs_start']) for y in rest if y['t1'] is not None],start=(r2,v2,sx+D,max(sx+D+1,t2+3601)),prefix=pre)
    for m in comp:
        d=try_dual(out,m)
        if d: out=d
    print('k',k,x['target'],'e %.2f'%(e/86400),'leg %.2f (old %.2f)'%(c,np.linalg.norm(x['dv1'])*1000+np.linalg.norm(x['dv2'])*1000),'->',len(out),'%.2f'%total(out),flush=True)
    if len(out)==len(base) and total(out)<T0-0.01 and (best is None or total(out)<total(best)): best=out
if best: json.dump(best,open(sys.argv[3],'w')); print('BEST',len(best),'%.2f'%total(best))
