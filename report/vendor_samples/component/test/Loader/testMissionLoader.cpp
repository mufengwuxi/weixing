/// @file      testMissionCommandLoader.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-04-15

#include "ast/MissionCommandLoader.hpp"
#include "ast/MissionCommand.hpp"
#include "ast/InitialState.hpp"
#include "ast/SpacecraftState.hpp"
#include "ast/Segment.hpp"
#include "ast/HPOP.hpp"
#include "ast/ValXMLLoader.hpp"
#include "ast/PropagatorLoader.hpp"
#include "ast/ValDict.hpp"
#include "ast/ObjectLinker.hpp"
#include "ast/Test.h"


AST_USING_NAMESPACE;



TEST(MissionLoaderTest, LoadValue)
{
    SharedPtr<Value> value;
    std::vector<std::string> files = aTestGetConfigStringVector("STK_VALUE_FILES");
    for(auto& file: files){
        printf("loading file: %s\n", file.c_str());
        errc_t rc = aLoadValue(file, value);
        EXPECT_TRUE(value != nullptr);
        EXPECT_EQ(rc, eNoError);
        printf("loaded file: %s\n", file.c_str());
        if(value)
        {
            auto dict = value->toValDict();
            if(dict)
            {
                std::string dictStr = dict->toJsonString(2);
                printf("%s\n", dictStr.c_str());
            }
        }
    }
}

TEST(MissionLoaderTest, LoadInitialState)
{
    InitialState initialState;
    std::vector<std::string> files = aTestGetConfigStringVector("STK_INITIALSTATE_FILES");
    for(auto& file: files){
        printf("loading file: %s\n", file.c_str());
        errc_t rc = aLoadInitialState(file, initialState);
        EXPECT_EQ(rc, eNoError);
        printf("loaded file: %s\n", file.c_str());
    }
}


TEST(MissionLoaderTest, LoadMissionCommand)
{
    SharedPtr<MissionCommand> missionCommand;
    std::vector<std::string> files = aTestGetConfigStringVector("STK_SEQUENCE_FILES");
    for(auto& file: files){
        printf("loading file: %s\n", file.c_str());
        errc_t rc = aLoadMissionCommand(file, missionCommand);
        EXPECT_TRUE(missionCommand != nullptr);
        EXPECT_EQ(rc, eNoError);
        printf("loaded file: %s\n", file.c_str());
    }
}

TEST(MissionLoaderTest, LoadPropagate)
{
    SharedPtr<MissionCommand> missionCommand;
    std::vector<std::string> files = aTestGetConfigStringVector("STK_PROPAGATE_FILES");
    for(auto& file: files){
        printf("loading file: %s\n", file.c_str());
        errc_t rc = aLoadMissionCommand(file, missionCommand);
        EXPECT_TRUE(missionCommand != nullptr);
        EXPECT_EQ(rc, eNoError);
        printf("loaded file: %s\n", file.c_str());
    }
}


TEST(MissionLoaderTest, LoadPropagator)
{
    std::vector<std::string> files = aTestGetConfigStringVector("STK_PROPAGATOR_FILES");
    for(auto& file: files){
        printf("loading file: %s\n", file.c_str());
        HPOP propagator;
        errc_t rc = aLoadPropagator(file, propagator);
        EXPECT_EQ(rc, eNoError);
        printf("loaded file: %s\n", file.c_str());
    }
}


TEST(MissionLoaderTest, LoadHohmannTransfer)
{
    std::string file = aTestGetConfigValue("STK_HOHMANNT_RANSFER_FILE").toString();
    {
        errc_t rc;
        printf("loading file: %s\n", file.c_str());
        SharedPtr<MissionCommand> missionCommand;
        rc = aLoadMissionCommand(file, missionCommand);
        EXPECT_EQ(rc, eNoError);
        rc = aResolveAllLinks();
        EXPECT_EQ(rc, eNoError);
        SharedPtr<SpacecraftState> initialState = SpacecraftState::NewDefault();
        initialState->setStateEpoch(TimePoint::FromUTC(2026, 4, 7, 4, 0, 0));
        if(auto segment = aobject_cast<Segment*>(missionCommand.get()))
        {
            segment->setInputState(initialState.get());
        }
        rc = missionCommand->execute();
        EXPECT_EQ(rc, eNoError);
        printf("loaded file: %s\n", file.c_str());
    }
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