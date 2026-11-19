/// @file      testAtmosphere.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-10-13

#include "ast/Test.hpp"
#include "ast/MSIS86.hpp"
#include "ast/MSISE90.hpp"
#include "ast/NRLMSIS00.hpp"
#include "ast/JacchiaRoberts.hpp"
#include "ast/RunTime.hpp"
#include "ast/CelestialBody.hpp"
#include "ast/Literals.hpp"
#include "ast/GeodeticPoint.hpp"
#include "ast/IO.hpp"

AST_USING_NAMESPACE

TEST(AtmosphereTest, MSIS86)
{
    auto earth = aGetEarth();
    auto shape = earth->getShape();
    MSIS86 msis86(earth->getFrameFixed(), shape, 150, 150, 15);
    auto tp = "2026-10-13 00:00:00"_utc;

    auto getDensity = [&msis86, shape, tp](double alt) -> double
    {
        GeodeticPoint lla(0, 0, alt);
        Vector3d pos;
        shape->transform(lla, pos);
        double density = msis86.getDensity(tp, pos);
        return density;
    };

    // MSIS 86 的有效高度范围是 [85 km, 1000 km]
    posix::printf("-----------------\n");
    posix::printf("测试高度在 85 km 到 1000 km 之间的的大气密度\n");
    for(double alt = 85_km; alt <= 1000_km; alt += 10_km)
    {
        double density = getDensity(alt);
        posix::printf("高度: %lf km, 大气密度: %g kg/m^3\n", alt/1_km, density);
        EXPECT_TRUE(!std::isnan(density));
    }

    // 测试高度小于 85 km 时的大气密度
    posix::printf("-----------------\n");
    posix::printf("测试高度小于 85 km 时的大气密度\n");
    for(double alt = 0_km; alt < 85_km; alt += 1_km)
    {
        double density = getDensity(alt);
        posix::printf("高度: %lf km, 大气密度: %g kg/m^3\n", alt/1_km, density);
        EXPECT_TRUE(std::isnan(density));
    }

    posix::printf("-----------------\n");
    posix::printf("测试高度 < 0 km 时的大气密度\n");
    for(double alt = 0_km; alt >= -10000_km; alt -= 1000_km)
    {
        double density = getDensity(alt);
        posix::printf("高度: %lf km, 大气密度: %g kg/m^3\n", alt/1_km, density);
        EXPECT_TRUE(std::isnan(density));
    }

    // 测试高度大于 1000 km 时的大气密度
    posix::printf("-----------------\n");
    posix::printf("测试高度大于 1000 km 时的大气密度\n");
    for(double alt=1000_km; alt<=100000_km; alt+=1000_km)
    {
        double density = getDensity(alt);
        posix::printf("高度: %lf km, 大气密度: %g kg/m^3\n", alt/1_km, density);
        EXPECT_TRUE(density < 1e-14);
    }
}

TEST(AtmosphereTest, MSISE90)
{
    auto earth = aGetEarth();
    auto shape = earth->getShape();
    MSISE90 msise90(earth->getFrameFixed(), shape, 150, 150, 15);
    auto tp = "2026-10-13 00:00:00"_utc;

    auto getDensity = [&msise90, shape, tp](double alt) -> double
    {
        GeodeticPoint lla(0, 0, alt);
        Vector3d pos;
        shape->transform(lla, pos);
        double density = msise90.getDensity(tp, pos);
        return density;
    };

    // MSISE 90 的有效高度范围是 [0 km, 1000 km]
    posix::printf("-----------------\n");
    posix::printf("测试高度在 0 km 到 1000 km 之间的的大气密度\n");
    for(double alt = 0_km; alt <= 1000_km; alt += 10_km)
    {
        double density = getDensity(alt);
        posix::printf("高度: %lf km, 大气密度: %g kg/m^3\n", alt/1_km, density);
        EXPECT_TRUE(!std::isnan(density));
    }

    // 测试高度小于 0 km 时的大气密度
    posix::printf("-----------------\n");
    posix::printf("测试高度小于 0 km 时的大气密度\n");
    for(double alt = -1_m; alt >= -10000_km; alt -= 1000_km)
    {
        double density = getDensity(alt);
        posix::printf("高度: %lf km, 大气密度: %g kg/m^3\n", alt/1_km, density);
        EXPECT_TRUE(std::isnan(density));
    }

    posix::printf("-----------------\n");
    posix::printf("测试高度大于 1000 km 时的大气密度\n");
    for(double alt = 1000_km; alt < 100000_km; alt += 1000_km)
    {
        double density = getDensity(alt);
        posix::printf("高度: %lf km, 大气密度: %g kg/m^3\n", alt/1_km, density);
        EXPECT_TRUE(!std::isnan(density));
        EXPECT_TRUE(density < 1e-14);
    }
}

