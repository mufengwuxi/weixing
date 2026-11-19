/// @file      testODE.cpp
/// @brief     ~
/// @details   ~
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-01-16

#include "ast/ODE.hpp"
#include "ast/Test.h"

AST_USING_NAMESPACE



// 定义一个简单的ODE类：dy/dt = -y
class SimpleODE : public ODE {
public:
    int getDimension() const override {
        return 1;
    }

    errc_t evaluate(const double* y, double* dy, double t) override {
        (void)t; // 未使用的参数
        dy[0] = -y[0];
        return eNoError;
    }
};

// 通用测试函数 - 可变步长积分器
template <typename IntegratorType>
void TestVarStepIntegrator(IntegratorType& integrator, double maxAbsErr, double tolerance) {
    // 创建ODE实例
    SimpleODE ode;

    // 设置积分器参数
    integrator.setMaxAbsErr(maxAbsErr);
    integrator.setMaxRelErr(1e-13);

    // 设置初始条件
    double t0 = 0.0;
    double y0[1] = {1.0};

    // 积分到t=1.0
    double tf = 1.0;
    double t = t0;
    double y[1]{y0[0]};

    // 执行积分
    errc_t result = integrator.integrate(ode, y, t, tf);

    // 检查是否成功
    EXPECT_EQ(result, eNoError);

    // 计算解析解
    double analyticalSolution = exp(-tf);

    // 检查数值解与解析解的误差
    EXPECT_NEAR(y[0], analyticalSolution, tolerance);
    EXPECT_EQ(t, tf);

    printf("largestStepSize_ = %f\n", integrator.getLargestStepSize());
    printf("smallestStepSize_ = %f\n", integrator.getSmallestStepSize());
    printf("numSteps_ = %d\n", integrator.getNumSteps());
}

template <typename IntegratorType>
void TestVarStepIntegratorVectorCollector(IntegratorType& integrator, double maxAbsErr, double tolerance) {
    // 创建ODE实例
    SimpleODE ode;

    // 设置积分器参数
    integrator.setMaxAbsErr(maxAbsErr);
    integrator.setMaxRelErr(1e-13);

    // 设置初始条件
    double t0 = 0.0;
    double y0[1] = {1.0};

    // 积分到t=1.0
    double tf = 1.0;
    double t = t0;
    double y[1]{y0[0]};

    // 执行积分
    std::vector<double> xlist;
    std::vector<std::vector<double>> ylist;
    errc_t result = integrator.integrate(ode, y, t, tf, xlist, ylist);
    EXPECT_EQ(xlist.size(), ylist.size());
    ASSERT_EQ(xlist.size(), integrator.getNumSteps() + 1);
    EXPECT_EQ(ylist[0][0], y0[0]);
    EXPECT_EQ(ylist[xlist.size() - 1][0], y[0]);
    EXPECT_EQ(xlist[xlist.size() - 1], tf);
    EXPECT_EQ(xlist[0], t0);
    EXPECT_EQ(t, tf);

    // 检查是否成功
    EXPECT_EQ(result, eNoError);

    // 计算解析解
    double analyticalSolution = exp(-tf);

    // 检查数值解与解析解的误差
    EXPECT_NEAR(y[0], analyticalSolution, tolerance);

    printf("largestStepSize_ = %f\n", integrator.getLargestStepSize());
    printf("smallestStepSize_ = %f\n", integrator.getSmallestStepSize());
    printf("numSteps_ = %d\n", integrator.getNumSteps());
    printf("x.size() = %zu\n", xlist.size());
    printf("y.size() = %zu\n", ylist.size());
}

// 通用测试函数 - 固定步长积分器
template <typename IntegratorType>
void TestFixedStepIntegrator(IntegratorType& integrator, double stepSize, double tolerance) {
    // 创建ODE实例
    SimpleODE ode;

    // 设置积分器参数
    integrator.setStepSize(stepSize);

    // 设置初始条件
    double t0 = 0.0;
    double y0[1] = {1.0};

    // 积分到t=1.0
    double tf = 1.0;
    double t = t0;
    double y[1]{y0[0]};

    // 执行积分
    errc_t result = integrator.integrate(ode, y, t, tf);

    // 检查是否成功
    EXPECT_EQ(result, eNoError);

    // 计算解析解
    double analyticalSolution = exp(-tf);

    // 检查数值解与解析解的误差
    EXPECT_NEAR(y[0], analyticalSolution, tolerance);
}

template <typename IntegratorType>
void TestFixedStepIntegratorVectorCollector(IntegratorType& integrator, double stepSize, double tolerance) {
    // 创建ODE实例
    SimpleODE ode;

    // 设置积分器参数
    integrator.setStepSize(stepSize);

    // 设置初始条件
    double t0 = 0.0;
    double y0[1] = {1.0};

    // 积分到t=1.0
    double tf = 1.0;
    double t = t0;
    double y[1]{y0[0]};

    // 执行积分
    std::vector<double> xlist;
    std::vector<std::vector<double>> ylist;
    errc_t result = integrator.integrate(ode, y, t, tf, xlist, ylist);
    EXPECT_EQ(xlist.size(), ylist.size());
    ASSERT_EQ(xlist.size(), integrator.getNumSteps() + 1);
    EXPECT_EQ(ylist[0][0], y0[0]);
    EXPECT_EQ(ylist[xlist.size() - 1][0], y[0]);
    EXPECT_EQ(xlist[xlist.size() - 1], tf);
    EXPECT_EQ(xlist[0], t0);

    // 检查是否成功
    EXPECT_EQ(result, eNoError);

    // 计算解析解
    double analyticalSolution = exp(-tf);

    // 检查数值解与解析解的误差
    EXPECT_NEAR(y[0], analyticalSolution, tolerance);
    printf("numSteps_ = %d\n", integrator.getNumSteps());
    printf("x.size() = %zu\n", xlist.size());
    printf("y.size() = %zu\n", ylist.size());
}



TEST(ODE, RKF78)
{
    RKF78 integrator;
    TestVarStepIntegrator(integrator, 1e-14, 1e-12);
}

TEST(ODE, RKF45)
{
    RKF45 integrator;
    TestVarStepIntegrator(integrator, 1e-10, 1e-8);
}

TEST(ODE, RKF56)
{
    RKF56 integrator;
    TestVarStepIntegrator(integrator, 1e-12, 1e-10);
}

TEST(ODE, RKCK)
{
    RKCK integrator;
    TestVarStepIntegrator(integrator, 1e-12, 1e-10);
}

TEST(ODE, RK4)
{
    RK4 integrator;
    TestFixedStepIntegrator(integrator, 1e-5, 1e-12); 
}

TEST(ODE, RK8)
{
    RK8 integrator;
    TestFixedStepIntegrator(integrator, 1e-2, 1e-12); 
}

TEST(ODE, RKV8)
{
    RKV8 integrator;
    TestFixedStepIntegrator(integrator, 1e-2, 1e-12); 
}


TEST(ODE, RKF78_VectorCollector)
{
    RKF78 integrator;
    TestVarStepIntegratorVectorCollector(integrator, 1e-14, 1e-12);
}

TEST(ODE, RKF45_VectorCollector)
{
    RKF45 integrator;
    TestVarStepIntegratorVectorCollector(integrator, 1e-10, 1e-8);
}


TEST(ODE, RK4_VectorCollector)
{
    RK4 integrator;
    TestFixedStepIntegratorVectorCollector(integrator, 1e-5, 1e-12); 
}

GTEST_MAIN();