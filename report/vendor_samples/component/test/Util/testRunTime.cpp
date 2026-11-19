/// @file      testRunTime.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-02-18

#include "ast/RunTime.hpp"
#include "ast/StringView.hpp"
#include "ast/Test.h"
#include "ast/FileSystem.hpp"
#include <cstdio>

AST_USING_NAMESPACE

TEST(RunTime, DataDir)
{
    std::string datadir = aDataDir();
    EXPECT_TRUE(datadir.size() > 0);
    const char newdatadir[] = "./newdatadir";
    fs::create_directories(newdatadir);
    aDataDirSet(newdatadir);
    datadir = aDataDir();
    printf("datadir: %s\n", datadir.c_str());
    EXPECT_TRUE(datadir == newdatadir);
    datadir = aDataDirGet();
    printf("datadir: %s\n", datadir.c_str());
    EXPECT_TRUE(datadir == newdatadir);
}


TEST(RunTime, CacheDir)
{
    std::string cachedir = aCacheDir();
    printf("cachedir: %s\n", cachedir.c_str());
    EXPECT_TRUE(cachedir.size() > 0);
}

GTEST_MAIN()