#pragma once
#include "../IAtkObj.h"
#include "../ICEnum.h"
#include "IFmDefCompute.h" 
#include <vector>
#include <string>

///
/// 品质因子定义：精度衰减因子
///
class IATKOBJECT_API IFmDefDilutionOfPrecision :public IFmDefCompute
{
public:
    IFmDefDilutionOfPrecision(const IFmDefDilutionOfPrecision&) = delete;
    IFmDefDilutionOfPrecision& operator=(const IFmDefDilutionOfPrecision&) = delete;

	IFmDefDilutionOfPrecision(AsCvFmDefn* pAsCvFmDefn);
	~IFmDefDilutionOfPrecision();

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
	/// 获得 支持的方法类型
	/// @Author	Gan Shu Long
	/// @Date	2026-05-08
	/// @Return 支持的方法类型
	//********************************************************************
	std::vector<std::string> GetSupportedMethods();

	//********************************************************************
	/// 获得 是否支持方法类型
	/// @Author	Gan Shu Long
	/// @Date	2026-05-08
	/// @Input
	/// @Param	eFmMethod  方法类型
	/// @Return	bool 是否支持方法类型	
	//********************************************************************
	bool IsMethodTypeSupported(EFmMethod eFmMethod);

	//********************************************************************
	/// 设置 方法类型
	/// @Author	Gan Shu Long
	/// @Date	2026-05-08
	/// @Input
	/// @Param	eFmMethod  方法类型
	/// @Return	
	//********************************************************************
	void SetMethod(EFmMethod eFmMethod);

	//********************************************************************
	/// 获得  方法类型
	/// @Author	Gan Shu Long
	/// @Date	2026-05-08
	/// @Return	 方法类型
	//********************************************************************
	EFmMethod GetMethod();

	//********************************************************************
	/// 获得 支持的类型
	/// @Author	Gan Shu Long
	/// @Date	2026-05-08
	/// @Return 支持的类型
	//********************************************************************
	std::vector<std::string> GetSupportedTypes();

	//********************************************************************
	/// 获得 是否支持类型
	/// @Author	Gan Shu Long
	/// @Date	2026-05-08
	/// @Input
	/// @Param	eFmComputeType  类型
	/// @Return	bool 是否支持类型	
	//********************************************************************
	bool IsTypeSupported(EFmComputeType eFmComputeType);

	//********************************************************************
	/// 设置 类型
	/// @Author	Gan Shu Long
	/// @Date	2026-05-08
	/// @Input
	/// @Param	eFmComputeType  类型
	/// @Return	
	//********************************************************************
	void SetType(EFmComputeType eFmComputeType);

	//********************************************************************
	/// 获得  类型
	/// @Author	Gan Shu Long
	/// @Date	2026-05-08
	/// @Return	 类型
	//********************************************************************
	EFmComputeType GetType();

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
	/// @Date	2026-05-08
	/// @Input
	/// @Param	eFmInvalidValueActionType  无效值类型
	/// @Return	
	//********************************************************************
	void SetInvalidValueAction(EFmInvalidValueActionType eFmInvalidValueActionType);

	//********************************************************************
	/// 获得  无效值类型
	/// @Author	Gan Shu Long
	/// @Date	2026-05-08
	/// @Return	 无效值类型
	//********************************************************************
	EFmInvalidValueActionType GetInvalidValueAction();
};
