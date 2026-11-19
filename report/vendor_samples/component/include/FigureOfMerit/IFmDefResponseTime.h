#pragma once
#include "../IAtkObj.h"
#include "IFmDefCompute.h" 

///
/// 品质因子定义：响应时间
///
class IATKOBJECT_API IFmDefResponseTime :public IFmDefCompute
{
public:
    IFmDefResponseTime(const IFmDefResponseTime&) = delete;
    IFmDefResponseTime& operator=(const IFmDefResponseTime&) = delete;

	IFmDefResponseTime(AsCvFmDefn* pAsCvFmDefn);
	~IFmDefResponseTime();

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
	/// 设置  最少重数
	/// @Author	Gan Shu Long
	/// @Date	2026-05-07
	/// @Input 
	/// @Param	nMinAssets 最少重数
	/// @Return	
	//********************************************************************
	void SetMinAssets(int nMinAssets);

	//********************************************************************
	/// 获得 最少重数
	/// @Author	Gan Shu Long
	/// @Date	2026-05-07
	/// @Return	最少重数
	//********************************************************************
	int GetMinAssets();
};
