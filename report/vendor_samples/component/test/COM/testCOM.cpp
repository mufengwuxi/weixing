/// @file      testVBScript.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-05-09
#include "ast/Test.hpp"

#if defined _WIN32 && !defined A_GCC && defined AST_WITH_COM
#include "ast/ScriptExecutor.hpp"
#include "ast/ScopedPtr.hpp"
#include "ast/COMAPI.hpp"
#include <comdef.h>
#include <comutil.h>

AST_USING_NAMESPACE


TEST(COMTest, ObjectRoot)
{
    auto root = aComObjectRoot();
    ASSERT_TRUE(root);
    BSTR memberName = SysAllocString(L"ExecuteCommand"); // 要查询的成员名
    DISPID dispid = DISPID_UNKNOWN;                      // 用于接收DISPID

    // 3. 调用 GetIDsOfNames
    HRESULT hr = root->GetIDsOfNames(
        IID_NULL,               // riid, 必须为 IID_NULL
        &memberName,            // rgszNames, 名称数组的地址
        1,                      // cNames, 名称数量为1
        LOCALE_SYSTEM_DEFAULT,  // lcid, 语言环境
        &dispid                 // rgDispId, 接收ID的数组
    );

    // 4. 检查结果并使用 DISPID
    EXPECT_TRUE(SUCCEEDED(hr));

    // 5. 释放资源
    SysFreeString(memberName);
}


TEST(COMTest, VBScript)
{
    #ifndef _WIN32
    GTEST_SKIP();
    #endif
    ScopedPtr<ScriptExecutor> executor = aNewScriptExecutor(EScriptLanguage::eVBScript);
    ASSERT_TRUE(executor);
    errc_t rc = executor->initialize();
    ASSERT_EQ(rc, eNoError);

    rc = executor->execute(R"(
        root.ExecuteCommand("echo hello world")
    )");
    ASSERT_EQ(rc, eNoError);

}

#endif

GTEST_MAIN();

