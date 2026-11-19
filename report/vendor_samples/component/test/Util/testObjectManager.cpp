/// @file      testObjectManager.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-04-03

#include "ast/ObjectManager.hpp"
#include "ast/RTTIAPI.hpp"
#include "ast/ObjectNamed.hpp"
#include "ast/Test.h"

AST_USING_NAMESPACE

TEST(ObjectManagerTest, MaxObjectCount)
{
    auto instance = &ObjectManager::CurrentInstance();
    uint32_t count = instance->getObjectCount();
    EXPECT_TRUE(count == 0);
    const uint32_t maxCount = 1000;
    instance->setMaxObjectCount(maxCount);
    Object* objectList[maxCount];
    // 测试添加对象到最大数量
    for(uint32_t i = 0; i < maxCount; i++)
    {
        ObjectNamed *obj = new ObjectNamed();
        uint32_t id = aAddObject(obj);
        objectList[i] = obj;
        EXPECT_TRUE(id != INVALID_ID);
    }
    // 测试添加对象到最大数量后，再添加对象
    {
        SharedPtr<Object> obj_sp = new ObjectNamed();
        uint32_t id = aAddObject(obj_sp);
        EXPECT_TRUE(id == INVALID_ID);
    }
    // 测试删除对象后，再添加对象，是否能成功添加
    {
        SharedPtr<Object> obj_sp = objectList[73];
        obj_sp = nullptr;
        Object* obj = new ObjectNamed();
        uint32_t id = aAddObject(obj);
        EXPECT_TRUE(id != INVALID_ID);
    }
}

TEST(ObjectManagerTest, ObjectCount)
{

}

GTEST_MAIN()


