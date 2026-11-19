/// @file      testArchive.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-04-13

#include "ast/Archive.hpp"
#include "ast/Test.h"
#include <string>


AST_USING_NAMESPACE

struct TestData
{
public:
    bool active_;
    double radius_;
    std::string name_;
};


static void archive(TestData& data, Archive& ar)
{
    ar & make_nvp("active", data.active_);
    ar & make_nvp("radius", data.radius_);
    ar & make_nvp("name", data.name_);
}


TEST(ArchiveTest, TestArchive)
{
    TestData data;

}

GTEST_MAIN()