TEST(AtmosphereTest, NRLMSIS00)
{
    auto earth = aGetEarth();
    auto shape = earth->getShape();
    NRLMSIS00 nrlmsis00(earth->getFrameFixed(), shape, 150, 150, 15);
    auto tp = "2026-10-13 00:00:00"_utc;

    auto getDensity = [&nrlmsis00, shape, tp](double alt) -> double
    {
        GeodeticPoint lla(0, 0, alt);
        Vector3d pos;
        shape->transform(lla, pos);
        double density = nrlmsis00.getDensity(tp, pos);
        return density;
    };

    // NRLMSIS 00 的有效高度范围是 [0 km, 1000 km]
    posix::printf("-----------------\n");
    posix::printf("测试高度在 0 km 到 1000 km 之间的的大气密度\n");
    for(double alt = 0_km; alt <= 1000_km; alt += 10_km)
    {
        double density = getDensity(alt);
        posix::printf("高度: %lf km, 大气密度: %g kg/m^3\n", alt/1_km, density);
        EXPECT_TRUE(!std::isnan(density));
    }

    // 测试高度小于 0 km 时的大气密度
    posix::printf("-----------------\n");
    posix::printf("测试高度小于 0 km 时的大气密度\n");
    for(double alt = -1_m; alt >= -10000_km; alt -= 1000_km)
    {
        double density = getDensity(alt);
        posix::printf("高度: %lf km, 大气密度: %g kg/m^3\n", alt/1_km, density);
        EXPECT_TRUE(std::isnan(density));
    }


    posix::printf("-----------------\n");
    posix::printf("测试高度大于 1000 km 时的大气密度\n");
    for(double alt = 1000_km; alt < 100000_km; alt += 1000_km)
    {
        double density = getDensity(alt);
        posix::printf("高度: %lf km, 大气密度: %g kg/m^3\n", alt/1_km, density);
        EXPECT_TRUE(!std::isnan(density));
        EXPECT_TRUE(density < 1e-14);
    }

}


TEST(AtmosphereTest, JacchiaRoberts)
{
    auto earth = aGetEarth();
    auto sun = aGetSun();
    auto shape = earth->getShape();
    JacchiaRoberts jacchiaRoberts(earth->getFrameFixed(), shape, sun, 150, 150, 3);
    auto tp = "2026-10-13 00:00:00"_utc;
    auto getDensity = [&jacchiaRoberts, shape, tp](double alt) -> double
    {
        GeodeticPoint lla(0, 0, alt);
        Vector3d pos;
        shape->transform(lla, pos);
        double density = jacchiaRoberts.getDensity(tp, pos);
        return density;
    };

    // Jacchia-Roberts 的有效高度范围是 [90 km, 2500 km]
    // 小于 90 km 时返回 修正系数 * 3.46e-9 g/cm^2, 大于 2500 km 时返回 0 

    posix::printf("-----------------\n");
    for(double alt = 90_km; alt <= 2500_km; alt += 10_km)
    {
        double density = getDensity(alt);
        posix::printf("高度: %lf km, 大气密度: %g kg/m^3\n", alt/1_km, density);
        EXPECT_TRUE(!std::isnan(density));
    }

    posix::printf("-----------------\n");
    for(double alt = 0_km; alt < 90_km; alt += 1_km)
    {
        double density = getDensity(alt);
        posix::printf("高度: %lf km, 大气密度: %g kg/m^3\n", alt/1_km, density);
        EXPECT_TRUE(!std::isnan(density));
        EXPECT_NEAR(density, 3.46e-9_g/cm3, 3.46e-9_g/cm3 * 0.2);
    }

    posix::printf("-----------------\n");
    for(double alt=-1_m; alt >= -10000_km; alt -= 1000_km)
    {
        double density = getDensity(alt);
        posix::printf("高度: %lf km, 大气密度: %g kg/m^3\n", alt/1_km, density);
        EXPECT_TRUE(std::isnan(density));
    }

    posix::printf("-----------------\n");
    for(double alt = 2501_km; alt <= 100000_km; alt += 1000_km)
    {
        double density = getDensity(alt);
        posix::printf("高度: %lf km, 大气密度: %g kg/m^3\n", alt/1_km, density);
        EXPECT_TRUE(!std::isnan(density));
        EXPECT_EQ(density, 0);
    }
}

GTEST_MAIN()