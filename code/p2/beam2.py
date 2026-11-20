import sys,time,pickle; sys.path.insert(0,'.'); from beam import *
HZ=np.cross(R1[:3],R1[3:]); HZ/=np.linalg.norm(HZ)
def rel_lon(r,v,t0,t,names):
    p=kepler(r,v,t-t0)[0][0]
    out={}
    for n in names:
        q=tpos(n,t)[0][0]; out[n]=np.degrees(np.arctan2(np.cross(p,q)@HZ, p@q))
    return out
def togo(r,v,t0,unv):
    """剩余目标的最少巡游角度（度）：从当前位置先去近端再扫到远端。"""
    if not unv: return 0.0
    L=list(rel_lon(r,v,t0,t0,unv).values()); lo,hi=min(L),max(L)
    lo,hi=min(lo,0),max(hi,0)
    return (hi-lo)+min(-lo,hi)
def run2(H_days=5.0,width=30,kids=8,mu=0.5,ct=1.0,rate_max=3.0,max_leg=10.0,log=True):
    OPP={n:np.array(opportunities(HZ,n)) for n in NAMES}
    beam=[(0.0,R1[:3],R1[3:],0.0,0.0,frozenset(),[])]
    hist=[]
    for depth in range(30):
        cand=[]
        for dv_tot,r,v,t0,tmin,vis,seq in beam:
            ch=[]
            for n in NAMES:
                if n in vis: continue
                for s in OPP[n][(OPP[n]>tmin+4000)&(OPP[n]<tmin+H_days*86400)&(OPP[n]<T_END-D)]:
                    T=s-tmin
                    if quick_sep(r,v,t0,n,s)/(3*T)*1000>max_leg*1.5: continue
                    e=est2(r,v,t0,tmin,n,s)
                    if e is None or e[0]*1000>max_leg: continue
                    ch.append((e[0]*1000,n,s,e))
            ch.sort(key=lambda c:c[0]+mu*(c[2]-tmin)/86400)
            for c,n,s,e in ch[:kids]:
                _,t1,d1,t2,d2=e
                rn,vn=apply(r,v,t0,t1,d1,t2,d2,s+D)
                cand.append((dv_tot+c,rn,vn,s+D,max(s+D,t2+3600),vis|{n},seq+[(n,s,t1,t2,c,e)]))
        if not cand: break
        scored={}
        for c in cand:
            unv=[n for n in NAMES if n not in c[5]]
            tg=togo(c[1],c[2],c[3],unv); rem=(T_END-c[3])/86400
            if rem<=0 or (unv and tg/rem>rate_max): continue
            sc=c[0]+mu*c[3]/86400+ct*tg
            key=(c[5], round(c[3]/43200))
            if key not in scored or sc<scored[key][0]: scored[key]=(sc,c)
        beam=[c for _,c in sorted(scored.values(),key=lambda x:x[0])[:width]]
        if not beam: break
        hist.append(beam)
        if log: print('depth',depth+1,'dv %.2f t=%.2f d togo=%.1f'%(beam[0][0],beam[0][3]/86400,togo(beam[0][1],beam[0][2],beam[0][3],[n for n in NAMES if n not in beam[0][5]])),flush=True)
    return hist
if __name__=='__main__':
    mu,ct,H,W=map(float,sys.argv[1:5]); W=int(W)
    t=time.time(); hist=run2(H,W,8,mu,ct)
    last=hist[-1]; b=min(last,key=lambda c:c[0])
    print('count',len(b[6]),'dv %.2f'%b[0],'t_end %.2f d'%(b[3]/86400),'elapsed %.0f s'%(time.time()-t))
    print(' '.join(f"{n}@{s/86400:.2f}:{c:.2f}" for n,s,t1,t2,c,e in b[6]))
    pickle.dump(hist,open(f'b2_{mu}_{ct}_{H}_{W}.pkl','wb'))
