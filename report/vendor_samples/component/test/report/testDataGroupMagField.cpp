/// @file      testDataGroupMagField.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-08-11

#include "ast/Test.hpp"
#include "ast/DataGroupMagField.hpp"
#include "ast/EphemerisTwoBody.hpp"
#include "ast/OrbitElement.hpp"
#include "ast/Literals.hpp"

AST_USING_NAMESPACE

TEST(TestDataGroupMagField, TestDataGroupMagField)
{
    auto earth = aGetEarth();
    auto eci = earth->getFrameInertial();
    double gm = earth->getGM();
    ModOrbElem elem{6678137, 0, 28.5_deg, 0, 0, 0};
    CartState initstate;
    aModOrbElemToCart(elem, gm, initstate.pos(), initstate.vel());
    auto epoch = "13 Aug 2026 04:00:00.000 UTCG"_utc;
    SharedPtr<EphemerisTwoBody> ephemeris = EphemerisTwoBody::New(eci, epoch, initstate);
    ScopedPtr<DataGroupMagField> magField = new DataGroupMagField();
    magField->setPoint(ephemeris);

    VariantVector result;
    TimeList timeList{epoch};
    timeList.seconds() = {0};
    errc_t rc = magField->calculate(timeList, result);
    EXPECT_EQ(rc, eNoError);

    auto data = result.asSpan<DataGroupMagField::Data>();
    EXPECT_EQ(data.size(), 1);

}




GTEST_MAIN()

