import os
import sys,json; sys.path.insert(0,'.'); from realize import *
import realize as R
LL=json.load(open(os.path.join(os.path.dirname(os.path.abspath(__file__)),'best29_real.json')))
def state_after(k):
    r,v,t0,tmin=R1[:3].copy(),R1[3:].copy(),0.0,0.0
    for x in LL[:k]:
        r,v=apply(r,v,t0,x['t1'],np.array(x['dv1']),x['t2'],np.array(x['dv2']),x['obs_start']+D); t0=x['obs_start']+D; tmin=max(t0+1,x['t2']+3601)
    return r,v,t0,tmin
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
if __name__=='__main__':
    k=[i for i,x in enumerate(LL) if x['target']=='b20'][0]
    r,v,t0,tmin=state_after(k); x20=LL[k]
    xa0=np.r_[x20['t1']/3600,x20['t2']/3600,np.array(x20['dv1'])*1000,np.array(x20['dv2'])*1000,x20['obs_start']/3600]
    s19=[x for x in opportunities(HZ,'b19') if 22<x/86400<23][0]
    for sh in (0, -300, 300, -600):
        res,oka,okb,ia,ib,z=dual_leg(r,v,t0,tmin,'b19','b20',xa0,s19+sh)
        print(sh, res.success, oka, okb, 'cost %.3f'%cost(z[:9]), 'old %.3f'%cost(xa0), 's19 %.4f s20 %.4f'%(z[9]/24,z[8]/24), ia, ib)
        if oka and okb: np.save('dual_z.npy',z); break
