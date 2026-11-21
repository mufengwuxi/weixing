# 第二问求解库（由 eng/plan/est4/realize/dual 合并）
import numpy as np, warnings, json, os
from scipy.interpolate import CubicSpline
from scipy.optimize import minimize
warnings.filterwarnings('ignore')
MU=398600.4418; AU=149597870.7; T_END=2592000.0; EPS=1e-6
_DJ=json.load(open(os.path.join(os.path.dirname(os.path.abspath(__file__)),'states.json')))
R1=np.array(_DJ['r1']); TG={k:np.array(v) for k,v in _DJ['targets'].items()}
if os.environ.get('P2_INIT'):
    R1=np.array(json.load(open(os.environ['P2_INIT']))['state'])
if os.environ.get('P2_TARGETS'):
    _keep=os.environ['P2_TARGETS'].split(',')
    TG={k:TG[k] for k in _keep}
NAMES=list(TG)

def kepler(r0,v0,dt):
    dt=np.atleast_1d(np.asarray(dt,float)); r0n=np.linalg.norm(r0); a=1/(2/r0n-v0@v0/MU); sa=np.sqrt(a)
    sig0=r0@v0/np.sqrt(MU); n=np.sqrt(MU/a**3); M=n*dt
    x=M.copy()
    for _ in range(40):
        f=x-(1-r0n/a)*np.sin(x)+sig0/sa*(1-np.cos(x))-M; fp=1-(1-r0n/a)*np.cos(x)+sig0/sa*np.sin(x)
        dx=f/fp; x-=dx
        if np.max(np.abs(dx))<1e-13: break
    r=a+(r0n-a)*np.cos(x)+sig0*sa*np.sin(x)
    F=1-a/r0n*(1-np.cos(x)); G=dt+(np.sin(x)-x)/n
    Fd=-np.sqrt(MU*a)/(r*r0n)*np.sin(x); Gd=1-a/r*(1-np.cos(x))
    return F[:,None]*r0+G[:,None]*v0, Fd[:,None]*r0+Gd[:,None]*v0

import erfa
d1,d2=erfa.dtf2d('UTC',2027,1,1,0,0,0.0); a1,a2=erfa.utctai(d1,d2); TT1,TT2=erfa.taitt(a1,a2)
_ts=np.arange(-7200,T_END+7201,1800.0)
SUN=CubicSpline(_ts,np.array([-erfa.epv00(TT1,TT2+t/86400)[0]['p']*AU for t in _ts]))

def tpos(name,t): return kepler(TG[name][:3],TG[name][3:],t)

def gamma_ok(name,t,margin=0.0):
    rT,_=tpos(name,t); S=SUN(np.atleast_1d(t)); us=S-rT; us/=np.linalg.norm(us,axis=1)[:,None]; ue=-rT/np.linalg.norm(rT,axis=1)[:,None]
    return np.degrees(np.arccos(np.clip(np.sum(us*ue,1),-1,1)))<=90-2*margin

def aimdir(name,t):
    rT,_=tpos(name,t); S=SUN(np.atleast_1d(t)); us=S-rT; us/=np.linalg.norm(us,axis=1)[:,None]; ue=-rT/np.linalg.norm(rT,axis=1)[:,None]
    b=us+ue; return rT, b/np.linalg.norm(b,axis=1)[:,None]

class Leg:
    """一次观测：从状态 (r,v,t0) 出发，最多两次脉冲，观测 name。"""
    def __init__(s,name,r,v,t0,tmin,md,ma,D):
        s.name,s.r,s.v,s.t0,s.tmin,s.md,s.ma,s.D=name,r,v,t0,tmin,md,ma,D
        s.ca=np.cos(np.radians(45-ma)); s.k=np.linspace(0,D,int(np.ceil(D))+1)
        s.cmd=float(os.environ.get('P2_CERT_MD',md)); s.cma=float(os.environ.get('P2_CERT_MA',ma))
    def traj(s,x,t):
        t1,t2,d1,d2=x[0]*3600,x[1]*3600,x[2:5]/1000,x[5:8]/1000
        R,V=kepler(s.r,s.v,t1-s.t0); r,v=R[0],V[0]+d1
        R,V=kepler(r,v,t2-t1); r,v=R[0],V[0]+d2
        return kepler(r,v,np.asarray(t)-t2)
    def geom(s,x,t):
        rO,_=s.traj(x,t); rT,_=tpos(s.name,t); rho=rO-rT; d=np.linalg.norm(rho,axis=1)
        S=SUN(t); us=S-rT; us/=np.linalg.norm(us,axis=1)[:,None]; ue=-rT/np.linalg.norm(rT,axis=1)[:,None]
        return d,np.sum(us*rho,1)/d,np.sum(ue*rho,1)/d
    def cons(s):
        def g(x):
            d,cA,cT=s.geom(x,x[8]*3600+s.k)
            return np.r_[(d-10-s.md)/10,(50-s.md-d)/10,(cA-s.ca)*10,(cT-s.ca)*10]
        lin=lambda x: np.array([x[0]-s.tmin/3600, x[1]-x[0]-1.0, x[8]-x[1], (T_END-s.D)/3600-x[8], 900-x[2:5]@x[2:5], 900-x[5:8]@x[5:8]])
        return [{'type':'ineq','fun':g},{'type':'ineq','fun':lin}]
    def aim_solve(s,x0,L):
        def eq(x):
            sm=x[8]*3600+s.D/2; rT,b=aimdir(s.name,sm); return (s.traj(x,[sm])[0][0]-(rT[0]+L*b[0]))/10
        c=[{'type':'eq','fun':eq},s.cons()[1]]
        return minimize(cost,x0,constraints=c,method='SLSQP',options={'maxiter':400,'ftol':1e-10})
    def full(s,x0,iters=500):
        return minimize(cost,x0,constraints=s.cons(),method='SLSQP',options={'maxiter':iters,'ftol':1e-12})
    def certify(s,x,step=0.05):
        t=np.arange(x[8]*3600,x[8]*3600+s.D+1e-9,step); d,cA,cT=s.geom(x,t)
        al=np.degrees(np.arccos(np.clip(cA,-1,1))); th=np.degrees(np.arccos(np.clip(cT,-1,1))); tol=1e-7
        ok=(d.min()>=10+s.cmd-tol and d.max()<=50-s.cmd+tol and al.max()<=45-s.cma+tol and th.max()<=45-s.cma+tol
            and x[0]*3600>=s.tmin-1e-3 and (x[1]-x[0])*3600>=3600-1e-3 and x[8]>=x[1]-1e-9 and np.linalg.norm(x[2:5])<=30 and np.linalg.norm(x[5:8])<=30)
        return ok, dict(d_min=d.min(),d_max=d.max(),alpha_max=al.max(),theta_max=th.max())
