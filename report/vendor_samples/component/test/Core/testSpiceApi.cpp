/// @file      testSpiceApi.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-03-22

#include "ast/SpiceAPI.hpp"
#include "ast/Test.h"
#include "ast/RunTime.hpp"
#include "ast/CelestialBody.hpp"

AST_USING_NAMESPACE

TEST(SpiceAPI, LoadUnload)
{
    SpiceAPI api;
    const auto libpaths = aTestGetConfigStringVector("CSPICE_LIBS");
    errc_t rc = api.tryload(libpaths);
    EXPECT_FALSE(api.failed());
    EXPECT_EQ(rc, eNoError);
    {
        SpiceAPI api2;
        rc = api2.tryload(libpaths);
        EXPECT_EQ(rc, eNoError);
        EXPECT_FALSE(api2.failed());
    }
    EXPECT_FALSE(api.failed());
}


TEST(SpiceAPI, Instance)
{
    EXPECT_TRUE(SpiceAPI::Instance()->isLoaded());
    bool failed = SpiceAPI::Instance()->failed();
    EXPECT_FALSE(failed);
}


TEST(SpiceAPI, HashBodyEphemeris)
{
    if(aIsCI())
        return;
    aInitialize();
    EXPECT_TRUE(aSpiceHasBodyEphemeris(ESpiceId::eEarth));
    EXPECT_TRUE(aSpiceHasBodyEphemeris(ESpiceId::eMercury));
    EXPECT_TRUE(aSpiceHasBodyEphemeris(ESpiceId::eGanymede));
    EXPECT_FALSE(aSpiceHasBodyEphemeris(12345678910));
}


GTEST_MAIN();



