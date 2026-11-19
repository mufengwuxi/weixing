/// @file      testGoldenSectionOptimizer.cpp
/// @brief     测试一维函数极小值求解器
/// @details   ~
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-08-17

#include "ast/GoldenSectionOptimizer.hpp"
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

TEST(GoldenSectionOptimizer, MinimumQuadratic) {
    GoldenSectionOptimizer solver;
    QuadraticFunction func;
    double result = 0.0;
    double funcValue = 0.0;
    errc_t err = solver.solve(func, 0.0, 5.0, result, &funcValue);
    ASSERT_EQ(err, eNoError);
    EXPECT_NEAR(result, 2.0, 1e-6);
    EXPECT_NEAR(funcValue, 0.0, 1e-6);
}

TEST(GoldenSectionOptimizer, QuarticMinimum) {
    GoldenSectionOptimizer solver;
    auto func = [](double x) -> double { return x * x * x * x - 2.0 * x * x + 1.0; };
    double result = 0.0;
    errc_t err = solver.solve(func, 0.0, 1.5, result);
    ASSERT_EQ(err, eNoError);
    EXPECT_NEAR(result, 1.0, 1e-6);
}

TEST(GoldenSectionOptimizer, MonotonicBoundary) {
    GoldenSectionOptimizer solver;
    MonotonicFunction func;
    double result = 0.0;
    errc_t err = solver.solve(func, 0.0, 1.0, result);
    ASSERT_EQ(err, eNoError);
    EXPECT_NEAR(result, 1.0, 1e-6);
}

TEST(GoldenSectionOptimizer, LambdaOverload) {
    GoldenSectionOptimizer solver;
    auto func = [](double x) -> double { return (x - 1.0) * (x - 1.0); };
    double result = 0.0;
    errc_t err = solver.solve(func, -2.0, 4.0, result);
    ASSERT_EQ(err, eNoError);
    EXPECT_NEAR(result, 1.0, 1e-6);
}

TEST(GoldenSectionOptimizer, FreeFunction) {
    {
        auto func = [](double x) -> double { return (x - 3.0) * (x - 3.0); };
        double result = 0.0;
        errc_t err = aGoldenSectionMinimize(func, 0.0, 6.0, result);
        ASSERT_EQ(err, eNoError);
        EXPECT_NEAR(result, 3.0, 1e-6);
    }
    {
        QuadraticFunction func;
        double result = 0.0;
        errc_t err = aGoldenSectionMinimize(func, 0.0, 5.0, result);
        ASSERT_EQ(err, eNoError);
        EXPECT_NEAR(result, 2.0, 1e-6);
    }
}

TEST(GoldenSectionOptimizer, InvalidBracket) {
    GoldenSectionOptimizer solver;
    QuadraticFunction func;
    double result = 0.0;
    errc_t err = solver.solve(func, 5.0, 5.0, result);
    EXPECT_EQ(err, eErrorInvalidParam);
}

GTEST_MAIN()
