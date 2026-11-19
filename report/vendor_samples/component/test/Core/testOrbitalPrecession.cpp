/// @file      testOrbitalPrecession.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-02-27

#include "ast/OrbitalPrecession.hpp"
#include "ast/Constants.hpp"
#include "ast/Test.hpp"
#include <vector>
#include <tuple>


AST_USING_NAMESPACE

const double EARTH_J2 = 1.0826261e-3;
const double EARTH_GM = 3.986004418e14;

TEST(OrbitalPrecession, MeanAnomalyRate)
{
    const static struct {
        double gm, j2, rb, a, e, i, expect;
    } testData[] = {
        {EARTH_GM, EARTH_J2, kEarthRadius, 4000e3, 0.1, 0.5, 0.00250247672293061},
        {EARTH_GM, EARTH_J2, kEarthRadius, 5000e3, 0.4, 0.2, 0.0017914885718043}
    };
    for(const auto& param : testData)
    {
        double gm, j2, rb, a, e, i, expect;
        gm = param.gm;
        j2 = param.j2;
        rb = param.rb;
        a = param.a;
        e = param.e;
        i = param.i;
        expect = param.expect;
        double meanMotn = aMeanAnomalyRate(gm, j2, rb, a, e, i);
        printf("meanMotn=%.15g\n", meanMotn);
        EXPECT_NEAR(meanMotn, expect, 1e-15);
    }
}


TEST(OrbitalPrecession, RAANRate)
{
    const static struct {
        double gm, j2, rb, a, e, i, expect;
    } testData[] = {
        {EARTH_GM, EARTH_J2, kEarthRadius, 5000e3, 0.4, 0.2, -6.57549541625983e-06},
        {EARTH_GM, EARTH_J2, kEarthRadius, 5000e3, 0.1, 0.5, -4.2326427773044e-06},
    };
    for(const auto& param : testData)
    {
        double gm, j2, rb, a, e, i, expect;
        gm = param.gm;
        j2 = param.j2;
        rb = param.rb;
        a = param.a;
        e = param.e;
        i = param.i;
        expect = param.expect;
        double raanRate = aRAANRate(gm, j2, rb, a, e, i);
        printf("raanRate=%.15g\n", raanRate);
        EXPECT_NEAR(raanRate, expect, 1e-15);
    }
}


TEST(OrbitalPrecession, ArgPerRate)
{
    const static struct {
        double gm, j2, rb, a, e, i, expect;
    } testData[] = {
        {kEarthGrav, EARTH_J2, kEarthRadius, 5000e3, 0.4, 0.2, 1.275644152659e-05},
        {kEarthGrav, EARTH_J2, kEarthRadius, 5000e3, 0.1, 0.5, 6.87469835954771e-06},
    };
    for(const auto& param : testData)
    {
        double gm, j2, rb, a, e, i, expect;
        gm = param.gm;
        j2 = param.j2;
        rb = param.rb;
        a = param.a;
        e = param.e;
        i = param.i;
        expect = param.expect;
        double argPerRate = aArgPeriRate(gm, j2, rb, a, e, i);
        printf("argPerRate=%.15g\n", argPerRate);
        EXPECT_NEAR(argPerRate, expect, 1e-15);
    }
}



GTEST_MAIN()




