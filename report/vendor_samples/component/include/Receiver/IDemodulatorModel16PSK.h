#pragma once
#include "../IAtkObj.h"
#include "IDemodulatorModel.h"

///
/// 解调器模型 16PSK
///
class IATKOBJECT_API IDemodulatorModel16PSK :public IDemodulatorModel
{
public:
    IDemodulatorModel16PSK(const IDemodulatorModel16PSK&) = delete;
    IDemodulatorModel16PSK& operator=(const IDemodulatorModel16PSK&) = delete;

	IDemodulatorModel16PSK();
	~IDemodulatorModel16PSK();

	//********************************************************************
	/// 获得 解调器模型名称
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Return	解调器模型名称
	//********************************************************************
	std::string GetName() { return "16PSK"; }

	//********************************************************************
	/// 获得 解调器模型类型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Return	解调器模型类型
	//********************************************************************
	EDemodulatorModelType GetType() { return eDemodulatorModelType16psk; }
};
