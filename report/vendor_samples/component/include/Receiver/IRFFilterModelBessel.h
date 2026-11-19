#pragma once
#include "../IAtkObj.h"
#include "IRFFilterModel.h" 
#include "../ICEnum.h"
struct CCRFilter;

///
/// 滤波器 Bessel
///
class IATKOBJECT_API IRFFilterModelBessel :public IRFFilterModel
{
public:
    IRFFilterModelBessel(const IRFFilterModelBessel&) = delete;
    IRFFilterModelBessel& operator=(const IRFFilterModelBessel&) = delete;

	IRFFilterModelBessel(CCRFilter* pCCRFilter);
	virtual ~IRFFilterModelBessel();

	//********************************************************************
	/// 获得 滤波器名称
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Return	滤波器名称
	//********************************************************************
	std::string GetName() { return "Bessel"; }

	//********************************************************************
	/// 获得 滤波器类型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Return	滤波器类型
	//********************************************************************
	ERFFilterModelType GetType() { return eRFFilterModelTypeBessel; }

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

private:
	CCRFilter*			 m_pCCRFilter;		//滤波器结构体
};
