/// @file      testTempFileGuard.cpp
/// @brief
/// @details   ~
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-08-27

#include "ast/TempFileGuard.hpp"
#include "ast/AstTestMacro.h"
#include <fstream>
#include <string>

AST_USING_NAMESPACE

namespace {
    namespace fs = _AST fs_simple;

    std::string makeTempPath(const std::string& name)
    {
        return "TempFileGuard_" + name + ".tmp";
    }

    void writeFile(const std::string& path, const std::string& content)
    {
        std::ofstream file(path);
        file << content;
    }
}

// 作用域结束时自动删除临时文件
TEST(TempFileGuard, RemovesOnScopeExit)
{
    std::string p = makeTempPath("scope_exit");
    writeFile(p, "data");
    ASSERT_TRUE(fs::exists(p));
    {
        TempFileGuard guard(p);
        EXPECT_TRUE(guard.isActive());
        EXPECT_TRUE(guard.exists());
    }
    EXPECT_FALSE(fs::exists(p));
}

// 提前移除并解除析构时删除
TEST(TempFileGuard, RemoveNow)
{
    std::string p = makeTempPath("remove_now");
    writeFile(p, "data");
    ASSERT_TRUE(fs::exists(p));
    TempFileGuard guard(p);
    guard.remove();
    EXPECT_FALSE(guard.isActive());
    EXPECT_FALSE(fs::exists(p));
    // 析构时不应再删除任何已不存在的文件
}

// release() 移交所有权，文件应被保留
TEST(TempFileGuard, ReleaseKeepsFile)
{
    std::string p = makeTempPath("release");
    writeFile(p, "data");
    ASSERT_TRUE(fs::exists(p));
    {
        TempFileGuard guard(p);
        guard.release();
        EXPECT_FALSE(guard.isActive());
    }
    EXPECT_TRUE(fs::exists(p));
    fs::remove(p);
}


// 空守卫与无效路径的容错
TEST(TempFileGuard, EmptyGuard)
{
    TempFileGuard guard;                 // 默认构造：不删除任何文件
    EXPECT_FALSE(guard.isActive());
    EXPECT_FALSE(guard.exists());
    guard.remove();                      // 不应崩溃
    guard.release();
}

GTEST_MAIN()
