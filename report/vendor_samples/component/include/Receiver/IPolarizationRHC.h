#pragma once
#include "../IAtkObj.h"
#include "IPolarization.h"
struct CCRAntenna;

///
/// 右旋圆极化基类
///
class IATKOBJECT_API IPolarizationRHC :public IPolarization
{
public:
    IPolarizationRHC(const IPolarizationRHC&) = delete;
    IPolarizationRHC& operator=(const IPolarizationRHC&) = delete;

	IPolarizationRHC(CCRAntenna* pCCRAntenna);
	~IPolarizationRHC();

	//********************************************************************
	/// 获得 天线极化类型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-10
	/// @Return	天线极化类型
	//********************************************************************
	EPolarizationType GetType() { return ePolarizationTypeRHC; }

	//********************************************************************
	/// 设置  天线极化参考轴
	/// @Author	Gan Shu Long
	/// @Date	2023-12-11
	/// @Input 
	/// @Param	double 新值
	/// @Return	
	//********************************************************************
	void SetReferenceAxis(EPolarizationReferenceAxis referenceAxis);
	
	//********************************************************************
	/// 获得 天线极化参考轴
	/// @Author	Gan Shu Long
	/// @Date	2026-03-10
	/// @Return	天线极化参考轴
	//********************************************************************
	EPolarizationReferenceAxis GetReferenceAxis();

	//********************************************************************
	/// 设置  极化角度
	/// @Author	Gan Shu Long
	/// @Date	2023-12-11
	/// @Input 
	/// @Param	double 新值
	/// @Return	
	//********************************************************************
	void SetTiltAngle(double tiltAngle);
	
	//********************************************************************
	/// 获得 极化角度
	/// @Author	Gan Shu Long
	/// @Date	2026-03-10
	/// @Return	极化角度
	//********************************************************************
	double GetTiltAngle();

private:
	CCRAntenna*			 m_pCCRAntenna;		//天线结构体
};
