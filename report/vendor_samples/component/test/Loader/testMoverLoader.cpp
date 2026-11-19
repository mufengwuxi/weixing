/// @file      testMoverLoader.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-03-14

#include "ast/Mover.hpp"
#include "ast/Aircraft.hpp"
#include "ast/Satellite.hpp"
#include "ast/MoverLoader.hpp"
#include "ast/RunTime.hpp"
#include "ast/Test.h"
#include "ast/StringView.hpp"
#include "ast/Vector.hpp"
#include <string>


AST_USING_NAMESPACE

template<typename MoverType>
void testLoadMover(StringView filepath)
{
    MoverType mover;
    errc_t ret = aLoadMover(filepath, mover);
    EXPECT_EQ(ret, eNoError);

    // ret = mover.generateEphemeris();
    // EXPECT_EQ(ret, eNoError);

    // 检查是否成功加载了运动模型
    auto motionProfile = mover.getMotionProfile();
    EXPECT_NE(motionProfile, nullptr);

    // 检查是否成功加载了星历
    // auto ephemeris = mover.getEphemeris();
    // EXPECT_NE(ephemeris, nullptr);

}

TEST(MoverLoaderTest, LoadAircraft)
{
    if(aIsGithubCI()) 
        GTEST_SKIP();

    aInitialize();
    std::vector<std::string> files = aTestGetConfigStringVector("STK_AIRCRAFT_FILES");
    for(auto& file: files)
    {
        printf("loading %s\n", file.c_str());

        testLoadMover<Aircraft>(file);

        printf("loaded %s\n", file.c_str());
    }
}



TEST(MoverLoaderTest, LoadSatellite)
{
    if(aIsGithubCI()) 
        GTEST_SKIP();

    aInitialize();
    std::vector<std::string> files = aTestGetConfigStringVector("STK_SATELLITE_FILES");

    for(auto& file: files)
    {
        printf("loading %s\n", file.c_str());
        testLoadMover<Satellite>(file);
        printf("loaded %s\n", file.c_str());
    }
}




GTEST_MAIN();

