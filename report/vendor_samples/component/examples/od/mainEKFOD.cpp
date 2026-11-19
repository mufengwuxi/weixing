
#include "AstOD/SatData.hpp"
#include "AstOD/DataPreprocess.hpp"
#include "AstOD/PropagationWrapper.hpp"
#include "AstCore/Propagator/HPOP/HPOP.hpp"
#include "AstCore/RunTime.hpp"
#include "AstCore/Time/TimePoint.hpp"
#include "AstCore/Orbit/Spacecraft/SpacecraftParam.hpp"
#include "AstMath/Vector.hpp"
#include "AstUtil/Logger.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <regex>
#include <chrono>
#include <cmath>

#if defined(AST_WITH_MATPLOT)
#define NOMINMAX
#include <matplot/matplot.h>
#include "ast/QwtBackend.hpp"
#endif

using namespace ast;

static void matMul7(const double A[7][7], const double B[7][7], double C[7][7]) {
    for (int i=0;i<7;++i) for (int j=0;j<7;++j)
        { C[i][j]=0; for (int k=0;k<7;++k) C[i][j]+=A[i][k]*B[k][j]; }
}
static void matTranspose7(const double A[7][7], double AT[7][7]) {
    for (int i=0;i<7;++i) for (int j=0;j<7;++j) AT[j][i]=A[i][j];
}
template<int N>
static bool matInv(const double A[N][N], double inv[N][N]) {
    double M[N][2*N]={};
    for (int i=0;i<N;++i){ for (int j=0;j<N;++j) M[i][j]=A[i][j]; M[i][N+i]=1.0; }
    for (int k=0;k<N;++k){
        int mr=k; double mv=std::fabs(M[k][k]);
        for (int i=k+1;i<N;++i) if (std::fabs(M[i][k])>mv){ mv=std::fabs(M[i][k]); mr=i; }
        if (mv<1e-30) return false;
        if (mr!=k) for (int j=k;j<2*N;++j) std::swap(M[k][j],M[mr][j]);
        double piv=M[k][k]; for (int j=k;j<2*N;++j) M[k][j]/=piv;
        for (int i=0;i<N;++i){ if(i==k)continue; double f=M[i][k];
            for (int j=k;j<2*N;++j) M[i][j]-=f*M[k][j]; }
    }
    for (int i=0;i<N;++i) for (int j=0;j<N;++j) inv[i][j]=M[i][j+N];
    return true;
}

