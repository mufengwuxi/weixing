import sys,time,pickle; sys.path.insert(0,'.'); from plan import *
K=2.85  # m/s per (deg/day)
H0=np.cross(R1[:3],R1[3:]); H0/=np.linalg.norm(H0)
X0=R1[:3]/np.linalg.norm(R1[:3]); Y0=np.cross(H0,X0)
A0=1/(2/np.linalg.norm(R1[:3])-R1[3:]@R1[3:]/MU); N0=np.sqrt(MU/A0**3)
def phase(p,t):  # deg, 在 r1 轨道面内的辐角减去参考角速度
    u=np.degrees(np.arctan2(p@Y0,p@X0)); return u-np.degrees(N0)*t
# 事件
EV=[]
for n in NAMES:
    for s in opportunities(H0,n):
        if s>T_END-D-60: continue
        p=tpos(n,s+D/2)[0][0]; EV.append((s,n,phase(p,s+D/2)))
EV.sort()
# 节点方向偏离主拱线的程度：决定交会时允许的漂移率
NODEANG={}
for n in NAMES:
    h=np.cross(TG[n][:3],TG[n][3:]); h/=np.linalg.norm(h); nd=np.cross(H0,h)
    NODEANG[n]=None if np.linalg.norm(nd)<1e-4 else np.degrees(np.arctan2(nd/np.linalg.norm(nd)@Y0,nd/np.linalg.norm(nd)@X0))%180
import os
WS=float(os.environ.get('WSCALE','1.0'))
def wmax(n):
    a=NODEANG[n]
    if a is None: return 0.6*WS
    off=min(abs(a-36.8),180-abs(a-36.8))
    return 3.0 if off<10 else (1.5*WS if off<30 else 0.4*WS)
def wrap(x): return (x+180)%360-180
PH0={n:[p for s_,m,p in EV if m==n] for n in NAMES}
PHT={n:[(s_,p) for s_,m,p in EV if m==n] for n in NAMES}
def ph_at(n,t):
    L=PHT[n]; i=min(range(len(L)),key=lambda k:abs(L[k][0]-t)); return L[i][1]
def run(width=3000, mu=0.3, min_gap=1.5*3600, lead=1.2*3600, maxdays=4.0, ct=0.0, rate=2.5):
    phi0=0.0  # r1 初始相位
    beam=[(0.0,0.0,phi0,0.0,frozenset(),())]  # cost,t,phi,omega,vis,seq
    best_by_count={}
    for depth in range(30):
        cand={}
        for cost,t,phi,om,vis,seq in beam:
            for s,n,ph in EV:
                if s<t+min_gap: continue
                if s>t+maxdays*86400: break
                if n in vis: continue
                tm=s+D/2
                need=wrap(ph-(phi+om*(tm-t)/86400))
                t1=t+lead*0.5 if depth>0 else 0.0
                T=(tm-t1)/86400
                wa=om+need/T  # 单脉冲
                c1=K*abs(wa-om); wb=wa
                opts=[]
                if abs(wb)<=wmax(n): opts.append((c1,wb))
                # 两脉冲：最后 T2 天降到 wb=目标允许范围
                for wbt in (0.0, np.sign(wa)*min(abs(wa),wmax(n))):
                    T2=min(1.0,T/2)
                    if T-T2<=0.05: continue
                    wa2=(need+om*0-wbt*T2+om*0)/(T-T2)+0  # 相对相位：Δφ = (wa2-om)(T-T2)+(wbt-om)T2
                    wa2=om+(need-(wbt-om)*T2)/(T-T2)
                    if abs(wa2)>4: continue
                    opts.append((K*(abs(wa2-om)+abs(wbt-wa2)),wbt))
                if not opts: continue
                c,wnew=min(opts)
                if c>20: continue
                nc=cost+c; key=(vis|{n})
                unv=[m for m in NAMES if m not in key]
                if unv and ct>0:
                    ps=np.array([wrap(ph_at(m,s)-ph) for m in unv]); lo,hi=min(ps.min(),0),max(ps.max(),0)
                    need=(hi-lo)+min(-lo,hi); rem=max((T_END-s)/86400,0.1)
                    sc=nc+mu*s/86400+ct*need+(100 if need/rem>rate else 0)
                else: sc=nc+mu*s/86400
                k2=(key,round(s/21600))
                if k2 not in cand or sc<cand[k2][0]: cand[k2]=(sc,(nc,s+D,ph+0.0,wnew,key,seq+((n,s,c),)))
        if not cand: break
        beam=[c for _,c in sorted(cand.values(),key=lambda x:x[0])[:width]]
        b=beam[0]; best_by_count[depth+1]=min(beam,key=lambda c:c[0])
        print('depth',depth+1,'cost %.1f t=%.2f'%(b[0],b[1]/86400),flush=True)
    return best_by_count
if __name__=='__main__':
    mu=float(sys.argv[1]); W=int(sys.argv[2]); md=float(sys.argv[3]); ct=float(sys.argv[4]); rt=float(sys.argv[5])
    t=time.time(); res=run(W,mu,maxdays=md,ct=ct,rate=rt)
    k=max(res); b=res[k]
    print('RESULT mu',mu,'count',k,'cost %.1f'%b[0],'end %.2f d'%(b[1]/86400),'%.0fs'%(time.time()-t))
    print(' '.join(f'{n}@{s/86400:.2f}:{c:.1f}' for n,s,c in b[5]))
    pickle.dump(res,open(f'sched_{mu}_{W}_{md}_{ct}_{rt}_ws{WS}.pkl','wb'))
