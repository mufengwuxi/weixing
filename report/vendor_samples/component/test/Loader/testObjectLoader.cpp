/// @file      testObjectLoader.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-04-08

#include "ast/ObjectLoader.hpp"
#include "ast/Test.h"
#include "ast/Facility.hpp"
#include "ast/Target.hpp"
#include "ast/Place.hpp"
#include "ast/Sensor.hpp"

AST_USING_NAMESPACE

TEST(ObjectLoaderTest, LoadFacility)
{
    if(aIsCI())
        GTEST_SKIP();

    std::vector<std::string> files = aTestGetConfigStringVector("STK_FACILITY_FILES");
    for (const auto &file : files)
    {
        SharedPtr<Object> object;
        errc_t ret = aLoadObject(file, "Facility", object);
        Facility* facility = dynamic_cast<Facility*>(object.get());
        auto body = facility->body();
        EXPECT_TRUE(body != nullptr);
        EXPECT_TRUE(facility != nullptr);
        EXPECT_EQ(ret, 0);
        EXPECT_TRUE(object != nullptr);
    }
}

TEST(ObjectLoaderTest, LoadTarget)
{
    if(aIsCI())
        GTEST_SKIP();

    std::vector<std::string> files = aTestGetConfigStringVector("STK_TARGET_FILES");
    for (const auto &file : files)
    {
        SharedPtr<Object> object;
        errc_t ret = aLoadObject(file, "Target", object);
        Target* target = dynamic_cast<Target*>(object.get());
        auto body = target->body();
        EXPECT_TRUE(body != nullptr);
        EXPECT_TRUE(target != nullptr);
        EXPECT_EQ(ret, 0);
        EXPECT_TRUE(object != nullptr);
    }
}

TEST(ObjectLoaderTest, LoadPlace)
{
    if(aIsCI())
        GTEST_SKIP();

    std::vector<std::string> files = aTestGetConfigStringVector("STK_PLACE_FILES");
    for (const auto &file : files)
    {
        SharedPtr<Object> object;
        errc_t ret = aLoadObject(file, "Place", object);
        Place* place = dynamic_cast<Place*>(object.get());
        auto body = place->body();
        EXPECT_TRUE(body != nullptr);
        EXPECT_TRUE(place != nullptr);
        EXPECT_EQ(ret, 0);
        EXPECT_TRUE(object != nullptr);
    }
}

TEST(ObjectLoaderTest, LoadSensor)
{
    if(aIsCI())
        GTEST_SKIP();

    std::vector<std::string> files = aTestGetConfigStringVector("STK_SENSOR_FILES");
    for (const auto &file : files)
    {
        SharedPtr<Object> object;
        errc_t ret = aLoadObject(file, "Sensor", object);
        Sensor* sensor = dynamic_cast<Sensor*>(object.get());
        auto fov = sensor->getFieldOfView();
        EXPECT_TRUE(fov != nullptr);
        EXPECT_TRUE(sensor != nullptr);
        EXPECT_EQ(ret, 0);
        EXPECT_TRUE(object != nullptr);
    }
}

GTEST_MAIN();
