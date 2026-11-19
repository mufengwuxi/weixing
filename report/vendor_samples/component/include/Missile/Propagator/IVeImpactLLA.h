#pragma once
#include "../../IAtkObj.h"
#include "IVeImpact.h"
class CLatLonAl;

///
/// 纬经高落点
///
class IATKOBJECT_API IVeImpactLLA : public IVeImpact
{
public:
    IVeImpactLLA(const IVeImpactLLA&) = delete;
    IVeImpactLLA& operator=(const IVeImpactLLA&) = delete;

	IVeImpactLLA(CLatLonAlt* pCLatLonAlt);
	~IVeImpactLLA();

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
