/// @file      testScriptControlFlow.cpp
/// @brief     测试控制流
/// @details   测试控制流相关的脚本
/// @author    金柯<jinke18@foxmail.com>
/// @date      2025-12-25

#include "ast/ScriptAPI.hpp"
#include "ast/Interpreter.hpp"
#include "ast/AstTestMacro.h"
#include "ast/Value.hpp"

AST_USING_NAMESPACE

class ScriptControlFlowTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 设置测试环境
        aScript_SetInterpreter(&interpreter);
    }

    void TearDown() override {
        // 清理测试环境
        aScript_RemoveInterpreter(&interpreter);
    }
protected:
    Interpreter interpreter;
};

void testScriptControlFlowDouble(const char* script, double expect)
{
    printf("script: %s\n", script);
    SharedPtr<Expr> expr = aParseExpr(script);
    std::string exprStr = aFormatExpr(expr);
    printf("parsed: %s\n", exprStr.c_str());
    SharedPtr<Value> val = aEvalExpr(expr);
    ASSERT_TRUE(val != nullptr);
    ASSERT_TRUE(aValueIsDouble(val));
    double result = aValueUnboxDouble(val);
    ASSERT_EQ(result, expect);
}


/// @test      测试if-else条件语句
TEST_F(ScriptControlFlowTest, TestIfElse) {
    // 测试if-else条件语句
    const char* script1 = "if true\n    a = 0\nelse\n    a = 1\nend";
    EXPECT_TRUE(aEval(script1) != nullptr);

    const char* script2 = R"(
if false
    a = 1
elseif false
    a = 0
else
    b = 122
end
a = 12.0
# true ? a : b

)";
    testScriptControlFlowDouble(script2, 12.0);

    const char* script3 = R"(
if false
    a = 1.0
elseif false
    a = 0
else
    b = 12.0
end

true ? b : a

)";
    testScriptControlFlowDouble(script3, 12.0);
}


TEST_F(ScriptControlFlowTest, TestWhileLoop) {
    // 测试while循环语句
    const char* script1 = R"(
    a = 0.0
    while a < 10
        a = a + 1
    end
    a
    )";
    testScriptControlFlowDouble(script1, 10.0);

    const char* script2 = R"(
    a = 0
    s = 0.0
    while a < 10
        s = s + a
        a = a + 1
    end
    s
    )";
    testScriptControlFlowDouble(script2, 45.0);
}

/// @test      测试for循环语句（Julia风格）
TEST_F(ScriptControlFlowTest, TestForLoop) {
    // 测试for范围循环
    const char* script1 = R"(
    a = 0.0
    for i in 1:5
        a = a + i
    end
    a
    )";
    testScriptControlFlowDouble(script1, 15.0);

    // 测试for循环嵌套
    const char* script2 = R"(
    a = 0.0
    for i in 1:3
        for j in 1:2
            a = a + i * j
        end
    end
    a
    )";
    testScriptControlFlowDouble(script2, 18.0);
}


GTEST_MAIN()