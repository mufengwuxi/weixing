#pragma once
#include "ISnPattern.h"
class CSensor;

///
/// 模式类型：简单圆锥
///
class IATKOBJECT_API ISnSimpleConicPattern : public ISnPattern
{
public:
    ISnSimpleConicPattern(const ISnSimpleConicPattern&) = delete;
    ISnSimpleConicPattern& operator=(const ISnSimpleConicPattern&) = delete;

	ISnSimpleConicPattern(CSensor* pCSensor);
	~ISnSimpleConicPattern();

	//********************************************************************
	/// 设置 半锥角
	/// @Author	Gan Shu Long
	/// @Date	2025-07-10
	/// @Input
	/// @Param	dConeAngle  半锥角
	/// @Return	
	//********************************************************************
	void SetConeAngle(double dConeAngle);

	//********************************************************************
	/// 获得 半锥角
	/// @Author	Gan Shu Long
	/// @Date	2025-07-10
	/// @Return	double 半锥角	
	//********************************************************************
	double GetConeAngle();

private:
	CSensor*			m_pCSensor;
};

