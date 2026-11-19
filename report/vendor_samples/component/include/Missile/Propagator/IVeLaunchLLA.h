#pragma once
#include "../../IAtkObj.h"
#include "IVeLaunch.h"
class CLatLonAl;

///
/// 纬经高发射点
///
class IATKOBJECT_API IVeLaunchLLA : public IVeLaunch
{
public:
    IVeLaunchLLA(const IVeLaunchLLA&) = delete;
    IVeLaunchLLA& operator=(const IVeLaunchLLA&) = delete;

	IVeLaunchLLA(CLatLonAlt* pCLatLonAlt);
	~IVeLaunchLLA();

	//********************************************************************
	/// 获得纬度
	/// @Author	Gan Shu Long
	/// @Date	2026-03-05
	/// @Return	纬度
	//********************************************************************
	double GetLat();

	//********************************************************************
	/// 设置纬度
	/// @Author	Gan Shu Long
	/// @Date	2026-03-05
	/// @Input
	/// @Param	dLat 纬度
	/// @Return	
	//********************************************************************
	void SetLat(double dLat);

	//********************************************************************
	/// 获得经度
	/// @Author	Gan Shu Long
	/// @Date	2026-03-05
	/// @Return	经度
	//********************************************************************
	double GetLon();

	//********************************************************************
	/// 设置经度
	/// @Author	Gan Shu Long
	/// @Date	2026-03-05
	/// @Input
	/// @Param	dLon 经度
	/// @Return	
	//********************************************************************
	void SetLon(double dLon);

	//********************************************************************
	/// 获得高度
	/// @Author	Gan Shu Long
	/// @Date	2026-03-05
	/// @Return	高度
	//********************************************************************
	double GetAlt();

	//********************************************************************
	/// 设置高度
	/// @Author	Gan Shu Long
	/// @Date	2026-03-05
	/// @Input
	/// @Param	dAlt 高度
	/// @Return	
	//********************************************************************
	void SetAlt(double dAlt);

private:
	CLatLonAlt*			m_pCLatLonAlt;
};
