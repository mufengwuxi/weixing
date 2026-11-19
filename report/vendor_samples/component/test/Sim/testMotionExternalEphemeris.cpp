/// @file      testMotionExternalEphemeris.cpp
/// @brief     Tests for MotionExternalEphemeris
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-04-21

#include "ast/MotionExternalEphemeris.hpp"
#include "ast/Test.h"

AST_USING_NAMESPACE

TEST(MotionExternalEphemerisTest, MakeEphemerisSpecReturnsNotImplemented)
{
    auto* motion = MotionExternalEphemeris::New();
    ASSERT_NE(motion, nullptr);

    ScopedPtr<Ephemeris> eph;
    errc_t ret = motion->makeEphemerisSpec(eph);
    EXPECT_EQ(ret, eErrorNotImplemented);

    delete motion;
}

TEST(MotionExternalEphemerisTest, MakeEphemerisSimpleReturnsNotImplemented)
{
    auto* motion = MotionExternalEphemeris::New();
    ASSERT_NE(motion, nullptr);

    ScopedPtr<Ephemeris> eph;
    errc_t ret = motion->makeEphemerisSimple(eph);
    EXPECT_EQ(ret, eErrorNotImplemented);

    delete motion;
}

GTEST_MAIN();
