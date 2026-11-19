/// @file      testDownloadLfs.cpp
/// @brief     Git LFS 下载相关测试
/// @details   ~
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-08-27


#include "ast/AstTestMacro.h"
#include "ast/DownloadLfs.hpp"
#include "ast/JsonValue.hpp"
#include "ast/FileSystem.hpp"
#include <string>
#include <cstdlib>

AST_USING_NAMESPACE

// 解析指针文本
TEST(Lfs, ParsePointer)
{
    const char* text =
        "version https://git-lfs.github.com/spec/v1\n"
        "oid sha256:06b739d9d14e5c64ddea77ecfba5c392e849b5ee79fcd5a3aad0d3c8d8c7b50d\n"
        "size 61440\n";

    LfsPointerInfo info;
    EXPECT_EQ(aLfsParsePointer(text, info), eNoError);
    EXPECT_EQ(info.oid, "06b739d9d14e5c64ddea77ecfba5c392e849b5ee79fcd5a3aad0d3c8d8c7b50d");
    EXPECT_EQ(info.size, static_cast<std::size_t>(61440));

    // 缺 oid 应报错
    LfsPointerInfo bad;
    EXPECT_NE(aLfsParsePointer("version https://git-lfs.github.com/spec/v1\nsize 3\n", bad),
              eNoError);
}

// 推导 LFS 批量端点：必须保留 .git
TEST(Lfs, Endpoint)
{
    EXPECT_EQ(aLfsEndpoint("https://github.com/<owner>/<repo>.git"),
              "https://github.com/<owner>/<repo>.git/info/lfs");
    EXPECT_EQ(aLfsEndpoint("https://github.com/<owner>/<repo>.git/"),
              "https://github.com/<owner>/<repo>.git/info/lfs");
}

// 构建 batch 请求体
TEST(Lfs, BatchRequestBody)
{
    const std::string oid = "06b739d9d14e5c64ddea77ecfba5c392e849b5ee79fcd5a3aad0d3c8d8c7b50d";
    std::string body = aLfsBatchRequestBody(oid, 61440);

    JsonValue root = JsonValue::FromString(body);
    EXPECT_TRUE(root.isObject());
    EXPECT_EQ(root["operation"].toString(), "download");
    EXPECT_TRUE(root["transfers"].isArray());
    ASSERT_TRUE(root["objects"].isArray());
    ASSERT_GE(root["objects"].size(), 1u);
    EXPECT_EQ(root["objects"][0]["oid"].toString(), oid);
    EXPECT_EQ(root["objects"][0]["size"].toInt(), 61440);
}

// 真实下载：需要网络与外部 LFS 端点。未设置环境变量时跳过，代码不硬编码具体仓库。
// 通过 AST_LFS_TEST_POINTER(指针文件) / AST_LFS_TEST_OUTPUT(输出路径) /
// AST_LFS_TEST_REMOTE(远端地址) 启用。
TEST(Lfs, LiveDownload)
{
    const char* pointer = std::getenv("AST_LFS_TEST_POINTER");
    const char* output = std::getenv("AST_LFS_TEST_OUTPUT");
    const char* remote = std::getenv("AST_LFS_TEST_REMOTE");
    if (!pointer || !output || !remote)
    {
        GTEST_SKIP() << "set AST_LFS_TEST_POINTER/OUTPUT/REMOTE to run the live download";
    }

    EXPECT_EQ(aDownloadLfs(pointer, output, remote), eNoError);
    fs::path out(output);
    EXPECT_TRUE(fs::exists(out));
    EXPECT_GT(fs::file_size(out), 0u);
}

GTEST_MAIN()
