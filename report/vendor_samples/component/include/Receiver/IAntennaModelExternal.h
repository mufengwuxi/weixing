#pragma once
#include "../IAtkObj.h"
#include "IAntennaModel.h" 
#include "../ICEnum.h"
struct CCRAntenna;

///
/// 天线模型 External
///
class IATKOBJECT_API IAntennaModelExternal :public IAntennaModel
{
public:
    IAntennaModelExternal(const IAntennaModelExternal&) = delete;
    IAntennaModelExternal& operator=(const IAntennaModelExternal&) = delete;

	IAntennaModelExternal(CCRAntenna* pCCRAntenna);
	virtual ~IAntennaModelExternal();

	//********************************************************************
	/// 获得 天线模型名称
	/// @Author	Gan Shu Long
	/// @Date	2026-03-16
	/// @Return	天线模型名称
	//********************************************************************
	std::string GetName() { return "External Antenna Pattern"; }

	//********************************************************************
	/// 获得 天线模型类型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-16
	/// @Return	天线模型类型
	//********************************************************************
	EAntennaModelType GetType() { return eAntennaModelTypeExternal; }

	//********************************************************************
	/// 设置 文件路径
	/// @Author	Gan Shu Long
	/// @Date	2026-03-16
	/// @Input
	/// @Param	strFilename 文件路径
	/// @Return	
	//********************************************************************
	void SetFilename(std::string  strFilename);

	//********************************************************************
	/// 获得 文件路径
	/// @Author	Gan Shu Long
	/// @Date	2026-03-16
	/// @Return	文件路径
	//********************************************************************
	std::string  GetFilename();

private:
	CCRAntenna*					m_pCCRAntenna;					//天线结构体
};
