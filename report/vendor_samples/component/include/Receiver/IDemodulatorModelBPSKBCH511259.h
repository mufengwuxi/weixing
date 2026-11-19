#pragma once
#include "../IAtkObj.h"
#include "IDemodulatorModel.h"

///
/// 解调器模型 BPSKBCH_511_259
///
class IATKOBJECT_API IDemodulatorModelBPSKBCH511259 :public IDemodulatorModel
{
public:
    IDemodulatorModelBPSKBCH511259(const IDemodulatorModelBPSKBCH511259&) = delete;
    IDemodulatorModelBPSKBCH511259& operator=(const IDemodulatorModelBPSKBCH511259&) = delete;

	IDemodulatorModelBPSKBCH511259();
	~IDemodulatorModelBPSKBCH511259();

	//********************************************************************
	/// 获得 解调器模型名称
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Return	解调器模型名称
	//********************************************************************
	std::string GetName() { return "BPSKBCH_511_259"; }

	//********************************************************************
	/// 获得 解调器模型类型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Return	解调器模型类型
	//********************************************************************
	EDemodulatorModelType GetType() { return eDemodulatorModelTypeBPSKBCH_511_259; }
};
