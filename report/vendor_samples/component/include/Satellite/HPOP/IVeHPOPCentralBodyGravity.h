#pragma once
#include <string>
#include "../../IAtkObj.h"
class CSatellite;
class ISatellite;

///
/// 地球引力
///
class IATKOBJECT_API IVeHPOPCentralBodyGravity
{
public:
    IVeHPOPCentralBodyGravity(const IVeHPOPCentralBodyGravity&) = delete;
    IVeHPOPCentralBodyGravity& operator=(const IVeHPOPCentralBodyGravity&) = delete;

	IVeHPOPCentralBodyGravity(CSatellite* pSate, ISatellite* pISate);
	~IVeHPOPCentralBodyGravity();

	//********************************************************************
	/// 设置HPOP轨道力模型中地球引力的阶数和次数
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	nMaximumDegree   阶数
	/// @Param	nMaximumOrder    次数
	/// @Return				
	//********************************************************************
	void SetMaximumDegreeAndOrder(int nMaximumDegree, int nMaximumOrder);

	//********************************************************************
	/// 设置HPOP轨道力模型中地球引力模型
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	strGravModel 地球引力模型
	/// @Return	
	//********************************************************************
	void SetGravModel(std::string strGravModel);
	//********************************************************************
	/// 获得HPOP轨道力模型中地球引力模型
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	地球引力模型
	//********************************************************************
	std::string GetGravModel();

	//********************************************************************
	/// 设置HPOP轨道力模型中地球引力的阶数
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	nDegree 阶数
	/// @Return	
	//********************************************************************
	void SetMaxDegree(int nDegree);
	//********************************************************************
	/// 获得HPOP轨道力模型中地球引力的阶数
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	阶数
	//********************************************************************
	int GetMaxDegree();

	//********************************************************************
	/// 设置HPOP轨道力模型中地球引力的次数
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	nOrder 次数
	/// @Return	
	//********************************************************************
	void SetMaxOrder(int nOrder);
	//********************************************************************
	/// 获得HPOP轨道力模型中地球引力的次数
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	次数
	//********************************************************************
	int GetMaxOrder();

private:
	CSatellite*     m_pSate;
	//ISatellite*     m_pISate;
};
