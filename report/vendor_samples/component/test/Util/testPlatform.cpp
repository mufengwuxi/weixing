/// @file      testPlatform.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-03-10

#include "ast/Endian.hpp"
#include "ast/Test.h"


AST_USING_NAMESPACE

TEST(Endian, GetEndian)
{
    EEndian endian = aGetEndian();
    printf("endian: %s\n", (endian == EEndian::eLittleEndian) ? "LittleEndian" : "BigEndian");
}


GTEST_MAIN()
