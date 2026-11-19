#pragma once
#include "../IAtkObj.h"
#include "../ICEnum.h"
#include "IFmDefCompute.h" 

///
/// 品质因子定义：可见约束
///
class IATKOBJECT_API IFmDefAccessConstraint :public IFmDefCompute
{
public:
    IFmDefAccessConstraint(const IFmDefAccessConstraint&) = delete;
    IFmDefAccessConstraint& operator=(const IFmDefAccessConstraint&) = delete;

	IFmDefAccessConstraint(AsCvFmDefn* pAsCvFmDefn);
	~IFmDefAccessConstraint();

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
	/// 设置 约束类型
	/// @Author	Gan Shu Long
	/// @Date	2026-05-08
	/// @Input
	/// @Param	eFmConstraint  约束类型
	/// @Return	
	//********************************************************************
	void SetConstraint(EFmConstraint eFmConstraint);

	//********************************************************************
	/// 获得  约束类型
	/// @Author	Gan Shu Long
	/// @Date	2026-05-08
	/// @Return	 约束类型
	//********************************************************************
	EFmConstraint GetConstraint();

	//********************************************************************
	/// 设置 跨对象类型
	/// @Author	Gan Shu Long
	/// @Date	2026-05-08
	/// @Input
	/// @Param	eFmAcrossAssets  跨对象类型
	/// @Return	
	//********************************************************************
	void SetAcrossAssets(EFmAcrossAssets eFmAcrossAssets);

	//********************************************************************
	/// 获得  跨对象类型
	/// @Author	Gan Shu Long
	/// @Date	2026-05-08
	/// @Return	 跨对象类型
	//********************************************************************
	EFmAcrossAssets GetAcrossAssets();

	//********************************************************************
	/// 设置 步长
	/// @Author	Gan Shu Long
	/// @Date	2026-05-08
	/// @Input
	/// @Param	dTimeStep  步长
	/// @Return	
	//********************************************************************
	void SetTimeStep(double dTimeStep);

	//********************************************************************
	/// 获得  步长
	/// @Author	Gan Shu Long
	/// @Date	2026-05-08
	/// @Return	 步长
	//********************************************************************
	double GetTimeStep();

	//********************************************************************
	/// 设置 无效值类型
	/// @Author	Gan Shu Long
	/// @Date	2026-05-11
	/// @Input
	/// @Param	eFmInvalidValueActionType  无效值类型
	/// @Return	
	//********************************************************************
	void SetInvalidValueAction(EFmInvalidValueActionType eFmInvalidValueActionType);

	//********************************************************************
	/// 获得  无效值类型
	/// @Author	Gan Shu Long
	/// @Date	2026-05-11
	/// @Return	 无效值类型
	//********************************************************************
	EFmInvalidValueActionType GetInvalidValueAction();
};
