/// @file      testBKVPSaxPrint.cpp
/// @brief     ~
/// @details   ~
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-01-08

#include "ast/Test.h"
#include "ast/BKVSaxPrint.hpp"
#include "ast/BKVParser.hpp"
#include "ast/StringView.hpp"
#include "ast/Color.hpp"
#include "ast/ColorNamed.hpp"
#include "ast/FileSystem.hpp"
#include "ast/IO.hpp"
#include "ast/RunTime.hpp"

AST_USING_NAMESPACE

TEST(BKVSaxPrint, SimplePrint)
{
    BKVSaxPrint printer{};
    printer.begin("root");
    {
        printer.keyValue("key", "value");
        printer.keyValue("key2", 123);
        printer.keyValue("key3", true);
        printer.keyValue("key4", 3.14);
        printer.keyValue("key5", Color(eRed));
        printer.begin("group");
        {
            printer.keyValue("key7", "value7");
        }
        printer.end("group");
        printer.keyValue("key6", "value6");
    }
    printer.end("root");
}

TEST(BKVParser, Parser)
{
    {
        BKVParser parser{};
        BKVSaxPrint printer{};
        fs::path filepath = fs::path(aTestDataDirGet()) / "TestBKV.txt";
        ast_printf("filepath: %s\n", filepath.string().c_str());
        parser.parseFile(filepath.string(), printer);
    }
    if(1)
    {
        BKVParser parser{};
        BKVSaxPrint printer{};
        fs::path filepath = fs::path(aTestDataDirGet()) / "STK/Satellite.sa3";
        ast_printf("filepath: %s\n", filepath.string().c_str());
        parser.parseFile(filepath.string(), printer);
    }
    if(1)
    {
        BKVParser parser{};
        BKVSaxPrint printer{};
        fs::path filepath = fs::path(aTestDataDirGet()) / "STK/Satellite.sa";
        ast_printf("filepath: %s\n", filepath.string().c_str());
        parser.parseFile(filepath.string(), printer);
    }
}


GTEST_MAIN()