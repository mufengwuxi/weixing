#pragma once
#include "../IAtkObj.h"
#include "IFmDefDilutionOfPrecision.h" 
class IFmUncertainties;

///
/// 品质因子定义：导航精度
///
class IATKOBJECT_API IFmDefNavigationAccuracy :public IFmDefDilutionOfPrecision
{
public:
    IFmDefNavigationAccuracy(const IFmDefNavigationAccuracy&) = delete;
    IFmDefNavigationAccuracy& operator=(const IFmDefNavigationAccuracy&) = delete;

	IFmDefNavigationAccuracy(AsCvFmDefn* pAsCvFmDefn);
	~IFmDefNavigationAccuracy();

	//********************************************************************
	/// 获得 支持的计算类型
	/// @Author	Gan Shu Long
	/// @Date	2026-05-11
	/// @Return 支持的计算类型
	//********************************************************************
	std::vector<std::string> GetComputeSupportedTypes() override;

	//********************************************************************
	/// 获得 是否支持计算类型
	/// @Author	Gan Shu Long
	/// @Date	2026-05-11
	/// @Input
	/// @Param	eFmCompute  计算类型
	/// @Return	bool 是否支持计算类型	
	//********************************************************************
	bool IsComputeTypeSupported(EFmCompute eFmCompute) override;

	//********************************************************************
	/// 获得  不确定度
	/// @Author	Gan Shu Long
	/// @Date	2026-05-07
	/// @Return	 不确定度
	//*******************************************************
	IFmUncertainties* GetUncertainties() { return m_Uncertainties; }

private:
	IFmUncertainties*					m_Uncertainties;
};
