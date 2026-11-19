/// @file      testSunPosition.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-05-08


#include "ast/AstTestMacro.h"
#include "ast/JplDe.hpp"
#include "ast/TimePoint.hpp"
#include "ast/RunTime.hpp"
#include "ast/Vector.hpp"
#include "ast/Literals.hpp"

AST_USING_NAMESPACE

TEST(SunPosition, Year2000)
{
    aInitialize();

    JplDe jplDe;
    errc_t err = jplDe.openDefault();
    ASSERT_FALSE(err) << "Failed to open JPL DE file";

    auto time = TimePoint::FromUTC({2000, 1, 1, 0, 0, 0});
    Vector3d pos, vel;

    err = jplDe.getPosVelICRF(time, JplDe::eSun, JplDe::eEarth, pos, vel);
    ASSERT_FALSE(err) << "Failed to get sun position";

    printf("=== 2000年1月1日 00:00:00 UTC ===\n");
    printf("太阳相对于地球在ICRF下的位置 (km):\n");
    printf("X: %.6f km\n", pos[0] / 1000.0);
    printf("Y: %.6f km\n", pos[1] / 1000.0);
    printf("Z: %.6f km\n", pos[2] / 1000.0);
    printf("\n");
    printf("太阳相对于地球在ICRF下的速度 (km/s):\n");
    printf("VX: %.8f km/s\n", vel[0] / 1000.0);
    printf("VY: %.8f km/s\n", vel[1] / 1000.0);
    printf("VZ: %.8f km/s\n", vel[2] / 1000.0);

    EXPECT_TRUE(true);
}

GTEST_MAIN()