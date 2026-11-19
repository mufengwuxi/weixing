#pragma once
#include "IClassicalSizeShape.h"
class CSatellite;

///
/// 轨道根数平均运动和偏心率
///
class IATKOBJECT_API IClassicalSizeShapeMeanMotion : public IClassicalSizeShape
{
public:
    IClassicalSizeShapeMeanMotion(const IClassicalSizeShapeMeanMotion&) = delete;
    IClassicalSizeShapeMeanMotion& operator=(const IClassicalSizeShapeMeanMotion&) = delete;

	IClassicalSizeShapeMeanMotion(CSatellite* pSate);
	~IClassicalSizeShapeMeanMotion();

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
	/// 设置平均运动
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dMeanMotion 平均运动
	/// @Return	
	//********************************************************************
	void SetMeanMotion(double dMeanMotion);
	//********************************************************************
	/// 获得平均运动
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	平均运动
	//********************************************************************
	double GetMeanMotion();

private:
	CSatellite* m_pSate;
};
