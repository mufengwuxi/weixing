/// @file      testVDFSTK.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-10-08

#include "ast/Test.hpp"
#include "ast/ScenarioLoader.hpp"
#include "ast/FileSystem.hpp"
#include "ast/Scenario.hpp"
#include "ast/Facility.hpp"
#include "ast/Satellite.hpp"
#include "ast/Sensor.hpp"
#include "ast/RTTIAPI.hpp"
#include <string>

AST_USING_NAMESPACE

const char scenarioPath[] = "./STK/Scenarios/testVDFSTK/testVDFSTK.vdf";

TEST(VDFSTKTest, LoadScenario)
{
    std::string scenarioFullPath = aTestDataDirGet() + "/" + scenarioPath;
    SharedPtr<Scenario> scenario = aMakeShared<Scenario>();
    errc_t rc = aLoadScenario(scenarioFullPath, *scenario);
    ASSERT_EQ(rc, eNoError);
    auto satellites = aFindChildren(scenario, Satellite::StaticType());
    auto facilities = aFindChildren(scenario, Facility::StaticType());
    ASSERT_EQ(satellites.size(), 1);
    ASSERT_EQ(facilities.size(), 1);
    auto sensors = aFindChildren(satellites[0], Sensor::StaticType());
    ASSERT_EQ(sensors.size(), 1);
}

GTEST_MAIN()

