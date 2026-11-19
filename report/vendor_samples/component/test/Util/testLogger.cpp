/// @file      testLogger.cpp
/// @brief     
/// @details   ~
/// @author    金柯<jinke18@foxmail.com>
/// @date      26.11.2025


#include "ast/AstTestMacro.h" 
#include "ast/Logger.hpp"



TEST(Logger, Warning)
{
    aWarning() << "message";
    aWarning("number: %d", 1);
}

GTEST_MAIN()