/// @file      testFrameTransform.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-03-05

#include "ast/FrameTransform.hpp"
#include "ast/RunTime.hpp"
#include "ast/CelestialBody.hpp"
#include "ast/Frame.hpp"
#include "ast/TimePoint.hpp"
#include "ast/MoonFrame.hpp"
#include "ast/JplDe.hpp"
#include "ast/Transform.hpp"
#include "ast/KinematicTransform.hpp"
#include "ast/Matrix.hpp"
#include "ast/Literals.hpp"
#include "ast/Test.h"

AST_USING_NAMESPACE


TEST(FrameTransformTest, Transform_Case1)
{
    aInitialize();
    KinematicTransform transform;
    auto earth = aGetEarth();
    auto moon = aGetMoon();
    auto earthICRF = earth->makeFrameICRF();
    auto moonICRF = moon->makeFrameICRF();
    TimePoint tp = TimePoint::FromUTC(2026, 3, 5, 0, 0, 0);
    errc_t rc = aFrameTransform(earthICRF, moonICRF, tp, transform);
    EXPECT_FALSE(rc);
    Vector3d pos{1000.0, 2000.0, 3000.0}, vel{1000, 2000, 3000};
    Vector3d posMoon1, velMoon1, posMoon2, velMoon2;
    /*!
    @bug 
    通过 aFrameTransform 获取的月球位置数值精度降低了
    因为他将月球星历转换到了ICRF，而其原始星历是相对于地球的
    */
    transform.transformPositionVelocity(pos, vel, posMoon1, velMoon1);
    rc = aEarthICRFToMoonICRF(tp, pos, vel, posMoon2, velMoon2);
    EXPECT_FALSE(rc);
    printf("posMoon1 = %s\nposMoon2 = %s\n", posMoon1.toString().c_str(), posMoon2.toString().c_str());
    printf("velMoon1 = %s\nvelMoon2 = %s\n", velMoon1.toString().c_str(), velMoon2.toString().c_str());
    for(int i = 0; i < 3; i++)
    {
        EXPECT_NEAR(posMoon1[i], posMoon2[i], posMoon2[i] * 1e-12);
        EXPECT_NEAR(velMoon1[i], velMoon2[i], velMoon2[i] * 1e-12);
    }

    {
        Vector3d posMoon, velMoon;
        Vector3d posEarth, velEarth;
        Vector3d posMoonInEarth1, velMoonInEarth1;
        Vector3d posMoonInEarth2, velMoonInEarth2;
        aJplDeGetPosVelICRF(tp, JplDe::eMoon, JplDe::eEarth, posMoonInEarth1, velMoonInEarth1);
        // 下面的计算会导致截断误差，导致结果不准确
        aJplDeGetPosVelICRF(tp, JplDe::eEarth, JplDe::eSSBarycenter, posEarth, velEarth);
        aJplDeGetPosVelICRF(tp, JplDe::eMoon, JplDe::eSSBarycenter, posMoon, velMoon);
        posMoonInEarth2 = posMoon - posEarth;
        velMoonInEarth2 = velMoon - velEarth;
        printf("posMoonInEarth1 = %s\nposMoonInEarth2 = %s\n", posMoonInEarth1.toString().c_str(), posMoonInEarth2.toString().c_str());
        printf("velMoonInEarth1 = %s\nvelMoonInEarth2 = %s\n", velMoonInEarth1.toString().c_str(), velMoonInEarth2.toString().c_str());
    }
}


TEST(FrameTransformTest, Transform_Case2)
{
    if(aIsGithubCI())
        GTEST_SKIP();
    aInitialize();
    auto mars = aGetMars();
    auto jupiter = aGetJupiter();
    {
        auto marsInertial = mars->makeFrameInertial();
        auto jupiterInertial = jupiter->makeFrameInertial();
        TimePoint tp = TimePoint::FromUTC(2026, 3, 5, 4, 0, 0);
        Transform transform;
        errc_t rc = aFrameTransform(marsInertial, jupiterInertial, tp, transform);
        EXPECT_FALSE(rc);
        Vector3d posMars{1000.0_km, 2000.0_km, 3000.0_km};
        Vector3d posJupiter{};
        transform.transformPosition(posMars, posJupiter);
        printf("posJupiter       = %s\n", posJupiter.toString().c_str());
        Vector3d posJupiterExpect{510988017.1644716858863831_km, -824472406.8249446153640747_km, 12126742.5698849204927683_km};
        printf("posJupiterExpect = %s\n", posJupiterExpect.toString().c_str());
        for(int i = 0; i < 3; i++)
        {
            EXPECT_NEAR(posJupiter[i], posJupiterExpect[i], fabs(posJupiterExpect[i]) * 1e-13);
        }
    }
    {
        auto marsICRF = mars->makeFrameICRF();
        auto jupiterICRF = jupiter->makeFrameICRF();
        TimePoint tp = TimePoint::FromUTC(2026, 3, 5, 4, 0, 0);
        Transform transform;
        errc_t rc = aFrameTransform(marsICRF, jupiterICRF, tp, transform);
        EXPECT_FALSE(rc);
        Vector3d posMars{1000.0_km, 2000.0_km, 3000.0_km};
        Vector3d posJupiter{};
        transform.transformPosition(posMars, posJupiter);
        printf("posJupiter       = %s\n", posJupiter.toString().c_str());
        Vector3d posJupiterExpect{485290149.6851636767387390_km, -766243403.2641737461090088_km, -344054501.0049201846122742_km};
        printf("posJupiterExpect = %s\n", posJupiterExpect.toString().c_str());
        for(int i = 0; i < 3; i++)
        {
            EXPECT_NEAR(posJupiter[i], posJupiterExpect[i], fabs(posJupiterExpect[i]) * 1e-15);
        }
    }
}


TEST(FrameTransformTest, Transform_Case3)
{
    aInitialize();
    KinematicTransform transform;
    auto earth = aGetEarth();
    auto eci = earth->makeFrameICRF();
    auto ecf = earth->makeFrameFixed();
    TimePoint tp = TimePoint::FromUTC(2026, 3, 5, 0, 0, 0);
    errc_t rc = aFrameTransform(eci, ecf, tp, transform);
    EXPECT_FALSE(rc);
}

GTEST_MAIN()