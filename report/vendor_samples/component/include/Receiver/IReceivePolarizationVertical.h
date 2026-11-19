#pragma once
#include "../IAtkObj.h"
#include "IPolarizationVertical.h"
struct CCRAntenna;

///
/// 垂直极化
///
class IATKOBJECT_API IReceivePolarizationVertical :public IPolarizationVertical
{
public:
    IReceivePolarizationVertical(const IReceivePolarizationVertical&) = delete;
    IReceivePolarizationVertical& operator=(const IReceivePolarizationVertical&) = delete;

	IReceivePolarizationVertical(CCRAntenna* pCCRAntenna);
	~IReceivePolarizationVertical();

	//********************************************************************
	/// 设置  交叉极化泄露阈值
	/// @Author	Gan Shu Long
	/// @Date	2023-12-11
	/// @Input 
	/// @Param	dCrossPolLeakage 交叉极化泄露阈值
	/// @Return	
	//********************************************************************
	void SetCrossPolLeakage(double dCrossPolLeakage);
	
	//********************************************************************
	/// 获得 交叉极化泄露阈值
	/// @Author	Gan Shu Long
	/// @Date	2026-03-10
	/// @Return	交叉极化泄露阈值
	//********************************************************************
	double GetCrossPolLeakage();

private:
	CCRAntenna*			 m_pCCRAntenna;		//天线结构体
};
