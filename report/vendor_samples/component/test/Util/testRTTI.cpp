/// @file      testRTTI.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-04-02

#include "ast/RTTIAPI.hpp"
#include "ast/IO.hpp"
#include "ast/RunTime.hpp"
#include "ast/StateCartesian.hpp"
#include "ast/StateKeplerian.hpp"
#include "ast/CelestialBody.hpp"
#include "ast/FrameAssembly.hpp"
#include "ast/Satellite.hpp"
#include "ast/Test.h"
#include <vector>
#include <string>
#include <cstdio>

AST_USING_NAMESPACE



class RTTITest: public testing::Test
{
public:
    void SetUp()
    {
        aInitialize();
    }
    void TearDown()
    {
        aUninitialize();
    }
};

TEST_F(RTTITest, NewObject) 
{
    std::vector<std::string> classNames;
    aGetAllClassNames(classNames);
    for(auto className: classNames)
    {
        bool isVirtual = aIsVirtualClass(className);
        if(!isVirtual){
            std::printf("test for className: %s\n", className.c_str());
            auto obj = aMakeObject(className);
            EXPECT_TRUE(obj != nullptr);
        }
    }
}

TEST_F(RTTITest, ClassNames)
{
    {Satellite sat;};
    std::vector<std::string> classNames;
    aGetAllClassNames(classNames);
    EXPECT_TRUE(!classNames.empty());
    for(auto className: classNames)
    {
        Class* cls = aGetClass(className);
        std::string desc = cls->desc();
        ast_printf("%s: %s\n", className.c_str(), desc.c_str());
    }
}

TEST_F(RTTITest, ClassDefaultObject)
{
    std::vector<std::string> classNames;
    aGetAllClassNames(classNames);
    for(auto className: classNames)
    {
        bool isVirtual = aIsVirtualClass(className);
        if(!isVirtual){
            std::printf("test for className: %s\n", className.c_str());
            auto obj = aGetClassDefaultObject(className);
            EXPECT_TRUE(obj != nullptr);
        }
    }
}

TEST_F(RTTITest, ObjectParentScope)
{
    // 测试父作用域能否正确设置和获取，以及父作用域被销毁后子对象的弱引用是否失效
    {
        SharedPtr<CelestialBody> body = new CelestialBody();
        auto frame = new FrameAssembly();
        frame->setParentScope(body);
        EXPECT_EQ(body.get(), frame->getParentScope());
        WeakPtr<FrameAssembly> frame_wp = frame;
        EXPECT_FALSE(frame_wp.expired());
        body.reset();
        EXPECT_TRUE(frame_wp.expired());
    }
    // 测试父作用域为临时对象时，子对象的弱引用是否失效
    {
        WeakPtr<FrameAssembly> frame_wp;
        {
            CelestialBody body;
            auto frame = new FrameAssembly();
            frame->setParentScope(&body);
            frame_wp = frame;
            EXPECT_FALSE(frame_wp.expired());
        }
        EXPECT_TRUE(frame_wp.expired());
    }
    {
        SharedPtr<CelestialBody> body = new CelestialBody();

        auto frame = new FrameAssembly();
        frame->setParentScope(body);
        WeakPtr<FrameAssembly> frame_wp = frame;

        StateCartesian* state = new StateCartesian();
        state->setParentScope(frame);
        WeakPtr<StateCartesian> state_wp = state;

        EXPECT_FALSE(frame_wp.expired());
        EXPECT_FALSE(state_wp.expired());
        body.reset();
        EXPECT_TRUE(frame_wp.expired());
        EXPECT_TRUE(state_wp.expired());
    }
}

TEST_F(RTTITest, ObjectCast)
{
    SharedPtr<Object> obj(aNewObject("CelestialBody"));
    EXPECT_NE(obj.get(), nullptr);

    CelestialBody* body = aobject_cast<CelestialBody*>(obj.get());
    EXPECT_NE(body, nullptr);

    StateCartesian* state = aobject_cast<StateCartesian*>(body);
    EXPECT_EQ(state, nullptr);

    Object* objPtr = aobject_cast<Object*>(body);
    EXPECT_NE(objPtr, nullptr);
}


GTEST_MAIN()
