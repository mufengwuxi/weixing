#pragma once
#include "../../IAtkObj.h"
#include "../../ICEnum.h"
class CSatellite;
class ISRPModelBase;

///
/// 太阳光压模型
///
class IATKOBJECT_API IVeHPOPSRPModel
{
public:
    IVeHPOPSRPModel(const IVeHPOPSRPModel&) = delete;
    IVeHPOPSRPModel& operator=(const IVeHPOPSRPModel&) = delete;

	IVeHPOPSRPModel(CSatellite* pSate);
	~IVeHPOPSRPModel();

	//********************************************************************
	/// 设置HPOP轨道力模型中太阳光压模型类型
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	eSRPModel 太阳光压模型类型
	/// @Return	
	//********************************************************************
	void SetModelType(ESRPModel eSRPModel);

	//********************************************************************
	/// 获得HPOP轨道力模型中太阳光压模型属性
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	太阳光压模型属性指针
	//********************************************************************
	ISRPModelBase* GetModel();

	//********************************************************************
	/// 获得HPOP轨道力模型中太阳光压模型类型
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	太阳光压模型类型
	//********************************************************************
	ESRPModel GetModelType();

private:
	CSatellite*        m_pSate;

	ISRPModelBase*     m_Model;       //太阳光压模型属性

};
