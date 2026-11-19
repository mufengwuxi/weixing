/// @file      testPlanetPoint.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-03-12

#include "ast/PlanetPoint.hpp"
#include "ast/RunTime.hpp"
#include "ast/JplDe.hpp"
#include "ast/TimePoint.hpp"
#include "ast/CelestialBody.hpp"
#include "ast/Vector.hpp"
#include "ast/Test.hpp"

AST_USING_NAMESPACE

TEST(PlanetPointTest, getPosICRF)
{
    if(aIsGithubCI())
        GTEST_SKIP();
    aInitialize();
    TimePoint tp = TimePoint::FromUTC(2026, 3, 12, 0, 0, 0);
    #define _AST_TEST_PLANET_POS_DE(NAME) \
    { \
        Vector3d pos1, pos2; \
        errc_t rc = aJplDeGetPosICRF(tp, JplDe::e##NAME, JplDe::eSSBarycenter, pos1); \
        EXPECT_EQ(rc, eNoError); \
        rc = a##NAME##PosInICRF(tp, pos2); \
        EXPECT_EQ(rc, eNoError); \
        for(int i = 0; i < 3; i++) \
        { \
            EXPECT_DOUBLE_EQ(pos1[i], pos2[i]); \
        }\
    }\

    #define _AST_TEST_PLANET_POS_SPK(NAME) \
    { \
        Vector3d pos1, pos2; \
        errc_t rc = aSpiceGetPosICRF(tp, ESpiceId::e##NAME, ESpiceId::eSolarSystemBarycenter, pos1); \
        EXPECT_EQ(rc, eNoError); \
        rc = a##NAME##PosInICRF(tp, pos2); \
        EXPECT_EQ(rc, eNoError); \
        for(int i = 0; i < 3; i++) \
        { \
            EXPECT_DOUBLE_EQ(pos1[i], pos2[i]); \
        }\
    }\

    _AST_TEST_PLANET_POS_SPK(Mercury);
    _AST_TEST_PLANET_POS_SPK(Venus);
    _AST_TEST_PLANET_POS_DE(Earth);
    _AST_TEST_PLANET_POS_SPK(Mars);
    _AST_TEST_PLANET_POS_SPK(Jupiter);
    _AST_TEST_PLANET_POS_SPK(Saturn);
    _AST_TEST_PLANET_POS_SPK(Uranus);
    _AST_TEST_PLANET_POS_SPK(Neptune);
    _AST_TEST_PLANET_POS_DE(Moon);
    _AST_TEST_PLANET_POS_DE(Sun);
    _AST_TEST_PLANET_POS_DE(EMBarycenter);
}


TEST(PlanetPointTest, getPosVelICRF)
{
    if(aIsGithubCI())
        GTEST_SKIP();
    aInitialize();
    TimePoint tp = TimePoint::FromUTC(2026, 3, 12, 0, 0, 0);
    #define _AST_TEST_PLANET_POSVEL_DE(NAME) \
    { \
        Vector3d pos1, pos2; \
        Vector3d vel1, vel2; \
        errc_t rc = aJplDeGetPosVelICRF(tp, JplDe::e##NAME, JplDe::eSSBarycenter, pos1, vel1); \
        EXPECT_EQ(rc, eNoError); \
        rc = a##NAME##PosVelInICRF(tp, pos2, vel2); \
        EXPECT_EQ(rc, eNoError); \
        for(int i = 0; i < 3; i++) \
        { \
            EXPECT_DOUBLE_EQ(pos1[i], pos2[i]); \
            EXPECT_DOUBLE_EQ(vel1[i], vel2[i]); \
        }\
    }\

    #define _AST_TEST_PLANET_POSVEL_SPK(NAME) \
    { \
        Vector3d pos1, pos2; \
        Vector3d vel1, vel2; \
        errc_t rc = aSpiceGetPosVelICRF(tp, ESpiceId::e##NAME, ESpiceId::eSolarSystemBarycenter, pos1, vel1); \
        EXPECT_EQ(rc, eNoError); \
        rc = a##NAME##PosVelInICRF(tp, pos2, vel2); \
        EXPECT_EQ(rc, eNoError); \
        for(int i = 0; i < 3; i++) \
        { \
            EXPECT_DOUBLE_EQ(pos1[i], pos2[i]); \
            EXPECT_DOUBLE_EQ(vel1[i], vel2[i]); \
        }\
    }\


    _AST_TEST_PLANET_POSVEL_SPK(Mercury);
    _AST_TEST_PLANET_POSVEL_SPK(Venus);
    _AST_TEST_PLANET_POSVEL_DE(Earth);
    _AST_TEST_PLANET_POSVEL_SPK(Mars);
    _AST_TEST_PLANET_POSVEL_SPK(Jupiter);
    _AST_TEST_PLANET_POSVEL_SPK(Saturn);
    _AST_TEST_PLANET_POSVEL_SPK(Uranus);
    _AST_TEST_PLANET_POSVEL_SPK(Neptune);
    _AST_TEST_PLANET_POSVEL_DE(Moon);
    _AST_TEST_PLANET_POSVEL_DE(Sun);
    _AST_TEST_PLANET_POSVEL_DE(EMBarycenter);
}

GTEST_MAIN()