static errc_t aEKFOrbitDetermination(
    const std::vector<SatData>& da1, const std::vector<SatData>& da2,
    double mass_sat, double Drag_A, double Drag_S,
    Vector3d& r1, Vector3d& v1, double& Drag_co, double SRP_co,
    int orderNum, bool use3HourlyAp, double sigmaMeas)
{
    if (da1.empty()) return 1;
    int nObs=(int)da1.size(), idxStart=nObs-1, idxEnd=0;
    double dragAreaMass=Drag_A/mass_sat, srpAreaMass=Drag_S/mass_sat;
    double cdEff=2.2;  // Cd directly (matching MATLAB Y0(7))

    Vector3d rCur{da1[idxStart].rx,da1[idxStart].ry,da1[idxStart].rz};
    Vector3d vCur{da1[idxStart].vx,da1[idxStart].vy,da1[idxStart].vz};
    TimePoint tPrev=TimePoint::FromUTC(da1[idxStart].utc_yy,da1[idxStart].utc_mm,
        da1[idxStart].utc_dd,da1[idxStart].utc_hh,da1[idxStart].utc_mm1,(double)da1[idxStart].utc_ss);

    aInfo("EKF: OD epoch=%04d-%02d-%02d %02d:%02d:%02d, %d obs, Cd0=%.4f",
          da2[0].utc_yy,da2[0].utc_mm,da2[0].utc_dd,da2[0].utc_hh,da2[0].utc_mm1,da2[0].utc_ss,nObs,cdEff);

    double P[7][7]={};
    P[0][0]=P[1][1]=P[2][2]=1e8; P[3][3]=P[4][4]=P[5][5]=100; P[6][6]=10.0;
    double Rpos=sigmaMeas*sigmaMeas;

    std::ofstream fdbg("EKF_steps.txt");
    fdbg<<"step rx ry rz vx vy vz Cd\n"<<std::fixed<<std::setprecision(3);

    for (int k=idxStart;k>=idxEnd;--k){
        const auto& obs=da1[k];
        TimePoint tk=TimePoint::FromUTC(obs.utc_yy,obs.utc_mm,obs.utc_dd,
            obs.utc_hh,obs.utc_mm1,(double)obs.utc_ss);
        double dt=tk-tPrev;
        int processed=idxStart-k+1;

        if (k==idxStart){
            aInfo("  [init] r=(%.0f,%.0f,%.0f) v=(%.1f,%.1f,%.1f) Cd=%.4f",
                  rCur.x(),rCur.y(),rCur.z(),vCur.x(),vCur.y(),vCur.z(),cdEff);
            fdbg<<processed<<" "<<rCur.x()<<" "<<rCur.y()<<" "<<rCur.z()<<" "
                <<vCur.x()<<" "<<vCur.y()<<" "<<vCur.z()<<" "<<cdEff<<"\n";
            continue;
        }

        Vector3d rPre=rCur, vPre=vCur;
        double cdPre=cdEff;
        double Phi[7][7]={};
        for (int i=0;i<7;++i) Phi[i][i]=1.0;

        if (dt>0.5){
            for (int i=0;i<7;++i) for (int j=0;j<7;++j) Phi[i][j]=(i==j)?1.0:0.0;
            TimePoint tEnd=tk;
            HPOPForceModel fm=aBuildODForceModel(orderNum,use3HourlyAp,dragAreaMass,srpAreaMass,cdEff,SRP_co);
            SpacecraftParam sc; sc.setDryMass(mass_sat); sc.setFuelMass(0);
            sc.setDragArea(Drag_A); sc.setCd(cdEff); sc.setSrpArea(Drag_S); sc.setCr(SRP_co);
            HPOP prop; prop.setForceModel(fm); prop.setSpacecraftParam(sc);
            TimePoint tN=tEnd;
            if (prop.propagate(tPrev,tN,rCur,vCur)){ aError("EKF: prop fail %d",k); return 3; }

            Phi[0][3]=Phi[1][4]=Phi[2][5]=dt;
            {
                double eps=std::max(0.1,0.01*std::fabs(cdPre));
                double cdP=cdPre+eps;
                HPOPForceModel fmP=aBuildODForceModel(orderNum,use3HourlyAp,dragAreaMass,srpAreaMass,cdP,SRP_co);
                SpacecraftParam scP; scP.setDryMass(mass_sat); scP.setFuelMass(0);
                scP.setDragArea(Drag_A); scP.setCd(cdP); scP.setSrpArea(Drag_S); scP.setCr(SRP_co);
                HPOP propP; propP.setForceModel(fmP); propP.setSpacecraftParam(scP);
                Vector3d rP=rPre, vP=vPre; TimePoint tP=tEnd;
                if (!propP.propagate(tPrev,tP,rP,vP)){
                    double invEps=1.0/eps;
                    Phi[0][6]=(rP.x()-rCur.x())*invEps;
                    Phi[1][6]=(rP.y()-rCur.y())*invEps;
                    Phi[2][6]=(rP.z()-rCur.z())*invEps;
                    Phi[3][6]=(vP.x()-vCur.x())*invEps;
                    Phi[4][6]=(vP.y()-vCur.y())*invEps;
                    Phi[5][6]=(vP.z()-vCur.z())*invEps;
                }
            }

            double PT[7][7],T[7][7]; matTranspose7(Phi,PT); matMul7(P,PT,T); matMul7(Phi,T,P);
            for (int i=0;i<7;++i) for (int j=i+1;j<7;++j) P[i][j]=P[j][i]=0.5*(P[i][j]+P[j][i]);
        }

        double innov[3]={obs.rx-rCur.x(),obs.ry-rCur.y(),obs.rz-rCur.z()};
        double S[3][3]={{P[0][0]+Rpos,P[0][1],P[0][2]},{P[1][0],P[1][1]+Rpos,P[1][2]},{P[2][0],P[2][1],P[2][2]+Rpos}};
        double Si[3][3]; if (!matInv<3>(S,Si)){ tPrev=tk; continue; }

        double K[7][3]={};
        for (int i=0;i<7;++i) for (int j=0;j<3;++j)
            K[i][j]=P[i][0]*Si[0][j]+P[i][1]*Si[1][j]+P[i][2]*Si[2][j];

        rCur.x()+=K[0][0]*innov[0]+K[0][1]*innov[1]+K[0][2]*innov[2];
        rCur.y()+=K[1][0]*innov[0]+K[1][1]*innov[1]+K[1][2]*innov[2];
        rCur.z()+=K[2][0]*innov[0]+K[2][1]*innov[1]+K[2][2]*innov[2];
        vCur.x()+=K[3][0]*innov[0]+K[3][1]*innov[1]+K[3][2]*innov[2];
        vCur.y()+=K[4][0]*innov[0]+K[4][1]*innov[1]+K[4][2]*innov[2];
        vCur.z()+=K[5][0]*innov[0]+K[5][1]*innov[1]+K[5][2]*innov[2];
        cdEff    +=K[6][0]*innov[0]+K[6][1]*innov[1]+K[6][2]*innov[2];

        double Pn[7][7]={};
        for (int i=0;i<7;++i) for (int j=0;j<7;++j)
            { double khp=0; for (int m=0;m<3;++m) khp+=K[i][m]*P[m][j]; Pn[i][j]=P[i][j]-khp; }
        for (int i=0;i<7;++i) for (int j=0;j<7;++j) P[i][j]=Pn[i][j];

        tPrev=tk;

        fdbg<<processed<<" "<<rCur.x()<<" "<<rCur.y()<<" "<<rCur.z()<<" "
            <<vCur.x()<<" "<<vCur.y()<<" "<<vCur.z()<<" "<<cdEff<<"\n";

        if (processed<=5||processed%50==0||k==idxEnd)
            aInfo("  step %d/%d: Cd=%.4f tr(P)=%.0f STMB_dr=(%.2e,%.2e,%.2e)",
                  processed,nObs,cdEff,P[0][0]+P[1][1]+P[2][2],Phi[0][6],Phi[1][6],Phi[2][6]);
    }

    r1=rCur; v1=vCur; Drag_co=cdEff; fdbg.close();
    aInfo("EKF final: r=(%.1f,%.1f,%.1f) v=(%.3f,%.3f,%.3f) Cd=%.4f",
          r1.x(),r1.y(),r1.z(),v1.x(),v1.y(),v1.z(),cdEff);
    return 0;
}

