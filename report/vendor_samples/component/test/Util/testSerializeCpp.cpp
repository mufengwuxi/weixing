/// @file      testSerializeCpp.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-03-30


#include "ast/SerdeAPI.hpp"
#include "ast/CppSerializer.hpp"
#include "ast/IO.hpp"
#include "ast/BuilderAPI.hpp"
#include "ast/Literals.hpp"
#include "ast/Test.h"
#include "ast/TestSerialize.hpp"
#include "ast/StateCartesian.hpp"
#include "ast/StateKeplerian.hpp"
#include "ast/TimePoint.hpp"
#include "ast/CelestialBody.hpp"
#include "ast/BodyEphemerisDE.hpp"
#include "ast/BodyEphemerisSPK.hpp"

AST_USING_NAMESPACE

class SerializeCppTest: public testing::Test{
public:
    void SetUp() override{
        aInitialize();
    }
    void TearDown() override{
        aUninitialize();
    }
};

TEST_F(SerializeCppTest, StateCartesian) {
    if(aIsCI())
        GTEST_SKIP();

    auto earth = aGetBody("Earth");
    StateCartesian* state = new StateCartesian();
    state->setStateEpoch(TimePoint::FromUTC(2022, 2, 4, 12, 0, 0));
    state->setFrame(earth->makeFrameICRF());
    state->setX(7000);
    state->setY(8000);
    state->setZ(9000);
    state->setVx(1000);
    state->setVy(2000);
    state->setVz(3000);

    errc_t rc = aTestSerializeCpp(state);
    EXPECT_EQ(rc, eNoError);
}


TEST_F(SerializeCppTest, StateKeplerian) {
    GTEST_SKIP();
    if(aIsCI())
        GTEST_SKIP();

    auto earth = aGetBody("Earth");
    StateKeplerian* state = new StateKeplerian();
    state->setStateEpoch(TimePoint::FromUTC(2022, 2, 4, 12, 0, 0));
    state->setFrame(earth->makeFrameICRF());
    state->setSMA(1000000_km);
    state->setEcc(0.1);
    state->setInc(10_deg);
    state->setRAAN(20_deg);
    state->setArgPeri(30_deg);
    state->setTrueAnomaly(30_deg);
    errc_t rc = aTestSerializeCpp(state);
    EXPECT_EQ(rc, eNoError);
}

TEST_F(SerializeCppTest, BodyEphemerisDE) {
    if(aIsCI())
        GTEST_SKIP();

    auto earth = aGetBody("Earth");
    BodyEphemerisDE* ephemeris = new BodyEphemerisDE();
    ephemeris->setJplIndex(JplDe::eJupiter);

    errc_t rc = aTestSerializeCpp(ephemeris);
    EXPECT_EQ(rc, eNoError);

    delete ephemeris;
}

TEST_F(SerializeCppTest, BodyEphemerisSPK) {
    if(aIsCI())
        GTEST_SKIP();

    auto earth = aGetBody("Earth");
    BodyEphemerisSPK* ephemeris = new BodyEphemerisSPK();
    ephemeris->setSpiceIndex(3);
    errc_t rc = ephemeris->openSPKFile(aTestGetConfigValue("SPK_FILE"));
    EXPECT_EQ(rc, eNoError);

    rc = aTestSerializeCpp(ephemeris);
    EXPECT_EQ(rc, eNoError);

    delete ephemeris;
}

GTEST_MAIN()

