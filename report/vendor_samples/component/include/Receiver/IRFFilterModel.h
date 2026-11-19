#pragma once
#include "../IAtkObj.h"
#include "../IClassInfo.h" 
#include "../ICEnum.h"
struct CCRFilter;

///
/// 滤波器
///
class IATKOBJECT_API IRFFilterModel :public IClassInfo
{
public:
    IRFFilterModel(const IRFFilterModel&) = delete;
    IRFFilterModel& operator=(const IRFFilterModel&) = delete;

	IRFFilterModel(CCRFilter* pCCRFilter);
	virtual ~IRFFilterModel();

	//********************************************************************
	/// 获得 滤波器名称
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Return	滤波器名称
	//********************************************************************
	std::string GetName() { return ""; }

	//********************************************************************
	/// 获得 滤波器类型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Return	滤波器类型
	//********************************************************************
	ERFFilterModelType GetType() { return eRFFilterModelTypeUnknown; }

	//********************************************************************
	/// 设置  高频段截至频率
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Input 
	/// @Param	dUpperBandwidthLimit 高频段截至频率
	/// @Return	
	//********************************************************************
	void SetUpperBandwidthLimit	(double dUpperBandwidthLimit);

	//********************************************************************
	/// 获得 高频段截至频率
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Return	高频段截至频率
	//********************************************************************
	double GetUpperBandwidthLimit();

	//********************************************************************
	/// 设置  低频段截至频率
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Input 
	/// @Param	dLowerBandwidthLimit 低频段截至频率
	/// @Return	
	//********************************************************************
	void SetLowerBandwidthLimit(double dLowerBandwidthLimit);

	//********************************************************************
	/// 获得 低频段截至频率
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Return	低频段截至频率
	//********************************************************************
	double GetLowerBandwidthLimit();

	//********************************************************************
	/// 设置  带宽
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Input 
	/// @Param	dBandwidth 带宽
	/// @Return	
	//********************************************************************
	void SetBandwidth(double dBandwidth);

	//********************************************************************
	/// 获得 带宽
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Return	带宽
	//********************************************************************
	double GetBandwidth();

	//********************************************************************
	/// 设置 插入损耗
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Input 
	/// @Param	dInsertionLoss 插入损耗
	/// @Return	
	//********************************************************************
	void SetInsertionLoss(double dInsertionLoss);

	//********************************************************************
	/// 获得 插入损耗
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Return	插入损耗
	//********************************************************************
	double GetInsertionLoss();

private:
	CCRFilter*			 m_pCCRFilter;		//滤波器结构体
};
