#pragma once
#include "IAtkObject.h"
class CObjManage;
class ICObjectFunction;

///
/// 场景对象
///
class IATKOBJECT_API IScenario :public IAtkObject
{
public:
    IScenario(const IScenario&) = delete;
    IScenario& operator=(const IScenario&) = delete;

	IScenario(CObjData* pObjData, CObjManage* IAsGObjManage, IAtkObjectRoot* Root);
	~IScenario();

	//********************************************************************
	/// 设置 场景时间
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	strStartUTC  开始时间，例："1 Jan 2000 00:00:00.000"
	/// @Param	strStopUTC   结束时间，例："2 Jan 2000 00:00:00.000"
	/// @Return	
	//********************************************************************
	void SetTimePeriod(std::string strStartUTC, std::string strStopUTC);

	//********************************************************************
	/// 设置开始时间
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	strStartTime  开始时间，例："1 Jan 2000 00:00:00.000"
	/// @Return	
	//********************************************************************
	void SetStartTime(std::string strStartTime);
	//********************************************************************
	/// 获取开始时间
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	开始时间
	//********************************************************************
	std::string GetStartTime();

	//********************************************************************
	/// 设置结束时间
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	strStopTime   结束时间，例："2 Jan 2000 00:00:00.000"
	/// @Return	
	//********************************************************************
	void SetStopTime(std::string strStopTime);
	//********************************************************************
	/// 获取结束时间
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	结束时间
	//********************************************************************
	std::string GetStopTime();

	//********************************************************************
	/// 获取仿真步长
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	结束时间
	//********************************************************************
	//double GetStepSize();

	//********************************************************************
	/// 获取开始时间BACC格式
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	开始时间
	//********************************************************************
	std::string GetStartTimeBACC();

	//********************************************************************
	/// 获取结束时间BACC格式
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	结束时间
	//********************************************************************
	std::string GetStopTimeBACC();

	//********************************************************************
	/// 获取增加时长时间
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	strTime   输入时间，例："2012-03-15T10:00:00"
	/// @Param	dSec   增加时间长度
	/// @Param	strAddTime   添加后的时间
	/// @Return	是否添加成功
	//********************************************************************
	bool GetAddTime(std::string strTime, double dSec, std::string& strAddTime);

};
