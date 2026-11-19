#pragma once
#include "../../IAtkObj.h"
#include "IVeImpactLocation.h"
struct CObjData;
class CMissile;

///
/// 发射仰角方位角落点
///
class IATKOBJECT_API IVeImpactLocationLaunchAzEl : public IVeImpactLocation
{
public:
    IVeImpactLocationLaunchAzEl(const IVeImpactLocationLaunchAzEl&) = delete;
    IVeImpactLocationLaunchAzEl& operator=(const IVeImpactLocationLaunchAzEl&) = delete;

	IVeImpactLocationLaunchAzEl(CObjData* pObjData);
	~IVeImpactLocationLaunchAzEl();

	//********************************************************************
	/// 获得速度增量
	/// @Author	Gan Shu Long
	/// @Date	2026-03-05
	/// @Return	速度增量
	//********************************************************************
	double GetDeltaV();

	//********************************************************************
	/// 设置速度增量
	/// @Author	Gan Shu Long
	/// @Date	2026-03-05
	/// @Input
	/// @Param	dDeltaV 速度增量
	/// @Return	
	//********************************************************************
	void SetDeltaV(double dDeltaV);

	//********************************************************************
	/// 获得发射仰角
	/// @Author	Gan Shu Long
	/// @Date	2026-03-05
	/// @Return	发射仰角
	//********************************************************************
	double GetElevation();

	//********************************************************************
	/// 设置发射仰角
	/// @Author	Gan Shu Long
	/// @Date	2026-03-05
	/// @Input
	/// @Param	dElevation 发射仰角
	/// @Return	
	//********************************************************************
	void SetElevation(double dElevation);

	//********************************************************************
	/// 获得发射点方位角
	/// @Author	Gan Shu Long
	/// @Date	2026-03-05
	/// @Return	发射点方位角
	//********************************************************************
	double GetAzimuth();

	//********************************************************************
	/// 设置发射点方位角
	/// @Author	Gan Shu Long
	/// @Date	2026-03-05
	/// @Input
	/// @Param	dAzimuth 发射点方位角
	/// @Return	
	//********************************************************************
	void SetAzimuth(double dAzimuth);

private:
	CObjData*											m_pObjData;
	CMissile*												m_pCMissile;
};
