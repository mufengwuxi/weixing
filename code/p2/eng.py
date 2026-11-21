import numpy as np, warnings, openpyxl
from scipy.interpolate import CubicSpline
from scipy.optimize import minimize
warnings.filterwarnings('ignore')
MU=398600.4418; AU=149597870.7; T_END=2592000.0; EPS=1e-6
import os
XLSX=os.path.join(os.path.dirname(os.path.abspath(__file__)),'..','..','data','数据.xlsx')
import sys; sys.path.insert(0,os.path.join(os.path.dirname(os.path.abspath(__file__)),'..'))
from datafile import read_states  # 自动识别 km 或 m 单位
R1,TG=read_states(XLSX)
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
        s.ca=np.cos(np.radians(45-ma)); s.k=np.arange(0,D+1e-9,1.0)
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
        ok=(d.min()>=10+s.md-tol and d.max()<=50-s.md+tol and al.max()<=45-s.ma+tol and th.max()<=45-s.ma+tol
            and x[0]*3600>=s.tmin-1e-3 and (x[1]-x[0])*3600>=3600-1e-3 and x[8]>=x[1]-1e-9 and np.linalg.norm(x[2:5])<=30 and np.linalg.norm(x[5:8])<=30)
        return ok, dict(d_min=d.min(),d_max=d.max(),alpha_max=al.max(),theta_max=th.max())
def cost(x): return np.sqrt(x[2:5]@x[2:5]+EPS)+np.sqrt(x[5:8]@x[5:8]+EPS)
