/// @file      testScriptComment.cpp
/// @brief     ~
/// @details   ~
/// @author    金柯<jinke18@foxmail.com>
/// @date      2025-12-24

#include "ast/ScriptAPI.hpp"
#include "ast/AstTestMacro.h"
#include "ast/StringView.hpp"
#include "ast/IO.hpp"
#include "ast/SharedPtr.hpp"
#include "ast/Value.hpp"

AST_USING_NAMESPACE




// 辅助函数：测试双精度浮点数值表达式
void testScriptComment(StringView str, double expectedValue)
{
    printf("testScriptComment: %s\n", str.data());
    SharedPtr<Value> value = aEval(str);
    EXPECT_TRUE(value.get() != nullptr);
    if(aValueIsDouble(value.get())){
        EXPECT_DOUBLE_EQ(aValueUnboxDouble(value.get()), expectedValue);
    }else if(aValueIsBool(value.get())){
        EXPECT_TRUE(aValueUnboxBool(value.get()) == (bool)(expectedValue != 0.0));
    }
    // else if(aValueIsString(value.get())){
        //@todo 字符串比较
        //EXPECT_TRUE(aValueUnboxString(value.get()) == expectedValue);
    // }
    else if(aValueIsInt(value.get())){
        EXPECT_EQ(aValueUnboxInt(value.get()), expectedValue);
    }
}

TEST(ScriptComment, Basic)
{
    // 测试#注释
    testScriptComment("1 + 2 # this is comment", 3.0);
    testScriptComment("# this is a comment line\n42", 42.0);
}

GTEST_MAIN()