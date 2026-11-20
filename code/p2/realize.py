import sys,time,pickle,json; sys.path.insert(0,'.'); from est4 import *
def solve_leg(r,v,t0,tmin,n,s,tries=(18.0,25.0,14.0,32.0)):
    best=None
    for L in tries:
        e=est4(r,v,t0,tmin,n,s,L=L)
        if e is None: continue
        _,t1,d1,t2,d2=e
        x0=np.r_[t1/3600,t2/3600,d1*1000,d2*1000,s/3600]
        leg=Leg(n,r,v,t0,tmin,MD,MA,D)
        a=leg.aim_solve(x0,L)
        xs=[a.x] if a.success else []
        xs.append(x0)
        for x in xs:
            f=leg.full(x)
            ok,info=leg.certify(f.x)
            if ok:
                c=cost(f.x)
                if best is None or c<best[0]: best=(c,f.x,info)
        if best is not None and best[0]<e[0]*1000*1.05: break
    return best
OPPS={}
def realize(seq,log=print):
    r,v,t0,tmin=R1[:3].copy(),R1[3:].copy(),0.0,0.0
    out=[]; tot=0
    if not OPPS:
        for m in NAMES: OPPS[m]=opportunities(HZ,m)
    for i,(n,s) in enumerate(seq):
        b=solve_leg(r,v,t0,tmin,n,s)
        if b is None:
            nxt=seq[i+1][1] if i+1<len(seq) else T_END-D
            alts=[x for x in OPPS[n] if tmin+4000<x<nxt-3*3600 and abs(x-s)>600]
            for x in sorted(alts,key=lambda x:abs(x-s))[:4]:
                b=solve_leg(r,v,t0,tmin,n,x)
                if b: log(f'  {n} 改到 {x/86400:.2f}d'); break
        if b is None:
            log(f'  {n}@{s/86400:.2f}d 失败，跳过'); continue
        c,x,info=b; t1,t2,d1,d2,sx=x[0]*3600,x[1]*3600,x[2:5]/1000,x[5:8]/1000,x[8]*3600
        r,v=apply(r,v,t0,t1,d1,t2,d2,sx+D); t0=sx+D; tmin=max(sx+D+1,t2+3601)
        tot+=np.linalg.norm(d1)*1000+np.linalg.norm(d2)*1000
        out.append(dict(target=n,t1=t1,dv1=d1.tolist(),t2=t2,dv2=d2.tolist(),obs_start=sx,**{k:float(vv) for k,vv in info.items()}))
        log(f'  {n}@{sx/86400:.3f}d leg {np.linalg.norm(d1)*1000+np.linalg.norm(d2)*1000:.3f} m/s total {tot:.2f}')
    return out,tot
if __name__=='__main__':
    res=pickle.load(open(sys.argv[1],'rb')); k=max(res); b=res[k]
    seq=[(n,s) for n,s,c in b[5]]
    t=time.time(); out,tot=realize(seq)
    print('REALIZED',len(out),'targets',f'{tot:.2f} m/s',f'{time.time()-t:.0f}s')
    json.dump(out,open(sys.argv[1].replace('.pkl','_real.json'),'w'),indent=1)
