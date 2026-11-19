/// @file      testODEGeneric.cpp
/// @brief     ~
/// @details   ~
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-01-29

#include "ast/ODE.hpp"
#include "ast/Test.h"

AST_USING_NAMESPACE

TEST(ODEGeneric, integrate) {
    auto ode = make_ode(1, [](const double* y, double* dy, const double t) -> errc_t {
        dy[0] = -y[0];
        return eNoError;
    });

    EXPECT_EQ(ode.getDimension(), 1);
    RKF78 integrator;
    // 设置初始条件
    double t0 = 0.0;
    double y0[1] = {1.0};

    // 积分到t=1.0
    double tf = 1.0;
    double t = t0;
    double y[1]{y0[0]};

    // 执行积分
    errc_t result = integrator.integrate(
        1, [](const double* y, double* dy, const double t) -> errc_t {
            dy[0] = -y[0];
            return eNoError;
        }, 
        y, t, tf
    );

    // 检查是否成功
    EXPECT_EQ(result, eNoError);

    // 计算解析解
    double analyticalSolution = exp(-tf);

    // 检查数值解与解析解的误差
    EXPECT_NEAR(y[0], analyticalSolution, 1e-8);
    EXPECT_EQ(t, tf);
}



TEST(ODEGeneric, EventDetector) {
    RKF78 integrator;
    // 设置初始条件
    double t0 = 0.0;
    double y0[1] = {1.0};

    double tf = 10.0;
    double t = t0;
    double y[1]{y0[0]};

    double value = 0.456;

    // 添加事件检测器
    integrator.addEventDetector([=](const double* y, const double t) -> double {
        return y[0] - value;
    });

    auto ode = make_ode(1, [](const double* y, double* dy, const double t) -> errc_t {
        dy[0] = -y[0];
        return eNoError;
    });

    // 执行积分
    errc_t result = integrator.integrate(ode, y, t, tf);

    // 检查是否成功
    EXPECT_EQ(result, eNoError);

    // 计算解析解
    // double analyticalSolution = exp(-tf);

    // 检查数值解与解析解的误差
    EXPECT_NEAR(y[0], value, 1e-10);
    EXPECT_NEAR(t, log(1./value), 1e-10);
}

GTEST_MAIN();