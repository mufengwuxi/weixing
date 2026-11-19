/// @file      testMath.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-02-27

#include "ast/Math.hpp"
#include "ast/Test.h"
#include "ast/Literals.hpp"


AST_USING_NAMESPACE
using namespace _AST literals;


TEST(MathTest, MathDegree)
{
    EXPECT_DOUBLE_EQ(sind(0.0), 0.0);
    EXPECT_DOUBLE_EQ(sind(1.0), sin(1.0_deg));

    EXPECT_DOUBLE_EQ(cosd(0.0), 1.0);
    EXPECT_DOUBLE_EQ(cosd(21.0), cos(21.0_deg));

    EXPECT_DOUBLE_EQ(tand(0.0), 0.0);
    EXPECT_DOUBLE_EQ(tand(41.0), tan(41.0_deg));

    EXPECT_DOUBLE_EQ(asind(0.0), 0.0);
    EXPECT_DOUBLE_EQ(asind(0.2), asin(0.2)/1_deg);

    EXPECT_DOUBLE_EQ(acosd(0.0), 90);
    EXPECT_DOUBLE_EQ(acosd(0.2), acos(0.2)/1_deg);

    EXPECT_DOUBLE_EQ(atand(100), atan(100)/1_deg);

    EXPECT_DOUBLE_EQ(atan2d(0.0, 0.0), 0.0);
    EXPECT_DOUBLE_EQ(atan2d(1.0, 1.0), 45.0);
    EXPECT_DOUBLE_EQ(atan2d(-1.0, 1.0), -45.0);
    EXPECT_DOUBLE_EQ(atan2d(-1.0, -1.0), -135.0);
    EXPECT_DOUBLE_EQ(atan2d(1.0, -1.0), 135.0);
    EXPECT_DOUBLE_EQ(atan2d(0.0, -1.0), 180.0);
    EXPECT_DOUBLE_EQ(atan2d(0.0, 1.0), 0.0);

}


GTEST_MAIN()


