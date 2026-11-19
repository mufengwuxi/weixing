/// @file      testMathOperator.cpp
/// @brief     
/// @details   ~
/// @author    金柯<jinke18@foxmail.com>
/// @date      15.11.2025



#include "ast/MathOperator.hpp"
#include "ast/Vector.hpp"
#include "ast/Matrix.hpp"
#include "ast/AstTestMacro.h"
#include <vector>
#include <array>
#include <cstdio>

using namespace _AST math;

TEST(MathOperatorTest, VectorFunction)
{
    AST_USING_NAMESPACE
    // norm
    const double expect = sqrt(9+16+25);
    {
        std::vector<double> a{ 3, 4, 5 };
        double retval = norm(a);
        printf("norm: %lf\n", retval);
        EXPECT_NEAR(retval, expect, 1e-15);
    }
    {
        std::array<double, 3> a{3, 4, 5};
        double retval = norm(a);
        printf("norm: %lf\n", retval);
        EXPECT_NEAR(retval, expect, 1e-15);
    }
    {
        double a[3]{3,4,5};
        double retval = norm(a);
        printf("norm: %lf\n", retval);
        EXPECT_NEAR(retval, expect, 1e-15);
    }
    {
        std::vector<double> a{ 3, 4 };
        double retval = norm(a);
        printf("norm: %lf\n", retval);
        EXPECT_NEAR(retval, 5, 1e-15);
    }
    {
        Vector3d a{3,4,5};
        double retval = norm(a);
        printf("norm: %lf\n", retval);
        EXPECT_NEAR(retval, expect, 1e-15);
    }

    // cross
    {
        double arr1[3]{ 1,2,3 };
        double arr2[3]{ 4,5, 6};
        auto arr = cross(arr1, arr2);
        A_UNUSED(arr);
        nothing();
    }
    {
        double arr1[3]{ 1,2,3 };
        double arr2[3]{ 4,5,6 };
        double arr3[3]{};
        double* a= arr1, *b = arr2;
        cross(arr3, a, b);
        nothing();
    }
    {
        std::vector<double> arr1{1,2, 3};
        std::vector<double> arr2{4,5,6};
        auto ret = cross(arr1, arr2);
        nothing();
    }
    {
        std::array<double,3> arr1{ 1,2,3 };
        std::array<double,3> arr2{ 4,5,6 };
        auto ret = cross(arr1, arr2);
        A_UNUSED(ret);
        nothing();
    }
    {
        std::vector<double> arr1{ 1,2,3 };
        std::array<double, 3> arr2{ 4,5,6 };
        auto ret = cross(arr1, arr2);
        A_UNUSED(ret);
        nothing();
    }
    {
        Vector3d a{3,4,5};
        double b[3]{1,2,3};
        auto ret = cross(a, b);
        A_UNUSED(ret);
    }

    // dot
    {
        double a[3]{1,2,3};
        double b[3]{1,2,3};
        double r = dot(a, b);
        A_UNUSED(r);
    }
    {
        std::vector<double> a{1,2,3};
        std::vector<double> b{1,2,3};
        double r = dot(a, b);
        A_UNUSED(r);
    }

    // normlized
    {
        double a[3]{1,2,3};
        auto ret = normalized(a);
        A_UNUSED(ret);
        nothing();
    }
    {
        double a[3]{1,2,3};
        double* b = a;
        auto ret = normalized<3>(b);
        A_UNUSED(ret);
        nothing();
    }
}


TEST(MathOperatorTest, VectorOperator)
{
    AST_USING_NAMESPACE
    {
        std::array<double,3> a{1,2,3};
        double b[3]{ 4,5,6 };
        auto ret = a + b;
        A_UNUSED(ret);
        nothing();
    }
    {
        Vector3d vec1{ 1,2,3 };
        Vector3d vec2{ 1,2,-3 };
        auto ret = vec1 + vec2;
        A_UNUSED(ret);
        nothing();
    }
    {
        Vector3d vec1{1,2,3};
        auto ret = vec1 * 2;
        A_UNUSED(ret);
        nothing();
    }
    {
        Vector3d vec1{1,2,3};
        auto ret = vec1 / 2;
        A_UNUSED(ret);
        nothing();
    }
    {
        std::array<int, 3> a{1,2,4},b{4,5,6};
        auto c = a - b;
        A_UNUSED(c);
        nothing();
    }
}

TEST(MathOperatorTest, MatrixOperator)
{
    AST_USING_NAMESPACE

    {
        double a[3][3]{};
        double b[3][3]{};
        auto c = mtimes(a,b);
        A_UNUSED(c);
    }
    {
        Matrix3d mtx1{};
        Matrix3d mtx2{};
        auto mtx3 = mtx1 * mtx2;
        A_UNUSED(mtx3);
    }

}

TEST(MathOperatorTest, Example1)
{
    AST_USING_NAMESPACE

    {
        // 使用 std::vector
        std::vector<double> v1 = { 1, 2, 3 };
        std::vector<double> v2 = { 4, 5, 6 };

        double dot_product = dot(v1, v2);           // 点积
        auto cross_product = cross(v1, v2);         // 叉积
        double magnitude = norm(v1);                // 范数
        auto normalized_v = normalized(v1);         // 归一化

        A_UNUSED(dot_product);
        A_UNUSED(cross_product);
        A_UNUSED(magnitude);
        A_UNUSED(normalized_v);
    }

    {
        // 使用原生数组
        double arr1[3] = { 1, 2, 3 };
        double arr2[3] = { 4, 5, 6 };

        auto dot_product = dot(arr1, arr2);
        auto cross_product = cross(arr1, arr2);

        A_UNUSED(dot_product);
        A_UNUSED(cross_product);
    }

    {
        std::vector<double> v = { 1, 2, 3 };

        // 标量运算
        auto v2 = 2.0 * v;      // {2, 4, 6}
        auto v3 = v + v;        // {2, 4, 6}
        auto v4 = v - 1.0;      // {0, 1, 2}

        A_UNUSED(v2);
        A_UNUSED(v3);
        A_UNUSED(v4);
    }
    {
        // 使用 MatrixMN 类
        MatrixMN<double, 2, 3> A = {  };
        MatrixMN<double, 3, 2> B = {  };
        auto C = A * B;  // 2x2 矩阵

        A_UNUSED(C);
    }
    {
        // 使用原生数组
        double A[2][3] = { {1,2,3}, {4,5,6} };
        double B[3][2] = { {7,8}, {9,10}, {11,12} };
        auto C = mtimes(A, B);  // 2x2 矩阵

        A_UNUSED(C);
    }
}

GTEST_MAIN()