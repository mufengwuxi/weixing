/// @file      testUiEditObject.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-04-02

#ifdef AST_WITH_QT
#include "ast/Object.hpp"
#include "ast/RTTIAPI.hpp"
#include "ast/RunTime.hpp"
#include "ast/Test.h"
#include "ast/MotionTwoBody.hpp"

AST_USING_NAMESPACE

TEST(UiEditObject, Test)
{
    SharedPtr<MotionTwoBody> motion = new MotionTwoBody();
    aInitialize();
    std::vector<std::string> names;
    names = {"BodyEphemerisSPK", "BodyEphemerisDE"};
    for(auto& name: names)
    {
        printf("test for className: %s\n", name.c_str());
        auto obj = aMakeObject(name);
        EXPECT_TRUE(obj);
        EXPECT_EQ(obj->showEditDialog(), 0);
    }

    aGetAllClassNames(names);
    for(auto& name: names)
    {
        printf("test for className: %s\n", name.c_str());
        auto obj = aMakeObject(name);
        EXPECT_TRUE(obj);
        EXPECT_EQ(obj->showEditDialog(), 0);
    }
}

GTEST_MAIN()

#else
#include <stdio.h>
int main(){
    puts("没有启用Qt库");
    return 0;
}
#endif
