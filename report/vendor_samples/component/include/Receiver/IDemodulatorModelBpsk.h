#pragma once
#include "../IAtkObj.h"
#include "IDemodulatorModel.h"

///
/// 解调器模型 BPSK
///
class IATKOBJECT_API IDemodulatorModelBpsk :public IDemodulatorModel
{
public:
    IDemodulatorModelBpsk(const IDemodulatorModelBpsk&) = delete;
    IDemodulatorModelBpsk& operator=(const IDemodulatorModelBpsk&) = delete;

	IDemodulatorModelBpsk();
	~IDemodulatorModelBpsk();

	//********************************************************************
	/// 获得 解调器模型名称
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Return	解调器模型名称
	//********************************************************************
	std::string GetName() { return "BPSK"; }

	//********************************************************************
	/// 获得 解调器模型类型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Return	解调器模型类型
	//********************************************************************
	EDemodulatorModelType GetType() { return eDemodulatorModelTypeBPSK; }
};
