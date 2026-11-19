/// @file      testFrameWithEpoch.cpp
/// @brief     带轴系历元的坐标系测试
/// @details   测试FrameWithEpoch类的创建和基本功能
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-04-11

#include "ast/FrameWithEpoch.hpp"
#include "ast/PointRoot.hpp"
#include "ast/AxesRoot.hpp"
#include "ast/TimePoint.hpp"
#include "ast/AstTestMacro.h"

AST_USING_NAMESPACE

class FrameWithEpochTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

// 测试基本构造
TEST_F(FrameWithEpochTest, BasicConstruction)
{
    // 使用PointRoot和AxesRoot的Instance()方法获取单例
    Point* origin = PointRoot::Instance();
    Axes* axes = AxesRoot::Instance();

    // 创建FrameWithEpoch
    auto frame = FrameWithEpoch::New(origin, axes);
    ASSERT_NE(frame, nullptr);

    // 验证基本属性
    EXPECT_EQ(frame->getOrigin(), origin);
    EXPECT_EQ(frame->getAxes(), axes);
    EXPECT_FALSE(frame->getUseEpoch());

    delete frame;
}

// 测试带历元的构造
TEST_F(FrameWithEpochTest, ConstructionWithEpoch)
{
    Point* origin = PointRoot::Instance();
    Axes* axes = AxesRoot::Instance();

    // 创建TimePoint
    TimePoint tp = TimePoint::FromUTC(2026, 4, 11, 12, 0, 0.0);

    // 创建FrameWithEpoch
    auto frame = FrameWithEpoch::New(origin, axes);
    ASSERT_NE(frame, nullptr);

    EXPECT_EQ(frame->getOrigin(), origin);
    EXPECT_EQ(frame->getAxes(), axes);

    delete frame;
}

// 测试MakeShared
TEST_F(FrameWithEpochTest, MakeSharedConstruction)
{
    Point* origin = PointRoot::Instance();
    Axes* axes = AxesRoot::Instance();

    auto frame = FrameWithEpoch::MakeShared(origin, axes);
    ASSERT_NE(frame, nullptr);

    EXPECT_EQ(frame->getOrigin(), origin);
    EXPECT_EQ(frame->getAxes(), axes);
}

// 测试UseEpoch设置
TEST_F(FrameWithEpochTest, SetUseEpoch)
{
    Point* origin = PointRoot::Instance();
    Axes* axes = AxesRoot::Instance();

    auto frame = FrameWithEpoch::New(origin, axes);
    ASSERT_NE(frame, nullptr);

    // 默认不使用历元
    EXPECT_FALSE(frame->getUseEpoch());

    // 设置使用历元
    frame->setUseEpoch(true);
    EXPECT_TRUE(frame->getUseEpoch());

    // 设置不使用历元
    frame->setUseEpoch(false);
    EXPECT_FALSE(frame->getUseEpoch());

    delete frame;
}

// 测试SourceAxes设置
TEST_F(FrameWithEpochTest, SetSourceAxes)
{
    Point* origin = PointRoot::Instance();
    Axes* axes = AxesRoot::Instance();

    auto frame = FrameWithEpoch::New(origin, axes);
    ASSERT_NE(frame, nullptr);

    // 获取并设置SourceAxes
    Axes* sourceAxes = frame->getSourceAxes();
    EXPECT_NE(sourceAxes, nullptr);

    frame->setSourceAxes(axes);
    EXPECT_EQ(frame->getSourceAxes(), axes);

    delete frame;
}

// 测试ReferenceAxes
TEST_F(FrameWithEpochTest, ReferenceAxes)
{
    Point* origin = PointRoot::Instance();
    Axes* axes = AxesRoot::Instance();

    auto frame = FrameWithEpoch::New(origin, axes);
    ASSERT_NE(frame, nullptr);

    // 设置ReferenceAxes
    frame->setReferenceAxes(axes);
    EXPECT_EQ(frame->getReferenceAxes(), axes);

    delete frame;
}

GTEST_MAIN()
