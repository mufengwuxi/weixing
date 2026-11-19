/// @file      testJplSpk.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-03-26

#include "ast/JplSpk.hpp"
#include "ast/SpiceAPI.hpp"
#include "ast/TimeInterval.hpp"
#include "ast/CelestialBody.hpp"
#include "ast/Vector.hpp"
#include "ast/Test.h"


AST_USING_NAMESPACE

TEST(JplSpkTest, open_twice)
{
    if(aIsGithubCI())
        GTEST_SKIP();
    Vector3d pos;
    JplSpk spk1;
    JplSpk spk2;
    const std::string spkfile = aTestGetConfigValue("SPK_FILE").toString();

    spk1.open(spkfile);
    spk2.open(spkfile);
    errc_t rc1 = spk1.getPosICRF(TimePoint{}, ESpiceId::eJupiter, ESpiceId::eSolarSystemBarycenter, pos);
    spk2.close();
    EXPECT_EQ(rc1, eNoError);

    errc_t rc2 = spk1.getPosICRF(TimePoint{}, ESpiceId::eJupiter, ESpiceId::eSolarSystemBarycenter, pos);
    spk1.close();
    EXPECT_EQ(rc2, eNoError);

    errc_t rc3 = spk1.getPosICRF(TimePoint{}, ESpiceId::eJupiter, ESpiceId::eSolarSystemBarycenter, pos);
    EXPECT_NE(rc3, eNoError);

    printf("rc1: %d, rc2: %d, rc3: %d\n", rc1, rc2, rc3);
}


TEST(JplSpkTest, open)
{
    if(aIsGithubCI())
        GTEST_SKIP();
    TimeInterval interval;
    std::string spkfile = aTestGetConfigValue("SPK_FILE").toString();

    int count = SpiceAPI::Instance()->ktotal("ALL");
    EXPECT_EQ(count, 0);
    JplSpk spk(spkfile);
    errc_t rc = spk.getInterval(ESpiceId::eJupiter, interval);
    EXPECT_EQ(rc, eNoError);
    EXPECT_NE(spk.handle(), 0);
    printf("interval: %s\n", interval.toString().c_str());

    std::vector<std::string> names;
    spk.getBodyNames(names);
    EXPECT_GT(names.size(), 0);
    for(const std::string& name : names)
    {
        printf("%s\n", name.c_str());
    }
}


GTEST_MAIN()
