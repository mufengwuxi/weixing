/// @file      testJ2Analytical.cpp
/// @brief     ~
/// @details   ~
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-01-05

#include "ast/J2Analytical.hpp"
#include "ast/J4Analytical.hpp"
#include "ast/OrbitElement.hpp"
#include "ast/RunTime.hpp"
#include "ast/Literals.hpp"
#include "ast/CelestialBody.hpp"
#include "ast/AstTestMacro.h"

AST_USING_NAMESPACE
using namespace _AST literals;

const double gm = 398600441500000;
const double re = 6378136.3;
const double j2 = 0.00108262617385222271;
const double j4 = -0.0000016198976;
const ModOrbElem initModOrbElem{6678137, 0.0, 28.5_deg, 0.0, 0.0, 0.0};
const double duration = 86400;

TEST(J2Analytical, J2PropEarth)
{
    ModOrbElem modOrbElem = initModOrbElem;
    aJ2AnalyticalProp(duration, gm, j2, re, modOrbElem);
    EXPECT_NEAR(modOrbElem.raan() * kRadToDeg, 352.53739173, 1e-2);
    double u = modOrbElem.trueA() + modOrbElem.argper();
    printf("u: %f\n", u * kRadToDeg);
    printf("raan: %f\n", modOrbElem.raan() * kRadToDeg);
    EXPECT_NEAR(u * kRadToDeg, 344.6712, 2e-2);
    printf("modOrbElem: %s\n", modOrbElem.toString().c_str());
}

TEST(J2Analytical, J2PropMars)
{
    auto mars = aGetMars();
    ModOrbElem modOrbElem;
    CartState stateMarsInertial{3694_km, 0, 0, 0, 3_km/s, 1.6_km/s};
    aCartToModOrbElem(stateMarsInertial.pos(), stateMarsInertial.vel(), mars->getGM(), modOrbElem);
    printf("modOrbElem: %s\n", modOrbElem.toString().c_str());
    double gm = mars->getGM();
    double j2 = mars->getJ2();
    double rb = mars->getGravityRefDistance(); // 采用引力场参考半径来计算
    printf("gm: %.15g j2: %.15g rb: %.15g\n", gm, j2, rb);
    double duration = 1_day;
    aJ2AnalyticalProp(duration, gm, j2, rb, modOrbElem);
    printf("modOrbElem: %s\n", modOrbElem.toString().c_str());
    EXPECT_NEAR(modOrbElem.raan(),   349.892991934_deg, 1e-5);
    EXPECT_NEAR(modOrbElem.argper(), 196.567566162_deg, 1e-5);
    EXPECT_NEAR(modOrbElem.trueA(),  91.1492978709_deg, 1e-5);
}

TEST(J4Analytical, J4PropEarth)
{
    ModOrbElem modOrbElem = initModOrbElem;
    aJ4AnalyticalProp(duration, gm, j2, j4, re, modOrbElem);
    EXPECT_NEAR(modOrbElem.raan() * kRadToDeg, 352.554857667, 4e-2);
    double u = modOrbElem.trueA() + modOrbElem.argper();
    printf("u: %f\n", u * kRadToDeg);
    printf("raan: %f\n", modOrbElem.raan() * kRadToDeg);
    EXPECT_NEAR(u * kRadToDeg, 344.701, 2e-2);

    printf("modOrbElem: %s\n", modOrbElem.toString().c_str());
}



GTEST_MAIN()


