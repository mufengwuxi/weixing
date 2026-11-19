
#include "AstOD/SatData.hpp"
#include "AstOD/DataPreprocess.hpp"
#include "AstOD/HerrickGibbs.hpp"
#include "AstOD/BatchLSEstimator.hpp"
#include "AstOD/TrackingAERModel.hpp"
#include "AstCore/RunTime.hpp"
#include "AstCore/GeodeticPoint.hpp"
#include "AstCore/Orbit/OrbitElement.hpp"
#include "AstUtil/Logger.hpp"
#include "AstUtil/Constants.h"
#include "AstUtil/Util/Math.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace ast;

int main()
{
    // ---- 1. Initialise runtime ----
    aInfo("Initialising runtime...");
    errc_t err = aInitialize();
    if (err) {
        aError("Failed to initialise: %d", err);
        return 1;
    }
    aInfo("Runtime initialised.");

    // ---- 2. Parse GEOSC file ----
    const std::string geoscPath =
        "C:/Users/Yangcey/Desktop/reference/V1001.geosc";

    std::vector<GeoscObs> observations;
    err = aProcessGeosc(geoscPath, observations);
    if (err) {
        aError("aProcessGeosc failed: %d", err);
        return err;
    }

    // ---- 3. Summary statistics ----
    std::cout << "\n========== GEOSC Parse Summary ==========" << std::endl;
    std::cout << "Total observations: " << observations.size() << std::endl;

    // Per-station count
    std::map<int, int> stationCount;
    for (const auto& o : observations)
        ++stationCount[o.gbSiteId];

    std::cout << "Ground stations found: " << stationCount.size() << std::endl;
    for (const auto& sc : stationCount)
        std::cout << "  Station " << sc.first << ": "
                  << sc.second << " epochs" << std::endl;

    // Time range (using UTC fields)
    if (!observations.empty()) {
        auto cmpTime = [](const GeoscObs& a, const GeoscObs& b) {
            if (a.utc_yy != b.utc_yy) return a.utc_yy < b.utc_yy;
            if (a.utc_mm != b.utc_mm) return a.utc_mm < b.utc_mm;
            if (a.utc_dd != b.utc_dd) return a.utc_dd < b.utc_dd;
            if (a.utc_hh != b.utc_hh) return a.utc_hh < b.utc_hh;
            if (a.utc_min != b.utc_min) return a.utc_min < b.utc_min;
            return a.utc_sec < b.utc_sec;
        };
        auto minObs = *std::min_element(observations.begin(), observations.end(), cmpTime);
        auto maxObs = *std::max_element(observations.begin(), observations.end(), cmpTime);

        auto fmtUTC = [](const GeoscObs& o) {
            char buf[32];
            std::snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%06.3f",
                          o.utc_yy, o.utc_mm, o.utc_dd,
                          o.utc_hh, o.utc_min, o.utc_sec);
            return std::string(buf);
        };

        std::cout << "First epoch: " << fmtUTC(minObs) << std::endl;
        std::cout << "Last epoch:  " << fmtUTC(maxObs) << std::endl;
    }

    // ---- 4. Print first 20 observations (table) ----
    std::cout << "\n========== First 20 Observations ==========" << std::endl;
    std::cout << std::left
              << std::setw(6)  << "Idx"
              << std::setw(8)  << "Station"
              << std::setw(22) << "UTC"
              << std::setw(18) << "Range(m)"
              << std::setw(14) << "Doppler(m/s)"
              << std::setw(14) << "Azimuth(deg)"
              << std::setw(14) << "Elev(deg)"
              << std::endl;
    std::cout << std::string(96, '-') << std::endl;

    auto fmtUTC = [](const GeoscObs& o) {
        char buf[32];
        std::snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%06.3f",
                      o.utc_yy, o.utc_mm, o.utc_dd,
                      o.utc_hh, o.utc_min, o.utc_sec);
        return std::string(buf);
    };

    int showCount = std::min(20, (int)observations.size());
    for (int i = 0; i < showCount; ++i) {
        const auto& o = observations[i];
        std::cout << std::left
                  << std::setw(6)  << i
                  << std::setw(8)  << o.gbSiteId
                  << std::setw(22) << fmtUTC(o)
                  << std::setw(18) << std::fixed << std::setprecision(7) << o.range
                  << std::setw(14) << std::fixed << std::setprecision(3) << o.doppler
                  << std::setw(14) << std::fixed << std::setprecision(5) << o.azimuth
                  << std::setw(14) << std::fixed << std::setprecision(5) << o.elevation
                  << std::endl;
    }

    // ---- 5. Write CSV output ----
    std::ofstream csv("data/geosc_output.csv");
    csv << "index,utc_yy,utc_mm,utc_dd,utc_hh,utc_min,utc_sec,"
        << "station,sat_id,"
        << "range_m,range_std_m,"
        << "doppler_m_s,doppler_std_m_s,"
        << "azimuth_deg,azimuth_std_deg,"
        << "elevation_deg,elevation_std_deg\n";
    for (size_t i = 0; i < observations.size(); ++i) {
        const auto& o = observations[i];
        csv << i << ","
            << o.utc_yy << ","
            << o.utc_mm << ","
            << o.utc_dd << ","
            << o.utc_hh << ","
            << o.utc_min << ","
            << std::fixed << std::setprecision(6) << o.utc_sec << ","
            << o.gbSiteId << ","
            << o.satId << ","
            << std::fixed << std::setprecision(7) << o.range << ","
            << std::fixed << std::setprecision(7) << o.rangeStdDev << ","
            << std::fixed << std::setprecision(7) << o.doppler << ","
            << std::fixed << std::setprecision(7) << o.dopplerStdDev << ","
            << std::fixed << std::setprecision(7) << o.azimuth << ","
            << std::fixed << std::setprecision(7) << o.azimuthStdDev << ","
            << std::fixed << std::setprecision(7) << o.elevation << ","
            << std::fixed << std::setprecision(7) << o.elevationStdDev
            << "\n";
    }
    csv.close();
    aInfo("Full results written to geosc_output.csv (%d rows)", (int)observations.size());

    // ---- 6. Herrick-Gibbs IOD test ----
    // Station 4105: lat=-7.270022778 deg, lon=72.370023056 deg, alt=-56.8 m
    // Use observations at indices 488, 490, 492 (station 4105 1st/3rd/5th obs)
    Vector3d r2_iod, v2_iod;
    TimePoint tpOD;
    bool iodOK = false;
    {
        double latRad = -7.270022778 * kDegToRad;
        double lonRad =  72.370023056 * kDegToRad;
        GeodeticPoint station4105(latRad, lonRad, -56.8);

        if ((int)observations.size() <= 492) {
            aError("Not enough observations for Herrick-Gibbs test "
                   "(need >= 493, got %d)", (int)observations.size());
        } else {
            const GeoscObs& obs1 = observations[488];
            const GeoscObs& obs2 = observations[490];
            const GeoscObs& obs3 = observations[492];

            errc_t errHG = aHerrickGibbsIOD(station4105, obs1, obs2, obs3,
                                            r2_iod, v2_iod);
            if (errHG) {
                aError("aHerrickGibbsIOD failed with error %d", errHG);
            } else {
                iodOK = true;
                tpOD = TimePoint::FromUTC(obs2.utc_yy, obs2.utc_mm,
                    obs2.utc_dd, obs2.utc_hh, obs2.utc_min, obs2.utc_sec);
            }
        }
    }

    // ---- 7. Batch LS-OD using all 4 stations ----
    if (iodOK) {
        std::cout << "\n========== Batch LS Orbit Determination ==========" << std::endl;

        // Print HPOP dynamics configuration
        std::cout << "\n  Dynamics config (HPOP):" << std::endl;
        std::cout << "    Gravity:    WGS84 30x30" << std::endl;
        std::cout << "    Drag:       NRLMSISE00 (SW-All.txt, F10.7=150, 3-hr Ap)" << std::endl;
        std::cout << "    SRP:        Dual-cone shadow, true Sun pos, Cr=1.0" << std::endl;
        std::cout << "    3rd-body:   Moon + Sun" << std::endl;
        std::cout << "    Relativity: on" << std::endl;
        std::cout << "    SC mass:    1000 kg" << std::endl;
        std::cout << "    Drag area:  20 m^2, Cd=2.2" << std::endl;
        std::cout << "    SRP area:   20 m^2" << std::endl;
        std::cout << "    Propagator: HPOP RKF78 (fwd+bwd + Lagrange interp)" << std::endl;
        std::cout << "    OD epoch:   2015-06-01 14:13:20.000 UTC" << std::endl;
        std::cout << "    Estimate:   6-D state (r,v), Cd fixed" << std::endl;

        // Build stations map
        std::map<int, GeodeticPoint> stations;
        stations[4105] = GeodeticPoint(-7.270022778 * kDegToRad,
                                        72.370023056 * kDegToRad, -56.8);
        stations[3707] = GeodeticPoint(13.615194166 * kDegToRad,
                                       144.856074166 * kDegToRad, 217.0);
        stations[3107] = GeodeticPoint(34.822609417 * kDegToRad,
                                       239.498148250 * kDegToRad, 271.51);
        stations[3507] = GeodeticPoint(21.562280000 * kDegToRad,
                                       201.757891389 * kDegToRad, 428.42);

        // Select observations at indices 491-506
        std::vector<GeoscObs> selObs;
        for (int idx = 491; idx <= 517; ++idx) {
            if (idx < (int)observations.size()) {
                const auto& o = observations[idx];
                if (stations.find(o.gbSiteId) != stations.end())
                    selObs.push_back(o);
            }
        }
        std::cout << "  Selected " << selObs.size() << " observations "
                  << "(indices 491-517)" << std::endl;

        // ---- BatchLSEstimator (Gauss-Newton, propagate+interpolate) ----
        constexpr double mass_sat = 1000.0;
        constexpr double Drag_A   = 20.0;
        constexpr double Drag_S   = 20.0;
        constexpr double Cd_init  = 2.2;
        constexpr double Cr_init  = 1.0;

        double dragAreaMass = Drag_A / mass_sat;
        double srpAreaMass  = Drag_S / mass_sat;
        double cdEffInit    = Cd_init * dragAreaMass;
        double crEff        = Cr_init * srpAreaMass;

        TrackingAERModel model(selObs, stations, tpOD, false);  // 6-D (r,v)
        BatchLSConfig   config;
        config.orderNum     = 21;
        config.use3HourlyAp = true;
        config.maxIter      = 10;

        BatchLSResult result;
        int lsErr = BatchLSEstimator::run(
            tpOD, r2_iod, v2_iod, cdEffInit,
            mass_sat, Drag_A, Drag_S, crEff,
            model, config, result);

        Vector3d r_ls  = result.rEst;
        Vector3d v_ls  = result.vEst;
        double   cd_ls = result.cdEst;

        if (lsErr) {
            aError("BatchLSEstimator failed: %d", lsErr);
        } else {
            // Print LS results with Keplerian elements
            const double km = 1.0e-3;
            constexpr double GM = kEarthGrav_EGM2008;
            const double rad2deg = 1.0 / kDegToRad;

            std::cout << std::fixed;
            std::cout << "\n  LS-OD Result (ICRF):\n"
                      << "    r = [" << std::setprecision(3)
                      << r_ls.x() * km << ", " << r_ls.y() * km << ", "
                      << r_ls.z() * km << "] km  |r|="
                      << r_ls.norm() * km << " km\n"
                      << "    v = [" << std::setprecision(6)
                      << v_ls.x() * km << ", " << v_ls.y() * km << ", "
                      << v_ls.z() * km << "] km/s  |v|="
                      << v_ls.norm() * km << " km/s\n";

            // Keplerian elements (using library rv2coe via aCartToOrbElem)
            OrbElem elem;
            aCartToOrbElem(r_ls, v_ls, GM, elem);

            // True argument of latitude = ω + ν, normalised to [0°, 360°)
            double argLat = aNormalizeAngle0To2Pi(elem.argper_ + elem.trueA_);

            std::cout << "\n  Keplerian elements (LS-OD, ICRF):\n"
                      << std::setprecision(3)
                      << "    SemiMajorAxis     = " << elem.a_ * km << " km\n"
                      << std::setprecision(6)
                      << "    Eccentricity      = " << elem.e_ << "\n"
                      << std::setprecision(4)
                      << "    TrueArgofLatitude = " << argLat * rad2deg << " deg\n"
                      << "    Inclination       = " << elem.i_ * rad2deg << " deg\n"
                      << "    RAAN              = " << elem.raan_ * rad2deg << " deg\n"
                      << "    ArgofPerigee      = " << elem.argper_ * rad2deg << " deg\n";

            // ---- Covariance matrix (6×6) ----
            if (!result.covariance.empty() && (int)result.covariance.size() >= 36) {
                const auto& C = result.covariance;  // row-major, 6×6
                const int dim = 6;

                // Standard deviations (sqrt of diagonal)
                double sigma[6];
                for (int i = 0; i < dim; ++i)
                    sigma[i] = std::sqrt(C[i * dim + i]);

                // Correlation coefficients
                auto corr = [&](int i, int j) {
                    return C[i * dim + j] / (sigma[i] * sigma[j]);
                };

                std::cout << "\n  Covariance (21 elements):\n"
                          << std::setprecision(6);
                std::cout << "    R_sigma              = " << sigma[0] << " m\n";
                std::cout << "    I_sigma              = " << sigma[1] << " m\n";
                std::cout << "    C_sigma              = " << sigma[2] << " m\n";
                std::cout << "    Rdot_sigma           = " << sigma[3] << " m/s\n";
                std::cout << "    Idot_sigma           = " << sigma[4] << " m/s\n";
                std::cout << "    Cdot_sigma           = " << sigma[5] << " m/s\n";
                std::cout << "    RI_correlation       = " << corr(0,1) << "\n";
                std::cout << "    RC_correlation       = " << corr(0,2) << "\n";
                std::cout << "    RRdot_correlation    = " << corr(0,3) << "\n";
                std::cout << "    RIdot_correlation    = " << corr(0,4) << "\n";
                std::cout << "    RCdot_correlation    = " << corr(0,5) << "\n";
                std::cout << "    IC_correlation       = " << corr(1,2) << "\n";
                std::cout << "    IRdot_correlation    = " << corr(1,3) << "\n";
                std::cout << "    IIdot_correlation    = " << corr(1,4) << "\n";
                std::cout << "    ICdot_correlation    = " << corr(1,5) << "\n";
                std::cout << "    CRdot_correlation    = " << corr(2,3) << "\n";
                std::cout << "    CIdot_correlation    = " << corr(2,4) << "\n";
                std::cout << "    CCdot_correlation    = " << corr(2,5) << "\n";
                std::cout << "    RdotIdot_correlation = " << corr(3,4) << "\n";
                std::cout << "    RdotCdot_correlation = " << corr(3,5) << "\n";
                std::cout << "    IdotCdot_correlation = " << corr(4,5) << "\n";
            }

            // Compare IOD vs LS
            double dR = (r_ls - r2_iod).norm();
            double dV = (v_ls - v2_iod).norm();
            std::cout << "\n  IOD → LS improvement:\n"
                      << "    |Δr| = " << std::setprecision(3) << dR*km << " km\n"
                      << "    |Δv| = " << std::setprecision(6) << dV*km << " km/s\n";
        }
    }

    std::cout << "\nDone." << std::endl;
    return 0;
}
