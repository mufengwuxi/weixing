#pragma once
#include "../IAtkObj.h"
#include <vector>
class IAreaTypeEllipse;
class IAreaTypePatternCollection;
class IAreaTarget;

///
/// 常用任务
///
class IATKOBJECT_API IATCommonTasks
{
public:
    IATCommonTasks(const IATCommonTasks&) = delete;
    IATCommonTasks& operator=(const IATCommonTasks&) = delete;

	IATCommonTasks(IAreaTarget* pIAreaTarget);
	~IATCommonTasks();

	//********************************************************************
	/// 设置椭圆数据
	/// @Author	Gan Shu Long
	/// @Date	2026-01-15
	/// @Input
	/// @Param	dSemiMajorAxis 椭圆数据半长轴
	/// @Param	dSemiMinorAxis 椭圆数据半短轴
	/// @Param	dBearing 椭圆数据方位角
	/// @Return
	//********************************************************************
	IAreaTypeEllipse* SetAreaTypeEllipse(double dSemiMajorAxis, double dSemiMinorAxis, double dBearing);

	//********************************************************************
	/// 设置椭圆数据
	/// @Author	Gan Shu Long
	/// @Date	2026-01-15
	/// @Input
	/// @Param	dSemiMajorAxis 椭圆数据半长轴
	/// @Param	dSemiMinorAxis 椭圆数据半短轴
	/// @Param	dBearing 椭圆数据方位角
	/// @Return
	//********************************************************************
	IAreaTypePatternCollection* SetAreaTypePattern(std::vector<std::vector<double>> vectorVectorDouble);

private:
	IAreaTarget*		m_pIAreaTarget;
};
