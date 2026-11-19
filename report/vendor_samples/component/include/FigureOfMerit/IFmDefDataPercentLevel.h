#pragma once
#include "../IAtkObj.h"
#include "IFmDefinitionData.h" 

///
/// 品质因子定义数据：百分比高于
///
class IATKOBJECT_API IFmDefDataPercentLevel :public IFmDefinitionData
{
public:
    IFmDefDataPercentLevel(const IFmDefDataPercentLevel&) = delete;
    IFmDefDataPercentLevel& operator=(const IFmDefDataPercentLevel&) = delete;

	IFmDefDataPercentLevel(AsCvFmDefn* pAsCvFmDefn);
	~IFmDefDataPercentLevel();

	//********************************************************************
	/// 设置  值
	/// @Author	Gan Shu Long
	/// @Date	2026-05-07
	/// @Input 
	/// @Param	dPercentLevel 值
	/// @Return	
	//********************************************************************
	void SetPercentLevel(double dPercentLevel);

	//********************************************************************
	/// 获得 值
	/// @Author	Gan Shu Long
	/// @Date	2026-05-07
	/// @Return	值
	//********************************************************************
	double GetPercentLevel();
};
