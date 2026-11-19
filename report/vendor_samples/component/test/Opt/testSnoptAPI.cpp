/// @file      testSnoptAPI.cpp
/// @brief     
/// @details   ~
/// @author    金柯<jinke18@foxmail.com>
/// @date      28.11.2025




#include "ast/AstTestMacro.h"
#include "ast/SnoptAPI.hpp"

AST_USING_NAMESPACE

TEST(SnoptAPI, BasicUsage)
{
    auto api = aSnopt_Find();
    printf("snoptapi address: %p\n", (void*)api);
    api = aSnopt_Load(AST_LIB_LINKNAME("AstMock"));
    printf("snoptapi address: %p\n", (void*)api);
    EXPECT_TRUE(api);
}

GTEST_MAIN()
