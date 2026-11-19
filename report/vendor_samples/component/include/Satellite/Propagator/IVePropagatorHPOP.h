#pragma once
#include "../../IAtkObj.h"
#include "IVePropagator.h"
struct CObjData;
class CObjManage;
class CSatellite;
class ISatellite;
class IVeHPOPForceModel;
class IVeInitialState;
class IVeIntegrator;
class ICrdnEventIntervalSmartInterval;

///
/// HPOP预报器
///
class IATKOBJECT_API IVePropagatorHPOP : public IVePropagator
{
public:
    IVePropagatorHPOP(const IVePropagatorHPOP&) = delete;
    IVePropagatorHPOP& operator=(const IVePropagatorHPOP&) = delete;

	IVePropagatorHPOP(CObjData* pObjData, ISatellite* pISate, CObjManage* IAsGObjManage);
	~IVePropagatorHPOP();

	//********************************************************************
	/// 获得HPOP轨道力模型属性
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	力模型属性指针			
	//********************************************************************
	IVeHPOPForceModel* GetForceModel();

	//********************************************************************
	/// 获得HPOP轨道轨道状态
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	轨道状态指针			
	//********************************************************************
	IVeInitialState* GetInitialState();

	//********************************************************************
	/// 获得HPOP轨道积分算法属性
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	积分算法属性指针			
	//********************************************************************
	IVeIntegrator* GetIntegrator();

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

	IVeHPOPForceModel*     m_ForceModel;     //HPOP轨道力模型属性
	IVeInitialState*       m_InitialState;   //HPOP轨道轨道状态
	IVeIntegrator*         m_Integrator;     //HPOP轨道积分算法属性
	ICrdnEventIntervalSmartInterval*	m_EphemerisInterval;	//历元时间间隔

};
