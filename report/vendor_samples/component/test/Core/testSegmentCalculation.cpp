/// @file      testSegmentCalculation.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-06-02


#include "ast/LandingSiteCalcSunElevation.hpp"
#include "ast/LandingSiteCalcSunAzimuth.hpp"
#include "ast/LandingSite.hpp"
#include "ast/Test.hpp"
#include "ast/RunTime.hpp"
#include "ast/TimePoint.hpp"
#include "ast/Literals.hpp"

AST_USING_NAMESPACE

TEST(SegmentCalculationTest, LandingSiteCalcSunElevation)
{
    using namespace math;
    auto moon = aGetMoon();
    {
        LandingSite landingSite;
        landingSite.setBody(moon);
        landingSite.setPosition({0, 0, 0});
        landingSite.setEpoch(TimePoint::FromUTC(2025, 1, 1, 0, 0, 0.0));
        LandingSiteCalcSunElevation calcElevation;
        LandingSiteCalcSunAzimuth calcAzimuth;

        double elevation;
        double azimuth;
        errc_t rc;
        rc = calcElevation.calculate(landingSite, elevation);
        EXPECT_EQ(rc, 0);
        rc = calcAzimuth.calculate(landingSite, azimuth);
        EXPECT_EQ(rc, 0);
        EXPECT_NEAR(elevation, -72.132035_deg, 1e-6);
        EXPECT_NEAR(azimuth, 94.9608645034_deg, 1e-6);
        printf("elevation: %.10f rad (%.10f deg)\n", elevation, rad2deg(elevation));
        printf("azimuth:   %.10f rad (%.10f deg)\n", azimuth, rad2deg(azimuth));
    }
    {
        LandingSite landingSite;
        landingSite.setBody(moon);
        //landingSite.setPosition({12_deg, 34_deg, 0});
        landingSite.setPosition({12_deg, 34_deg, 0});
        landingSite.setEpoch(TimePoint::FromUTC(2026, 1, 2, 3, 4, 0.0));
        LandingSiteCalcSunElevation calcElevation;
        LandingSiteCalcSunAzimuth calcAzimuth;

        double elevation;
        double azimuth;
        errc_t rc;
        rc = calcElevation.calculate(landingSite, elevation);
        EXPECT_EQ(rc, 0);
        rc = calcAzimuth.calculate(landingSite, azimuth);
        EXPECT_EQ(rc, 0);
        EXPECT_NEAR(elevation, 69.897727_deg, 1e-6);
        EXPECT_NEAR(azimuth, -130.518514_deg, 1e-6);
        printf("elevation: %.10f rad (%.10f deg)\n", elevation, rad2deg(elevation));
        printf("azimuth:   %.10f rad (%.10f deg)\n", azimuth, rad2deg(azimuth));
    }
}

GTEST_MAIN()


