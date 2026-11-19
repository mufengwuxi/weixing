#pragma once
#include "IVePropagator.h"
struct CObjData;
class CObjManage;
class CSatellite;
class ISatellite;
class IVeInitialState;
class ICrdnEventIntervalSmartInterval;

///
/// 二体预报器
///
class IATKOBJECT_API IVePropagatorTwoBody : public IVePropagator
{
public:
    IVePropagatorTwoBody(const IVePropagatorTwoBody&) = delete;
    IVePropagatorTwoBody& operator=(const IVePropagatorTwoBody&) = delete;

	IVePropagatorTwoBody(CObjData* pObjData, ISatellite* pISate, CObjManage* IAsGObjManage);
	~IVePropagatorTwoBody();

	//********************************************************************
	/// 获得二体轨道轨道状态
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	轨道状态指针			
	//********************************************************************
	IVeInitialState* GetInitialState();

	//********************************************************************
	/// 设置HPOP轨道模型步长
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dStep 步长
	/// @Return	
	//********************************************************************
	void SetStep(double dStep);
	//********************************************************************
	/// 获得HPOP轨道模型步长
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	步长
	//********************************************************************
	double GetStep();

	//********************************************************************
	/// 获取 历元时间间隔
	/// @Author	Gan Shu Long
	/// @Date	2026-10-21
	/// @Input
	/// @Return	历元时间间隔
	//********************************************************************
	ICrdnEventIntervalSmartInterval* GetEphemerisInterval() { return m_EphemerisInterval; }

private:
	CObjManage*            m_IAsGObjManage;
	CObjData*              m_pObjData;
	CSatellite*            m_pSate;
	ISatellite*            m_pISate;

	IVeInitialState*					m_InitialState;			//轨道轨道状态
	ICrdnEventIntervalSmartInterval*	m_EphemerisInterval;	//历元时间间隔

};
