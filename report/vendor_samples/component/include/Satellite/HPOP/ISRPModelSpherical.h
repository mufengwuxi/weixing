#pragma once
#include "ISRPModelBase.h"
class CSatellite;

///
/// 太阳光压
///
class IATKOBJECT_API ISRPModelSpherical : public ISRPModelBase
{
public:
    ISRPModelSpherical(const ISRPModelSpherical&) = delete;
    ISRPModelSpherical& operator=(const ISRPModelSpherical&) = delete;

	ISRPModelSpherical(CSatellite* pSate);
	~ISRPModelSpherical();

	//********************************************************************
	/// 设置HPOP轨道力模型中太阳光压的面积
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dAreaMassRatio 太阳光压面积
	/// @Return	
	//********************************************************************
	void SetAreaMassRatio(double dAreaMassRatio);
	//********************************************************************
	/// 获得HPOP轨道力模型中太阳光压的面积
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	太阳光压面积
	//********************************************************************
	double GetAreaMassRatio();

	//********************************************************************
	/// 设置HPOP轨道力模型中太阳光压的光压系数
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dCr 光压系数
	/// @Return	
	//********************************************************************
	void SetCr(double dCr);
	//********************************************************************
	/// 获得HPOP轨道力模型中太阳光压的光压系数
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	光压系数
	//********************************************************************
	double GetCr();

private:
	CSatellite*     m_pSate;

};
