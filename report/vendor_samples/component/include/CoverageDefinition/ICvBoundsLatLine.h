#pragma once
#include "../IAtkObj.h"
#include "ICvBounds.h"
class CCoverageDefinition;

///
/// 纬度线
///
class IATKOBJECT_API ICvBoundsLatLine :public ICvBounds
{
public:
    ICvBoundsLatLine(const ICvBoundsLatLine&) = delete;
    ICvBoundsLatLine& operator=(const ICvBoundsLatLine&) = delete;

	ICvBoundsLatLine(CCoverageDefinition* pCCoverageDefinition);
	~ICvBoundsLatLine();

	//********************************************************************
	/// 设置 纬度
	/// @Author	Gan Shu Long
	/// @Date	2023-12-11
	/// @Input 
	/// @Param	double 新值
	/// @Return	
	//********************************************************************
	void SetLatitude(double latitude);

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
	/// 获取 纬度
	/// @Author	Gan Shu Long
	/// @Date	2023-12-11
	/// @Return	 纬度
	//********************************************************************
	double GetLatitude();

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
