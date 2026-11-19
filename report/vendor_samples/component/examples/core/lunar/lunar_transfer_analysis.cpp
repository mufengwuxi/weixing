/// @file      lunar_transfer_analysis.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-09-04


#include "ast/HPOP.hpp"
#include "ast/HPOPForceModel.hpp"
#include "ast/Resolve.hpp"
#include "ast/OrbitElement.hpp"
#include "ast/SpacecraftParam.hpp"
#include "ast/Literals.hpp"
#include "ast/DetectorPeriapsis.hpp"

AST_USING_NAMESPACE

HPOPForceModel forceModel()
{
    HPOPForceModel fm;

    fm.setCentralBody("Earth"_body);
    fm.gravity().maxOrder_  = 8;
    fm.gravity().maxDegree_ = 8;
    fm.gravity().model_ = "WGS84";

    auto moonForce = fm.addThirdBody("Moon"_body);
    moonForce->setAttractionType(EBodyAttractionType::ePointMass);
    moonForce->pointMass().setSpecifiedGM(4902800000000);

    auto sunForce   = fm.addThirdBody("Sun"_body);
    sunForce->setAttractionType(EBodyAttractionType::ePointMass);
    sunForce->pointMass().setSpecifiedGM(1.32712000000000e+20);

    fm.useDrag(false);
    fm.useSRP(false);

    return fm;
}


void analyseLunarTransfer()
{
    HPOPForceModel fm = forceModel();
    CartState initState{6159968.33397138, 2393641.1690744, 922487.447464321, -2872.54217681962, 9257.74498172147, -4877.87739768414};
    TimePoint initTime = "2017-12-31 20:51:50.678"_utc;
    auto state = initState;
    SpacecraftParam spacecraftParam;
    HPOP hpop;
    SharedPtr<DetectorPeriapsis> detector = aMakeShared<DetectorPeriapsis>();
    detector->setBody("Moon"_body);
    hpop.setSpacecraftParam(spacecraftParam);
    hpop.setForceModel(fm);
    hpop.addEventDetector(detector.get());
    TimePoint stopTime = initTime + 10_day;
    errc_t rc = hpop.propagate(initTime, stopTime, state.pos(), state.vel());
    printf("stopTime: %s\n", stopTime.toString().c_str());
    printf("state: %s\n", state.toString().c_str());
}

int main()
{
    analyseLunarTransfer();
}
