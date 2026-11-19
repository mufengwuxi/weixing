#pragma once
#include "../../IAtkObj.h"
class ILLAPosition;
class CLaunchVehicle;

///
/// 轨道初始状态
///
class IATKOBJECT_API IVeLvInitialState
{
public:
    IVeLvInitialState(const IVeLvInitialState&) = delete;
    IVeLvInitialState& operator=(const IVeLvInitialState&) = delete;

	IVeLvInitialState(CLaunchVehicle* pCLaunchVehicle);
	~IVeLvInitialState();

	//********************************************************************
	/// 获得发射点LLA
	/// @Author	Gan Shu Long
	/// @Date	2024-09-23
	/// @Return	发射点LLA
	//********************************************************************
	ILLAPosition*  GetLaunch() { return m_Launch; }

	//********************************************************************
	/// 获得发射终点LLA
	/// @Author	Gan Shu Long
	/// @Date	2024-09-23
	/// @Return	发射终点LLA
	//********************************************************************
	ILLAPosition*  GetBurnout() { return m_Burnout; }

	//********************************************************************
	/// 获得发射终点速度大小(m/s)
	/// @Author	Gan Shu Long
	/// @Date	2024-09-23
	/// @Return	发射终点速度大小(m/s)
	//********************************************************************
	double  GetBurnoutVel();

	//********************************************************************
	/// 设置发射终点速度大小(m/s)
	/// @Author	Gan Shu Long
	/// @Date	2024-09-23
	/// @Input
	/// @Param	burnoutVel 发射终点速度大小(m/s)
	/// @Return	
	//********************************************************************
	void SetBurnoutVel(double  burnoutVel);

private:
	CLaunchVehicle*		m_pCLaunchVehicle;

	ILLAPosition*			m_Launch;						//发射点LLA
	ILLAPosition*			m_Burnout;						//发射终点LLA
};

