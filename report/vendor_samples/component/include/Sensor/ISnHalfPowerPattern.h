#pragma once
#include "ISnPattern.h"
class CSensor;

///
/// 模式类型：半功率
///
class IATKOBJECT_API ISnHalfPowerPattern : public ISnPattern
{
public:
    ISnHalfPowerPattern(const ISnHalfPowerPattern&) = delete;
    ISnHalfPowerPattern& operator=(const ISnHalfPowerPattern&) = delete;

	ISnHalfPowerPattern(CSensor* pCSensor);
	~ISnHalfPowerPattern();

	//********************************************************************
	/// 设置 频率
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Input
	/// @Param	dFrequency  频率
	/// @Return	
	//********************************************************************
	void SetFrequency(double dFrequency);

	//********************************************************************
	/// 获得 频率
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Return	double 频率	
	//********************************************************************
	double GetFrequency();

	//********************************************************************
	/// 设置 直径
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Input
	/// @Param	dAntennaDiameter  直径
	/// @Return	
	//********************************************************************
	void SetAntennaDiameter(double dAntennaDiameter);

	//********************************************************************
	/// 获得 直径
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Return	double 直径
	//********************************************************************
	double GetAntennaDiameter();

private:
	CSensor*			m_pCSensor;
};

