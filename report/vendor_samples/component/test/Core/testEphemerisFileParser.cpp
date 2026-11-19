/// @file      testEphemerisParser.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-03-24


#include "ast/EphemerisFileParser.hpp"
#include "ast/STKEphemerisFileParser.hpp"
#include "ast/EphemerisLagrangeVar.hpp"
#include "ast/RunTime.hpp"
#include "ast/Test.h"


AST_USING_NAMESPACE

class EphemerisFileParserTest : public testing::Test
{
public:
    void SetUp() override
    {
        aInitialize();
    }
    void TearDown() override
    {
        aUninitialize();
    }
};


TEST_F(EphemerisFileParserTest, STKEphemerisFileParser)
{
    STKEphemerisFileParser parser;
    std::vector<std::string> files = aTestGetConfigStringVector("STK_EPHEMERIS_FILES");
    printf("num files: %d\n", (int)files.size());
    for (const auto& file : files)
    {
        printf("parse file: %s\n", file.c_str());
        HEphemeris eph;
        errc_t rc = parser.parse(file, eph);
        EXPECT_TRUE(eph != nullptr);
        EXPECT_FALSE(rc);
        EphemerisLagrangeVar* ephLagrange = dynamic_cast<EphemerisLagrangeVar*>(eph.get());
        EXPECT_TRUE(ephLagrange != nullptr);
        EXPECT_GT(ephLagrange->size(), 0);
        printf("parse file: %s done\n", file.c_str());
    }
}


GTEST_MAIN();

