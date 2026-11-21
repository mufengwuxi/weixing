import json,glob,sys,re,numpy as np
MU=398600.4418;AGEO=42164.1696
def el(r,v):
    rn=np.linalg.norm(r);vn=np.linalg.norm(v);a=1/(2/rn-vn*vn/MU)
    return a,np.linalg.norm(((vn*vn-MU/rn)*r-np.dot(r,v)*v)/MU)
rows=[]
for f in sorted(glob.glob('out3m/r2E*.json')+glob.glob('out3/m_*.json')):
    tag=re.sub(r'.*/|\.json','',f);L=json.load(open(f))
    try: ini=json.load(open(f'tmp3/{tag}_init.json'))['state']
    except: continue
    r,v=np.array(ini[:3]),np.array(ini[3:]); a0,e0=el(r,v)
    n=len({s['target'] for s in L}|{s.get('also') for s in L if s.get('also')})
    dv=sum(np.linalg.norm(s[k])*1000 for s in L for k in('dv1','dv2') if s.get(k) is not None)
    s0=L[0]; ab=dv; a1,e1=a0,e0
    if s0['t1'] is not None and s0['t1']<1:
        a1,e1=el(r,v+np.array(s0['dv1']))
        if e1<1e-3: ab=dv-np.linalg.norm(s0['dv1'])*1000
        else: a1,e1=a0,e0
    rows.append((tag,n,dv,ab,a1-AGEO,e1,a0-AGEO))
for r in sorted(rows,key=lambda r:(-r[1],r[3])): print("%-32s n=%d dv=%7.3f eff=%7.3f a-AGEO=%7.2f e=%.5f a0=%6.1f"%r)
