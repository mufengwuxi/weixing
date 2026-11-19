/// @file      testScanner.cpp
/// @brief     ~
/// @details   ~
/// @author    金柯<jinke18@foxmail.com>
/// @date      2025-12-20

#include "ast/AstTestMacro.h"
#include "ast/ScriptAPI.hpp"
#include "ast/Parser.hpp"
#include "ast/Lexer.hpp"
#include "ast/Scanner.hpp"
#include <fstream>
#include <string>
#include <cstdio>

AST_USING_NAMESPACE

/// @brief 创建临时测试文件
/// @param content 文件内容
/// @param filename 文件名
/// @return 是否创建成功
bool createTestFile(const std::string& content, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    file << content;
    file.close();
    return true;
}

/// @brief 删除临时测试文件
/// @param filename 文件名
void deleteTestFile(const std::string& filename) {
    std::remove(filename.c_str());
}

/// @brief 通过FileScanner解析文件脚本
/// @param filename 文件名
/// @return 解析结果表达式
Expr* parseScriptFromFile(const std::string& filename) {
    FILE* file = std::fopen(filename.c_str(), "r");
    if (!file) {
        return nullptr;
    }

    FileScanner scanner(file);
    Lexer lexer(&scanner);
    Parser parser(lexer);

    Expr* expr = parser.parseExpression();

    // 检查是否解析到文件末尾
    if (expr && parser.currentTokenType() != Lexer::eEndOfFile) {
        delete expr;
        expr = nullptr;
    }

    std::fclose(file);
    return expr;
}

/// @brief 通过StreamScanner解析文件脚本
/// @param filename 文件名
/// @return 解析结果表达式
Expr* parseScriptFromStream(const std::string& filename) {
    std::fstream file(filename);
    if (!file.is_open()) {
        return nullptr;
    }

    StreamScanner scanner(file);
    Lexer lexer(&scanner);
    Parser parser(lexer);

    Expr* expr = parser.parseExpression();

    // 检查是否解析到文件末尾
    if (expr && parser.currentTokenType() != Lexer::eEndOfFile) {
        delete expr;
        expr = nullptr;
    }

    file.close();
    return expr;
}

TEST(ScannerTest, FileScannerTest) {
    // 创建临时测试文件
    const std::string filename = "test_scanner_file.txt";
    const std::string content = "123.0 + 456";
    ASSERT_TRUE(createTestFile(content, filename));

    // 通过FileScanner间接测试脚本解析
    Expr* expr = parseScriptFromFile(filename);
    ASSERT_NE(expr, nullptr);

    // 格式化表达式并验证解析结果
    std::string exprStr = aFormatExpr(expr);
    std::cout << "Input: " << content << std::endl;
    std::cout << "Parsed: " << exprStr << std::endl;

    // 清理资源
    delete expr;
    deleteTestFile(filename);
}

TEST(ScannerTest, FileScannerComplexTest) {
    // 创建包含复杂表达式的临时测试文件
    const std::string filename = "test_scanner_complex.txt";
    const std::string content = "(10.0 + 20) * 30 / 5 - 15.0";
    ASSERT_TRUE(createTestFile(content, filename));

    // 通过FileScanner间接测试复杂脚本解析
    Expr* expr = parseScriptFromFile(filename);
    ASSERT_NE(expr, nullptr);

    // 格式化表达式并验证解析结果
    std::string exprStr = aFormatExpr(expr);
    std::cout << "Input: " << content << std::endl;
    std::cout << "Parsed: " << exprStr << std::endl;

    // 清理资源
    delete expr;
    deleteTestFile(filename);
}

TEST(ScannerTest, StreamScannerTest) {
    // 创建临时测试文件
    const std::string filename = "test_stream_scanner.txt";
    const std::string content = "789.0 - 345 * 2.0";
    ASSERT_TRUE(createTestFile(content, filename));

    // 通过StreamScanner间接测试脚本解析
    Expr* expr = parseScriptFromStream(filename);
    ASSERT_NE(expr, nullptr);

    // 格式化表达式并验证解析结果
    std::string exprStr = aFormatExpr(expr);
    std::cout << "Input: " << content << std::endl;
    std::cout << "Parsed: " << exprStr << std::endl;

    // 清理资源
    delete expr;
    deleteTestFile(filename);
}

TEST(ScannerTest, StreamScannerComplexTest) {
    // 创建包含复杂表达式的临时测试文件
    const std::string filename = "test_stream_complex.txt";
    const std::string content = "2^3 + 4 * 5.0";
    ASSERT_TRUE(createTestFile(content, filename));

    // 通过StreamScanner间接测试复杂脚本解析
    Expr* expr = parseScriptFromStream(filename);
    ASSERT_NE(expr, nullptr);

    // 格式化表达式并验证解析结果
    std::string exprStr = aFormatExpr(expr);
    std::cout << "Input: " << content << std::endl;
    std::cout << "Parsed: " << exprStr << std::endl;

    // 清理资源
    delete expr;
    deleteTestFile(filename);
}

GTEST_MAIN()