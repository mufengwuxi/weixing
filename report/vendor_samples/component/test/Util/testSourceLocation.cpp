/// @file      testSourceLocation.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-09-09


#include "ast/SourceLocation.hpp"
#include "ast/Test.hpp"
#include <stdio.h>

AST_USING_NAMESPACE

TEST(SourceLocation, ExtractAfterLastDotDot) {
    constexpr const char* path = extract_after_last_dotdot("abc/../../a/b/c");
    printf("%s\n", path);
    EXPECT_EQ(path, std::string("a/b/c"));
}


GTEST_MAIN()