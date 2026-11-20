import sys,time,pickle; sys.path.insert(0,'.'); from beam2 import *
import beam2
P_GEO=86164.09
def est3(r,v,t0,tmin,n,s,L=LAIM):
    sm=s+D/2
    if s-tmin<3600+600: return None
    rT,b=aimdir(n,sm); aim=rT[0]+L*b[0]
    C=[tmin]+[s-k*P_GEO/2 for k in range(1,13) if s-k*P_GEO/2>=tmin]+[s-1800.0]
    C=sorted(set([c for c in C if c>=tmin and c<=s-600]))
    A={}; pn=None
    for c in C:
        Ac,_,_,p=stm_rv(r,v,t0,c,sm); A[c]=Ac; pn=p
    dr=aim-pn
    best=None
    for i,c1 in enumerate(C):
        try: d1=np.linalg.solve(A[c1],dr); cc=np.linalg.norm(d1)
        except Exception: continue
        if cc<=0.03 and (best is None or cc<best[0]): best=(cc,c1,d1,c1+3600,np.zeros(3))
        for c2 in C[i+1:]:
            if c2-c1<3600: continue
            M=np.hstack([A[c1],A[c2]]); dv=np.linalg.lstsq(M,dr,rcond=None)[0]
            for _ in range(8):
                n1=max(np.linalg.norm(dv[:3]),1e-8); n2=max(np.linalg.norm(dv[3:]),1e-8)
                W=np.diag([n1]*3+[n2]*3); dv=W@M.T@np.linalg.solve(M@W@M.T,dr)
            c=np.linalg.norm(dv[:3])+np.linalg.norm(dv[3:])
            if np.linalg.norm(dv[:3])<=0.03 and np.linalg.norm(dv[3:])<=0.03 and (best is None or c<best[0]): best=(c,c1,dv[:3],c2,dv[3:])
    return best
beam2.est2=est3
if __name__=='__main__':
    mu,ct,H,W,rm,ml=map(float,sys.argv[1:7]); W=int(W)
    t=time.time(); hist=run2(H,W,8,mu,ct,rate_max=rm,max_leg=ml)
    for last in hist[::-1]:
        b=min(last,key=lambda c:c[0]); break
    print('count',len(b[6]),'dv %.2f'%b[0],'t_end %.2f d'%(b[3]/86400),'elapsed %.0f s'%(time.time()-t))
    print(' '.join(f"{n}@{s/86400:.2f}:{c:.2f}" for n,s,t1,t2,c,e in b[6]))
    pickle.dump(hist,open(f'b3_{mu}_{ct}_{H}_{W}_{rm}.pkl','wb'))
