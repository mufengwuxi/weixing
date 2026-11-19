#pragma once
#include "../IAtkObj.h"
#include "ICvBounds.h"
class CCoverageDefinition;

///
/// 纬度边界
///
class IATKOBJECT_API ICvBoundsLat :public ICvBounds
{
public:
    ICvBoundsLat(const ICvBoundsLat&) = delete;
    ICvBoundsLat& operator=(const ICvBoundsLat&) = delete;

	ICvBoundsLat(CCoverageDefinition* pCCoverageDefinition);
	~ICvBoundsLat();

	//********************************************************************
	/// 设置 最小纬度
	/// @Author	Gan Shu Long
	/// @Date	2023-12-11
	/// @Input 
	/// @Param	double 新值
	/// @Return	
	//********************************************************************
	void SetMinLatitude(double minLatitude);

	//********************************************************************
	/// 设置 最大纬度
	/// @Author	Gan Shu Long
	/// @Date	2023-12-11
	/// @Input 
	/// @Param	double 新值
	/// @Return	
	//********************************************************************
	void SetMaxLatitude(double maxLatitude);

	//********************************************************************
	/// 获取 最小纬度
	/// @Author	Gan Shu Long
	/// @Date	2023-12-11
	/// @Return	 最小纬度
	//********************************************************************
	double GetMinLatitude();

	//********************************************************************
	/// 获取 最大纬度
	/// @Author	Gan Shu Long
	/// @Date	2023-12-11
	/// @Return	最大纬度
	//********************************************************************
	double GetMaxLatitude();
	
private:
	CCoverageDefinition* m_pCCoverageDefinition;

};
