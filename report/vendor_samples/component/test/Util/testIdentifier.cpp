/// @file      testIdentifier.cpp
/// @brief     ~
/// @details   ~
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-01-18

#include "ast/IdentifierAPI.hpp"
#include "ast/Identifier.hpp"
#include "ast/Test.h"

AST_USING_NAMESPACE

TEST(IdentifierTest, BasicOperations) {
    auto id1 = aIdentifier("testIdentifier");
    auto id2 = aIdentifier(std::string("testIdentifier"));
    auto id3 = aIdentifier("testIdentifier");
    printf("id1: %s, id2: %s, id3: %s\n", id1->c_str(), id2->c_str(), id3->c_str());
    EXPECT_EQ(id1, id2);
    EXPECT_EQ(id2, id3);
    EXPECT_EQ(id3, id1);
}


GTEST_MAIN()