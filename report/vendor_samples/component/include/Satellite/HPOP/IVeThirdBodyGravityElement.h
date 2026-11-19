#pragma once
#include <string>
#include "../../IAtkObj.h"
#include "../../ICEnum.h"
class CSatellite;

///
/// 三体引力
///
class IATKOBJECT_API IVeThirdBodyGravityElement
{
public:
    IVeThirdBodyGravityElement(const IVeThirdBodyGravityElement&) = delete;
    IVeThirdBodyGravityElement& operator=(const IVeThirdBodyGravityElement&) = delete;

	IVeThirdBodyGravityElement(CSatellite* pSate, std::string strCentralBody);
	~IVeThirdBodyGravityElement();

	//********************************************************************
	/// 获得三体引力集合里的元素中心天体名称
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	中心天体名称
	//********************************************************************
	std::string GetCentralBody() { return m_CentralBody; }
	
	//********************************************************************
	/// 设置三体引力集合里的阶数
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dGravDegree 三体引力集合里的阶数
	/// @Return	
	//********************************************************************
	void SetGravityDegree(int dGravDegree);
	//********************************************************************
	/// 获得三体引力集合里的阶数
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	阶数
	//********************************************************************
	double GetGravityDegree();

	//********************************************************************
	/// 设置三体引力集合里的次数
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dGravOrder 三体引力集合里的次数
	/// @Return	
	//********************************************************************
	void SetGravityOrder(int dGravOrder);
	//********************************************************************
	/// 获得三体引力集合里的次数
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	次数
	//********************************************************************
	double GetGravityOrder();

	//********************************************************************
	/// 设置三体引力集合里的类型
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	eTHGType 三体引力集合里的类型
	/// @Return	
	//********************************************************************
	void SetType(EThirdBodyGravType eTHGType);
	//********************************************************************
	/// 获得三体引力集合里的类型
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	类型
	//********************************************************************
	EThirdBodyGravType GetType();

	//********************************************************************
	/// 设置三体引力集合里的模型
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	eTHGType 三体引力集合里的模型
	/// @Return	
	//********************************************************************
	void SetModel(std::string strModel);
	//********************************************************************
	/// 获得三体引力集合里的模型
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	模型
	//********************************************************************
	std::string GetModel();

	//********************************************************************
	/// 设置三体引力集合里的引力常数
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	eTHGType 三体引力集合里的引力常数
	/// @Return	
	//********************************************************************
	void SetGravityValue(double dGM);
	//********************************************************************
	/// 获得三体引力集合里的引力常数
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	引力常数
	//********************************************************************
	double GetGravityValue();

private:
	CSatellite*           m_pSate;

	std::string            m_CentralBody;     //三体引力集合里的元素中心天体名称

	int GetGravityID(std::string strCentralBody);

};

