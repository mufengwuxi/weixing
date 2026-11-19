/// @file      testHPOPMoon_Gravity.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-08-31

#include "ast/HPOP.hpp"
#include "ast/HPOPEquation.hpp"
#include "ast/EOP.hpp"
#include "ast/RunTime.hpp"
#include "ast/Test.hpp"
#include "ast/Vector.hpp"
#include "ast/Environment.hpp"
#include "ast/ODEVarStepIntegrator.hpp"
#include "ast/Resolve.hpp"
#include "ast/OrbitElement.hpp"
#include "ast/Literals.hpp"


AST_USING_NAMESPACE


class HPOPMoonGravityTest : public ::testing::Test
{
    void SetUp() override
    {
        aInitialize();
        aDataContext_GetEOP()->unload();  // 卸载EOP数据，确保测试不受EOP修正数据的影响
    }

    void TearDown() override
    {
        aUninitialize();
    }
};


TEST_F(HPOPMoonGravityTest, GLGM2_20_20)
{
    auto moon = "Moon"_body;
    HPOP hpop;
    SpacecraftParam spacecraftParam;
    HPOPForceModel forceModel;
    forceModel.setCentralBody(moon);
    forceModel.gravity().model_ = "GLGM2";
    forceModel.gravity().maxDegree_ = 20;
    forceModel.gravity().maxOrder_ = 20;
    hpop.setForceModel(forceModel);
    hpop.setSpacecraftParam(spacecraftParam);
    hpop.setPropagationFrame("Moon ICRF"_frame);

    auto startTime = "2029-10-07 19:25:24"_utc;
    auto stopTime  = "2029-10-12 13:11:44"_utc;
    CartState initState{-1193.88677_km, 1400.059587_km, 605.2966457_km, 1.2525667_km/s, 0.9161693_km/s, 0.3560754_km/s};
    auto state = initState;
    errc_t rc = hpop.propagate(startTime, stopTime, state.pos(), state.vel());
    EXPECT_EQ(rc, eNoError);
    CartState expectedState{1695443_m, -914709_m, -399116_m, -795.867_m/s, -1256.948_m/s, -493.587_m/s};
    EXPECT_NEAR(state.pos().x(), expectedState.pos().x(), 1);
    EXPECT_NEAR(state.pos().y(), expectedState.pos().y(), 1);
    EXPECT_NEAR(state.pos().z(), expectedState.pos().z(), 1);
    EXPECT_NEAR(state.vel().x(), expectedState.vel().x(), 1e-3);
    EXPECT_NEAR(state.vel().y(), expectedState.vel().y(), 1e-3);
    EXPECT_NEAR(state.vel().z(), expectedState.vel().z(), 1e-3);

    ModOrbElem orbElem;
    aCartToModOrbElem(state.pos(), state.vel(), moon->getGM(), orbElem);
    double periAlt = orbElem.getPeriAlt(moon->getRadius());
    double apoAlt = orbElem.getApoAlt (moon->getRadius());
    printf("state:   %s\n",    state.toString().c_str());
    printf("orbElem: %s\n",    orbElem.toString().c_str());
    printf("periAlt: %.15g\n", periAlt);
    printf("apoAlt:  %.15g\n", apoAlt);
    EXPECT_NEAR(periAlt, 175238_m, 1);
    EXPECT_NEAR(apoAlt, 230011_m, 1);
}


GTEST_MAIN()