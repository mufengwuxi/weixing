#pragma once
#include "../../Satellite/Attitude/IVeAttitude.h"
struct CObjData;

///
/// 姿态：STK文件
///
class IATKOBJECT_API IVeAttitudeStkExternal : public IVeAttitude
{
public:
    IVeAttitudeStkExternal(const IVeAttitudeStkExternal&) = delete;
    IVeAttitudeStkExternal& operator=(const IVeAttitudeStkExternal&) = delete;

	IVeAttitudeStkExternal(CObjData* pObjData);
	~IVeAttitudeStkExternal();

	//********************************************************************
	/// 设置STK星历文件路径
	/// @Author	Gan Shu Long
	/// @Date	2024-09-24
	/// @Input
	/// @Param	strFilename STK星历文件路径
	/// @Return	
	//********************************************************************
	void SetFilename(std::string  strFilename);

	//********************************************************************
	/// 获得STK星历文件路径
	/// @Author	Gan Shu Long
	/// @Date	2024-09-24
	/// @Return	STK星历文件路径
	//********************************************************************
	std::string  GetFilename();

	//********************************************************************
	/// 获得开始时间
	/// @Author	Gan Shu Long
	/// @Date	2024-09-24
	/// @Return	开始时间
	//********************************************************************
	std::string  GetStartTime();

	//********************************************************************
	/// 获得结束时间
	/// @Author	Gan Shu Long
	/// @Date	2024-09-24
	/// @Return	结束时间
	//********************************************************************
	std::string  GetStopTime();

private:
	CObjData*							m_pObjData;
};
