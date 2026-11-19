#pragma once
#include "../IAtkObj.h"
#include "IPolarization.h"
struct CCRAntenna;

///
/// 水平极化基类
///
class IATKOBJECT_API IPolarizationHorizontal :public IPolarization
{
public:
    IPolarizationHorizontal(const IPolarizationHorizontal&) = delete;
    IPolarizationHorizontal& operator=(const IPolarizationHorizontal&) = delete;

	IPolarizationHorizontal(CCRAntenna* pCCRAntenna);
	~IPolarizationHorizontal();

	//********************************************************************
	/// 获得 天线极化类型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-10
	/// @Return	天线极化类型
	//********************************************************************
	EPolarizationType GetType() { return ePolarizationTypeHorizontal; }

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
