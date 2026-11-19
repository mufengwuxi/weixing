/// @file      testXYS.cpp
/// @brief     ~
/// @details   ~
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-01-22

#include "ast/IAUXYS.hpp"
#include "ast/IAUXYSPrecomputed.hpp"
#include "ast/RunTime.hpp"
#include "ast/TimePoint.hpp"
#include "ast/JulianDate.hpp"
#include "ast/MathOperator.hpp"
#include "ast/Literals.hpp"
#include "ast/Environment.hpp"
#include "ast/Test.hpp"

AST_USING_NAMESPACE

TEST(IAUXYSPrecomputed, load)
{
    IAUXYSPrecomputed xys;
    std::string datadir = aDataDirGet();
    errc_t ret = xys.load(aTestGetConfigValue("IAUXYS_PRECOMPUTED_FILE").toString());
    EXPECT_EQ(ret, 0);
}



TEST(IAUXYSPrecomputed, getValue)
{
    errc_t ret = 0;
    aInitialize();
    IAUXYSPrecomputed xysData;
    std::string datadir = aDataDirGet();
    ret = xysData.load(aTestGetConfigValue("IAUXYS_PRECOMPUTED_FILE").toString());
    EXPECT_EQ(ret, 0);
    double jd;
    {
        jd = xysData.getRefEpochJED();
        JulianDate jdTT = JulianDate::FromImpreciseDay(jd);
        array3d xys_tt;
        ret = xysData.getValueTT(jdTT, xys_tt);
        EXPECT_EQ(ret, 0);
        printf("jd: %lf xys_tt: %.15lf %.15lf %.15lf\n", jd, xys_tt[0], xys_tt[1], xys_tt[2]);
    }
    {
        jd = xysData.getRefEpochJED() + xysData.getStepSize()*(xysData.getNumPoints() - 1);
        JulianDate jdTT = JulianDate::FromImpreciseDay(jd);
        array3d xys_tt;
        ret = xysData.getValueTT(jdTT, xys_tt);
        EXPECT_EQ(ret, 0);
        printf("jd: %lf xys_tt: %.15lf %.15lf %.15lf\n", jd, xys_tt[0], xys_tt[1], xys_tt[2]);
    }
    {
        jd = xysData.getRefEpochJED() + 365 * 12.34567891011;
        JulianDate jdTT = JulianDate::FromImpreciseDay(jd);
        array3d xys_tt;
        ret = xysData.getValueTT(jdTT, xys_tt);
        EXPECT_EQ(ret, 0);
        printf("jd: %lf xys_tt: %.15lf %.15lf %.15lf\n", jd, xys_tt[0], xys_tt[1], xys_tt[2]);
    }
    // test for errors
    {
        jd = xysData.getRefEpochJED() - 1.0;
        JulianDate jdTT = JulianDate::FromImpreciseDay(jd);
        array3d xys_tt;
        ret = xysData.getValueTT(jdTT, xys_tt);
        EXPECT_NE(ret, 0);
    }
    {
        jd = xysData.getRefEpochJED() + xysData.getStepSize()*xysData.getNumPoints();
        JulianDate jdTT = JulianDate::FromImpreciseDay(jd);
        array3d xys_tt;
        ret = xysData.getValueTT(jdTT, xys_tt);
        EXPECT_NE(ret, 0);
    }
    {
        jd = xysData.getRefEpochJED() - 1e-9;
        JulianDate jdTT = JulianDate::FromImpreciseDay(jd);
        array3d xys_tt;
        ret = xysData.getValueTT(jdTT, xys_tt);
        EXPECT_NE(ret, 0);
    }
    {
        jd = xysData.getRefEpochJED() + xysData.getStepSize()*(xysData.getNumPoints() - 1);
        jd += 1e-9;
        JulianDate jdTT = JulianDate::FromImpreciseDay(jd);
        array3d xys_tt;
        ret = xysData.getValueTT(jdTT, xys_tt);
        EXPECT_NE(ret, 0);
    }
}

using namespace _AST math;
using namespace _AST literals;

void testEps()
{
    {
        auto xysData = aDataContext_GetIAUXYSPrecomputed();
        EXPECT_NE(xysData, nullptr);
        double refepoch_jed = xysData->getRefEpochJED();
        A_UNUSED(refepoch_jed);
        array3d xys_precomputed;
        array3d xys_iers2010;
        TimePoint tp = TimePoint::FromUTC(2000, 1, 1, 0, 0, 0);
        for(int i = 0; i < 1000; i++){
            tp = tp + 10_day;
            aTheoreticalXYS_IERS2010(tp, xys_iers2010);
            errc_t ret = aTheoreticalXYS_IERS2010Precomputed(tp, xys_precomputed);
            EXPECT_EQ(ret, 0);
            auto delta = xys_precomputed - xys_iers2010;
            // printf("xys_precompu: %.15lf %.15lf %.15lf\n", xys_precomputed[0], xys_precomputed[1], xys_precomputed[2]);
            // printf("xys_iers2010: %.15lf %.15lf %.15lf\n", xys_iers2010[0], xys_iers2010[1], xys_iers2010[2]);
            // printf("delta: %g %g %g\n", delta[0], delta[1], delta[2]);
            EXPECT_LT(abs(delta[0]), 1.6e-6_arcsec);
            EXPECT_LT(abs(delta[1]), 2.4e-6_arcsec);
            EXPECT_LT(abs(delta[2]), 1.2e-6_arcsec);
        }
    }
}


TEST(IAUXYSPrecomputed, eps)
{
    aInitialize();
    testEps();
    aUninitialize();
}

TEST(IAUXYSPrecomputed, precompute)
{
    if(!aIsCI()){
        GTEST_SKIP();
    }
    aInitialize();
    {    
        auto xys = aDataContext_GetIAUXYSPrecomputed();
        EXPECT_NE(xys, nullptr);
        double start = 2451545.0;
        double duration = 27426.0;
        xys->precompute(start, start + duration, 1.0);
        testEps();
    }
    aUninitialize();
}


GTEST_MAIN();
