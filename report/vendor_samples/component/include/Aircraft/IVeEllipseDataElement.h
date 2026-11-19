#pragma once
#include "../IAtkObj.h"
#include "../ICEnum.h"
#include <string>
struct CVehEllipse;
///
/// 椭圆数据
///
class IATKOBJECT_API IVeEllipseDataElement
{
public:
    IVeEllipseDataElement(const IVeEllipseDataElement&) = delete;
    IVeEllipseDataElement& operator=(const IVeEllipseDataElement&) = delete;

	IVeEllipseDataElement(CVehEllipse* pCVehEllipse);
	~IVeEllipseDataElement();

	//********************************************************************
	/// 设置椭圆数据时间
	/// @Author	Gan Shu Long
	/// @Date	2026-01-08
	/// @Input
	/// @Param	strTime 椭圆数据时间
	/// @Return
	//********************************************************************
	void SetTime(std::string strTime);

	//********************************************************************
	/// 获取椭圆数据时间
	/// @Author	Gan Shu Long
	/// @Date	2026-01-08
	/// @Return 椭圆数据时间
	//********************************************************************
	std::string GetTime();

	//********************************************************************
	/// 设置椭圆数据是否自定义位置
	/// @Author	Gan Shu Long
	/// @Date	2026-01-08
	/// @Input
	/// @Param	bCustomPosition 椭圆数据是否自定义位置
	/// @Return
	//********************************************************************
	void SetCustomPosition(bool bCustomPosition);

	//********************************************************************
	/// 获取椭圆数据是否自定义位置
	/// @Author	Gan Shu Long
	/// @Date	2026-01-08
	/// @Return 椭圆数据是否自定义位置
	//********************************************************************
	bool GetCustomPosition();

	//********************************************************************
	/// 设置椭圆数据纬度
	/// @Author	Gan Shu Long
	/// @Date	2026-01-08
	/// @Input
	/// @Param	dLatitude 椭圆数据纬度
	/// @Return
	//********************************************************************
	void SetLatitude(double dLatitude);

	//********************************************************************
	/// 获取椭圆数据纬度
	/// @Author	Gan Shu Long
	/// @Date	2026-01-08
	/// @Return 椭圆数据纬度
	//********************************************************************
	double GetLatitude();

	//********************************************************************
	/// 设置椭圆数据经度
	/// @Author	Gan Shu Long
	/// @Date	2026-01-08
	/// @Input
	/// @Param	dLongitude 椭圆数据经度
	/// @Return
	//********************************************************************
	void SetLongitude(double dLongitude);

	//********************************************************************
	/// 获取椭圆数据经度
	/// @Author	Gan Shu Long
	/// @Date	2026-01-08
	/// @Return 椭圆数据经度
	//********************************************************************
	double GetLongitude();

	//********************************************************************
	/// 设置椭圆数据半长轴
	/// @Author	Gan Shu Long
	/// @Date	2026-01-08
	/// @Input
	/// @Param	dSemiMajorAxis 椭圆数据半长轴
	/// @Return
	//********************************************************************
	void SetSemiMajorAxis(double dSemiMajorAxis);

	//********************************************************************
	/// 获取椭圆数据半长轴
	/// @Author	Gan Shu Long
	/// @Date	2026-01-08
	/// @Return 椭圆数据半长轴
	//********************************************************************
	double GetSemiMajorAxis();

	//********************************************************************
	/// 设置椭圆数据半短轴
	/// @Author	Gan Shu Long
	/// @Date	2026-01-08
	/// @Input
	/// @Param	dSemiMinorAxis 椭圆数据半短轴
	/// @Return
	//********************************************************************
	void SetSemiMinorAxis(double dSemiMinorAxis);

	//********************************************************************
	/// 获取椭圆数据半短轴
	/// @Author	Gan Shu Long
	/// @Date	2026-01-08
	/// @Return 椭圆数据半短轴
	//********************************************************************
	double GetSemiMinorAxis();

	//********************************************************************
	/// 设置椭圆数据方位角
	/// @Author	Gan Shu Long
	/// @Date	2026-01-08
	/// @Input
	/// @Param	dBearing 椭圆数据方位角
	/// @Return
	//********************************************************************
	void SetBearing(double dBearing);

	//********************************************************************
	/// 获取椭圆数据方位角
	/// @Author	Gan Shu Long
	/// @Date	2026-01-08
	/// @Return 椭圆数据方位角
	//********************************************************************
	double GetBearing();

private:
	CVehEllipse*					m_pCVehEllipse;						//C对象椭圆数据
};
