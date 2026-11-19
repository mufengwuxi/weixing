/// @file      testRPO.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-05-08

#include "ast/MissionCommandLoader.hpp"
#include "ast/Segment.hpp"
#include "ast/Sequence.hpp"
#include "ast/OrbitElement.hpp"
#include "ast/RunTime.hpp"
#include "ast/EOP.hpp"
#include "ast/Scenario.hpp"
#include "ast/Satellite.hpp"
#include "ast/ObjectLinker.hpp"
#include "ast/Literals.hpp"
#include "ast/RTTIAPI.hpp"
#include "ast/MathOperator.hpp"
#include "ast/Test.h"


AST_USING_NAMESPACE
using namespace math;

TEST(RPOTest, Set_Reference_Vehicle)
{
    // GTEST_SKIP();
    SharedPtr<Scenario> scenario = aNewObject<Scenario>();
    SharedPtr<Satellite> rpo = aNewObject<Satellite>(scenario.get());
    rpo->setName("RPO");
    SharedPtr<Satellite> target = aNewObject<Satellite>(scenario.get());
    target->setName("Target");

    errc_t rc;
    std::string filepath = aTestGetConfigValue("STK_RPO_SET_REFERENCE_VEHICLE_FILE").toString();
    printf("loading file: %s\n", filepath.c_str());
    SharedPtr<MissionCommand> missionCommand;
    rc = aLoadMissionCommand(filepath, missionCommand);
    EXPECT_EQ(rc, eNoError);
    rc = aResolveAllLinks();
    EXPECT_EQ(rc, eNoError);
    rc = missionCommand->execute();
    EXPECT_EQ(rc, eNoError);
}


TEST(RPOTest, Set_Initial_State)
{
    SharedPtr<Scenario> scenario = aNewObject<Scenario>();
    SharedPtr<Satellite> rpo = aNewObject<Satellite>(scenario.get());
    rpo->setName("RPO");
    SharedPtr<Satellite> target = aNewObject<Satellite>(scenario.get());
    target->setName("Target");

    errc_t rc;
    std::string filepath = aTestGetConfigValue("STK_RPO_SET_INITIAL_STATE_FILE").toString();
    printf("loading file: %s\n", filepath.c_str());
    SharedPtr<MissionCommand> missionCommand;
    rc = aLoadMissionCommand(filepath, missionCommand);
    EXPECT_EQ(rc, eNoError);
    rc = aResolveAllLinks();
    EXPECT_EQ(rc, eNoError);
    rc = missionCommand->execute();
    EXPECT_EQ(rc, eNoError);
}


TEST(RPOTest, Coast)
{
    errc_t rc;
    std::string filepath = aTestGetConfigValue("STK_RPO_COAST_FILE").toString();
    printf("loading file: %s\n", filepath.c_str());
    SharedPtr<MissionCommand> missionCommand;
    rc = aLoadMissionCommand(filepath, missionCommand);
    EXPECT_EQ(rc, eNoError);
    rc = aResolveAllLinks();
    EXPECT_EQ(rc, eNoError);
    rc = missionCommand->execute();
    EXPECT_EQ(rc, eNoError);
}


// GTEST_MAIN()

int main(int argc, char **argv) {
    if(aIsCI())
        return 0;
    printf("Running main() from %s\n", __FILE__); 
    testing::GTEST_FLAG(catch_exceptions) = false;
    testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS(); 
}
