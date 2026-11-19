/// @file      testSpaceWeather.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-02-17


#include "ast/SpaceWeather.hpp"
#include "ast/Date.hpp"
#include "ast/Test.hpp"


AST_USING_NAMESPACE

TEST(SpaceWeather, Load)
{
    if(!aIsGithubCI()) GTEST_SKIP();

    SpaceWeather sw;
    errc_t rc = sw.loadDefault();
    EXPECT_TRUE(rc == eNoError);
    {
        int mjd = aDateToMJD(2026, 2, 12);
        auto entry = sw.getEntry(mjd);
        ASSERT_TRUE(entry != nullptr);
        EXPECT_TRUE(entry->mjd == mjd);
        EXPECT_EQ(entry->F10p7Adj, 125.7);
        EXPECT_EQ(entry->F10p7Obs, 129.0);
    }
    {
        int mjd = aDateToMJD(2026, 2, 18);
        auto entry = sw.getEntry(mjd);
        ASSERT_TRUE(entry != nullptr);
        EXPECT_TRUE(entry->mjd == mjd);
        EXPECT_EQ(entry->F10p7Adj, 180.0);
        EXPECT_EQ(entry->F10p7Obs, 184.3);
    }
    {
        int mjd = aDateToMJD(2041, 9, 1);
        auto entry = sw.getEntry(mjd);
        ASSERT_TRUE(entry != nullptr);
        EXPECT_TRUE(entry->mjd == mjd);
        EXPECT_EQ(entry->F10p7Adj, 70.2);
        EXPECT_EQ(entry->F10p7Obs, 68.9);
    }
    {
        int mjd = aDateToMJD(2041, 10, 1);
        auto entry = sw.getEntry(mjd);
        ASSERT_TRUE(entry != nullptr);
        EXPECT_TRUE(entry->mjd == mjd);
        EXPECT_EQ(entry->F10p7Adj, 70.0);
        EXPECT_EQ(entry->F10p7Obs, 69.8);
    }
}


GTEST_MAIN();


