/// @file      testTimelinePrefsLoader.cpp
/// @brief
/// @details
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-10-08

#include "ast/Test.hpp"
#include "ast/TimelinePrefsLoader.hpp"
#include "ast/FileSystem.hpp"
#include <string>

AST_USING_NAMESPACE

const char timelinePrefsPath[] = "./test-data/STK/Scenarios/testAccessSTK/testAccessTimelinePrefs.xml";

TEST(TimelinePrefsLoaderTest, Load)
{
    std::string filePath = aExeDir() + "/" + timelinePrefsPath;
    TimelinePrefs prefs;
    errc_t rc = aLoadTimeLinePrefs(filePath, prefs);
    ASSERT_EQ(rc, eNoError);

    ASSERT_EQ(prefs.rows_.size(), 3u);

    const TimelinePrefs::Row& availability = prefs.rows_[0];
    EXPECT_EQ(availability.componentName_, "AvailabilityIntervals");
    EXPECT_EQ(availability.className_, "Scenario");
    EXPECT_EQ(availability.type_, "IntervalList");
    EXPECT_EQ(availability.intervals_.size(), 1u);

    const TimelinePrefs::Row& access = prefs.rows_[1];
    EXPECT_EQ(access.componentName_, "AccessIntervals");
    EXPECT_EQ(access.className_, "Access");
    EXPECT_EQ(access.intervals_.size(), 19u);
    // Path 是区间归属的唯一标识，必须保留
    EXPECT_FALSE(access.path_.empty());

    // 时间视图的全局分析时段：8 Oct 2026 04:00 ~ 12 Oct 2026 04:00
    EXPECT_FALSE(prefs.globalInterval_.isEmpty());
    EXPECT_DOUBLE_EQ(prefs.globalInterval_.duration(), 4.0 * 86400.0);

    // 可用性行与全局分析时段一致
    ASSERT_EQ(availability.intervals_.size(), 1u);
    EXPECT_DOUBLE_EQ(availability.intervals_[0].duration(), prefs.globalInterval_.duration());
}

GTEST_MAIN()
