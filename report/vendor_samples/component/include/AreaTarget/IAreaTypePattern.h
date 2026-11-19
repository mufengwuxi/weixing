#pragma once
#include "../IAtkObj.h"
class CLatLonAlt;

///
/// 区域边界坐标
///
class IATKOBJECT_API IAreaTypePattern
{
public:
    IAreaTypePattern(const IAreaTypePattern&) = delete;
    IAreaTypePattern& operator=(const IAreaTypePattern&) = delete;

	IAreaTypePattern(CLatLonAlt* pCLatLonAlt);
	~IAreaTypePattern();

	//********************************************************************
	/// 设置维度
	/// @Author	Gan Shu Long
	/// @Date	2026-01-16
	/// @Input
	/// @Param	dLat 维度
	/// @Return
	//********************************************************************
	void SetLat(double dLat);

	//********************************************************************
	/// 获取维度
	/// @Author	Gan Shu Long
	/// @Date	2026-01-16
	/// @Return 维度
	//********************************************************************
	double GetLat();

	//********************************************************************
	/// 设置经度
	/// @Author	Gan Shu Long
	/// @Date	2026-01-16
	/// @Input
	/// @Param	dLon 经度
	/// @Return
	//********************************************************************
	void SetLon(double dLon);

	//********************************************************************
	/// 获取经度
	/// @Author	Gan Shu Long
	/// @Date	2026-01-16
	/// @Return 经度
	//********************************************************************
	double GetLon();

private:
	CLatLonAlt*		m_pCLatLonAlt;
};
