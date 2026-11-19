#pragma once
#include "../../IAtkObj.h"
class CSatellite;
class ICrdnEventSmartEpoch;

///
/// 轨道坐标系统
///
class IATKOBJECT_API IOrbitStateCoordinateSystem
{
public:
	IOrbitStateCoordinateSystem(const IOrbitStateCoordinateSystem&) = delete;
	IOrbitStateCoordinateSystem& operator=(const IOrbitStateCoordinateSystem&) = delete;

	IOrbitStateCoordinateSystem(CSatellite* pSate);
	virtual ~IOrbitStateCoordinateSystem();

	//********************************************************************
	/// 获取 坐标历元时间
	/// @Author	Gan Shu Long
	/// @Date	2026-10-22
	/// @Input
	/// @Return	坐标历元时间
	//********************************************************************
	ICrdnEventSmartEpoch* GetCoordinateSystemEpoch() { return m_CoordinateSystemEpoch; }


private:
	ICrdnEventSmartEpoch*	m_CoordinateSystemEpoch;		//坐标历元时间
};
