import sys,time,pickle; sys.path.insert(0,'.'); from plan import *
import plan
def est2(r,v,t0,tmin,n,s):
    best=None
    for f1 in (0.0,0.15,0.3,0.5,0.7):
        sm=s+D/2
        t1=tmin+f1*(s-3600-tmin)
        if s-t1<3600+300: continue
        rT,b=aimdir(n,sm); aim=rT[0]+LAIM*b[0]
        A1,_,_,pn=stm_rv(r,v,t0,t1,sm); dr=aim-pn
        for f2 in (0.4,0.8):
            t2=t1+3600+f2*(s-t1-3600)
            A2=stm_rv(r,v,t0,t2,sm)[0]; M=np.hstack([A1,A2]); dv=np.linalg.lstsq(M,dr,rcond=None)[0]
            for _ in range(6):
                n1=max(np.linalg.norm(dv[:3]),1e-7); n2=max(np.linalg.norm(dv[3:]),1e-7)
                W=np.diag([n1]*3+[n2]*3); dv=W@M.T@np.linalg.solve(M@W@M.T,dr)
            c=np.linalg.norm(dv[:3])+np.linalg.norm(dv[3:])
            if np.linalg.norm(dv[:3])<=0.03 and np.linalg.norm(dv[3:])<=0.03 and (best is None or c<best[0]): best=(c,t1,dv[:3],t2,dv[3:])
    return best
def quick_sep(r,v,t0,n,s):
    p=kepler(r,v,s-t0)[0][0]; q=tpos(n,s)[0][0]; return np.linalg.norm(p-q)
def run(H_days=3.0, width=12, kids=6, mu=1.0, max_leg=12.0, log=True):
    h=np.cross(R1[:3],R1[3:]); h/=np.linalg.norm(h)
    OPP={n:np.array(opportunities(h,n)) for n in NAMES}
    beam=[(0.0,R1[:3],R1[3:],0.0,0.0,frozenset(),[])]
    final=[]
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
                cand.append((dv_tot+c,rn,vn,s+D,max(s+D,t2+3600),vis|{n},seq+[(n,s,t1,t2,c)]))
        if not cand: break
        # 去重：同一访问集合只留最优
        best={}
        for c in cand:
            k=c[5]
            sc=c[0]+mu*c[3]/86400
            if k not in best or sc<best[k][0]: best[k]=(sc,c)
        beam=[c for _,c in sorted(best.values(),key=lambda x:x[0])[:width]]
        if log: print('depth',depth+1,'best dv %.2f m/s t=%.2f d'%(beam[0][0],beam[0][3]/86400),flush=True)
        final=beam
    return final
if __name__=='__main__':
    mu=float(sys.argv[1]); H=float(sys.argv[2]); W=int(sys.argv[3])
    t=time.time(); fin=run(H,W,6,mu)
    b=min(fin,key=lambda c:c[0]); print('count',len(b[6]),'dv',b[0],'time',b[3]/86400,'elapsed',time.time()-t)
    for n,s,t1,t2,c in b[6]: print(n,'%.2f d'%(s/86400),'%.3f m/s'%c)
    pickle.dump(fin,open(f'beam_mu{mu}_H{H}_W{W}.pkl','wb'))
