import sys; sys.path.insert(0,'.')
from p2lib import *
AGEO=(MU*(86164.0905/(2*np.pi))**2)**(1/3)
TTn=np.arange(0,T_END+1,120.0)
TP={n:tpos(n,TTn)[0] for n in NAMES}
SUNn=SUN(TTn)
UE={n:-TP[n]/np.linalg.norm(TP[n],axis=1)[:,None] for n in NAMES}
US={}
for n in NAMES:
    u=SUNn-TP[n]; US[n]=u/np.linalg.norm(u,axis=1)[:,None]
C45=np.cos(np.radians(45))
def elem2rv(a,e,i,O,w,M):
    i,O,w,M=map(np.radians,(i,O,w,M))
    E=M
    for _ in range(20): E=E-(E-e*np.sin(E)-M)/(1-e*np.cos(E))
    nu=2*np.arctan2(np.sqrt(1+e)*np.sin(E/2),np.sqrt(1-e)*np.cos(E/2)); r=a*(1-e*np.cos(E))
    p=a*(1-e*e); h=np.sqrt(MU*p)
    rp=np.array([r*np.cos(nu),r*np.sin(nu),0]); vp=np.array([-MU/h*np.sin(nu),MU/h*(e+np.cos(nu)),0])
    cO,sO,ci,si,cw,sw=np.cos(O),np.sin(O),np.cos(i),np.sin(i),np.cos(w),np.sin(w)
    R=np.array([[cO*cw-sO*sw*ci,-cO*sw-sO*cw*ci,sO*si],[sO*cw+cO*sw*ci,-sO*sw+cO*cw*ci,-cO*si],[sw*si,cw*si,ci]])
    return R@rp,R@vp
def natural(r,v,margin_d=0.5,margin_a=0.5,minlen=62):
    """不机动时，按 120 s 网格找各目标的可观测区间，返回 {目标: [(开始,结束)]}。"""
    P=kepler(r,v,TTn)[0]; ca=np.cos(np.radians(45-margin_a)); out={}
    for n in NAMES:
        rho=P-TP[n]; d=np.linalg.norm(rho,axis=1)
        ok=(d>=10+margin_d)&(d<=50-margin_d)
        if not ok.any(): continue
        cs=np.sum(US[n]*rho,1)/d; ce=np.sum(UE[n]*rho,1)/d
        ok&=(cs>=ca)&(ce>=ca)
        if not ok.any(): continue
        idx=np.flatnonzero(ok); runs=np.split(idx,np.flatnonzero(np.diff(idx)>1)+1)
        w=[(TTn[g[0]],TTn[g[-1]]) for g in runs if TTn[g[-1]]-TTn[g[0]]>=minlen+120]
        if w: out[n]=w
    return out
