#pragma once
#include "../IAtkObj.h"
#include "IAreaTypeData.h"
struct AsTAtEllipse;

///
/// 区域椭圆
///
class IATKOBJECT_API IAreaTypeEllipse :public IAreaTypeData
{
public:
    IAreaTypeEllipse(const IAreaTypeEllipse&) = delete;
    IAreaTypeEllipse& operator=(const IAreaTypeEllipse&) = delete;

	IAreaTypeEllipse(AsTAtEllipse* pAsTAtEllipse);
	~IAreaTypeEllipse();

	//********************************************************************
	/// 设置椭圆数据半长轴
	/// @Author	Gan Shu Long
	/// @Date	2026-01-15
	/// @Input
	/// @Param	dSemiMajorAxis 椭圆数据半长轴
	/// @Return
	//********************************************************************
	void SetSemiMajorAxis(double dSemiMajorAxis);

	//********************************************************************
	/// 获取椭圆数据半长轴
	/// @Author	Gan Shu Long
	/// @Date	2026-01-15
	/// @Return 椭圆数据半长轴
	//********************************************************************
	double GetSemiMajorAxis();

	//********************************************************************
	/// 设置椭圆数据半短轴
	/// @Author	Gan Shu Long
	/// @Date	2026-01-15
	/// @Input
	/// @Param	dSemiMinorAxis 椭圆数据半短轴
	/// @Return
	//********************************************************************
	void SetSemiMinorAxis(double dSemiMinorAxis);

	//********************************************************************
	/// 获取椭圆数据半短轴
	/// @Author	Gan Shu Long
	/// @Date	2026-01-15
	/// @Return 椭圆数据半短轴
	//********************************************************************
	double GetSemiMinorAxis();

	//********************************************************************
	/// 设置椭圆数据方位角
	/// @Author	Gan Shu Long
	/// @Date	2026-01-15
	/// @Input
	/// @Param	dBearing 椭圆数据方位角
	/// @Return
	//********************************************************************
	void SetBearing(double dBearing);

	//********************************************************************
	/// 获取椭圆数据方位角
	/// @Author	Gan Shu Long
	/// @Date	2026-01-15
	/// @Return 椭圆数据方位角
	//********************************************************************
	double GetBearing();

private:
	AsTAtEllipse*					m_pAsTAtEllipse;						//C对象椭圆数据
};
