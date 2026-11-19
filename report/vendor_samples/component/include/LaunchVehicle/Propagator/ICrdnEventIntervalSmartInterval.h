#pragma once
#include "../../IAtkObj.h"
#include "../../ICEnum.h"
#include <string>
class CLaunchVehicle;
struct CObjData;

///
/// 历元间隔
///
class IATKOBJECT_API ICrdnEventIntervalSmartInterval
{
public:
    ICrdnEventIntervalSmartInterval(const ICrdnEventIntervalSmartInterval&) = delete;
    ICrdnEventIntervalSmartInterval& operator=(const ICrdnEventIntervalSmartInterval&) = delete;

	ICrdnEventIntervalSmartInterval(CObjData* pObjData);
	~ICrdnEventIntervalSmartInterval();

	//********************************************************************
	/// 设置开始历元
	/// @Author	Gan Shu Long
	/// @Date	2024-09-23
	/// @Input
	/// @Param	startEpoch 开始历元
	/// @Return	
	//********************************************************************
	void SetStartEpoch(std::string  startEpoch);

	//********************************************************************
	/// 获得开始历元
	/// @Author	Gan Shu Long
	/// @Date	2024-09-23
	/// @Return	开始历元
	//********************************************************************
	std::string  GetStartEpoch();

	//********************************************************************
	/// 设置结束历元
	/// @Author	Gan Shu Long
	/// @Date	2024-09-23
	/// @Input
	/// @Param	stopEpoch 结束历元
	/// @Return	
	//********************************************************************
	void SetStopEpoch(std::string  stopEpoch);

	//********************************************************************
	/// 获得结束历元
	/// @Author	Gan Shu Long
	/// @Date	2024-09-23
	/// @Return	结束历元
	//********************************************************************
	std::string  GetStopEpoch();

	//********************************************************************
	/// 设置 历元类型
	/// @Author	Gan Shu Long
	/// @Date	2026-10-22
	/// @Input
	/// @Param	eCrdnSmartIntervalState 历元类型
	/// @Return	
	//********************************************************************
	void SetState(ECrdnSmartIntervalState eCrdnSmartIntervalState);

	//********************************************************************
	/// 获得 历元类型
	/// @Author	Gan Shu Long
	/// @Date	2026-10-22
	/// @Return	历元类型
	//********************************************************************
	ECrdnSmartIntervalState GetState();

private:
	CObjData*		m_pObjData;
};

