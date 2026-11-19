/// @file      testSunSynchronousOrbit.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-05-08


#include "ast/AstTestMacro.h"
#include "ast/SunSynchronousOrbitDesigner.hpp"
#include "ast/CelestialBody.hpp"
#include "ast/RunTime.hpp"
#include "ast/Literals.hpp"

AST_USING_NAMESPACE
using namespace _AST literals;

TEST(SunSynchronousOrbit, DesignByAltitude)
{
    aInitialize();

    auto earth = aGetEarth();
    earth->setGravityModel("WGS84");

    SunSynchronousOrbitDesigner designer(earth);

    printf("=== 太阳同步轨道设计 - 按高度设计 ===\n");

    double altitudes[] = {400_km, 500_km, 600_km, 800_km, 1000_km};

    for (double alt : altitudes)
    {
        designer.setGeometryDefinition(SunSynchronousOrbitDesigner::eAltitude);
        designer.setAltitude(alt);

        ModOrbElem orbElem;
        errc_t rc = designer.getOrbitState(orbElem);
        ASSERT_EQ(rc, eNoError);

        double inc = designer.getInclination();

        printf("\n高度: %.0f km\n", alt / 1000.0);
        printf("半长轴: %.6f km\n", orbElem.getA() / 1000.0);
        printf("偏心率: %.12f\n", orbElem.getE());
        printf("倾角: %.10f deg\n", inc * 180.0 / kPI);
        printf("轨道周期: %.6f 分钟\n", orbElem.getPeriod(earth->getGM()) / 60.0);
    }
}

TEST(SunSynchronousOrbit, DesignByInclination)
{
    aInitialize();

    auto earth = aGetEarth();
    earth->setGravityModel("WGS84");

    SunSynchronousOrbitDesigner designer(earth);

    printf("\n=== 太阳同步轨道设计 - 按倾角设计 ===\n");

    double inclinations[] = {97.0_deg, 100.0_deg, 110.0_deg, 120.0_deg};

    for (double inc : inclinations)
    {
        designer.setGeometryDefinition(SunSynchronousOrbitDesigner::eInclination);
        designer.setInclination(inc);

        ModOrbElem orbElem;
        errc_t rc = designer.getOrbitState(orbElem);
        ASSERT_EQ(rc, eNoError);

        double alt = designer.getAltitude();

        printf("\n倾角: %.4f deg\n", inc * 180.0 / kPI);
        printf("高度: %.6f km\n", alt / 1000.0);
        printf("半长轴: %.6f km\n", orbElem.getA() / 1000.0);
        printf("偏心率: %.12f\n", orbElem.getE());
        printf("轨道周期: %.6f 分钟\n", orbElem.getPeriod(earth->getGM()) / 60.0);
    }
}

TEST(SunSynchronousOrbit, Verification)
{
    aInitialize();

    auto earth = aGetEarth();
    earth->setGravityModel("WGS84");

    SunSynchronousOrbitDesigner designer(earth);

    printf("\n=== 太阳同步轨道验证 ===\n");

    double testAlt = 700_km;
    designer.setAltitude(testAlt);

    double calcInc = designer.getInclination();
    designer.setInclination(calcInc);
    double calcAlt = designer.getAltitude();

    printf("\n原始高度: %.6f km\n", testAlt / 1000.0);
    printf("计算倾角: %.10f deg\n", calcInc * 180.0 / kPI);
    printf("由倾角反算高度: %.6f km\n", calcAlt / 1000.0);
    printf("误差: %.6f m\n", (calcAlt - testAlt));

    EXPECT_NEAR(calcAlt, testAlt, 1e-3);
}

TEST(SunSynchronousOrbit, CommonSunSyncOrbits)
{
    aInitialize();

    auto earth = aGetEarth();
    earth->setGravityModel("WGS84");

    SunSynchronousOrbitDesigner designer(earth);

    printf("\n=== 常见太阳同步轨道参数 ===\n");

    struct {
        const char* name;
        double altitude;
    } commonOrbits[] = {
        {"Landsat 7", 705_km},
        {"Sentinel-2A/B", 786_km},
        {"MODIS/Terra", 705_km},
        {"Aqua", 705_km},
        {"IKONOS", 681_km},
        {"QuickBird", 450_km},
        {"WorldView-2", 770_km},
        {"SPOT-5", 822_km}
    };

    for (auto& orbit : commonOrbits)
    {
        designer.setAltitude(orbit.altitude);
        double inc = designer.getInclination();

        printf("\n%s:\n", orbit.name);
        printf("  高度: %.0f km\n", orbit.altitude / 1000.0);
        printf("  倾角: %.6f deg\n", inc * 180.0 / kPI);
    }
}

GTEST_MAIN()