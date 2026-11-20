import sys; sys.path.insert(0,'.'); from beam3 import *
import beam2
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
beam2.est2=est4
if __name__=='__main__':
    import time; t=time.time()
    e=est4(R1[:3],R1[3:],0,0,'b1',38.43*3600); print(e[0]*1000, e[1]/3600, e[3]/3600, time.time()-t)
