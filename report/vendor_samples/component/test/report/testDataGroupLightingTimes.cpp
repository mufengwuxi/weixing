/// @file      testDataGroupLightingTimes.cpp
/// @brief     光照时刻数据组测试
/// @details   构建 LEO 圆轨道二体星历，用 DataGroupLightingTimes 求解一天的全光照/半影/本影
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-08-18

#include "ast/Test.hpp"
#include "ast/DataGroupLightingTimes.hpp"
#include "ast/EphemerisTwoBody.hpp"
#include "ast/OrbitElement.hpp"
#include "ast/RunTime.hpp"
#include "ast/RunTimeSolarSystem.hpp"
#include "ast/EOP.hpp"
#include "ast/TimePoint.hpp"
#include "ast/TimeInterval.hpp"
#include "ast/Literals.hpp"

#include <algorithm>
#include <utility>
#include <vector>

AST_USING_NAMESPACE

using namespace ast::literals;

static TimeInterval DefaultInterval()
{
    return {
        TimePoint::FromUTC(2026, 8, 13, 4, 0, 0.0),
        TimePoint::FromUTC(2026, 8, 14, 4, 0, 0.0)
    };
}

/// @brief 构建 LEO 圆轨道二体星历（~300 km 高度，i=28.5°）
static SharedPtr<EphemerisTwoBody> MakeLeoEphemeris()
{
    auto earth = aGetEarth();
    auto eci   = earth->getFrameInertial();
    double gm   = earth->getGM();
    ModOrbElem elem{6678137, 0, 28.5_deg, 0, 0, 0};
    CartState initstate;
    aModOrbElemToCart(elem, gm, initstate.pos(), initstate.vel());
    auto epoch = "13 Aug 2026 04:00:00.000 UTCG"_utc;
    return EphemerisTwoBody::New(eci, epoch, initstate);
}

class DataGroupLightingTimesTest : public testing::Test
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

TEST_F(DataGroupLightingTimesTest, LeoOrbitOneDay)
{
    DataGroupLightingTimes dg;
    SharedPtr<EphemerisTwoBody> ephemeris = MakeLeoEphemeris();
    dg.setPoint(ephemeris);
    dg.setLightSource(aGetSun());
    dg.setOccultingBodies({aGetEarth()});
    dg.setStepSize(5.0);

    struct Case
    {
        ELightingType type;
        bool          expectObstruction;
    };
    const Case cases[] = {
        {ELightingType::eSunlight, false},
        {ELightingType::ePenumbra, true},
        {ELightingType::eUmbra,    true},
    };

    for (const auto& c : cases)
    {
        dg.setLightingType(c.type);

        VariantVector result;
        errc_t rc = dg.calculate(DefaultInterval(), result);
        ASSERT_EQ(rc, eNoError) << "type " << static_cast<int>(c.type);

        auto data = result.asSpan<DataGroupLightingTimes::Data>();
        ASSERT_GT(data.size(), 0u) << "expected at least one row for type " << static_cast<int>(c.type);

        for (size_t i = 0; i < data.size(); ++i)
        {
            const auto& row = data[i];
            // 时长守恒：duration == stopTime - startTime，且为正
            EXPECT_GT(row.getDuration(), 0.0) << "type " << static_cast<int>(c.type) << " row " << i;
            EXPECT_NEAR(row.getDuration(), row.getStopTime() - row.getStartTime(), 1e-6)
                << "type " << static_cast<int>(c.type) << " row " << i;

            // 遮挡体一致性：半影/本影应有遮挡体，全光照应无
            if (c.expectObstruction)
                EXPECT_FALSE(row.getObstruction().empty())
                    << "type " << static_cast<int>(c.type) << " row " << i;
            else
                EXPECT_TRUE(row.getObstruction().empty())
                    << "type " << static_cast<int>(c.type) << " row " << i;
        }
    }
}

TEST_F(DataGroupLightingTimesTest, LightingTiling)
{
    DataGroupLightingTimes dg;
    SharedPtr<EphemerisTwoBody> ephemeris = MakeLeoEphemeris();
    dg.setPoint(ephemeris);
    dg.setLightSource(aGetSun());
    dg.setOccultingBodies({aGetEarth()});
    dg.setStepSize(5.0);

    const TimeInterval interval = DefaultInterval();
    const TimePoint    start    = interval.start();

    // 收集三种光照段（sunlight / penumbra / umbra），转为相对区间起点的秒偏移
    std::vector<std::pair<double, double>> segs;
    for (ELightingType t : {ELightingType::eSunlight, ELightingType::ePenumbra, ELightingType::eUmbra})
    {
        dg.setLightingType(t);

        VariantVector result;
        ASSERT_EQ(dg.calculate(interval, result), eNoError) << "type " << static_cast<int>(t);

        auto data = result.asSpan<DataGroupLightingTimes::Data>();
        for (size_t i = 0; i < data.size(); ++i)
            segs.emplace_back(data[i].getStartTime() - start, data[i].getStopTime() - start);
    }

    ASSERT_FALSE(segs.empty());
    std::sort(segs.begin(), segs.end());

    // 三种光照段应无重叠、无缝隙地铺满整个区间
    const double kTol = 1e-3;
    EXPECT_NEAR(segs.front().first, 0.0, kTol);
    EXPECT_NEAR(segs.back().second, interval.duration(), kTol);
    for (size_t i = 1; i < segs.size(); ++i)
        EXPECT_NEAR(segs[i].first, segs[i - 1].second, kTol) << "gap/overlap at segment " << i;
}

GTEST_MAIN()
