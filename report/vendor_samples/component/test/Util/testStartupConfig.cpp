/// @file      testGmatStartupFile.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-03-20

#include "ast/StartupConfig.hpp"
#include "ast/RunTime.hpp"
#include "ast/Test.hpp"

AST_USING_NAMESPACE

TEST(StartupConfig, gmat_startup_file)
{
    if(!aIsGithubCI()) GTEST_SKIP();
    const std::string filepath = aTestDataDirGet() + "/GMAT/gmat_startup_file.txt";
    StartupConfig config;
    errc_t rc = config.load(filepath);
    EXPECT_EQ(rc, eNoError);
    config.printConfig();
    {
        ValueView value = config.getConfig("EOP_FILE");
        EXPECT_EQ(value.toStringView(), "../data/planetary_coeff/eopc04_08.62-now");
    }
    {
        ValueView value = config.getConfig("LSK_FILE");
        EXPECT_EQ(value.toStringView(), "../data/time/SPICELeapSecondKernel.tls");
    }

}

TEST(StartupConfig, ast_startup_file)
{
    const std::string filepath = aTestDataDirGet() + "/ast_startup_file.txt";
    errc_t rc = aInitializeByConfig(filepath);
    // EXPECT_EQ(rc, eNoError);
}

GTEST_MAIN()