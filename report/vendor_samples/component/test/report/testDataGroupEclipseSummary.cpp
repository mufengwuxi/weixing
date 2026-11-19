/// @file      testDataGroupEclipseSummary.cpp
/// @brief     日食摘要数据组测试
/// @details   构建 LEO 圆轨道二体星历，用 DataGroupEclipseSummary 求解一天的日食摘要，
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-08-17

#include "ast/Test.hpp"
#include "ast/DataGroupEclipseSummary.hpp"
#include "ast/EphemerisTwoBody.hpp"
#include "ast/OrbitElement.hpp"
#include "ast/RunTime.hpp"
#include "ast/RunTimeSolarSystem.hpp"
#include "ast/EOP.hpp"
#include "ast/TimePoint.hpp"
#include "ast/TimeInterval.hpp"
#include "ast/Literals.hpp"

AST_USING_NAMESPACE

using namespace ast::literals;

static TimeInterval DefaultInterval()
{
    return {
        TimePoint::FromUTC(2026, 8, 13, 4, 0, 0.0),
        TimePoint::FromUTC(2026, 8, 14, 4, 0, 0.0)
    };
}

class DataGroupEclipseSummaryTest : public testing::Test
{
public:
    void SetUp() override
    {
        aInitialize();
        aDataContext_GetEOP()->unload();
    }
    void TearDown() override
    {
        aUninitialize();
    }
};

TEST_F(DataGroupEclipseSummaryTest, LeoOrbitOneDay)
{
    // 构建 LEO 圆轨道二体星历（~300 km 高度，i=28.5°）
    auto earth = aGetEarth();
    auto eci = earth->getFrameInertial();
    double gm = earth->getGM();
    ModOrbElem elem{6678137, 0, 28.5_deg, 0, 0, 0};
    CartState initstate;
    aModOrbElemToCart(elem, gm, initstate.pos(), initstate.vel());
    auto epoch = "13 Aug 2026 04:00:00.000 UTCG"_utc;
    SharedPtr<EphemerisTwoBody> ephemeris = EphemerisTwoBody::New(eci, epoch, initstate);

    ScopedPtr<DataGroupEclipseSummary> summary = new DataGroupEclipseSummary();
    summary->setPoint(ephemeris);
    summary->setLightSource(aGetSun());
    summary->setOccultingBodies({aGetEarth()});
    summary->setStepSize(5.0);

    VariantVector result;
    errc_t rc = summary->calculate(DefaultInterval(), result);
    EXPECT_EQ(rc, eNoError);

    auto data = result.asSpan<DataGroupEclipseSummary::Data>();
    ASSERT_GT(data.size(), 0u) << "expected at least one eclipse event";

    for (size_t i = 0; i < data.size(); ++i)
    {
        const auto& ev = data[i];
        // 边界顺序：半影进入 <= 本影进入 <= 本影退出 <= 半影退出
        EXPECT_LE(ev.getPenumbraStart() - ev.getUmbraStart(), 0.0) << "row " << i;
        EXPECT_LE(ev.getUmbraStart()    - ev.getUmbraStop(),  0.0) << "row " << i;
        EXPECT_LE(ev.getUmbraStop()     - ev.getPenumbraStop(), 0.0) << "row " << i;

        // 遮蔽比例 ∈ [0,1]，最小强度 = 1 - 最大遮蔽比例
        EXPECT_GE(ev.getMaxShadowRatio(), 0.0) << "row " << i;
        EXPECT_LE(ev.getMaxShadowRatio(), 1.0) << "row " << i;
        EXPECT_NEAR(ev.getMinIntensity(), 1.0 - ev.getMaxShadowRatio(), 1e-12) << "row " << i;

        // 时长守恒：总时长 = 本影时长 + 半影时长
        EXPECT_NEAR(ev.getTotalDuration(), ev.getUmbraDuration() + ev.getPenumbraDuration(), 1e-6) << "row " << i;
    }
}

GTEST_MAIN()
