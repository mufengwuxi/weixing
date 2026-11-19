/// @file      testPosix.cpp
/// @brief     ~
/// @details   ~
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-02-02

#include "ast/Posix.hpp"
#include "ast/FileSystem.hpp"
#include "ast/Test.h"

AST_USING_NAMESPACE 



// TEST(PosixTest, isdir)
// {
//     struct posix::stat st;
//     ASSERT_EQ(posix::fstat(0, &st), 0);
//     ASSERT_FALSE(posix::isdir(st));
// }

TEST(PosixTest, unicode)
{
    int rc;
    fs::path unicode_dir = u8"./测试目录_for_posix_Unicode";
    EXPECT_TRUE(fs::create_directory(unicode_dir));
    EXPECT_TRUE(fs::exists(unicode_dir));
    EXPECT_TRUE(fs::is_directory(unicode_dir));
    rc = posix::chdir(unicode_dir.string().c_str());
    EXPECT_EQ(rc, 0);

    char buf[256];
    char* cwd = posix::getcwd(buf, 256);
    posix::printf("cwd: %s\n", cwd);
    rc = posix::chdir(cwd);
    EXPECT_EQ(rc, 0);

    rc = posix::chdir("..");
    EXPECT_EQ(rc, 0);

    rc = posix::rmdir(unicode_dir.string().c_str());
    EXPECT_EQ(rc, 0);
    EXPECT_FALSE(fs::exists(unicode_dir));
    EXPECT_FALSE(fs::is_directory(unicode_dir));
}


GTEST_MAIN()

