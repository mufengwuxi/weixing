#pragma once
#include "../../IAtkObj.h"
#include "IVeLaunchControl.h"

///
/// 增量：固定飞行时间
///
class IATKOBJECT_API IVeLaunchControlFixedTimeOfFlight : public IVeLaunchControl
{
public:
    IVeLaunchControlFixedTimeOfFlight(const IVeLaunchControlFixedTimeOfFlight&) = delete;
    IVeLaunchControlFixedTimeOfFlight& operator=(const IVeLaunchControlFixedTimeOfFlight&) = delete;

	IVeLaunchControlFixedTimeOfFlight(double* pTimeOfFlight);
	~IVeLaunchControlFixedTimeOfFlight();

	//********************************************************************
	/// 获得飞行时间增量
	/// @Author	Gan Shu Long
	/// @Date	2026-03-05
	/// @Return	飞行时间增量
	//********************************************************************
	double GetTimeOfFlight();

	//********************************************************************
	/// 设置飞行时间增量
	/// @Author	Gan Shu Long
	/// @Date	2026-03-05
	/// @Input
	/// @Param	dTimeOfFlight 飞行时间增量
	/// @Return	
	//********************************************************************
	void SetTimeOfFlight(double dTimeOfFlight);

private:
	double*			m_pTimeOfFlight;
};