int main(int argc, char* argv[]){
    aInfo("Initializing runtime...");
    if (aInitialize()){ aError("Init failed"); return 1; }
    aInfo("Runtime initialized.");

    std::string utcIn;
    if (argc>=7) utcIn=std::string(argv[1])+" "+argv[2]+" "+argv[3]+" "+argv[4]+" "+argv[5]+" "+argv[6];
    else { std::cout<<"\nInput POD Start Time (YYYY MM DD HH MM SS):\n"; std::getline(std::cin,utcIn); }
    std::smatch m; std::regex_match(utcIn,m,std::regex(R"((\d{4})\s+(\d{1,2})\s+(\d{1,2})\s+(\d{1,2})\s+(\d{1,2})\s+(\d{1,2}))"));
    UTCData b{}; b.utc_yy=std::stoi(m[1]); b.utc_mm=std::stoi(m[2]); b.utc_dd=std::stoi(m[3]);
    b.utc_hh=std::stoi(m[4]); b.utc_mm1=std::stoi(m[5]); b.utc_ss=std::stoi(m[6]);

    auto t0=std::chrono::high_resolution_clock::now();
    std::vector<SatData> da1,da2;
    if (aDataDeal("E:/NUDT/2025-2026Doc2Autumn3/Research/ZJLab/AAreport/GNSSdata20250922.dat",b,da1,da2,1000,false))
        { aError("Data preprocessing failed"); return 1; }
    std::cout<<"DataDeal: "<<std::chrono::duration<double>(std::chrono::high_resolution_clock::now()-t0).count()<<" s\n";
    aInfo("Pre-OD: %d, Post-OD: %d",(int)da1.size(),(int)da2.size());

    Vector3d r0,v0; double DragCo,SolarCo=1.0;
    errc_t err=aEKFOrbitDetermination(da1,da2,173.52,2.193,13.91,r0,v0,DragCo,SolarCo,30,true,10.0);
    auto t2=std::chrono::high_resolution_clock::now();
    std::cout<<"EKF cost: "<<std::chrono::duration<double>(t2-t0).count()<<" s\n";
    if (err){ aError("EKF failed: %d",err); return err; }

    std::cout<<"\n=== EKF Results ==="<<std::endl;
    std::cout<<"Position: "<<r0.x()<<" "<<r0.y()<<" "<<r0.z()<<" m\n";
    std::cout<<"Velocity: "<<v0.x()<<" "<<v0.y()<<" "<<v0.z()<<" m/s\n";
    std::cout<<"Cd: "<<DragCo<<" (Cr fixed at "<<SolarCo<<")"<<std::endl;

    TimePoint tpOD=TimePoint::FromUTC(da2[0].utc_yy,da2[0].utc_mm,da2[0].utc_dd,
        da2[0].utc_hh,da2[0].utc_mm1,(double)da2[0].utc_ss);
    double dam=2.193/173.52,sam=13.91/173.52;
    HPOPForceModel fm=aBuildODForceModel(30,true,dam,sam,DragCo,SolarCo);
    SpacecraftParam sc; sc.setDryMass(173.52); sc.setFuelMass(0);
    sc.setDragArea(2.193); sc.setCd(DragCo); sc.setSrpArea(13.91); sc.setCr(SolarCo);

    std::vector<double> tB; for (auto& row:da2){
        TimePoint tp=TimePoint::FromUTC(row.utc_yy,row.utc_mm,row.utc_dd,row.utc_hh,row.utc_mm1,(double)row.utc_ss);
        tB.push_back(std::abs(tp-tpOD)); }
    std::vector<Vector3d> pV,vV;
    err=aPropagateWithOutput(tpOD,r0,v0,tB,fm,sc,pV,vV);
    std::cout<<"Prediction: "<<std::chrono::duration<double>(std::chrono::high_resolution_clock::now()-t2).count()<<" s\n";

    if (!err){
        double rms=0,m12=0,m24=0; std::vector<double> tS,dX,dY,dZ,dR;
        for (size_t i=0;i<da2.size()&&i<pV.size();++i){
            double dx=da2[i].rx-pV[i].x(),dy=da2[i].ry-pV[i].y(),dz=da2[i].rz-pV[i].z();
            double d3=std::sqrt(dx*dx+dy*dy+dz*dz); rms+=d3*d3;
            double dt=(i<tB.size())?tB[i]:0;
            if (dt<=43200&&d3>m12)m12=d3; if (dt<=86400&&d3>m24)m24=d3;
            tS.push_back(dt); dX.push_back(dx); dY.push_back(dy); dZ.push_back(dz); dR.push_back(d3);
        }
        rms=std::sqrt(rms/std::min(da2.size(),pV.size()));
        std::cout<<"\n===== 12h:"<<m12<<" m, 24h:"<<m24<<" m ====="<<std::endl;
        std::cout<<"RMS: "<<rms<<" m, Max: "<<m24<<" m\n";
        std::ofstream csv("OD_errors.csv"); csv<<"time_s,err_x_m,err_y_m,err_z_m,err_r_m\n";
        for (size_t i=0;i<tS.size();++i) csv<<tS[i]<<","<<dX[i]<<","<<dY[i]<<","<<dZ[i]<<","<<dR[i]<<"\n";
        csv.close();

#if defined(AST_WITH_MATPLOT)
        { AST_USING_NAMESPACE; aUseQwtBackend(); using namespace matplot;
            std::vector<double> tH(tS.size()); for(size_t i=0;i<tS.size();++i)tH[i]=tS[i]/3600.0;
            auto a1=subplot(4,1,0);plot(tH,dX,"-b");ylabel("dX (m)");grid(on);
            auto a2=subplot(4,1,1);plot(tH,dY,"-g");ylabel("dY (m)");grid(on);
            auto a3=subplot(4,1,2);plot(tH,dZ,"-r");ylabel("dZ (m)");grid(on);
            auto a4=subplot(4,1,3);plot(tH,dR,"-k");xlabel("Time (h)");ylabel("dR (m)");grid(on);
            for(auto ax:{a1,a2,a3,a4}){ax->font("Times New Roman");ax->font_size(11);} show(); }
#endif
    }
    aInfo("Done."); return 0;
}
