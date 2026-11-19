/// @file      testAER.cpp
/// @brief     AER坐标转换测试
/// @details   验证NED/ENU坐标与AER坐标之间的正反变换
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-06-01

#include "ast/AER.hpp"
#include "ast/AstTestMacro.h"
#include "ast/Math.hpp"
#include <cmath>

using namespace ast;


TEST(AERTest, NEDToAER)
{
    Vector3d ned{-9.1013, 4.1617, 4.2812};
    AER aer;
    aNEDToAER(ned, aer);

    EXPECT_NEAR(aer.azimuth(),   deg2rad(155.4271),  1e-6);
    EXPECT_NEAR(aer.elevation(), deg2rad(-23.1609),  1e-6);
    EXPECT_NEAR(aer.range(),     10.8849,            1e-4);
}

TEST(AERTest, ENUToAER)
{
    // NED(-9.1013, 4.1617, 4.2812) 对应 ENU(4.1617, -9.1013, -4.2812)
    Vector3d enu{4.1617, -9.1013, -4.2812};
    AER aer;
    aENUToAER(enu, aer);

    EXPECT_NEAR(aer.azimuth(),   deg2rad(155.4271),  1e-6);
    EXPECT_NEAR(aer.elevation(), deg2rad(-23.1609),  1e-6);
    EXPECT_NEAR(aer.range(),     10.8849,            1e-4);
}

TEST(AERTest, AERToNED)
{
    AER aer;
    aer.setAzimuth(deg2rad(155.4271));
    aer.setElevation(deg2rad(-23.1609));
    aer.setRange(10.8849);

    Vector3d ned;
    aAERToNED(aer, ned);

    EXPECT_NEAR(ned.x(), -9.1013, 1e-4);
    EXPECT_NEAR(ned.y(),  4.1617, 1e-4);
    EXPECT_NEAR(ned.z(),  4.2812, 1e-4);
}

TEST(AERTest, AERToENU)
{
    AER aer;
    aer.setAzimuth(deg2rad(155.4271));
    aer.setElevation(deg2rad(-23.1609));
    aer.setRange(10.8849);

    Vector3d enu;
    aAERToENU(aer, enu);

    EXPECT_NEAR(enu.x(),  4.1617, 1e-4);
    EXPECT_NEAR(enu.y(), -9.1013, 1e-4);
    EXPECT_NEAR(enu.z(), -4.2812, 1e-4);
}

TEST(AERTest, NEDRoundTrip)
{
    Vector3d ned{-9.1013, 4.1617, 4.2812};
    AER aer;
    aNEDToAER(ned, aer);

    Vector3d ned2;
    aAERToNED(aer, ned2);

    EXPECT_NEAR(ned2.x(), ned.x(), 1e-14);
    EXPECT_NEAR(ned2.y(), ned.y(), 1e-14);
    EXPECT_NEAR(ned2.z(), ned.z(), 1e-14);
}

TEST(AERTest, ENURoundTrip)
{
    Vector3d enu{4.1617, -9.1013, -4.2812};
    AER aer;
    aENUToAER(enu, aer);

    Vector3d enu2;
    aAERToENU(aer, enu2);

    EXPECT_NEAR(enu2.x(), enu.x(), 1e-14);
    EXPECT_NEAR(enu2.y(), enu.y(), 1e-14);
    EXPECT_NEAR(enu2.z(), enu.z(), 1e-14);
}


TEST(AERTest, SensorAERToENU)
{
    AER aer;
    aer.setAzimuth(deg2rad(34.1160));
    aer.setElevation(deg2rad(4.1931));
    aer.setRange(15.1070);

    Vector3d enu;
    aAERToENU(aer, enu);

    EXPECT_NEAR(enu.x(),  8.4504, 1e-4);
    EXPECT_NEAR(enu.y(), 12.4737, 1e-4);
    EXPECT_NEAR(enu.z(),  1.1046, 1e-4);
}

TEST(AERTest, SensorENUToAER)
{
    Vector3d enu{8.4504, 12.4737, 1.1046};
    AER aer;
    aENUToAER(enu, aer);

    EXPECT_NEAR(aer.azimuth(),   deg2rad(34.1160), 1e-6);
    EXPECT_NEAR(aer.elevation(), deg2rad(4.1931),  1e-6);
    EXPECT_NEAR(aer.range(),     15.1070,           1e-4);
}

GTEST_MAIN()
