#pragma once
#include "ISnPattern.h"
class CSensor;

///
/// 模式类型：复杂圆锥
///
class IATKOBJECT_API ISnComplexConicPattern : public ISnPattern
{
public:
    ISnComplexConicPattern(const ISnComplexConicPattern&) = delete;
    ISnComplexConicPattern& operator=(const ISnComplexConicPattern&) = delete;

	ISnComplexConicPattern(CSensor* pCSensor);
	~ISnComplexConicPattern();

	//********************************************************************
	/// 设置 半锥角内部
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Input
	/// @Param	dInnerConeHalfAngle  半锥角内部
	/// @Return	
	//********************************************************************
	void SetInnerConeHalfAngle(double dInnerConeHalfAngle);

	//********************************************************************
	/// 获得 半锥角内部
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Return	半锥角内部	
	//********************************************************************
	double GetInnerConeHalfAngle();

	//********************************************************************
	/// 设置 半锥角外部
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Input
	/// @Param	dOuterConeHalfAngle  半锥角外部
	/// @Return	
	//********************************************************************
	void SetOuterConeHalfAngle(double dOuterConeHalfAngle);

	//********************************************************************
	/// 获得 半锥角外部
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Return	半锥角外部	
	//********************************************************************
	double GetOuterConeHalfAngle();

	//********************************************************************
	/// 设置 钟角最小值
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Input
	/// @Param	dMinimumClockAngle  钟角最小值
	/// @Return	
	//********************************************************************
	void SetMinimumClockAngle(double dMinimumClockAngle);

	//********************************************************************
	/// 获得 钟角最小值
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Return	钟角最小值	
	//********************************************************************
	double GetMinimumClockAngle();

	//********************************************************************
	/// 设置 钟角最大值
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Input
	/// @Param	dMaximumClockAngle  钟角最大值
	/// @Return	
	//********************************************************************
	void SetMaximumClockAngle(double dMaximumClockAngle);

	//********************************************************************
	/// 获得 钟角最大值
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Return	钟角最大值	
	//********************************************************************
	double GetMaximumClockAngle();

private:
	CSensor*			m_pCSensor;
};

