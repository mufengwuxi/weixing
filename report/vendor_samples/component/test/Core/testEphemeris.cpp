/// @file      testEphemeris.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-03-24

#include "ast/EphemerisLagrangeVar.hpp"
#include "ast/BuiltinFrame.hpp"
#include "ast/CelestialBody.hpp"
#include "ast/RunTime.hpp"
#include "ast/Test.h"


AST_USING_NAMESPACE

class EphemerisTest : public testing::Test
{
public:
    void SetUp() override
    {
        aInitialize();
    }
    void TearDown() override
    {
        aUninitialize();
    }
};

TEST_F(EphemerisTest, EphemerisLagrangeVar_Empty)
{
    EphemerisLagrangeVar ephem;
    TimePoint tp = TimePoint::FromUTC(2026, 3, 24, 0, 0, 0);
    Vector3d pos, vel;
    errc_t rc = ephem.getPos(tp, pos);
    EXPECT_TRUE(rc != eNoError);
    rc = ephem.getPosVel(tp, pos, vel);
    EXPECT_TRUE(rc != eNoError);
}

TEST_F(EphemerisTest, EphemerisLagrangeVar_GetPosVel)
{
    EphemerisLagrangeVar ephem;
    ephem.setFrame(aGetEarth()->makeFrameICRF());
    const auto epoch = TimePoint::FromUTC(2026, 3, 24, 0, 0, 0);
    ephem.setEpoch(epoch);
    ephem.setTimes({0, 60, 120});
    ephem.setPositions({{0, 0, 0}, {1, 0, 0}, {0, 1, 0}});
    ephem.setVelocities({{0, 0, 0}, {0, 0, 1}, {0, 1, 0}});
    // 测试正常的插值
    {
        TimePoint tp = epoch + 61;
        Vector3d pos, vel;
        Vector3d pos_expected = {1, 0, 0};
        Vector3d vel_expected = {0, 0, 1};
        errc_t rc = ephem.getPos(tp, pos);
        EXPECT_TRUE(rc == eNoError);
        printf("pos = %s\n", pos.toString().c_str());
        EXPECT_NEAR(pos[0], pos_expected[0], 1e-2);
        EXPECT_NEAR(pos[1], pos_expected[1], 1e-2);
        EXPECT_NEAR(pos[2], pos_expected[2], 1e-2);

        rc = ephem.getPosVel(tp, pos, vel);
        EXPECT_TRUE(rc == eNoError);
        printf("pos = %s\n", pos.toString().c_str());
        printf("vel = %s\n", vel.toString().c_str());
        EXPECT_NEAR(pos[0], pos_expected[0], 1e-2);
        EXPECT_NEAR(pos[1], pos_expected[1], 1e-2);
        EXPECT_NEAR(pos[2], pos_expected[2], 1e-2);
        EXPECT_NEAR(vel[0], vel_expected[0], 1e-2);
        EXPECT_NEAR(vel[1], vel_expected[1], 1e-2);
        EXPECT_NEAR(vel[2], vel_expected[2], 1e-2);
    }

    // 测试在准确的时间点上获取位置和速度是否正确
    {
        TimePoint tp = epoch + 60;
        Vector3d pos, vel;
        Vector3d pos_expected = {1, 0, 0};
        Vector3d vel_expected = {0, 0, 1};
        errc_t rc = ephem.getPos(tp, pos);
        EXPECT_TRUE(rc == eNoError);
        EXPECT_EQ(pos[0], pos_expected[0]);
        EXPECT_EQ(pos[1], pos_expected[1]);
        EXPECT_EQ(pos[2], pos_expected[2]);

        rc = ephem.getPosVel(tp, pos, vel);
        EXPECT_TRUE(rc == eNoError);
        EXPECT_EQ(pos[0], pos_expected[0]);
        EXPECT_EQ(pos[1], pos_expected[1]);
        EXPECT_EQ(pos[2], pos_expected[2]);
        EXPECT_EQ(vel[0], vel_expected[0]);
        EXPECT_EQ(vel[1], vel_expected[1]);
        EXPECT_EQ(vel[2], vel_expected[2]);
    }

    // 测试在时间点外获取位置和速度是否返回错误
    {
        Vector3d pos, vel;
        TimePoint tp = epoch + 120.01;
        errc_t rc = ephem.getPos(tp, pos);
        EXPECT_TRUE(rc != eNoError);
        rc = ephem.getPosVel(tp, pos, vel);
        EXPECT_TRUE(rc != eNoError);

        tp = epoch - 0.001;
        rc = ephem.getPos(tp, pos);
        EXPECT_TRUE(rc != eNoError);
        rc = ephem.getPosVel(tp, pos, vel);
        EXPECT_TRUE(rc != eNoError);
    }

    // 测试在时间点的边界处获取位置和速度是否正确
    {
        TimePoint tp = epoch;
        Vector3d pos, vel;
        errc_t rc = ephem.getPos(tp, pos);
        EXPECT_TRUE(rc == eNoError);
        rc = ephem.getPosVel(tp, pos, vel);
        EXPECT_TRUE(rc == eNoError);
        Vector3d pos_expected = {0, 1, 0};
        Vector3d vel_expected = {0, 1, 0};

        tp = epoch + 120;
        rc = ephem.getPos(tp, pos);
        EXPECT_EQ(pos[0], pos_expected[0]);
        EXPECT_EQ(pos[1], pos_expected[1]);
        EXPECT_EQ(pos[2], pos_expected[2]);
        EXPECT_TRUE(rc == eNoError);

        rc = ephem.getPosVel(tp, pos, vel);
        EXPECT_TRUE(rc == eNoError);
        EXPECT_EQ(pos[0], pos_expected[0]);
        EXPECT_EQ(pos[1], pos_expected[1]);
        EXPECT_EQ(pos[2], pos_expected[2]);
        EXPECT_EQ(vel[0], vel_expected[0]);
        EXPECT_EQ(vel[1], vel_expected[1]);
        EXPECT_EQ(vel[2], vel_expected[2]);
    }
}


GTEST_MAIN()
