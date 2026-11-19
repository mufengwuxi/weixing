/// @file      testBuilder.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-03-30

#include "ast/BuildTarget.hpp"
#include "ast/Test.h"


AST_USING_NAMESPACE

TEST(BuilderTest, Build)
{
    if(aIsCI())
        GTEST_SKIP();
    std::string filepath = "example-for-builder-test.cpp";
    FILE* f = fopen(filepath.c_str(), "w");
    std::string cppcode = 
    R"(
    #include <stdio.h>
    
    int main() { 
        printf("Hello, World!\n");
        return 0; 
    }
    )";
    fwrite(cppcode.c_str(), 1, strlen(cppcode.c_str()), f);
    fclose(f);
    BuildTarget target("test-builder");
    errc_t rc = target
        .setKind("binary")
        .addFiles({filepath})
        .build();
    EXPECT_EQ(rc, 0);
    rc = target.run();
    EXPECT_EQ(rc, 0);
}


GTEST_MAIN()

