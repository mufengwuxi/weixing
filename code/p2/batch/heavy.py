import sys; sys.path.insert(0,'.')
from opt import *
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
