/// @file      testEphemerisVinti.cpp
/// @brief     EphemerisVinti 用例
/// @details   参考 testVinti6.cpp 中 VintiWrap 的基准值，通过 EphemerisVinti 类
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-09-05

#include "ast/EphemerisVinti.hpp"
#include "ast/TimePoint.hpp"
#include "ast/Vector.hpp"
#include "ast/AstTestMacro.h"
#include <cmath>

namespace
{
const double kJ2 = 1.0826266835531513e-3;
const double kJ3 = -2.5326564853313e-6;
}


TEST(EphemerisVintiTest, GetPosVel)
{
    AST_USING_NAMESPACE
    {
        // ---- SI 输入：中心引力 + J2/J3（与 VintiWrap 相同）
        const double re = 6378137.0;                         // 赤道半径 [m]
        const double gm = 3.986004418e14;                    // 引力常数 [m^3/s^2]
        const double c45 = 0.7071067811865476;               // cos(45°)
        const TimePoint epoch = TimePoint::FromUTC(2026, 3, 24, 0, 0, 0);

        // 45° 近地圆轨道：r = 7000 km，v = sqrt(gm/r)
        CartState state;
        state.pos_ = Vector3d{7000000.0, 0.0, 0.0};
        const double v = std::sqrt(gm / 7000000.0);
        state.vel_ = Vector3d{0.0, v * c45, v * c45};

        EphemerisVinti vinti(nullptr, gm, kJ2, kJ3, re, epoch, state);
        TimePoint tp = epoch + 600.0;                        // 预报 600 s

        Vector3d pos, vel;
        errc_t rc = vinti.getPosVel(tp, pos, vel);
        EXPECT_EQ(rc, eNoError);
        EXPECT_TRUE(std::isfinite(pos.norm()));
        EXPECT_TRUE(std::isfinite(vel.norm()));

        // 基准值来源于 testVinti6.cpp 的 VintiWrap 用例
        const Vector3d rExp{5584369.559661, 2982657.21046936, 2982075.4195079};
        const Vector3d vExp{-4552.55513343154, 4256.94369250608, 4254.1246163785};
        // ubuntu aarch64 上实测差 4.4e-8 
        EXPECT_NEAR(pos.x(), rExp.x(), 1e-7);                // 位置 [m]
        EXPECT_NEAR(pos.y(), rExp.y(), 1e-7);
        EXPECT_NEAR(pos.z(), rExp.z(), 1e-7);
        EXPECT_NEAR(vel.x(), vExp.x(), 1e-10);               // 速度 [m/s]
        EXPECT_NEAR(vel.y(), vExp.y(), 1e-10);
        EXPECT_NEAR(vel.z(), vExp.z(), 1e-10);
    }
}


TEST(EphemerisVintiTest, GetPos)
{
    AST_USING_NAMESPACE
    {
        const double re = 6378137.0;
        const double gm = 3.986004418e14;
        const TimePoint epoch = TimePoint::FromUTC(2026, 3, 24, 0, 0, 0);

        CartState state;
        state.pos_ = Vector3d{7000000.0, 0.0, 0.0};
        state.vel_ = Vector3d{0.0, 60.0, 80.0};              // 零预报时长应保持初值

        EphemerisVinti vinti(nullptr, gm, kJ2, kJ3, re, epoch, state);

        // 历元时刻：durations=0，应精确返回初始位置
        Vector3d pos, vel;
        errc_t rc = vinti.getPos(epoch, pos);
        EXPECT_EQ(rc, eNoError);
        EXPECT_NEAR(pos.x(), state.pos_.x(), 1e-6);
        EXPECT_NEAR(pos.y(), state.pos_.y(), 1e-6);
        EXPECT_NEAR(pos.z(), state.pos_.z(), 1e-6);

        // getPos 仅改写位置，不应破坏速度输出
        rc = vinti.getPosVel(epoch, pos, vel);
        EXPECT_EQ(rc, eNoError);
        EXPECT_NEAR(vel.x(), state.vel_.x(), 1e-6);
        EXPECT_NEAR(vel.y(), state.vel_.y(), 1e-6);
        EXPECT_NEAR(vel.z(), state.vel_.z(), 1e-6);

        // 区间应为全区间
        TimeInterval interval;
        rc = vinti.getInterval(interval);
        EXPECT_EQ(rc, eNoError);
        EXPECT_TRUE(interval.isWhole());
    }
}

GTEST_MAIN()
