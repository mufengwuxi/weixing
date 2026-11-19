#pragma once
#include "../IAtkObj.h"
#include "IRFFilterModel.h" 
#include "../ICEnum.h"
struct CCRFilter;

///
/// 滤波器 GaussianWindows
///
class IATKOBJECT_API IRFFilterModelGaussianWindow :public IRFFilterModel
{
public:
    IRFFilterModelGaussianWindow(const IRFFilterModelGaussianWindow&) = delete;
    IRFFilterModelGaussianWindow& operator=(const IRFFilterModelGaussianWindow&) = delete;

	IRFFilterModelGaussianWindow(CCRFilter* pCCRFilter);
	virtual ~IRFFilterModelGaussianWindow();

	//********************************************************************
	/// 获得 滤波器名称
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Return	滤波器名称
	//********************************************************************
	std::string GetName() { return "GaussianWindows"; }

	//********************************************************************
	/// 获得 滤波器类型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Return	滤波器类型
	//********************************************************************
	ERFFilterModelType GetType() { return eRFFilterModelTypeGaussianWindow; }													

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
	/// 设置  采样频率
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Input 
	/// @Param	dSamplingFrequency 采样频率
	/// @Return	
	//********************************************************************
	void SetSamplingFrequency(double dSamplingFrequency);

	//********************************************************************
	/// 获得 采样频率
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Return	采样频率
	//********************************************************************
	double GetSamplingFrequency();

private:
	CCRFilter*			 m_pCCRFilter;		//滤波器结构体
};
