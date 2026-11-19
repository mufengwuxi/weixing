#pragma once
#include "../IAtkObj.h"
#include "IFmDefCompute.h" 

///
/// 品质因子定义：覆盖时间
///
class IATKOBJECT_API IFmDefCoverageTime :public IFmDefCompute
{
public:
    IFmDefCoverageTime(const IFmDefCoverageTime&) = delete;
    IFmDefCoverageTime& operator=(const IFmDefCoverageTime&) = delete;

	IFmDefCoverageTime(AsCvFmDefn* pAsCvFmDefn);
	~IFmDefCoverageTime();

	//********************************************************************
	/// 获得 支持的计算类型
	/// @Author	Gan Shu Long
	/// @Date	2026-05-09
	/// @Return 支持的计算类型
	//********************************************************************
	std::vector<std::string> GetComputeSupportedTypes() override;

	//********************************************************************
	/// 获得 是否支持计算类型
	/// @Author	Gan Shu Long
	/// @Date	2026-05-09
	/// @Input
	/// @Param	eFmCompute  计算类型
	/// @Return	bool 是否支持计算类型	
	//********************************************************************
	bool IsComputeTypeSupported(EFmCompute eFmCompute) override;
};
