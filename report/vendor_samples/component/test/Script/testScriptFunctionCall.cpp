/// @file      testScriptFunctionCall.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-04-15

#include "ast/ScriptAPI.hpp"
#include "ast/TestScript.hpp"
#include "ast/Test.h"

AST_USING_NAMESPACE

TEST(ScriptFunctionCallTest, Parse)
{
    EXPECT_FALSE(aTestScriptParse("f(1, 2, 3)"));
    EXPECT_FALSE(aTestScriptParse("f(a, b, c)"));
}


TEST(ScriptFunctionCallTest, Eval)
{
    GTEST_SKIP();
    EXPECT_FALSE(aTestScriptEval("sin(0)", 0.0));
}


GTEST_MAIN()


