/// @file      testGravityCalculator.cpp
/// @brief     ~
/// @details   ~
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-01-15

#include "ast/GravityCalculator.hpp"
#include "ast/RunTime.hpp"
#include "ast/String.hpp"
#include "ast/Test.h"
#include "ast/Vector.hpp"



AST_USING_NAMESPACE 

TEST(GravityCalculator, WGS84)
{
    GravityField gf;
    errc_t err = gf.load(aTestGetConfigValue("SOLARSYSTEM_DIR").toString() + "/Earth/WGS84.grv");
    EXPECT_FALSE(err);
    // test pert acceleration
    if(1)
    {
        GravityCalculatorDefault gc(gf, 2, 0);
        {
            Vector3d r{30000e3, 20000e3, 10e3}, a;
            gc.calcPertAcceleration(r, a);
            printf("a = %g, %g, %g\n", a(0), a(1), a(2));
            EXPECT_NEAR(a[0], -1.2964608245266108e-05, 1e-20);
            EXPECT_NEAR(a[1], -8.6430721635107374e-06, 1e-20);
            EXPECT_NEAR(a[2], -1.2964611569525655e-08, 1e-20);
        }
        {
            Vector3d r{20000e3, 30000e3, 40000e3}, a;
            gc.calcPertAcceleration(r, a);
            printf("a = %g, %g, %g\n", a(0), a(1), a(2));
            EXPECT_NEAR(a[0], 2.0450501091438087e-06, 1e-20);
            EXPECT_NEAR(a[1], 3.0675751637157131e-06, 1e-20);
            EXPECT_NEAR(a[2], -5.6138630447084890e-07, 1e-20);
        }
    }
    if(1)
    {
        GravityCalculatorDefault gc(gf, 30, 12);
        {
            Vector3d r{30000e3, 20000e3, 10e3}, a;
            gc.calcPertAcceleration(r, a);
            printf("a = %g, %g, %g\n", a(0), a(1), a(2));
            EXPECT_NEAR(a[0], -1.2884493339353285e-05, 1e-20);
            EXPECT_NEAR(a[1], -8.7236120632445672e-06, 1e-20);
            EXPECT_NEAR(a[2], -2.0402094409548659e-08, 1e-20);
        }
        {
            Vector3d r{20000e3, 30000e3, 40000e3}, a;
            gc.calcPertAcceleration(r, a);
            printf("a = %g, %g, %g\n", a(0), a(1), a(2));
            EXPECT_NEAR(a[0], 2.0540839571721316e-06, 1e-20);
            EXPECT_NEAR(a[1], 3.0606136485786208e-06, 1e-20);
            EXPECT_NEAR(a[2], -5.4589503678110374e-07, 1e-20);
        }
    }
    // test total acceleration
    if(1)
    {
        GravityCalculatorDefault gc(gf, 20, 20);
        {
            Vector3d r{ 1000e3, 2000e3, 3000e3 }, a;
            gc.calcTotalAcceleration(r, a);
            printf("a = %g, %g, %g\n", a(0), a(1), a(2));
            EXPECT_NEAR(a[0], -6.3464397029805024, 1e-14);
            EXPECT_NEAR(a[1], -18.089020841843208, 1e-14);
            EXPECT_NEAR(a[2], -27.137832131016708, 1e-14);
        }
        {
            Vector3d r{ 8000e3, 0.0, 0.0 }, a;
            gc.calcTotalAcceleration(r, a);
            printf("a = %g, %g, %g\n", a(0), a(1), a(2));
            EXPECT_NEAR(a[0], -6.2346046893759057, 1e-15);
            EXPECT_NEAR(a[1], -9.2522379702784185e-06, 1e-15);
            EXPECT_NEAR(a[2], 1.4847004365950058e-05, 1e-15);
        }
        {
            Vector3d r{30000e3, 20000e3, 10e3}, a;
            gc.calcTotalAcceleration(r, a);
            printf("a = %g, %g, %g\n", a(0), a(1), a(2));
            EXPECT_NEAR(a[0], -0.25513255884328773, 1e-15);
            EXPECT_NEAR(a[1], -0.17008850651202864, 1e-15);
            EXPECT_NEAR(a[2], -8.5060293544385971e-05, 1e-15);
        }
    }

    // test calcPertAcceleration
    if(1)
    {
        int degree = 2;
        int order = 2;
        GravityCalculatorDefault gc1(gf, degree, order);
        GravityCalculator2 gc2(gf, degree, order);
        GravityCalculator3 gc3(gf, degree, order);
        Vector3d r{ 30000e3, 20000e3, 10e3 };
        {
            Vector3d pertAccel1;
            Vector3d pertAccel2;
            Vector3d pertAccel3;

            gc1.calcPertAcceleration(r, pertAccel1);
            gc2.calcPertAcceleration(r, pertAccel2);
            gc3.calcPertAcceleration(r, pertAccel3);

            printf("pert accel1 = %g, %g, %g\n", pertAccel1(0), pertAccel1(1), pertAccel1(2));
            printf("pert accel2 = %g, %g, %g\n", pertAccel2(0), pertAccel2(1), pertAccel2(2));
            printf("pert accel3 = %g, %g, %g\n", pertAccel3(0), pertAccel3(1), pertAccel3(2));

            EXPECT_NEAR(pertAccel1(0), pertAccel2(0), 1e-20);
            EXPECT_NEAR(pertAccel1(1), pertAccel2(1), 1e-20);
            EXPECT_NEAR(pertAccel1(2), pertAccel2(2), 1e-20);

            EXPECT_NEAR(pertAccel1(0), pertAccel3(0), 1e-20);
            EXPECT_NEAR(pertAccel1(1), pertAccel3(1), 1e-20);
            EXPECT_NEAR(pertAccel1(2), pertAccel3(2), 1e-20);
        }
    }

    {
        int degree = 20;
        int order = 20;
        GravityCalculatorDefault gc1(gf, degree, order);
        GravityCalculator2 gc2(gf, degree, order);
        GravityCalculator3 gc3(gf, degree, order);
        Vector3d r{ 30000e3, 20000e3, 10e3 };
        {
            Vector3d pertAccel1;
            Vector3d pertAccel2;
            Vector3d pertAccel3;

            gc1.calcPertAcceleration(r, pertAccel1);
            gc2.calcPertAcceleration(r, pertAccel2);
            gc3.calcPertAcceleration(r, pertAccel3);

            printf("pert accel1 = %g, %g, %g\n", pertAccel1(0), pertAccel1(1), pertAccel1(2));
            printf("pert accel2 = %g, %g, %g\n", pertAccel2(0), pertAccel2(1), pertAccel2(2));
            printf("pert accel3 = %g, %g, %g\n", pertAccel3(0), pertAccel3(1), pertAccel3(2));

            EXPECT_NEAR(pertAccel1(0), pertAccel2(0), 1e-20);
            EXPECT_NEAR(pertAccel1(1), pertAccel2(1), 1e-20);
            EXPECT_NEAR(pertAccel1(2), pertAccel2(2), 1e-20);
            EXPECT_NEAR(pertAccel1(0), pertAccel3(0), 1e-19);
            EXPECT_NEAR(pertAccel1(1), pertAccel3(1), 1e-20);
            EXPECT_NEAR(pertAccel1(2), pertAccel3(2), 1e-20);
        }
    }

    {
        int degree = 30;
        int order = 10;
        GravityCalculatorDefault gc1(gf, degree, order);
        GravityCalculator2 gc2(gf, degree, order);
        GravityCalculator3 gc3(gf, degree, order);
        Vector3d r{ 20000e3, 30000e3, 40000e3 };
        {
            Vector3d pertAccel1;
            Vector3d pertAccel2;
            Vector3d pertAccel3;
            gc1.calcPertAcceleration(r, pertAccel1);
            gc2.calcPertAcceleration(r, pertAccel2);
            gc3.calcPertAcceleration(r, pertAccel3);

            printf("pert accel1 = %g, %g, %g\n", pertAccel1(0), pertAccel1(1), pertAccel1(2));
            printf("pert accel2 = %g, %g, %g\n", pertAccel2(0), pertAccel2(1), pertAccel2(2));
            printf("pert accel3 = %g, %g, %g\n", pertAccel3(0), pertAccel3(1), pertAccel3(2));

            EXPECT_NEAR(pertAccel1(0), pertAccel2(0), 1e-20);
            EXPECT_NEAR(pertAccel1(1), pertAccel2(1), 1e-20);
            EXPECT_NEAR(pertAccel1(2), pertAccel2(2), 1e-20);
            EXPECT_NEAR(pertAccel1(0), pertAccel3(0), 1e-20);
            EXPECT_NEAR(pertAccel1(1), pertAccel3(1), 1e-20);
            EXPECT_NEAR(pertAccel1(2), pertAccel3(2), 1e-20);
        }
    }
    {
        int degree = 60;
        int order = 50;
        GravityCalculatorDefault gc1(gf, degree, order);
        GravityCalculator2 gc2(gf, degree, order);
        GravityCalculator3 gc3(gf, degree, order);
        Vector3d r{ 40000e3, 30000e3, 20000e3 };
        {
            Vector3d pertAccel1;
            Vector3d pertAccel2;
            Vector3d pertAccel3;
            gc1.calcPertAcceleration(r, pertAccel1);
            gc2.calcPertAcceleration(r, pertAccel2);
            gc3.calcPertAcceleration(r, pertAccel3);

            printf("pert accel1 = %g, %g, %g\n", pertAccel1(0), pertAccel1(1), pertAccel1(2));
            printf("pert accel2 = %g, %g, %g\n", pertAccel2(0), pertAccel2(1), pertAccel2(2));
            printf("pert accel3 = %g, %g, %g\n", pertAccel3(0), pertAccel3(1), pertAccel3(2));

            EXPECT_NEAR(pertAccel1(0), pertAccel2(0), 1e-20);
            EXPECT_NEAR(pertAccel1(1), pertAccel2(1), 1e-20);
            EXPECT_NEAR(pertAccel1(2), pertAccel2(2), 1e-20);
            EXPECT_NEAR(pertAccel1(0), pertAccel3(0), 1e-20);
            EXPECT_NEAR(pertAccel1(1), pertAccel3(1), 1e-20);
            EXPECT_NEAR(pertAccel1(2), pertAccel3(2), 1e-20);
        }
    }
}



GTEST_MAIN()