/// @file      testMission.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-05-07

#include "ast/MissionCommandLoader.hpp"
#include "ast/Segment.hpp"
#include "ast/Sequence.hpp"
#include "ast/OrbitElement.hpp"
#include "ast/RunTime.hpp"
#include "ast/EOP.hpp"
#include "ast/ObjectLinker.hpp"
#include "ast/Literals.hpp"
#include "ast/MathOperator.hpp"
#include "ast/Test.h"

AST_USING_NAMESPACE

// 测试 Hohmann 转移
TEST(MissionTest, HohmannTransfer)
{
    if(aIsCI())
        GTEST_SKIP();
    using namespace math;
    aInitialize();
    aDataContext_GetEOP()->unload();
    errc_t rc;
    std::string filepath = aTestGetConfigValue("STK_HOHMANNT_RANSFER_FILE").toString();
    printf("loading file: %s\n", filepath.c_str());
    SharedPtr<MissionCommand> missionCommand;
    rc = aLoadMissionCommand(filepath, missionCommand);
    EXPECT_EQ(rc, eNoError);
    rc = aResolveAllLinks();
    EXPECT_EQ(rc, eNoError);
    SharedPtr<SpacecraftState> initialState = SpacecraftState::NewDefault();
    initialState->setStateEpoch(TimePoint::FromUTC(2026, 4, 7, 4, 0, 0));
    Segment* segment = aobject_cast<Segment*>(missionCommand.get());
    ASSERT_TRUE(segment != nullptr);
    segment->setInputState(initialState.get());
    rc = missionCommand->execute();
    EXPECT_EQ(rc, eNoError);
    const auto outputState = segment->getOutputState();
    ASSERT_TRUE(outputState != nullptr);
    CartState cartState{};
    TimePoint tp{};
    outputState->getState(cartState);
    outputState->getStateEpoch(tp);
    printf("tp: %s\n", tp.toString().c_str());
    printf("cartState: %s\n", cartState.toString().c_str());
    CartState expectCartState{
        -9999.9956695325836336_km, 23.9548574078268430_km, -22.4634031187308558_km,
        -0.0065201289543048_km/sec, -5.5488192323528542_km/sec, -3.0117026995629019_km/sec 
    };
    TimePoint expectTime = TimePoint::Parse("7 Apr 2026 07:48:46.474  UTC");
    printf("expectTime: %s\n", expectTime.toString().c_str());
    printf("expectCartState: %s\n", expectCartState.toString().c_str());
    double timeError = tp - expectTime;
    printf("timeError: %f\n", timeError);
    EXPECT_NEAR(timeError, 0.0, 1e-2);
    CartState cartError = cartState - expectCartState;
    printf("cartError: %s\n", cartError.toString().c_str());
    EXPECT_NEAR(cartError.pos().norm(), 0.0, 50);
    EXPECT_NEAR(cartError.vel().norm(), 0.0, 0.05);
}



TEST(MissionTest, GeoRendezvous)
{
    if(aIsCI())
        GTEST_SKIP();
    using namespace math;
    aInitialize();
    aDataContext_GetEOP()->unload();
    errc_t rc;
    std::string filepath = aTestGetConfigValue("STK_GEO_RENDEZVOUS_FILE").toString();
    printf("loading file: %s\n", filepath.c_str());
    SharedPtr<MissionCommand> missionCommand;
    rc = aLoadMissionCommand(filepath, missionCommand);
    EXPECT_EQ(rc, eNoError);
    rc = aResolveAllLinks();
    EXPECT_EQ(rc, eNoError);
    return;
    // SharedPtr<SpacecraftState> initialState = SpacecraftState::NewDefault();
    // initialState->setStateEpoch(TimePoint::FromUTC(2026, 4, 7, 4, 0, 0));
    // Segment* segment = aobject_cast<Segment*>(missionCommand.get());
    // ASSERT_TRUE(segment != nullptr);
    // segment->setInputState(initialState.get());
    // rc = missionCommand->execute();
    // EXPECT_EQ(rc, eNoError);
    // const auto outputState = segment->getOutputState();
    // ASSERT_TRUE(outputState != nullptr);
    // CartState cartState{};
    // TimePoint tp{};
    // outputState->getState(cartState);
    // outputState->getStateEpoch(tp);
    // printf("tp: %s\n", tp.toString().c_str());
    // printf("cartState: %s\n", cartState.toString().c_str());
}

GTEST_MAIN()



