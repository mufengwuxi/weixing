/// @file      testCmdSplit.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-05-11

#pragma once

#include "AstGlobal.h"
#include "ast/CommandDispatcher.hpp"
#include "ast/Test.h"


AST_USING_NAMESPACE


TEST(CmdSplitTest, Basic) 
{
    using StringViewVector = std::vector<StringView>;
    using namespace detail;
    {
        auto result = split("a b c");
        EXPECT_EQ(result, StringViewVector({"a", "b", "c"}));
    }
    {
        auto result = split("  a  b  c  ");
        EXPECT_EQ(result, StringViewVector({"a", "b", "c"}));
    }
    {
        auto result = split("");
        EXPECT_EQ(result, StringViewVector({}));
    }
    {
        auto result = split("  ");
        EXPECT_EQ(result, StringViewVector({}));
    }
    {
        auto result = split(R"("a"  "b c")");
        EXPECT_EQ(result, StringViewVector({"a", "b c"}));
    }
    {
        auto result = split(R"(  ""  b c )");
        EXPECT_EQ(result, StringViewVector({"", "b", "c"}));
    }
    {
        auto result = split(R"(  "  "  b c )");
        EXPECT_EQ(result, StringViewVector({"  ", "b", "c"}));
    }
    {
        auto result = split(R"(  a"b c" )");
        EXPECT_EQ(result, StringViewVector({"a", "b c"}));
    }
    {
        auto result = split(R"(a"b c"d)");
        EXPECT_EQ(result, StringViewVector({"a", "b c", "d"}));
    }
}


GTEST_MAIN()
