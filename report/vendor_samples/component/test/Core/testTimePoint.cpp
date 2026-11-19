/// @file      testTimePoint.cpp
/// @brief     ~
/// @details   ~
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-01-13

#include "ast/TimePoint.hpp"
#include "ast/DateTime.hpp"
#include "ast/Test.h"

AST_USING_NAMESPACE

TEST(TimePoint, BitIntCase)
{
    {
        DateTime dt{2068, 1, 1, 0, 0, 0};
        TimePoint tp = TimePoint::FromUTC(dt);
        DateTime dt2;
        aTimePointToUTC(tp, dt2);

        EXPECT_EQ(dt.year(), dt2.year());
        EXPECT_EQ(dt.month(), dt2.month());
        EXPECT_EQ(dt.day(), dt2.day());
        EXPECT_EQ(dt.hour(), dt2.hour());
        EXPECT_EQ(dt.minute(), dt2.minute());
        EXPECT_EQ(dt.second(), dt2.second());
    }
    {
        DateTime dt{2069, 1, 1, 0, 0, 0};
        TimePoint tp = TimePoint::FromUTC(dt);
        DateTime dt2;
        aTimePointToUTC(tp, dt2);

        EXPECT_EQ(dt.year(), dt2.year());
        EXPECT_EQ(dt.month(), dt2.month());
        EXPECT_EQ(dt.day(), dt2.day());
        EXPECT_EQ(dt.hour(), dt2.hour());
        EXPECT_EQ(dt.minute(), dt2.minute());
        EXPECT_EQ(dt.second(), dt2.second());
    }
    {
        DateTime dt{3070, 3, 1, 0, 0, 0};
        TimePoint tp = TimePoint::FromUTC(dt);
        DateTime dt2;
        aTimePointToUTC(tp, dt2);

        EXPECT_EQ(dt.year(), dt2.year());
        EXPECT_EQ(dt.month(), dt2.month());
        EXPECT_EQ(dt.day(), dt2.day());
        EXPECT_EQ(dt.hour(), dt2.hour());
        EXPECT_EQ(dt.minute(), dt2.minute());
        EXPECT_EQ(dt.second(), dt2.second());
    }
    {
        DateTime dt{99071, 1, 1, 0, 0, 0};
        TimePoint tp = TimePoint::FromUTC(dt);
        DateTime dt2;
        aTimePointToUTC(tp, dt2);

        EXPECT_EQ(dt.year(), dt2.year());
        EXPECT_EQ(dt.month(), dt2.month());
        EXPECT_EQ(dt.day(), dt2.day());
        EXPECT_EQ(dt.hour(), dt2.hour());
        EXPECT_EQ(dt.minute(), dt2.minute());
        EXPECT_EQ(dt.second(), dt2.second());
    }
    {
        DateTime dt{-9072, 1, 1, 0, 0, 0};
        TimePoint tp = TimePoint::FromUTC(dt);
        DateTime dt2;
        aTimePointToUTC(tp, dt2);
        TimePoint tp2 = TimePoint::FromUTC(dt2);


        EXPECT_EQ(dt.year(), dt2.year());
        EXPECT_EQ(dt.month(), dt2.month());
        EXPECT_EQ(dt.day(), dt2.day());
        EXPECT_EQ(dt.hour(), dt2.hour());
        EXPECT_EQ(dt.minute(), dt2.minute());
        EXPECT_EQ(dt.second(), dt2.second());

        EXPECT_EQ(tp.daysFromJ2000TT(), tp2.daysFromJ2000TT());
        EXPECT_EQ(tp.integerPart(), tp2.integerPart());
        EXPECT_EQ(tp.fractionalPart(), tp2.fractionalPart());

        printf("jd2k1: %lf\njd2k2: %lf\n", tp.daysFromJ2000TT(), tp2.daysFromJ2000TT());
        printf("dt:  %s\ndt2: %s\n", dt.toString().c_str(), dt2.toString().c_str());
    }
}



TEST(TimePoint, NaN)
{
    TimePoint bad = TimePoint::NaN();
    EXPECT_TRUE(bad.isNaN());
    EXPECT_FALSE(bad.isValid());

    TimePoint ok = TimePoint::Epoch();
    EXPECT_FALSE(ok.isNaN());
    EXPECT_TRUE(ok.isValid());

    // IEEE 语义：NaN != 任何值（含 NaN 自身）
    EXPECT_FALSE(bad == bad);
    // NaN 参与运算仍为 NaN
    TimePoint bad2 = bad + 1.0;
    EXPECT_TRUE(bad2.isNaN());
}



GTEST_MAIN();