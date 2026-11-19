#pragma once
#include "ISnPattern.h"
class CSensor;

///
/// 模式类型：矩形
///
class IATKOBJECT_API ISnRectangularPattern : public ISnPattern
{
public:
    ISnRectangularPattern(const ISnRectangularPattern&) = delete;
    ISnRectangularPattern& operator=(const ISnRectangularPattern&) = delete;

	ISnRectangularPattern(CSensor* pCSensor);
	~ISnRectangularPattern();

	//********************************************************************
	/// 设置 水平半推角
	/// @Author	Gan Shu Long
	/// @Date	2025-07-10
	/// @Input
	/// @Param	dHorizontalHalfAngle  水平半推角
	/// @Return	
	//********************************************************************
	void SetHorizontalHalfAngle(double dHorizontalHalfAngle);

	//********************************************************************
	/// 获得 水平半推角
	/// @Author	Gan Shu Long
	/// @Date	2025-07-10
	/// @Return	double 水平半推角	
	//********************************************************************
	double GetHorizontalHalfAngle();

	//********************************************************************
	/// 设置 垂直半推角
	/// @Author	Gan Shu Long
	/// @Date	2025-07-10
	/// @Input
	/// @Param	dVerticalHalfAngle  垂直半推角
	/// @Return	
	//********************************************************************
	void SetVerticalHalfAngle(double dVerticalHalfAngle);

	//********************************************************************
	/// 获得 垂直半推角
	/// @Author	Gan Shu Long
	/// @Date	2025-07-10
	/// @Return	double 垂直半推角	
	//********************************************************************
	double GetVerticalHalfAngle();

private:
	CSensor*			m_pCSensor;
};

