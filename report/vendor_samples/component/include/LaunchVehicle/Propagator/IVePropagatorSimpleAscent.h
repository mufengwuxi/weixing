#pragma once
#include "../../Satellite/Propagator/IVePropagator.h"
struct CObjData;
class CLaunchVehicle;
class ICrdnEventIntervalSmartInterval;
class IVeLvInitialState;

///
/// 简单上升预报器
///
class IATKOBJECT_API IVePropagatorSimpleAscent : public IVePropagator
{
public:
    IVePropagatorSimpleAscent(const IVePropagatorSimpleAscent&) = delete;
    IVePropagatorSimpleAscent& operator=(const IVePropagatorSimpleAscent&) = delete;

	IVePropagatorSimpleAscent(CObjData* pObjData);
	~IVePropagatorSimpleAscent();

	//********************************************************************
	/// 获得轨道传播间隔
	/// @Author	Gan Shu Long
	/// @Date	2024-09-23
	/// @Return	轨道传播间隔			
	//********************************************************************
	ICrdnEventIntervalSmartInterval* GetEphemerisInterval() { return m_EphemerisInterval; }

	//********************************************************************
	/// 获得轨道初始状态
	/// @Author	Gan Shu Long
	/// @Date	2024-09-23
	/// @Return	轨道状态指针			
	//********************************************************************
	IVeLvInitialState* GetInitialState() { return m_InitialState; }

	//********************************************************************
	/// 获得轨道步长
	/// @Author	Gan Shu Long
	/// @Date	2024-09-23
	/// @Return	步长
	//********************************************************************
	double GetStep();

	//********************************************************************
	/// 设置轨道步长
	/// @Author	Gan Shu Long
	/// @Date	2024-09-23
	/// @Input
	/// @Param	dStep 步长
	/// @Return	
	//********************************************************************
	void SetStep(double dStep);

private:
	CLaunchVehicle*								m_pCLaunchVehicle;

	ICrdnEventIntervalSmartInterval*	m_EphemerisInterval;		//轨道传播间隔
	IVeLvInitialState*								m_InitialState;					//轨道初始状态
};
