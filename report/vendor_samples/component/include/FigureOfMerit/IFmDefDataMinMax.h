#pragma once
#include "../IAtkObj.h"
#include "IFmDefinitionData.h" 

///
/// 品质因子定义数据：最大值最小值
///
class IATKOBJECT_API IFmDefDataMinMax :public IFmDefinitionData
{
public:
    IFmDefDataMinMax(const IFmDefDataMinMax&) = delete;
    IFmDefDataMinMax& operator=(const IFmDefDataMinMax&) = delete;

	IFmDefDataMinMax(AsCvFmDefn* pAsCvFmDefn);
	~IFmDefDataMinMax();

	//********************************************************************
	/// 设置  最大值
	/// @Author	Gan Shu Long
	/// @Date	2026-05-08
	/// @Input 
	/// @Param	dMaxValue 最大值
	/// @Return	
	//********************************************************************
	void SetMaxValue(double dMaxValue);

	//********************************************************************
	/// 获得 最大值
	/// @Author	Gan Shu Long
	/// @Date	2026-05-08
	/// @Return	最大值
	//********************************************************************
	double GetMaxValue();

	//********************************************************************
	/// 设置  最小值
	/// @Author	Gan Shu Long
	/// @Date	2026-05-08
	/// @Input 
	/// @Param	dMinValue 最小值
	/// @Return	
	//********************************************************************
	void SetMinValue(double dMinValue);

	//********************************************************************
	/// 获得 最小值
	/// @Author	Gan Shu Long
	/// @Date	2026-05-08
	/// @Return	最小值
	//********************************************************************
	double GetMinValue();
};
