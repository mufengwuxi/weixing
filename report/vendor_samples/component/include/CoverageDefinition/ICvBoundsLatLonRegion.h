#pragma once
#include "../IAtkObj.h"
#include "ICvBounds.h"
class CCoverageDefinition;

///
/// 经纬度区域
///
class IATKOBJECT_API ICvBoundsLatLonRegion :public ICvBounds
{
public:
    ICvBoundsLatLonRegion(const ICvBoundsLatLonRegion&) = delete;
    ICvBoundsLatLonRegion& operator=(const ICvBoundsLatLonRegion&) = delete;

	ICvBoundsLatLonRegion(CCoverageDefinition* pCCoverageDefinition);
	~ICvBoundsLatLonRegion();

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
	/// 设置 最小经度
	/// @Author	Gan Shu Long
	/// @Date	2023-12-11
	/// @Input 
	/// @Param	double 新值
	/// @Return	
	//********************************************************************
	void SetMinLongitude(double minLongitude);

	//********************************************************************
	/// 设置 最大经度
	/// @Author	Gan Shu Long
	/// @Date	2023-12-11
	/// @Input 
	/// @Param	double 新值
	/// @Return	
	//********************************************************************
	void SetMaxLongitude(double maxLongitude);

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
	/// @Return	 最大纬度
	//********************************************************************
	double GetMaxLatitude();

	//********************************************************************
	/// 获取 最小经度
	/// @Author	Gan Shu Long
	/// @Date	2023-12-11
	/// @Return	 最小经度
	//********************************************************************
	double GetMinLongitude();

	//********************************************************************
	/// 获取 最大经度
	/// @Author	Gan Shu Long
	/// @Date	2023-12-11
	/// @Return	 最大经度
	//********************************************************************
	double GetMaxLongitude();

private:
	CCoverageDefinition* m_pCCoverageDefinition;
};
