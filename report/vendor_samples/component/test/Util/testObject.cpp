/// @file      testObject.cpp
/// @brief     ~
/// @details   ~
/// @author    金柯<jinke18@foxmail.com>
/// @date      2025-12-10

#include "ast/Object.hpp"
#include "ast/ObjectNamed.hpp"
#include "ast/ReflectAPI.hpp"
#include "ast/Class.hpp"
#include "ast/AstTestMacro.h"

AST_USING_NAMESPACE


static Class* testObjectClass();


// 测试Object类的基本功能
class TestObject : public ObjectNamed
{
public:
    TestObject()
        : ObjectNamed()
        , propBool_(true)
        , propInt_(42)
        , propDouble_(3.14)
        , propString_("test")
    {}
    Class* getType() const override { return testObjectClass(); }

    // 成员变量
    bool propBool_;
    int propInt_;
    double propDouble_;
    std::string propString_;

    // 成员函数（getter和setter）
    bool propBool() const { return propBool_; }
    int propInt() const { return propInt_; }
    double propDouble() const { return propDouble_; }
    const std::string& propString() const { return propString_; }

    errc_t setPropBool(bool value) { propBool_ = value; return eNoError; }
    errc_t setPropInt(int value) { propInt_ = value; return eNoError; }
    errc_t setPropDouble(double value) { propDouble_ = value; return eNoError; }
    errc_t setPropString(StringView value) { propString_ = std::string(value); return eNoError; }
};



// 创建TestObject的Class对象
static Class* testObjectClass()
{
    static auto* cls = new Class();
    static bool first = true;
    if(first){
        // 添加属性
        cls->addProperty("propBool", aNewPropertyBoolMem<TestObject, &TestObject::propBool_>());
        cls->addProperty("propInt", aNewPropertyIntMem<TestObject, &TestObject::propInt_>());
        cls->addProperty("propDouble", aNewPropertyDoubleMem<TestObject, &TestObject::propDouble_>());
        cls->addProperty("propString", aNewPropertyStringMem<TestObject, &TestObject::propString_>());
        first = false;
    }
    return cls;
}

// 测试Object类的属性访问功能
TEST(Object, PropertyAccess)
{

    // 创建TestObject对象
    TestObject obj{};

    // 测试getAttrBool
    bool boolValue;
    errc_t err = obj.getAttrBool("propBool", boolValue);
    EXPECT_FALSE(err);
    EXPECT_EQ(boolValue, true);

    // 测试setAttrBool
    err = obj.setAttrBool("propBool", false);
    EXPECT_FALSE(err);
    EXPECT_EQ(obj.propBool_, false);

    // 测试getAttrInt
    int intValue;
    err = obj.getAttrInt("propInt", intValue);
    EXPECT_FALSE(err);
    EXPECT_EQ(intValue, 42);

    // 测试setAttrInt
    err = obj.setAttrInt("propInt", 84);
    EXPECT_FALSE(err);
    EXPECT_EQ(obj.propInt_, 84);

    // 测试getAttrDouble
    double doubleValue;
    err = obj.getAttrDouble("propDouble", doubleValue);
    EXPECT_FALSE(err);
    EXPECT_EQ(doubleValue, 3.14);

    // 测试setAttrDouble
    err = obj.setAttrDouble("propDouble", 6.28);
    EXPECT_FALSE(err);
    EXPECT_EQ(obj.propDouble_, 6.28);

    // 测试getAttrString
    std::string stringValue;
    err = obj.getAttrString("propString", stringValue);
    EXPECT_FALSE(err);
    EXPECT_EQ(stringValue, "test");

    // 测试setAttrString
    err = obj.setAttrString("propString", "test2");
    EXPECT_FALSE(err);
    EXPECT_EQ(obj.propString_, "test2");

    // 测试获取不存在的属性
    err = obj.getAttrBool("nonExistentProp", boolValue);
    EXPECT_TRUE(err);

    // 测试设置不存在的属性
    err = obj.setAttrBool("nonExistentProp", true);
    EXPECT_TRUE(err);

}

// 测试Object类的getProperty方法
TEST(Object, GetProperty)
{

    // 创建TestObject对象
    TestObject obj{};

    // 测试getProperty方法
    auto* propBool = obj.getProperty("propBool");
    EXPECT_NE(propBool, nullptr);

    auto* propInt = obj.getProperty("propInt");
    EXPECT_NE(propInt, nullptr);

    auto* propDouble = obj.getProperty("propDouble");
    EXPECT_NE(propDouble, nullptr);

    auto* propString = obj.getProperty("propString");
    EXPECT_NE(propString, nullptr);

    // 测试获取不存在的属性
    auto* propNonExistent = obj.getProperty("nonExistentProp");
    EXPECT_EQ(propNonExistent, nullptr);

}

// 测试Object类的引用计数功能
TEST(Object, ReferenceCount)
{

    // 创建TestObject对象
    TestObject* obj = new TestObject();

    // 测试初始引用计数
    EXPECT_EQ(obj->refCount(), 0);
    EXPECT_EQ(obj->weakRefCount(), 1);

    // 测试incRef和decRef
    uint32_t refCount = obj->incRef();
    EXPECT_EQ(refCount, 1);
    EXPECT_EQ(obj->refCount(), 1);

    refCount = obj->incRef();
    EXPECT_EQ(refCount, 2);
    EXPECT_EQ(obj->refCount(), 2);

    refCount = obj->decRef();
    EXPECT_EQ(refCount, 1);
    EXPECT_EQ(obj->refCount(), 1);

    refCount = obj->decRef();
    EXPECT_EQ(refCount, 0);

    // 注意：当decRef将引用计数减到0时，对象会被析构，所以不能再访问obj指针

}

// 测试Object类的弱引用计数功能
TEST(Object, WeakReferenceCount)
{

    // 创建TestObject对象
    TestObject* obj = new TestObject();

    // 测试初始弱引用计数
    EXPECT_EQ(obj->weakRefCount(), 1);

    // 测试incWeakRef和decWeakRef
    uint32_t weakRefCount = obj->incWeakRef();
    EXPECT_EQ(weakRefCount, 2);
    EXPECT_EQ(obj->weakRefCount(), 2);

    weakRefCount = obj->incWeakRef();
    EXPECT_EQ(weakRefCount, 3);
    EXPECT_EQ(obj->weakRefCount(), 3);

    weakRefCount = obj->decWeakRef();
    EXPECT_EQ(weakRefCount, 2);
    EXPECT_EQ(obj->weakRefCount(), 2);

    weakRefCount = obj->decWeakRef();
    EXPECT_EQ(weakRefCount, 1);
    EXPECT_EQ(obj->weakRefCount(), 1);

    // 测试isDestructed方法
    EXPECT_FALSE(obj->isDestructed());

    // 清理
    obj->decWeakRef(); // 这会删除对象
}

// 测试Object类的type方法
TEST(Object, Type)
{
    // 创建TestObject对象
    TestObject obj{};
    auto type = obj.type();
    EXPECT_NE(type, nullptr);
}

GTEST_MAIN()
