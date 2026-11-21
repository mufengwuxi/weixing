"""一条龙：排日程 -> 逐段精确求解 -> 对缺失目标尝试同次飞越插入。用法：python opt.py <sched.pkl> <out.json>"""
import sys,json,pickle,time; sys.path.insert(0,'.')
from p2lib import *
import p2lib as P
def chain(seq,start=None,prefix=None,log=print):
    if not P.OPPS:
        for m in NAMES: P.OPPS[m]=opportunities(HZ,m)
    r,v,t0,tmin=start if start else (R1[:3].copy(),R1[3:].copy(),0.0,0.0)
    out=list(prefix or [])
    for i,(n,s) in enumerate(seq):
        b=solve_leg(r,v,t0,tmin,n,s)
        if b is None:
            nxt=seq[i+1][1] if i+1<len(seq) else T_END-D
            for x in sorted([x for x in P.OPPS[n] if tmin+4000<x<nxt-3*3600 and abs(x-s)>600],key=lambda x:abs(x-s))[:4]:
                b=solve_leg(r,v,t0,tmin,n,x)
                if b: break
        if b is None: continue
        c,x,info=b; t1,t2,d1,d2,sx=x[0]*3600,x[1]*3600,x[2:5]/1000,x[5:8]/1000,x[8]*3600
        r,v=apply(r,v,t0,t1,d1,t2,d2,sx+D); t0=sx+D; tmin=max(sx+D+1,t2+3601)
        out.append(dict(target=n,t1=t1,dv1=d1.tolist(),t2=t2,dv2=d2.tolist(),obs_start=sx,**{k:float(vv) for k,vv in info.items()}))
    return out
def state_after(L,k):
    r,v,t0,tmin=R1[:3].copy(),R1[3:].copy(),0.0,0.0
    for x in L[:k]:
        if x['t1'] is None: continue
        r,v=apply(r,v,t0,x['t1'],np.array(x['dv1']),x['t2'],np.array(x['dv2']),x['obs_start']+D); t0=x['obs_start']+D; tmin=max(t0+1,x['t2']+3601)
    return r,v,t0,tmin
def total(L): return sum(np.linalg.norm(x['dv1'])*1000+np.linalg.norm(x['dv2'])*1000 for x in L if x['t1'] is not None)
def try_dual(L,m):
    best=None
    for k,x in enumerate(L):
        if x['t1'] is None: continue
        ev=[e for e in P.OPPS[m] if abs(e-x['obs_start'])<40*60]
        for e in ev:
            r,v,t0,tmin=state_after(L,k)
            xa0=np.r_[x['t1']/3600,x['t2']/3600,np.array(x['dv1'])*1000,np.array(x['dv2'])*1000,x['obs_start']/3600]
            res,oka,okb,ia,ib,z=dual_leg(r,v,t0,tmin,m,x['target'],xa0,e)
            if not(oka and okb): continue
            xx=z[:9]; t1,t2,d1,d2,s=xx[0]*3600,xx[1]*3600,xx[2:5]/1000,xx[5:8]/1000,xx[8]*3600; sm=z[9]*3600
            pre=list(L[:k])+[dict(target=m,t1=None,dv1=None,t2=None,dv2=None,obs_start=sm,**{q:float(w) for q,w in ia.items()}),
                             dict(target=x['target'],t1=t1,dv1=d1.tolist(),t2=t2,dv2=d2.tolist(),obs_start=s,**{q:float(w) for q,w in ib.items()})]
            r2,v2=apply(r,v,t0,t1,d1,t2,d2,max(s,sm)+D); st=(r2,v2,max(s,sm)+D,max(max(s,sm)+D+1,t2+3601))
            out=chain([(y['target'],y['obs_start']) for y in L[k+1:]],start=st,prefix=pre)
            if len(out)==len(L)+1 and (best is None or total(out)<total(best)): best=out
    return best
if __name__=='__main__':
    t=time.time()
    res=pickle.load(open(sys.argv[1],'rb')); kmax=max(res); b=res[kmax]
    L=chain([(n,s) for n,s,c in b[5]])
    print('chain',len(L),'%.2f'%total(L),'%.0fs'%(time.time()-t),flush=True)
    for m in [n for n in NAMES if n not in {x['target'] for x in L}]:
        d=try_dual(L,m)
        if d: L=d; print('dual add',m,len(L),'%.2f'%total(L),flush=True)
    json.dump(L,open(sys.argv[2],'w'))
    print('FINAL',len(L),'%.2f'%total(L),'%.0fs'%(time.time()-t))
