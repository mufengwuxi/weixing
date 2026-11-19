/// @file      testBrentOptimizer.cpp
/// @brief     测试 Brent 法一维函数极小值求解器
/// @details   ~
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-08-17

#include "ast/BrentOptimizer.hpp"
#include "ast/Function.hpp"
#include "ast/Test.hpp"
#include <cmath>

AST_USING_NAMESPACE

// 测试函数：f(x) = (x - 2)^2
class QuadraticFunction : public UnaryScalarFunc {
public:
    void evaluate(double x, double* out) const override {
        *out = (x - 2.0) * (x - 2.0);
    }
};

// 测试函数：f(x) = -x
class MonotonicFunction : public UnaryScalarFunc {
public:
    void evaluate(double x, double* out) const override {
        *out = -x;
    }
};

TEST(BrentOptimizer, MinimumQuadratic) {
    BrentOptimizer solver;
    QuadraticFunction func;
    double result = 0.0;
    double funcValue = 0.0;
    errc_t err = solver.solve(func, 0.0, 5.0, result, &funcValue);
    ASSERT_EQ(err, eNoError);
    EXPECT_NEAR(result, 2.0, 1e-6);
    EXPECT_NEAR(funcValue, 0.0, 1e-6);
}

TEST(BrentOptimizer, QuarticMinimum) {
    BrentOptimizer solver;
    auto func = [](double x) -> double { return x * x * x * x - 2.0 * x * x + 1.0; };
    double result = 0.0;
    errc_t err = solver.solve(func, 0.0, 1.5, result);
    ASSERT_EQ(err, eNoError);
    EXPECT_NEAR(result, 1.0, 1e-6);
}

TEST(BrentOptimizer, MonotonicBoundary) {
    BrentOptimizer solver;
    MonotonicFunction func;
    double result = 0.0;
    errc_t err = solver.solve(func, 0.0, 1.0, result);
    ASSERT_EQ(err, eNoError);
    EXPECT_NEAR(result, 1.0, 1e-6);
}

TEST(BrentOptimizer, LambdaOverload) {
    BrentOptimizer solver;
    auto func = [](double x) -> double { return (x - 1.0) * (x - 1.0); };
    double result = 0.0;
    errc_t err = solver.solve(func, -2.0, 4.0, result);
    ASSERT_EQ(err, eNoError);
    EXPECT_NEAR(result, 1.0, 1e-6);
}

TEST(BrentOptimizer, FreeFunction) {
    {
        auto func = [](double x) -> double { return (x - 3.0) * (x - 3.0); };
        double result = 0.0;
        errc_t err = aBrentMinimize(func, 0.0, 6.0, result);
        ASSERT_EQ(err, eNoError);
        EXPECT_NEAR(result, 3.0, 1e-6);
    }
    {
        QuadraticFunction func;
        double result = 0.0;
        errc_t err = aBrentMinimize(func, 0.0, 5.0, result);
        ASSERT_EQ(err, eNoError);
        EXPECT_NEAR(result, 2.0, 1e-6);
    }
}

TEST(BrentOptimizer, InvalidBracket) {
    BrentOptimizer solver;
    QuadraticFunction func;
    double result = 0.0;
    errc_t err = solver.solve(func, 5.0, 5.0, result);
    EXPECT_EQ(err, eErrorInvalidParam);
}

GTEST_MAIN()
