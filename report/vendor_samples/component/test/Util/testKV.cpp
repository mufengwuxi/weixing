/// @file      testLoadGmatStartupFile.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-03-20

#include "ast/KVParser.hpp"
#include "ast/Test.h"


AST_USING_NAMESPACE

TEST(KVTest, LoadGmatStartupFile)
{
    KVParser parser;
    parser.open(aTestDataDirGet() + "/GMAT/gmat_startup_file.txt");
    BKVItemView item;
    while(parser.getNext(item) == eNoError){
        printf("key: '%.*s'\n", (int)item.key().size(), item.key().data());
        printf("value: '%.*s'\n", (int)item.value().size(), item.value().data());
    }
}



GTEST_MAIN()

