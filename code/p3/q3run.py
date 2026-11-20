"""问题三单星子任务：python q3run.py <tag> <sat r1|r2> <targets逗号> [da i O lam_rel] -- 结果写 out3/<tag>.json"""
import sys,os,json,subprocess,glob,numpy as np
sys.path.insert(0,'.')
tag,sat,tg=sys.argv[1],sys.argv[2],sys.argv[3]
env=dict(os.environ,P2_TARGETS=tg,P2_TAG='tmp3/'+tag,OMP_NUM_THREADS='1',OPENBLAS_NUM_THREADS='1')
os.makedirs('tmp3',exist_ok=True); os.makedirs('out3',exist_ok=True)
if sat=='r2':
    da,i,O,lr=map(float,sys.argv[4:8])
    from nat import elem2rv,AGEO
    sp=json.load(open('split.json'))
    r,v=elem2rv(AGEO+da,0.0,i,O,0.0,sp['lam0']+lr-O)
    json.dump({'state':list(r)+list(v),'design':{'da':da,'i':i,'O':O,'lam_rel':lr}},open(f'tmp3/{tag}_init.json','w'))
    env['P2_INIT']=f'tmp3/{tag}_init.json'
mu,W,md,ct,rt,ws=os.environ.get('SCHED','4 6000 3 4 2.6 2.5').split()
env['WSCALE']=ws
subprocess.run(['python3','sched.py',mu,W,md,ct,rt],env=env,stdout=open(f'tmp3/{tag}_sched.log','w'),stderr=subprocess.STDOUT)
pk=sorted(glob.glob(f'tmp3/{tag}_*.pkl'))[-1]
subprocess.run(['python3','opt.py',pk,f'out3/{tag}.json'],env=env,stdout=open(f'tmp3/{tag}_opt.log','w'),stderr=subprocess.STDOUT)
print(open(f'tmp3/{tag}_opt.log').read().strip().splitlines()[-1])
