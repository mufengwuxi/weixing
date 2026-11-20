import sys,json,time; sys.path.insert(0,'.'); from nat import *
seed=int(sys.argv[1]); N=int(sys.argv[2]); da_max=float(sys.argv[3]) if len(sys.argv)>3 else 90
rng=np.random.default_rng(seed); best=[]
for k in range(N):
    da=rng.uniform(-da_max,da_max); e=rng.uniform(0,0.00099); w=rng.uniform(0,360)
    if rng.random()<0.5: i=rng.uniform(0,0.5); O=rng.uniform(0,360)
    else: i=rng.uniform(0,10); O=rng.uniform(0,360)
    M=rng.uniform(-170,-100)-O-w  # 初始经度落在目标带附近
    r,v=elem2rv(AGEO+da,e,i,O,w,M); res=natural(r,v)
    best.append((len(res),[da,e,i,O,w,M],sorted(res)))
best.sort(key=lambda b:-b[0])
json.dump(best[:50],open(f'rs_{seed}.json','w'))
print(seed,[b[0] for b in best[:10]])
