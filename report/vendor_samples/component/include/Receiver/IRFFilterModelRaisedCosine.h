#pragma once
#include "../IAtkObj.h"
#include "IRFFilterModel.h" 
#include "../ICEnum.h"
struct CCRFilter;

///
/// 滤波器 RaisedCosine
///
class IATKOBJECT_API IRFFilterModelRaisedCosine :public IRFFilterModel
{
public:
    IRFFilterModelRaisedCosine(const IRFFilterModelRaisedCosine&) = delete;
    IRFFilterModelRaisedCosine& operator=(const IRFFilterModelRaisedCosine&) = delete;

	IRFFilterModelRaisedCosine(CCRFilter* pCCRFilter);
	virtual ~IRFFilterModelRaisedCosine();

	//********************************************************************
	/// 获得 滤波器名称
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Return	滤波器名称
	//********************************************************************
	std::string GetName() { return "RaisedCosine"; }

	//********************************************************************
	/// 获得 滤波器类型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Return	滤波器类型
	//********************************************************************
	ERFFilterModelType GetType() { return eRFFilterModelTypeRaisedCosine; }
																	

	//********************************************************************
	/// 设置  滚降因子
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Input 
	/// @Param	dRollOffFactor 滚降因子
	/// @Return	
	//********************************************************************
	void SetRollOffFactorr(double dRollOffFactor);

	//********************************************************************
	/// 获得 滚降因子
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Return	滚降因子
	//********************************************************************
	double GetRollOffFactor();

	//********************************************************************
	/// 设置  符号率
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Input 
	/// @Param	dSymbolRate 符号率
	/// @Return	
	//********************************************************************
	void SetSymbolRate(double dSymbolRate);

	//********************************************************************
	/// 获得 符号率
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Return	符号率
	//********************************************************************
	double GetSymbolRate();

private:
	CCRFilter*			 m_pCCRFilter;		//滤波器结构体
};
