#pragma once
#include "../IAtkObj.h"
#include "IFmDefinition.h" 
#include "../ICEnum.h"
#include <vector>
#include <string>
class IFmDefinitionData;

///
/// 品质因子定义：多重覆盖
///
class IATKOBJECT_API IFmDefCompute :public IFmDefinition
{
public:
    IFmDefCompute(const IFmDefCompute&) = delete;
    IFmDefCompute& operator=(const IFmDefCompute&) = delete;

	IFmDefCompute(AsCvFmDefn* pAsCvFmDefn);
	~IFmDefCompute();

	//********************************************************************
	/// 获得 支持的计算类型
	/// @Author	Gan Shu Long
	/// @Date	2026-05-09
	/// @Return 支持的计算类型
	//********************************************************************
	virtual std::vector<std::string> GetComputeSupportedTypes();

	//********************************************************************
	/// 获得 是否支持计算类型
	/// @Author	Gan Shu Long
	/// @Date	2026-05-09
	/// @Input
	/// @Param	eFmCompute  计算类型
	/// @Return	bool 是否支持计算类型	
	//********************************************************************
	virtual bool IsComputeTypeSupported(EFmCompute eFmCompute);

	//********************************************************************
	/// 设置 计算类型
	/// @Author	Gan Shu Long
	/// @Date	2026-05-07
	/// @Input
	/// @Param	eFmCompute  计算类型
	/// @Return	
	//********************************************************************
	void SetComputeType(EFmCompute eFmCompute);

	//********************************************************************
	/// 获得  计算类型
	/// @Author	Gan Shu Long
	/// @Date	2026-05-07
	/// @Return	 计算类型
	//********************************************************************
	EFmCompute GetComputeType();

	//********************************************************************
	/// 获得  计算
	/// @Author	Gan Shu Long
	/// @Date	2026-05-07
	/// @Return	 计算
	//*******************************************************
	IFmDefinitionData* GetCompute() { return m_Compute; }

private:
	IFmDefinitionData*					m_Compute;
};
