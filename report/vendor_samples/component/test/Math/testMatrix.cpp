/// @file      testMatrix.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-05-06

#include "ast/Matrix.hpp"
#include "ast/Test.h"

AST_USING_NAMESPACE

TEST(MatrixTest, Basic)
{
    Matrix<double> m(2, 4);
    m(1, 2) = 1.0;
    m(0, 1) = 2;
}


GTEST_MAIN()




