/// @file      testEOP.cpp
/// @brief     ~
/// @details   ~
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-01-08

#include "ast/EOP.hpp"
#include "ast/Test.hpp"
#include "ast/RunTime.hpp"
#include "ast/StringView.hpp"
#include "ast/JulianDate.hpp"
#include "ast/TestConfig.hpp"
#include "ast/Environment.hpp"

AST_USING_NAMESPACE

TEST(EOP, Load)
{
    EOP eop;
    errc_t err = eop.load(aTestGetConfigValue("EOP_FILE"));
    EXPECT_EQ(err, 0);
    EXPECT_TRUE(eop.size() > 0);
    printf("eop size: %zu\n", eop.size());
    // EXPECT_TRUE(eop.size() >=  23382 + 181);
}

TEST(EOP, UT1MinusUTC_UTC)
{
    EOP eop;
    errc_t err = eop.load(aTestGetConfigValue("EOP_FILE"));
    EXPECT_EQ(err, 0);
    EXPECT_TRUE(eop.size() > 0);

    if(!aIsGithubCI())
        GTEST_SKIP();

    {
        JulianDate jdUTC = JulianDate::FromDateTime(2026, 1, 1, 0, 0, 0.0);
        double ut1_minus_utc = eop.getUT1MinusUTC_UTC(jdUTC);
        printf("ut1_minus_utc: %.15lf\n", ut1_minus_utc);
        EXPECT_TRUE(ut1_minus_utc != 0.0);
        EXPECT_EQ(ut1_minus_utc, 0.0740852);
    }
    {
        JulianDate jdUTC = JulianDate::FromDateTime(2026, 1, 1, 8, 0, 0.0);
        double ut1_minus_utc = eop.getUT1MinusUTC_UTC(jdUTC);
        double expect = 0.07411696666666667;
        printf("ut1_minus_utc: %.18lf\n", ut1_minus_utc);
        printf("expect       : %.18lf\n", expect);
        EXPECT_TRUE(ut1_minus_utc != 0.0);

        EXPECT_NEAR(ut1_minus_utc, expect, 1e-15);
    }
}


GTEST_MAIN();