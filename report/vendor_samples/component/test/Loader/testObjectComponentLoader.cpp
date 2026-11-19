/// @file      testObjectComponentLoader.cpp
/// @brief     测试对象扩展组件（AccessConstraints）加载
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-10-09

#include "ast/Facility.hpp"
#include "ast/ObjectAccessConstraints.hpp"
#include "ast/ObjectComponent.hpp"
#include "ast/FacilityLoader.hpp"
#include "ast/RTTIAPI.hpp"
#include "ast/RunTime.hpp"
#include "ast/Test.h"
#include "ast/TestConfig.hpp"
#include "ast/StringUtil.hpp"
#include <string>


AST_USING_NAMESPACE

TEST(ObjectComponentLoaderTest, LoadFacilityAccessConstraints)
{
    aInitialize();

    std::string file = aTestDataDirGet() + "/STK/Scenarios/testAccessSTK/Facility1.f";
    Facility facility;
    errc_t rc = aLoadFacility(file, facility);
    EXPECT_EQ(rc, eNoError);

    auto* constraints = aFindChild<ObjectAccessConstraints*>(&facility);
    ASSERT_NE(constraints, nullptr);
    const auto& list = constraints->constraints();
    ASSERT_EQ(list.size(), 3u);

    // LineOfSight IncludeIntervals
    EXPECT_TRUE(list[0]->type() == EAccessConstraint::eLineOfSight);
    EXPECT_TRUE(list[0]->enabled());
    EXPECT_FALSE(list[0]->exclude());
    EXPECT_FALSE(list[0]->useMin());
    EXPECT_FALSE(list[0]->useMax());

    // ElevationAngle Min 0.0 Max 50.0 IncludeIntervals
    EXPECT_TRUE(list[1]->type() == EAccessConstraint::eElevationAngle);
    EXPECT_TRUE(list[1]->enabled());
    EXPECT_FALSE(list[1]->exclude());
    EXPECT_TRUE(list[1]->useMin());
    EXPECT_TRUE(list[1]->useMax());
    EXPECT_NEAR(list[1]->min(), 0.0, 1e-12);
    EXPECT_NEAR(list[1]->max(), 50.0, 1e-12);

    // Range Min 4.0e+04 ExcludeIntervals
    EXPECT_TRUE(list[2]->type() == EAccessConstraint::eRange);
    EXPECT_TRUE(list[2]->enabled());
    EXPECT_FALSE(list[2]->exclude());
    EXPECT_TRUE(list[2]->useMin());
    EXPECT_FALSE(list[2]->useMax());
    EXPECT_NEAR(list[2]->min(), 4.0e4, 1e-6);
}


GTEST_MAIN();

