#pragma once
#include "../IAtkObj.h"
#include "IRFFilterModel.h" 
#include "../ICEnum.h"
struct CCRFilter;

///
/// 滤波器 Chebyshev
///
class IATKOBJECT_API IRFFilterModelChebyshev :public IRFFilterModel
{
public:
    IRFFilterModelChebyshev(const IRFFilterModelChebyshev&) = delete;
    IRFFilterModelChebyshev& operator=(const IRFFilterModelChebyshev&) = delete;

	IRFFilterModelChebyshev(CCRFilter* pCCRFilter);
	virtual ~IRFFilterModelChebyshev();

	//********************************************************************
	/// 获得 滤波器名称
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Return	滤波器名称
	//********************************************************************
	std::string GetName() { return "Chebyshev"; }

	//********************************************************************
	/// 获得 滤波器类型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Return	滤波器类型
	//********************************************************************
	ERFFilterModelType GetType() { return eRFFilterModelTypeChebyshev; }

	//********************************************************************
	/// 设置  次数
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Input 
	/// @Param	iOrder 次数
	/// @Return	
	//********************************************************************
	void SetOrder(int iOrder);

	//********************************************************************
	/// 获得 次数
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Return	次数
	//********************************************************************
	int GetOrder();

	//********************************************************************
	/// 设置  截至频率
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Input 
	/// @Param	dCutoffFrequency 截至频率
	/// @Return	
	//********************************************************************
	void SetCutoffFrequency(double dCutoffFrequency);

	//********************************************************************
	/// 获得 截至频率
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Return	截至频率
	//********************************************************************
	double GetCutoffFrequency();

	//********************************************************************
	/// 设置  脉冲因子
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Input 
	/// @Param	dRipple 脉冲因子
	/// @Return	
	//********************************************************************
	void SetRipple(double dRipple);

	//********************************************************************
	/// 获得 脉冲因子
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Return	脉冲因子
	//********************************************************************
	double GetRipple();

private:
	CCRFilter*			 m_pCCRFilter;		//滤波器结构体
};
