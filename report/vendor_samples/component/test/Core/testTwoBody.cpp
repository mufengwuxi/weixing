/// @file      testTwoBody.cpp
/// @brief     
/// @details   ~
/// @author    金柯<jinke18@foxmail.com>
/// @date      15.11.2025




#include "ast/TwoBody.hpp"
#include "ast/Vector.hpp"
#include "ast/AstTestMacro.h"


TEST(TwoBodyTest, Prop)
{
	AST_USING_NAMESPACE
	{
		Vector3d pos{6678137.000000000000, 0, 0};
		Vector3d vel { 0, 6789.5302977176515924, 3686.4141730136516344 };

		double dttm = 86400;
		double gm = 3.986004418e14;
		aTwoBodyProp(dttm, gm, pos, vel);

		Vector3d posExpect{
			5596646.34376213,
			-3201966.68063242,
			-1738526.05930174
		};
		Vector3d velExpect{
			4215.06430913537,
			5690.00005775394,
			3089.41796230054
		};

		for (int i = 0; i < 3; i++)
		{
			EXPECT_NEAR(pos[i], posExpect[i], 1e-5);
			EXPECT_NEAR(vel[i], velExpect[i], 1e-5);
		}
	}
}

/// @brief 零时长传播必须是恒等变换
TEST(TwoBodyTest, PropZeroDurationIsIdentity)
{
	AST_USING_NAMESPACE

	const double gm = 4.9028029535968e12;   // 月球
	const double R  = 1.35e8;               // 距中心距离 [m]
	const double vEsc = std::sqrt(2.0 * gm / R);

	// 三种轨道类型都应满足恒等性
	const double speeds[] = {0.5 * vEsc, vEsc / std::sqrt(2.0), 1.5 * vEsc, 3.0 * vEsc};
	const char*  names[]  = {"ellipse", "circle", "hyperbola(1.5)", "hyperbola(3.0)"};

	for (int k = 0; k < 4; ++k)
	{
		for (double t : {0.0, -0.0})
		{
			Vector3d pos{R, 0.0, 0.0};
			Vector3d vel{0.0, speeds[k], 0.0};

			aTwoBodyProp(t, gm, pos, vel);

			EXPECT_FALSE(std::isnan(pos.norm())) << names[k] << " t=" << t;
			EXPECT_FALSE(std::isnan(vel.norm())) << names[k] << " t=" << t;
			EXPECT_NEAR(pos.norm(), R, 1e-6) << names[k] << " t=" << t;
			EXPECT_NEAR(vel.norm(), speeds[k], 1e-9) << names[k] << " t=" << t;
		}
	}
}

GTEST_MAIN()