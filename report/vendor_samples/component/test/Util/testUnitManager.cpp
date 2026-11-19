/// @file      testUnitManager.cpp
/// @brief     ~
/// @details   ~
/// @author    金柯<jinke18@foxmail.com>
/// @date      2025-12-15

#include "ast/AstTestMacro.h"
#include "ast/UnitManager.hpp"

AST_USING_NAMESPACE 

TEST(UnitManager, AddUnit)
{
    UnitManager unitManager;
}


TEST(UnitManager, GetUnit)
{
    UnitManager unitManager;
    auto unit = unitManager.getUnit("m");
    nothing();

    A_UNUSED(unit);
}





// 验证温度 SI 单位判定(应为开尔文而非摄氏度)与按量纲取单位的去重
TEST(UnitManager, TemperatureSiAndDimension)
{
    // 温度 SI 单位应为 K(纯乘法 scale=1)，而非摄氏度(仿射)
    auto si = aUnitGetSI(Dimension::Temperature());
    ASSERT_NE(si, nullptr);
    EXPECT_EQ(si->name(), "K");

    // K/°C/°F/°R 四种不同换算，去重后仍应保留 ≥4 个
    auto list = aUnitsGetByDimension(Dimension::Temperature());
    EXPECT_GE(list.size(), 4u);

    // 且应包含开尔文
    bool hasK = false;
    for (auto& u : list) {
        if (u.name() == "K") hasK = true;
    }
    EXPECT_TRUE(hasK);
}

GTEST_MAIN()