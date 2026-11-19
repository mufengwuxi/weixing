/// @file      testValueView.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-03-07

#include "ast/ValueView.hpp"
#include "ast/Test.h"


AST_USING_NAMESPACE

TEST(ValueViewTest, toVector)
{
    {
        ValueView value("");
        auto vec = value.toVector();
        EXPECT_EQ(vec.size(), 1);
    }
    {
        ValueView value(" ");
        auto vec = value.toVector();
        EXPECT_EQ(vec.size(), 1);
        EXPECT_EQ(vec[0].value(), "");
    }
    {
        ValueView value("12345");
        auto vec = value.toVector();
        EXPECT_EQ(vec.size(), 1);
        EXPECT_EQ(vec[0].value(), "12345");
    }
    {
        ValueView value(" 12.34");
        auto vec = value.toVector();
        EXPECT_EQ(vec.size(), 1);
        EXPECT_EQ(vec[0].value(), "12.34");
    }
    {
        ValueView value("a, b, c");
        auto vec = value.toVector();
        EXPECT_EQ(vec.size(), 3);
        EXPECT_EQ(vec[0].value(), "a");
        EXPECT_EQ(vec[1].value(), "b");
        EXPECT_EQ(vec[2].value(), "c");
    }
    {
        ValueView value("a b , c \n d");
        auto vec = value.toVector();
        EXPECT_EQ(vec.size(), 4);
        EXPECT_EQ(vec[0].value(), "a");
        EXPECT_EQ(vec[1].value(), "b");
        EXPECT_EQ(vec[2].value(), "c");
        EXPECT_EQ(vec[3].value(), "d");
    }
    {
        ValueView value(" a b ,, c \n d\n");
        auto vec = value.toVector();
        EXPECT_EQ(vec.size(), 5);
        EXPECT_EQ(vec[0].value(), "a");
        EXPECT_EQ(vec[1].value(), "b");
        EXPECT_EQ(vec[2].value(), "");
        EXPECT_EQ(vec[3].value(), "c");
        EXPECT_EQ(vec[4].value(), "d");
    }
    {
        ValueView value(" ( a b ,12.34D3, c \n d\n) ");
        auto vec = value.toVector();
        EXPECT_EQ(vec.size(), 5);
        EXPECT_EQ(vec[0].value(), "a");
        EXPECT_EQ(vec[1].value(), "b");
        EXPECT_EQ(vec[2].value(), "12.34D3");
        EXPECT_EQ(vec[3].value(), "c");
        EXPECT_EQ(vec[4].value(), "d");
    }
}


GTEST_MAIN();

