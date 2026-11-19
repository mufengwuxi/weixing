
#include "AstOD/SatData.hpp"
#include "AstOD/DataPreprocess.hpp"
#include "AstOD/CoordConvert.hpp"
#include "AstOD/PropagationWrapper.hpp"
#include "AstOD/BatchOD.hpp"
#include "AstCore/RunTime.hpp"
#include "AstCore/Time/TimePoint.hpp"
#include "AstCore/Coordinate/Frame/BuiltinFrame.hpp"
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

// Interactive plotting via matplot++ Qwt backend
#if defined(AST_WITH_MATPLOT)
#define NOMINMAX
#include <matplot/matplot.h>
#include "ast/QwtBackend.hpp"
#endif

using namespace ast;

int main(int argc, char* argv[])
{
    // ---- 1. Initialize project runtime (load ephemeris, leap seconds, etc.) ----
    aInfo("Initializing runtime...");
    errc_t err = aInitialize();
    if (err) {
        aError("Failed to initialize: %d", err);
        return 1;
    }
    aInfo("Runtime initialized successfully.");

    // ---- 2. Configuration ----
    bool dealWGS84 = false;
    std::string pathWGS84 = "../data/PG31.dat";  // WGS84 observation data

    if (dealWGS84) {
        aInfo("Converting WGS84 data to J2000...");
        aConvertWGS84ToJ2000File(pathWGS84);
        aInfo("WGS84->J2000 conversion complete.");
    }

    // ---- 3. Input OD start time ----
    std::string utcIn;
    if (argc >= 7) {
        utcIn = std::string(argv[1]) + " " + argv[2] + " " + argv[3] + " "
              + argv[4] + " " + argv[5] + " " + argv[6];
    } else {
        std::cout << "\nInput POD Start Time, format: YYYY MM DD HH MM SS\n";
        std::getline(std::cin, utcIn);
    }

    std::smatch m;
    std::regex_match(utcIn, m,
        std::regex(R"((\d{4})\s+(\d{1,2})\s+(\d{1,2})\s+(\d{1,2})\s+(\d{1,2})\s+(\d{1,2}))"));

    UTCData beginOD{};
    beginOD.utc_yy  = std::stoi(m[1]);
    beginOD.utc_mm  = std::stoi(m[2]);
    beginOD.utc_dd  = std::stoi(m[3]);
    beginOD.utc_hh  = std::stoi(m[4]);
    beginOD.utc_mm1 = std::stoi(m[5]);
    beginOD.utc_ss  = std::stoi(m[6]);

    // ---- 4. Preprocess GNSS data ----
    std::string dataPath;
    if (dealWGS84) {
        dataPath = pathWGS84.substr(0, pathWGS84.rfind('.')) + "toJ2000.dat";
    } else {
        dataPath = "E:/NUDT/2025-2026Doc2Autumn3/Research/ZJLab/AAreport/GNSSdata20250922.dat";
    }

    auto t0 = std::chrono::high_resolution_clock::now();

    std::vector<SatData> da1, da2;
    err = aDataDeal(dataPath, beginOD, da1, da2, 1000, false);
    if (err) {
        aError("Data preprocessing failed: %d", err);
        return err;
    }

    auto t1 = std::chrono::high_resolution_clock::now();
    std::cout << "DataDeal cost: "
              << std::chrono::duration<double>(t1 - t0).count() << " s\n";

    aInfo("Pre-OD arc (da1): %d points", (int)da1.size());
    aInfo("Post-OD arc (da2): %d points", (int)da2.size());

    // ---- 5. Batch least-squares OD ----
    Vector3d r0, v0;
    double DragCo;
    double SolarCo = 1.0;      // Cr fixed at 1.0, not estimated
    double mass_sat = 173.52;  // satellite mass [kg]
    double Drag_A   = 2.193;   // drag reference area [m^2]
    double Drag_S   = 13.91;   // SRP reference area [m^2]
    int    OrderNum = 30;      // gravity field max degree/order
    bool   Use3HourlyAp = true;

    err = aPreciseOrbitDetermination(
        da1, da2, mass_sat, Drag_A, Drag_S,
        r0, v0, DragCo, SolarCo,  // SolarCo=1.0 fixed, not changed
        OrderNum, Use3HourlyAp,
        5,    // max iterations
        35.0, // divergence threshold [m]
        10.0  // observation sigma [m]
    );

    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "POD cost: "
              << std::chrono::duration<double>(t2 - t1).count() << " s\n";

    if (err) {
        aError("POD failed: %d", err);
        return err;
    }

    std::cout << "\n=== POD Results ===" << std::endl;
    std::cout << "Position:  " << r0.x() << "  " << r0.y() << "  " << r0.z() << " m" << std::endl;
    std::cout << "Velocity:  " << v0.x() << "  " << v0.y() << "  " << v0.z() << " m/s" << std::endl;
    std::cout << "Cd: " << DragCo << "  (Cr fixed at " << SolarCo << ")" << std::endl;

    // ---- 6. Forward propagation for prediction ----
    TimePoint tpOD = TimePoint::FromUTC(
        da2[0].utc_yy, da2[0].utc_mm, da2[0].utc_dd,
        da2[0].utc_hh, da2[0].utc_mm1, (double)da2[0].utc_ss);

    double dragAreaMass = Drag_A / mass_sat;
    double srpAreaMass  = Drag_S / mass_sat;

    HPOPForceModel fm = aBuildODForceModel(
        OrderNum, Use3HourlyAp, dragAreaMass, srpAreaMass, DragCo, SolarCo);

    SpacecraftParam scParam;
    scParam.setDryMass(mass_sat);
    scParam.setFuelMass(0.0);
    scParam.setDragArea(Drag_A);
    scParam.setCd(DragCo);
    scParam.setSrpArea(Drag_S);
    scParam.setCr(SolarCo);

    // 24-hour prediction at 60-second steps
    std::vector<double> timeVec;
    for (double dt = 0.0; dt <= 86400.0; dt += 60.0)
        timeVec.push_back(dt);

    std::vector<Vector3d> posPred, velPred;
    err = aPropagateWithOutput(tpOD, r0, v0, timeVec, fm, scParam, posPred, velPred);

    auto t3 = std::chrono::high_resolution_clock::now();
    std::cout << "Prediction cost: "
              << std::chrono::duration<double>(t3 - t2).count() << " s\n";

    // ---- 7. Compute residuals against post-OD arc observations ----
    std::vector<double> timeB;
    TimePoint tpRef = TimePoint::FromUTC(
        da2[0].utc_yy, da2[0].utc_mm, da2[0].utc_dd,
        da2[0].utc_hh, da2[0].utc_mm1, (double)da2[0].utc_ss);

    for (const auto& row : da2) {
        TimePoint tp = TimePoint::FromUTC(
            row.utc_yy, row.utc_mm, row.utc_dd,
            row.utc_hh, row.utc_mm1, (double)row.utc_ss);
        timeB.push_back(std::abs(tp - tpRef));
    }

    std::vector<Vector3d> posVal, velVal;
    err = aPropagateWithOutput(tpOD, r0, v0, timeB, fm, scParam, posVal, velVal);

    if (!err) {
        double rms3d = 0.0, max12h = 0.0, max24h = 0.0;
        for (size_t i = 0; i < da2.size() && i < posVal.size(); ++i) {
            double dx = da2[i].rx - posVal[i].x();
            double dy = da2[i].ry - posVal[i].y();
            double dz = da2[i].rz - posVal[i].z();
            double d3 = std::sqrt(dx*dx + dy*dy + dz*dz);
            rms3d += d3 * d3;
            double dt = (i < timeB.size()) ? timeB[i] : 0.0;
            if (dt <= 43200.0) { if (d3 > max12h) max12h = d3; }
            if (dt <= 86400.0) { if (d3 > max24h) max24h = d3; }
        }
        rms3d = std::sqrt(rms3d / std::min(da2.size(), posVal.size()));

        std::cout << "\n===== 12h精度为:" << max12h << " m,24h精度为:" << max24h << " m =====" << std::endl;
        std::cout << "\n=== Validation Residuals ===" << std::endl;
        std::cout << "Prediction arc RMS: " << rms3d << " m" << std::endl;
        std::cout << "Prediction arc Max: " << max24h << " m" << std::endl;

        // ---- 8. Write error CSV + interactive plot ----
        std::vector<double> tS, dX, dY, dZ, dR;
        for (size_t i = 0; i < da2.size() && i < posVal.size(); ++i) {
            double dt = (i < timeB.size()) ? timeB[i] : 0.0;
            double dxv = da2[i].rx - posVal[i].x();
            double dyv = da2[i].ry - posVal[i].y();
            double dzv = da2[i].rz - posVal[i].z();
            tS.push_back(dt);
            dX.push_back(dxv); dY.push_back(dyv); dZ.push_back(dzv);
            dR.push_back(std::sqrt(dxv*dxv + dyv*dyv + dzv*dzv));
        }

        std::ofstream csv("OD_errors.csv");
        csv << "time_s,err_x_m,err_y_m,err_z_m,err_r_m\n";
        for (size_t i = 0; i < tS.size(); ++i)
            csv << tS[i] << "," << dX[i] << "," << dY[i] << "," << dZ[i] << "," << dR[i] << "\n";
        csv.close();
        aInfo("Error data written to OD_errors.csv");

#if defined(AST_WITH_MATPLOT)
        {
            aUseQwtBackend();
            using namespace matplot;
            std::vector<double> tH(tS.size());
            for (size_t i = 0; i < tS.size(); ++i) tH[i] = tS[i] / 3600.0;

            auto ax1 = subplot(4, 1, 0);
            plot(tH, dX, "-b"); ylabel("dX (m)"); grid(on);
            auto ax2 = subplot(4, 1, 1);
            plot(tH, dY, "-g"); ylabel("dY (m)"); grid(on);
            auto ax3 = subplot(4, 1, 2);
            plot(tH, dZ, "-r"); ylabel("dZ (m)"); grid(on);
            auto ax4 = subplot(4, 1, 3);
            plot(tH, dR, "-k"); xlabel("Time (hours)"); ylabel("dR (m)"); grid(on);
            for (auto ax : {ax1, ax2, ax3, ax4}) { ax->font("Times New Roman"); ax->font_size(11); }
            show();
        }
#endif
    }

    aInfo("Done.");
    return 0;
}
