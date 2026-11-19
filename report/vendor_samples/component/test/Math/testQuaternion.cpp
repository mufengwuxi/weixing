/// @file      testQuaternion.cpp
/// @brief     ~
/// @details   ~
/// @author    金柯<jinke18@foxmail.com>
/// @date      2025-12-08

#include "ast/Quaternion.hpp"
#include "ast/AstTestMacro.h"



#if defined(AST_WITH_EIGEN)  && defined(A_CXX14)
#   define _AST_USE_EIGEN
#endif

#ifdef _AST_USE_EIGEN
#include "Eigen/Dense"
#endif

AST_USING_NAMESPACE


template<typename Quaternion>
void testQuaternion()
{
    Quaternion q{1,2,3,4};
    EXPECT_EQ(q.w(), 1);
    EXPECT_EQ(q.x(), 2);
    EXPECT_EQ(q.y(), 3);
    EXPECT_EQ(q.z(), 4);

    Quaternion q2{2,3,4,5};
    auto result = q * q2;
    A_UNUSED(result);
    nothing();

}


TEST(Quaternion, Init)
{
    testQuaternion<Quaternion>();
    #if defined(_AST_USE_EIGEN)
    testQuaternion<Eigen::Quaterniond>();
    #endif
}


GTEST_MAIN()