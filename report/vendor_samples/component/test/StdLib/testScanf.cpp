/// @file      testScanf.cpp
/// @brief     
/// @details   
/// @author    金柯<jinke18@foxmail.com>
/// @date      2026-03-20

#include <stdio.h>
#include "ast/Test.h"



TEST(Scanf, Basic)
{

    const std::string str1 = 
R"(36
36
3.98600436000e+14
6378137.0

 2  0     -0.484164990000e-03   0.000000000e+00

)";
    const std::string str2 = 
R"(36
3.98600436000e+14
6378137.0

 2  0     -0.484164990000e-03   0.000000000e+00
)";

    const std::string str3 =
R"(
stk.v.4.0

Model			 MRO110C
CentralBody		 Mars
Degree			 110
Order			 110
Gm				 0.4282837564100000E+14
RefDistance		 3.39600000000000e+006
Normalized		 Yes
)";
    {
        double a, b, c, d;
        int status1 = sscanf(str1.c_str(), "%lf %lf %lf %lf", &a, &b, &c, &d);
        int status2 = sscanf(str2.c_str(), "%lf %lf %lf %lf", &a, &b, &c, &d);
        int status3 = sscanf(str3.c_str(), "%lf %lf %lf %lf", &a, &b, &c, &d);
        printf("status1: %d\n", status1);
        printf("status2: %d\n", status2);
        printf("status3: %d\n", status3);
        EXPECT_EQ(status3, 0);
    }
    {
        int a, b;
        double c, d;
        int status11 = sscanf(str1.c_str(), "%d %d %lf %lf", 
               &a, &b, &c, &d);
        int status12 = sscanf(str1.c_str(), "%d %lf %lf", 
               &a, &c, &d);
        int status21 = sscanf(str2.c_str(), "%d %d %lf %lf", 
               &a, &b, &c, &d);
        int status22 = sscanf(str2.c_str(), "%d %lf %lf", 
               &a, &c, &d);
        int status31 = sscanf(str3.c_str(), "%d %d %lf %lf", 
               &a, &b, &c, &d);
        int status32 = sscanf(str3.c_str(), "%d %lf %lf", 
               &a, &c, &d);
        printf("status11: %d\n", status11);
        printf("status12: %d\n", status12);
        printf("status21: %d\n", status21);
        printf("status22: %d\n", status22);
        printf("status31: %d\n", status31);
        printf("status32: %d\n", status32);
    }
}

GTEST_MAIN();

