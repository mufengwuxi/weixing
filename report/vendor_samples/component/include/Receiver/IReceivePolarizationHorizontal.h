#pragma once
#include "../IAtkObj.h"
#include "IPolarizationHorizontal.h"
struct CCRAntenna;

///
/// 水平极化
///
class IATKOBJECT_API IReceivePolarizationHorizontal :public IPolarizationHorizontal
{
public:
    IReceivePolarizationHorizontal(const IReceivePolarizationHorizontal&) = delete;
    IReceivePolarizationHorizontal& operator=(const IReceivePolarizationHorizontal&) = delete;

	IReceivePolarizationHorizontal(CCRAntenna* pCCRAntenna);
	~IReceivePolarizationHorizontal();

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
