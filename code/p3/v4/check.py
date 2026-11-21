import sys,json; sys.path.insert(0,'.')
from p2lib import *
L=json.load(open(sys.argv[1]))
burns=sorted([(x['t1'],np.array(x['dv1']),x['target']) for x in L if x['t1'] is not None]+[(x['t2'],np.array(x['dv2']),x['target']) for x in L if x['t1'] is not None],key=lambda b:b[0])
gaps=np.diff([b[0] for b in burns]); dv=[np.linalg.norm(b[1])*1000 for b in burns]
def st(T):
    r,v,tc=R1[:3].copy(),R1[3:].copy(),0.0
    for tb,d,_ in burns:
        if tb>T: break
        R,V=kepler(r,v,tb-tc); r,v,tc=R[0],V[0]+d,tb
    return r,v,tc
bad=[]
for x in L:
    s=x['obs_start']; r,v,tc=st(s); tt=np.arange(s,s+D+1e-4,0.05)
    if any(s<b[0]<s+D for b in burns): bad.append((x['target'],'burn in obs'))
    rO,_=kepler(r,v,tt-tc); rT,_=tpos(x['target'],tt); rho=rO-rT; d=np.linalg.norm(rho,axis=1)
    S=SUN(tt); us=S-rT; us/=np.linalg.norm(us,axis=1)[:,None]; ue=-rT/np.linalg.norm(rT,axis=1)[:,None]
    al=np.degrees(np.arccos(np.clip(np.sum(us*rho,1)/d,-1,1))); th=np.degrees(np.arccos(np.clip(np.sum(ue*rho,1)/d,-1,1)))
    if not(d.min()>=10 and d.max()<=50 and al.max()<=45 and th.max()<=45): bad.append((x['target'],round(d.min(),2),round(d.max(),2),round(al.max(),3),round(th.max(),3)))
print(sys.argv[1],'targets',len({x['target'] for x in L}),'dv %.3f'%sum(dv),'maxburn %.2f'%max(dv),'mingap %.3f'%gaps.min(),'bad',bad)
