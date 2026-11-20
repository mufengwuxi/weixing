import sys,json; sys.path.insert(0,'.')
from dual import *
k=[i for i,x in enumerate(LL) if x['target']=='b20'][0]
z=np.load('dual_z.npy'); x=z[:9]
r,v,t0,tmin=state_after(k)
A=Leg('b19',r,v,t0,tmin,MD,MA,D); B=Leg('b20',r,v,t0,tmin,MD,MA,D)
xa=x.copy(); xa[8]=z[9]
_,ia=A.certify(xa); _,ib=B.certify(x)
t1,t2,d1,d2,s20=x[0]*3600,x[1]*3600,x[2:5]/1000,x[5:8]/1000,x[8]*3600; s19=z[9]*3600
out=list(LL[:k])
out.append(dict(target='b19',t1=None,dv1=None,t2=None,dv2=None,obs_start=s19,**{kk:float(vv) for kk,vv in ia.items()}))
out.append(dict(target='b20',t1=t1,dv1=d1.tolist(),t2=t2,dv2=d2.tolist(),obs_start=s20,**{kk:float(vv) for kk,vv in ib.items()}))
r,v=apply(r,v,t0,t1,d1,t2,d2,s20+D); t0=s20+D; tmin=max(s20+D+1,t2+3601)
OP={m:opportunities(HZ,m) for m in NAMES}
rest=[(y['target'],y['obs_start']) for y in LL[k+1:]]
tot=0
for i,(n,s) in enumerate(rest):
    b=solve_leg(r,v,t0,tmin,n,s)
    if b is None:
        nxt=rest[i+1][1] if i+1<len(rest) else T_END-D
        for xx in sorted([xx for xx in OP[n] if tmin+4000<xx<nxt-3*3600 and abs(xx-s)>600],key=lambda xx:abs(xx-s))[:4]:
            b=solve_leg(r,v,t0,tmin,n,xx)
            if b: break
    if b is None: print('FAIL',n); continue
    c,xx,info=b; a1,a2,e1,e2,sx=xx[0]*3600,xx[1]*3600,xx[2:5]/1000,xx[5:8]/1000,xx[8]*3600
    r,v=apply(r,v,t0,a1,e1,a2,e2,sx+D); t0=sx+D; tmin=max(sx+D+1,a2+3601)
    out.append(dict(target=n,t1=a1,dv1=e1.tolist(),t2=a2,dv2=e2.tolist(),obs_start=sx,**{kk:float(vv) for kk,vv in info.items()}))
    print(n,'%.3f'%c,flush=True)
json.dump(out,open('best30_real.json','w'))
burns=[]
for y in out:
    if y['t1'] is None: continue
    burns+= [{"t_s":y['t1'],"dv_km_s":y['dv1'],"for":y['target']},{"t_s":y['t2'],"dv_km_s":y['dv2'],"for":y['target']}]
print('targets',len(out),'dv',sum(np.linalg.norm(b['dv_km_s']) for b in burns)*1000)
