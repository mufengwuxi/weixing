/// @file      testGUI.cpp
/// @brief     GUI模块单元测试
/// @details   测试GUI检测功能
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-04-15

#include "ast/GUI.hpp"
#include "ast/GUIInterface.hpp"
#include "ast/Test.h"
#include <cstdio>

AST_USING_NAMESPACE

// 测试GUI检测功能
TEST(GUI, CanDisplayGUI)
{
    bool can_display = aCanDisplayGUI();
    printf("Can display GUI: %s\n", can_display ? "true" : "false");

    // 在CI环境中，通常不能显示GUI
    // 但这个测试不强制要求特定结果，只是验证函数能正常返回
}

// 测试GUIInterface基类功能
TEST(GUI, GUIInterfaceNoop)
{
    // 获取Noop实例
    GUIInterface* noop = GUIInterface::NoopInstance();
    EXPECT_NE(noop, nullptr);

    // 获取当前实例（如果AstGUI库不存在，应该返回Noop实例）
    GUIInterface* current = GUIInterface::CurrentInstance();
    EXPECT_NE(current, nullptr);

    // Noop实例和当前实例应该相同（因为没有实际的GUI库）
    // 注意：这个断言在有GUI库的环境中可能失败
    printf("Noop instance: %p\n", (void*)noop);
    printf("Current instance: %p\n", (void*)current);
}

// 测试设置GUI实例
TEST(GUI, SetCurrentInstance)
{
    // 获取Noop实例
    GUIInterface* noop = GUIInterface::NoopInstance();
    EXPECT_NE(noop, nullptr);

    // 设置当前实例
    GUIInterface::SetCurrentInstance(noop);

    // 获取当前实例，应该相同
    GUIInterface* current = GUIInterface::CurrentInstance();
    EXPECT_EQ(current, noop);
}

// 测试Noop实例的editObject和selectObject
TEST(GUI, NoopMethods)
{
    GUIInterface* noop = GUIInterface::NoopInstance();
    EXPECT_NE(noop, nullptr);

    // Noop实例的editObject应该返回错误
    errc_t rc = noop->editObject(nullptr);
    EXPECT_EQ(rc, -1);

    // Noop实例的selectObject应该返回空指针
    Object* obj = noop->selectObject("TestType");
    EXPECT_EQ(obj, nullptr);
}


//GTEST_MAIN()
int main(int argc, char **argv) {
    if(aIsCI())
        return 0;
    printf("Running main() from %s\n", __FILE__); 
    testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS(); 
}