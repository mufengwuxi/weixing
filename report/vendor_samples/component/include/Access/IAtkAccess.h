#pragma once
#include "../IAtkObj.h"
#include "../ICEnum.h"
#include <string>
#include <vector>
class CObjManage;
class IAtkObject;
class CCTime;

///
/// 可见性
///
class IATKOBJECT_API IAtkAccess
{
public:
    IAtkAccess(const IAtkAccess&) = delete;
    IAtkAccess& operator=(const IAtkAccess&) = delete;

	IAtkAccess(CObjManage* IAsGObjManage, IAtkObject* pIAtkObject, IAtkObject* pIAccessObject);
	~IAtkAccess();

	//********************************************************************
	/// 清除可见性计算结果
	/// @Author	Gan Shu Long
	/// @Date	2026-09-22
	/// @Return	
	//********************************************************************
	void ClearAccess();

	//********************************************************************
	/// 可见性计算
	/// @Author	Gan Shu Long
	/// @Date	2026-09-22
	/// @Return	
	//********************************************************************
	void ComputeAccess();

	//********************************************************************
	/// 获取可见性计算结果
	/// @Author	gan shu long
	/// @Date	2026-09-22
	/// @Return	
	//********************************************************************
	std::vector<std::vector<std::string>> ComputedAccessIntervalTimes();

	//********************************************************************
	/// 移除可见性
	/// @Author	Gan Shu Long
	/// @Date	2026-09-22
	/// @Return	
	//********************************************************************
	void RemoveAccess();

	//********************************************************************
	/// 设置可见性计算时间
	/// @Author	Gan Shu Long
	/// @Date	2026-09-22
	/// @Input
	/// @Param	strStartTime  开始历元
	/// @Param	strStopTime	  结束历元
	/// @Return	
	//********************************************************************
	void SetAccessTimePeriod(std::string strStartTime, std::string strStopTime);

	//********************************************************************
	/// 获得可见性计算时间
	/// @Author	Gan Shu Long
	/// @Date	2026-09-22
	/// @Return	开始历元
	//********************************************************************
	std::string GetAccessStartTime();

	//********************************************************************
	/// 获得可见性计算时间
	/// @Author	Gan Shu Long
	/// @Date	2026-09-22
	/// @Return	结束历元
	//********************************************************************
	std::string GetAccessStopTime();

	//********************************************************************
	/// 设置可见性计算步长
	/// @Author	gan shu long 
	/// @Date	2026-09-03
	/// @Input
	/// @Param	dStepSize  计算步长
	/// @Return	
	//********************************************************************
	void SetTimeStep(double dTimeStep);

	//********************************************************************
	/// 获得可见性计算步长
	/// @Author	gan shu long 
	/// @Date	2026-09-03
	/// @Return	计算步长
	//********************************************************************
	double GetTimeStep();

	//********************************************************************
	/// 设置使用光延迟
	/// @Author	gan shu long 
	/// @Date	2026-09-03
	/// @Input
	/// @Param	bUseLTD  是否使用光延迟
	/// @Return	
	//********************************************************************
	void SetUseLTD(bool bUseLTD);

	//********************************************************************
	/// 获得是否使用光延迟
	/// @Author	gan shu long 
	/// @Date	2026-09-03
	/// @Return 是否使用光延迟
	//********************************************************************
	bool GetUseLTD();

	//********************************************************************
	/// 输出可见性报告
	/// @Author	Gan Shu Long
	/// @Date	2026-09-03
	/// @Input
	/// @Param 	eAccessReportType 可见性报告类型
	/// @Return	可见性报告数据
	//********************************************************************
	std::string OutputAccessReport(EAccessReportType eAccessReportType);


private:
	CObjManage*     m_IAsGObjManage;
	IAtkObject*     m_pIAtkObject;
	IAtkObject*     m_pIAccessObject;

	//AccessCoverageDataTree* GetAccessData(AccessCore* pAccessCore);

};
