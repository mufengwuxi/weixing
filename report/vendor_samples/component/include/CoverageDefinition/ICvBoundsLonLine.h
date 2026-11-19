#pragma once
#include "../IAtkObj.h"
#include "ICvBounds.h"
class CCoverageDefinition;

///
/// 纬度线
///
class IATKOBJECT_API ICvBoundsLonLine :public ICvBounds
{
public:
    ICvBoundsLonLine(const ICvBoundsLonLine&) = delete;
    ICvBoundsLonLine& operator=(const ICvBoundsLonLine&) = delete;

	ICvBoundsLonLine(CCoverageDefinition* pCCoverageDefinition);
	~ICvBoundsLonLine();

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
	/// 设置 经度
	/// @Author	Gan Shu Long
	/// @Date	2023-12-11
	/// @Input 
	/// @Param	double 新值
	/// @Return	
	//********************************************************************
	void SetLongitude(double longitude);
	
	//********************************************************************
	/// 获取 最大纬度
	/// @Author	Gan Shu Long
	/// @Date	2023-12-11
	/// @Return	 最大纬度
	//********************************************************************
	double GetMinLatitude();

	//********************************************************************
	/// 获取 最小纬度
	/// @Author	Gan Shu Long
	/// @Date	2023-12-11
	/// @Return	 最小纬度
	//********************************************************************
	double GetMaxLatitude();

	//********************************************************************
	/// 获取 经度
	/// @Author	Gan Shu Long
	/// @Date	2023-12-11
	/// @Return	 经度
	//********************************************************************
	double GetLongitude();

private:
	CCoverageDefinition* m_pCCoverageDefinition;
};
