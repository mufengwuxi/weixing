/// @file      testScenarioLoader.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-04-07

#include "ast/ScenarioLoader.hpp"
#include "ast/Scenario.hpp"
#include "ast/RTTIAPI.hpp"
#include "ast/ObjectManager.hpp"
#include "ast/Test.h"

AST_USING_NAMESPACE

TEST(ScenarioLoader, LoadScenario)
{
    if(aIsCI())
        GTEST_SKIP();

    // 测试加载场景文件
    auto testLoadScenario = []()
    {
        Scenario scenario;
        StringView scenarioFile = aTestGetConfigValue("STK_SCENARIO_FILE");
        errc_t rc = aLoadScenario(scenarioFile, scenario);
        EXPECT_EQ(rc, eNoError);
        auto objectCount = ObjectManager::CurrentInstance().getObjectCount();
        EXPECT_GT(objectCount, 0);
        aPrintObjectTree(&scenario);
    };
    testLoadScenario();

    // 检查对象是否已经销毁
    {
        auto objectCount1 = ObjectManager::CurrentInstance().getObjectCount();
        testLoadScenario();
        auto objectCount2 = ObjectManager::CurrentInstance().getObjectCount();
        EXPECT_EQ(objectCount2, objectCount1);
        auto objects = ObjectManager::CurrentInstance().getAllObjects();
        EXPECT_EQ(objects.size(), objectCount2);
    }
}

GTEST_MAIN()
