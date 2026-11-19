#pragma once
#include "../../Satellite/Propagator/IVePropagator.h"

///
/// STK星历
///
class IATKOBJECT_API IVePropagatorStkExternal : public IVePropagator
{
public:
    IVePropagatorStkExternal(const IVePropagatorStkExternal&) = delete;
    IVePropagatorStkExternal& operator=(const IVePropagatorStkExternal&) = delete;

	IVePropagatorStkExternal(CObjData* pObjData);
	~IVePropagatorStkExternal();

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