def cost(x): return np.sqrt(x[2:5]@x[2:5]+EPS)+np.sqrt(x[5:8]@x[5:8]+EPS)

D=float(os.environ.get("P2_D",62.0)); MD=float(os.environ.get("P2_MD",0.5)); MA=float(os.environ.get("P2_MA",0.5)); LAIM=25.0
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

HZ=np.cross(R1[:3],R1[3:]); HZ/=np.linalg.norm(HZ)
P_GEO=86164.09
def quick_sep(r,v,t0,n,s):
    p=kepler(r,v,s-t0)[0][0]; q=tpos(n,s)[0][0]; return np.linalg.norm(p-q)

from scipy.optimize import minimize as _min
def sumnorm_solve(M,dr,x0):
    f=lambda x: np.sqrt(x[:3]@x[:3]+1e-14)+np.sqrt(x[3:]@x[3:]+1e-14)
    r=_min(f,x0,constraints=[{'type':'eq','fun':lambda x:(M@x-dr)*1e3}],method='SLSQP',options={'maxiter':100,'ftol':1e-14})
    return r.x if r.success else x0
def est4(r,v,t0,tmin,n,s,L=18.0,top=4):
    sm=s+D/2
    if s-tmin<3600+600: return None
    rT,b=aimdir(n,sm); aim=rT[0]+L*b[0]
    C=[tmin,tmin+1800]
    for k in range(1,13):
        for off in (-0.3*3600,0.3*3600):
            c=s-k*P_GEO/2+off
            if c>=tmin: C.append(c)
    C.append(s-2400.0)
    C=sorted(set([c for c in C if tmin<=c<=s-600]))
    A={}
    for c in C:
        Ac,_,_,p=stm_rv(r,v,t0,c,sm); A[c]=Ac
    dr=aim-p
    pairs=[]
    for i,c1 in enumerate(C):
        for c2 in C[i+1:]:
            if c2-c1<3600: continue
            M=np.hstack([A[c1],A[c2]]); dv=M.T@np.linalg.solve(M@M.T,dr)
            pairs.append((np.linalg.norm(dv[:3])+np.linalg.norm(dv[3:]),c1,c2,M,dv))
    pairs.sort(key=lambda p:p[0])
    best=None
    for c,c1,c2,M,dv in pairs[:top]:
        dv=sumnorm_solve(M,dr,dv); cc=np.linalg.norm(dv[:3])+np.linalg.norm(dv[3:])
        if np.linalg.norm(M@dv-dr)>1e-3: continue
        if np.linalg.norm(dv[:3])<=0.03 and np.linalg.norm(dv[3:])<=0.03 and (best is None or cc<best[0]): best=(cc,c1,dv[:3],c2,dv[3:])
    return best

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

def dual_leg(r,v,t0,tmin,na,nb,xa0,sa0,iters=800):
    A=Leg(na,r,v,t0,tmin,MD,MA,D); B=Leg(nb,r,v,t0,tmin,MD,MA,D)
    def g(z):
        x=z[:9]; xa=x.copy(); xa[8]=z[9]
        ca=A.cons()[0]['fun'](xa); cb=B.cons()[0]['fun'](x)
        return np.r_[ca,cb]
    lin=lambda z: np.r_[B.cons()[1]['fun'](z[:9]), z[9]-z[1], (z[8]*3600-(z[9]*3600+D+1))/3600 if z[8]>z[9] else (z[9]*3600-(z[8]*3600+D+1))/3600]
    z0=np.r_[xa0,sa0/3600]
    from scipy.optimize import minimize
    res=minimize(lambda z:cost(z[:9]),z0,constraints=[{'type':'ineq','fun':g},{'type':'ineq','fun':lin}],method='SLSQP',options={'maxiter':iters,'ftol':1e-12})
    z=res.x; x=z[:9]; xa=x.copy(); xa[8]=z[9]
    oka,ia=A.certify(xa); okb,ib=B.certify(x)
    return res, oka, okb, ia, ib, z
