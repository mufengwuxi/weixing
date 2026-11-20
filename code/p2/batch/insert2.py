"""对缺失目标 m 在第 k 个间隙插入（重度多起点），之后接力。用法：python insert2.py base.json m k out.json"""
import sys,json; sys.path.insert(0,'.')
from opt import *
import p2lib as P
def solve_heavy(r,v,t0,tmin,n,s,rng):
    best=None
    for L in (12,18,25,32,40):
        e=est4(r,v,t0,tmin,n,s,L=L,top=8)
        if e is None: continue
        _,t1,d1,t2,d2=e
        base=np.r_[t1/3600,t2/3600,d1*1000,d2*1000,s/3600]
        leg=Leg(n,r,v,t0,tmin,MD,MA,D)
        inits=[base]
        a=leg.aim_solve(base,L)
        if a.success: inits.append(a.x)
        for _ in range(4):
            x=base.copy(); x[0]=max(tmin/3600,x[0]+rng.normal(0,3)); x[1]=max(x[0]+1.01,min(s/3600-0.05,x[1]+rng.normal(0,3)))
            x[2:8]+=rng.normal(0,0.5,6); inits.append(x)
        for x0 in inits:
            f=leg.full(x0,iters=800); ok,info=leg.certify(f.x)
            if ok and (best is None or cost(f.x)<best[0]): best=(cost(f.x),f.x,info)
    return best
base=json.load(open(sys.argv[1])); m=sys.argv[2]; k=int(sys.argv[3])
for q in NAMES: P.OPPS[q]=opportunities(HZ,q)
rng=np.random.default_rng(k)
lo=base[k-1]['obs_start']+D if k>0 else 0; hi=base[k]['obs_start'] if k<len(base) else T_END-D
best=None
for e in [e for e in P.OPPS[m] if lo+2*3600<e<hi-2*3600]:
    r,v,t0,tmin=state_after(base,k)
    b=solve_heavy(r,v,t0,tmin,m,e,rng)
    if b is None: continue
    c,x,info=b; t1,t2,d1,d2,sx=x[0]*3600,x[1]*3600,x[2:5]/1000,x[5:8]/1000,x[8]*3600
    pre=list(base[:k])+[dict(target=m,t1=t1,dv1=d1.tolist(),t2=t2,dv2=d2.tolist(),obs_start=sx,**{q:float(w) for q,w in info.items()})]
    r2,v2=apply(r,v,t0,t1,d1,t2,d2,sx+D)
    out=chain([(y['target'],y['obs_start']) for y in base[k:]],start=(r2,v2,sx+D,max(sx+D+1,t2+3601)),prefix=pre)
    print('k',k,'e %.2f'%(e/86400),'leg %.2f'%c,'->',len(out),'%.2f'%total(out),flush=True)
    if len(out)==len(base)+1 and (best is None or total(out)<total(best)): best=out
if best: json.dump(best,open(sys.argv[4],'w')); print('BEST',len(best),'%.2f'%total(best))
