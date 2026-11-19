#pragma once
#include "IClassicalSizeShape.h"
class CSatellite;

///
/// 轨道根数周期和偏心率
///
class IATKOBJECT_API IClassicalSizeShapePeriod : public IClassicalSizeShape
{
public:
    IClassicalSizeShapePeriod(const IClassicalSizeShapePeriod&) = delete;
    IClassicalSizeShapePeriod& operator=(const IClassicalSizeShapePeriod&) = delete;

	IClassicalSizeShapePeriod(CSatellite* pSate);
	~IClassicalSizeShapePeriod();

	//********************************************************************
	/// 设置偏心率
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dEcc 偏心率
	/// @Return	
	//********************************************************************
	void SetEccentricity(double dEcc);
	//********************************************************************
	/// 获得偏心率
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	偏心率
	//********************************************************************
	double GetEccentricity();

	//********************************************************************
	/// 设置周期
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dPeriod 周期
	/// @Return	
	//********************************************************************
	void SetPeriod(double dPeriod);
	//********************************************************************
	/// 获得周期
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	周期
	//********************************************************************
	double GetPeriod();

private:
	CSatellite* m_pSate;

};
