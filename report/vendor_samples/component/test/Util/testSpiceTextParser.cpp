/// @file      testSpiceParser.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-03-08

#include "ast/PCKParser.hpp"
#include "ast/RunTime.hpp"
#include "ast/Test.h"

AST_USING_NAMESPACE

TEST(PCKParser, KernelData)
{
    {
        char buf[] = "KEY1 = VALUE12\n";
        KernelData data = std::vector<char>(buf, buf + sizeof(buf));
        {
            auto doubleData = data.getDoubleData();
            EXPECT_TRUE(doubleData != nullptr);
            std::vector<double> dData = *doubleData;
            printf("dData.size(): %d\n", (int)dData.size());
        }
        int size = (int)data.getCharData()->size();
        EXPECT_EQ(size, sizeof(buf));
    }
    {
        KernelData data = std::vector<double>{1.0, 2.0, 3.0};
        auto charData = data.getCharData();
        EXPECT_EQ(charData->size(), 3 * sizeof(double));
    }
}

TEST(PCKParser, readData)
{
    PCKParser parser(aTestGetConfigValue("PCK_FILE").toString());
    KernelPool kernelPool;
    errc_t ret = parser.readData(kernelPool);
    EXPECT_EQ(ret, 0);
    size_t size = kernelPool.size();
    EXPECT_GT(size, 0);
    {
        auto doubleData = kernelPool.getDoubleData("BODY609_POLE_DEC");
        EXPECT_TRUE(doubleData != nullptr);
    }
}



GTEST_MAIN()
