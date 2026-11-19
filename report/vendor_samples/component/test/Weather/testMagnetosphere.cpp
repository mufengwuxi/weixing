/// @file      testMagnetosphere.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-02-15


#include "ast/Magnetosphere.hpp"
#include "ast/Test.h"

AST_USING_NAMESPACE

TEST(Magnetosphere, igrf13)
{
    if(!aIsGithubCI()) GTEST_SKIP();

    printf("igrf13syn test\n");
    {
        double date = 2010.5260273972603;
        double alt = 0.0;
        double lat = 65;
        double elong = 85;
        double colat = 90 - lat;
        double x = 0.0;
        double y = 0.0;
        double z = 0.0;
        double f = 0.0;
        int ret = igrf13syn(0, date, 1, alt, colat, elong, x, y, z, f);
        printf("ret = %d\n", ret);
        printf("x = %f, y = %f, z = %f, f = %f\n", x, y, z, f);
        EXPECT_NEAR(x, 9295.415328, 0.001);
        EXPECT_NEAR(y, 2559.788960, 0.001);
        EXPECT_NEAR(z, 59670.378618, 0.001);
        EXPECT_NEAR(f, 60444.283022, 0.001);
    }

    {
        double date = 2010;
        double alt = 0.0;
        double lat = 65;
        double elong = 85;
        double colat = 90 - lat;
        double x = 0.0;
        double y = 0.0;
        double z = 0.0;
        double f = 0.0;
        int ret = igrf13syn(0, date, 1, alt, colat, elong, x, y, z, f);
        printf("ret = %d\n", ret);
        printf("x = %f, y = %f, z = %f, f = %f\n", x, y, z, f);
        EXPECT_NEAR(x, 9315.267451, 0.001);
        EXPECT_NEAR(y, 2568.004564, 0.001);
        EXPECT_NEAR(z, 59659.366911, 0.001);
        EXPECT_NEAR(f, 60436.817548, 0.001);
    }
}


TEST(Magnetosphere, aep8)
{
    if(!aIsGithubCI()) GTEST_SKIP();

    double flux;
    {
        double e = 20;
        double l = 1.413809061050415;
        double bb0 = 1.785343885421753;

        flux = aep8(e, l, bb0, 1);
        printf("flux = %.15g\n", flux);
        EXPECT_EQ(flux, 0);

        flux = aep8(e, l, bb0, 2);
        printf("flux = %.15g\n", flux);
        EXPECT_EQ(flux, 0);

        flux = aep8(e, l, bb0, 3);
        printf("flux = %.15g\n", flux);
        EXPECT_NEAR(flux, 4018.36108398, 0.002);

        flux = aep8(e, l, bb0, 4);
        printf("flux = %.15g\n", flux);
        EXPECT_NEAR(flux, 2642.50370051986, 0.002);
    }
    {
        double e = 1;
        double l = 1.413809061050415;
        double bb0 = 1.785343885421753;

        flux = aep8(e, l, bb0, 1);
        printf("flux = %.15g\n", flux);
        EXPECT_NEAR(flux, 36796.8515625, 0.01);

        flux = aep8(e, l, bb0, 2);
        printf("flux = %.15g\n", flux);
        EXPECT_NEAR(flux, 36796.8515625, 0.01);

        flux = aep8(e, l, bb0, 3);
        printf("flux = %.15g\n", flux);
        EXPECT_NEAR(flux, 5329.69628906, 0.002);

        flux = aep8(e, l, bb0, 4);
        printf("flux = %.15g\n", flux);
        EXPECT_NEAR(flux, 3400.19946289, 0.002);
    }
}


TEST(Magnetosphere, igrf)
{
    if(!aIsGithubCI()) GTEST_SKIP();

    {
        double lon = 85;
        double lat = 65;
        double height = 0;
        double year = 2010.5260273972603;
        double xl, bbx;
        igrf(lon, lat, height, year, xl, bbx);
        printf("xl = %f, bbx = %f\n", xl, bbx);
        EXPECT_NEAR(xl, 4.15171957, 0.00001);
        EXPECT_NEAR(bbx, 145.20530701, 0.0001);
    }
    {
        double lon = -45;
        double lat = -30;
        double height = 500;
        double year = 2021.1616438356164;
        double xl, bbx;
        igrf(lon, lat, height, year, xl, bbx);
        printf("xl = %f, bbx = %f\n", xl, bbx);
        EXPECT_NEAR(xl, 1.41380906, 0.00001);
        EXPECT_NEAR(bbx, 1.78534389, 0.0001);
    }
}


TEST(Magnetosphere, get_flux)
{
    if(!aIsGithubCI()) GTEST_SKIP();

    {
        double lon = -45;
        double lat = -30;
        double height = 500;
        double energy = 20;
        double year = 2021.1616438356164;
        int iname = 4; // pmax
        double flux = get_flux(lon, lat, height, energy, year, iname);
        printf("flux = %.15g\n", flux);
        EXPECT_NEAR(flux, 2642.50268555, 0.002);
    }
}

GTEST_MAIN()

