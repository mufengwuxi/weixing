import sys; sys.path.insert(0,'.'); from eng import *
D=62.0; MD=0.5; MA=0.5; LAIM=25.0
TT=np.arange(0,T_END+1,120.0)
TPOS={n:tpos(n,TT)[0] for n in NAMES}
def gam_arr(n):
    rT=TPOS[n]; S=SUN(TT); us=S-rT; us/=np.linalg.norm(us,axis=1)[:,None]; ue=-rT/np.linalg.norm(rT,axis=1)[:,None]
    return np.degrees(np.arccos(np.clip(np.sum(us*ue,1),-1,1)))
GAM={n:gam_arr(n) for n in NAMES}
def opportunities(h, n):
    """目标 n 穿过 r1 轨道面（法向 h）且在日照窗口内的时刻；近共面目标则窗口内每 2 h 一个。"""
    z=TPOS[n]@h; ok=GAM[n]<=90-2*MA-3  # 留出余量
    if np.max(np.abs(z))<35:
        return [t for t in TT[ok][::60]]
    idx=np.where(np.diff(np.sign(z))!=0)[0]
    out=[]
    for i in idx:
        t=TT[i]+120*z[i]/(z[i]-z[i+1])
        j=int(round(t/120))
        if ok[min(j,len(ok)-1)]: out.append(t)
    return out
def stm_rv(r,v,t0,tb,te,h=1e-6):
    R,V=kepler(r,v,tb-t0); rb,vb=R[0],V[0]
    base=kepler(rb,vb,te-tb)[0][0]; A=np.zeros((3,3))
    for k in range(3):
        e=np.zeros(3); e[k]=h; A[:,k]=(kepler(rb,vb+e,te-tb)[0][0]-base)/h
    return A, rb, vb, base
def estimate(r,v,t0,tmin,n,s):
    """线性化估计：从 (r,v,t0) 出发，第一次机动不早于 tmin，在 s 开始观测 n 的最小 Δv（km/s）。"""
    sm=s+D/2
    if s-tmin<3600+600: return None
    rT,b=aimdir(n,sm); aim=rT[0]+LAIM*b[0]
    best=None
    for f1 in (0.0,0.3):
        t1=tmin+f1*(s-3600-tmin)
        A1,_,_,pn=stm_rv(r,v,t0,t1,sm)
        dr=aim-pn
        try: d1=np.linalg.solve(A1,dr)
        except np.linalg.LinAlgError: continue
        c1=np.linalg.norm(d1)
        if best is None or c1<best[0]: best=(c1,t1,d1,t1+3600,np.zeros(3))
        for f2 in (0.5,0.85):
            t2=t1+3600+f2*(s-t1-3600)
            A2=stm_rv(r,v,t0,t2,sm)[0]
            M=np.hstack([A1,A2]); dv=np.linalg.lstsq(M,dr,rcond=None)[0]
            # 再做几步 IRLS 逼近 sum-of-norms
            for _ in range(5):
                w1=1/max(np.linalg.norm(dv[:3]),1e-7); w2=1/max(np.linalg.norm(dv[3:]),1e-7)
                W=np.diag(np.r_[[1/w1]*3,[1/w2]*3]); dv=W@M.T@np.linalg.solve(M@W@M.T,dr)
            c=np.linalg.norm(dv[:3])+np.linalg.norm(dv[3:])
            if c<best[0]: best=(c,t1,dv[:3],t2,dv[3:])
    c,t1,d1,t2,d2=best
    if np.linalg.norm(d1)>0.03 or np.linalg.norm(d2)>0.03: return None
    return best
def apply(r,v,t0,t1,d1,t2,d2,tend):
    R,V=kepler(r,v,t1-t0); r1,v1=R[0],V[0]+d1
    R,V=kepler(r1,v1,t2-t1); r2,v2=R[0],V[0]+d2
    R,V=kepler(r2,v2,tend-t2); return R[0],V[0]
