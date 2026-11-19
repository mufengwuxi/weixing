#pragma once
#include "../IAtkObj.h"
#include "ICvResolution.h"
class CCoverageDefinition;

///
/// 网格定义：经纬度
///
class IATKOBJECT_API ICvResolutionLatLon :public ICvResolution
{
public:
    ICvResolutionLatLon(const ICvResolutionLatLon&) = delete;
    ICvResolutionLatLon& operator=(const ICvResolutionLatLon&) = delete;

	ICvResolutionLatLon(CCoverageDefinition* pCCoverageDefinition);
	~ICvResolutionLatLon();

	//********************************************************************
	/// 设置 经纬度网格分辨率
	/// @Author	Gan Shu Long
	/// @Date	2023-12-11
	/// @Input 
	/// @Param	double 新值
	/// @Return	
	//********************************************************************
	void SetLatLon(double latLon);
	
	//********************************************************************
	/// 获取 经纬度网格分辨率
	/// @Author	Gan Shu Long
	/// @Date	2023-12-11
	/// @Return	 经纬度网格分辨率
	//********************************************************************
	double GetLatLon();

private:
	CCoverageDefinition* m_pCCoverageDefinition;
};
