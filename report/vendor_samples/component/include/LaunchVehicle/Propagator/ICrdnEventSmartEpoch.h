#pragma once
#include "../../IAtkObj.h"
#include "../../ICEnum.h"
#include <string>
class CSatellite;

///
/// 坐标历元时间
///
class IATKOBJECT_API ICrdnEventSmartEpoch
{
public:
	ICrdnEventSmartEpoch(const ICrdnEventSmartEpoch&) = delete;
	ICrdnEventSmartEpoch& operator=(const ICrdnEventSmartEpoch&) = delete;

	ICrdnEventSmartEpoch(CSatellite* pSate);
	~ICrdnEventSmartEpoch();

	//********************************************************************
	/// 设置 坐标历元时间
	/// @Author	Gan Shu Long
	/// @Date	2026-10-22
	/// @Input
	/// @Param	strTimeInstant 坐标历元时间
	/// @Return	
	//********************************************************************
	void SetTimeInstant(std::string  strTimeInstant);

	//********************************************************************
	/// 获得 坐标历元时间
	/// @Author	Gan Shu Long
	/// @Date	2026-10-22
	/// @Return	坐标历元时间
	//********************************************************************
	std::string  GetTimeInstant();

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
	CSatellite*		m_pSate;
};

