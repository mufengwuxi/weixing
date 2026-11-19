#pragma once
#include "../IAtkObj.h" 
#include "../ICEnum.h"
struct AsCvFmDefn;

///
/// 品质因子有效条件
///
class IATKOBJECT_API IFmSatisfaction
{
public:
    IFmSatisfaction(const IFmSatisfaction&) = delete;
    IFmSatisfaction& operator=(const IFmSatisfaction&) = delete;

	IFmSatisfaction(AsCvFmDefn* pAsCvFmDefn);
	~IFmSatisfaction();
	
	//********************************************************************
	/// 设置 是否使用有效条件
	/// @Author	Gan Shu Long
	/// @Date	2026-05-07
	/// @Input
	/// @Param	bEnableSatisfaction  是否使用有效条件
	/// @Return	
	//********************************************************************
	void SetEnableSatisfaction(bool bEnableSatisfaction);

	//********************************************************************
	/// 获得 是否使用有效条件
	/// @Author	Gan Shu Long
	/// @Date	2026-05-07
	/// @Return	是否使用有效条件
	//********************************************************************
	bool GetEnableSatisfaction();

	//********************************************************************
	/// 设置 有效条件类型
	/// @Author	Gan Shu Long
	/// @Date	2026-05-07
	/// @Input
	/// @Param	eFmSatisfactionType  有效条件类型
	/// @Return	
	//********************************************************************
	void SetSatisfactionType(EFmSatisfactionType eFmSatisfactionType);

	//********************************************************************
	/// 获得 有效条件类型
	/// @Author	Gan Shu Long
	/// @Date	2026-05-07
	/// @Return	有效条件类型
	//********************************************************************
	EFmSatisfactionType GetSatisfactionType();

	//********************************************************************
	/// 设置  阈值
	/// @Author	Gan Shu Long
	/// @Date	2026-05-07
	/// @Input 
	/// @Param	dSatisfactionThreshold 阈值
	/// @Return	
	//********************************************************************
	void SetSatisfactionThreshold(double dSatisfactionThreshold);

	//********************************************************************
	/// 获得 阈值
	/// @Author	Gan Shu Long
	/// @Date	2026-05-07
	/// @Return	阈值
	//********************************************************************
	double GetSatisfactionThreshold();

	//********************************************************************
	/// 设置  无效数据指标
	/// @Author	Gan Shu Long
	/// @Date	2026-05-07
	/// @Input 
	/// @Param	dInvalidDataIndicator 无效数据指标
	/// @Return	
	//********************************************************************
	void SetInvalidDataIndicator(double dInvalidDataIndicator);

	//********************************************************************
	/// 获得 无效数据指标
	/// @Author	Gan Shu Long
	/// @Date	2026-05-07
	/// @Return	无效数据指标
	//********************************************************************
	double GetInvalidDataIndicator();

	//********************************************************************
	/// 设置 是否使用范围内的值
	/// @Author	Gan Shu Long
	/// @Date	2026-05-07
	/// @Input
	/// @Param	bUseValueRangeCheck  是否使用范围内的值
	/// @Return	
	//********************************************************************
	void SetUseValueRangeCheck(bool bUseValueRangeCheck);

	//********************************************************************
	/// 获得 是否使用范围内的值
	/// @Author	Gan Shu Long
	/// @Date	2026-05-07
	/// @Return	是否使用范围内的值
	//********************************************************************
	bool GetUseValueRangeCheck();

	//********************************************************************
	/// 设置  最小值
	/// @Author	Gan Shu Long
	/// @Date	2026-05-07
	/// @Input 
	/// @Param	dMinValueRange 最小值
	/// @Return	
	//********************************************************************
	void SetMinValueRange(double dMinValueRange);

	//********************************************************************
	/// 获得 最小值
	/// @Author	Gan Shu Long
	/// @Date	2026-05-07
	/// @Return	最小值
	//********************************************************************
	double GetMinValueRange();

	//********************************************************************
	/// 设置  最大值
	/// @Author	Gan Shu Long
	/// @Date	2026-05-07
	/// @Input 
	/// @Param	dMaxValueRange 最大值
	/// @Return	
	//********************************************************************
	void SetMaxValueRange(double dMaxValueRange);

	//********************************************************************
	/// 获得 最大值
	/// @Author	Gan Shu Long
	/// @Date	2026-05-07
	/// @Return	最大值
	//********************************************************************
	double GetMaxValueRange();

	//********************************************************************
	/// 设置 是否使用反向选择
	/// @Author	Gan Shu Long
	/// @Date	2026-05-07
	/// @Input
	/// @Param	bExcludeValueRange  是否使用反向选择
	/// @Return	
	//********************************************************************
	void SetExcludeValueRange(bool bExcludeValueRange);

	//********************************************************************
	/// 获得 是否使用反向选择
	/// @Author	Gan Shu Long
	/// @Date	2026-05-07
	/// @Return	是否使用反向选择
	//********************************************************************
	bool GetExcludeValueRange();

private:
	AsCvFmDefn*		m_pAsCvFmDefn;
};
