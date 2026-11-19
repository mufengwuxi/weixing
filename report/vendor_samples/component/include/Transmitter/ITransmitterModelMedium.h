#pragma once
#include "../IAtkObj.h"
#include "ITransmitterModel.h"
#include "../ICEnum.h"
#include <vector>
#include <string>
class CTransmitter;
class IPolarization;
class IModulatorModel;
class IRFFilterModel;

///
/// 发射器模型：中等发射器
///
class IATKOBJECT_API ITransmitterModelMedium :public ITransmitterModel
{
public:
    ITransmitterModelMedium(const ITransmitterModelMedium&) = delete;
    ITransmitterModelMedium& operator=(const ITransmitterModelMedium&) = delete;

	ITransmitterModelMedium(CTransmitter* pCTransmitter);
	~ITransmitterModelMedium();
	
	//********************************************************************
	/// 设置  频率
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Input 
	/// @Param	dFrequency 频率
	/// @Return	
	//********************************************************************
	void SetFrequency(double dFrequency);

	//********************************************************************
	/// 获得 频率
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Return	频率
	//********************************************************************
	double GetFrequency();

	//********************************************************************
	/// 设置  数据速率
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Input 
	/// @Param	dDataRate 数据速率
	/// @Return	
	//********************************************************************
	void SetDataRate(double dDataRate);
	
	//********************************************************************
	/// 获得 数据速率
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Return	数据速率
	//********************************************************************
	double GetDataRate();

	//********************************************************************
	/// 设置  功率
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Input 
	/// @Param	dPower 功率
	/// @Return	
	//********************************************************************
	void SetPower(double dPower);
		
	//********************************************************************
	/// 获得 功率
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Return	功率
	//********************************************************************
	double GetPower();

	//********************************************************************
	/// 设置  增益
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Input 
	/// @Param	dAntennaGain 增益
	/// @Return	
	//********************************************************************
	void SetAntennaGain(double dAntennaGain);

	//********************************************************************
	/// 获得 增益
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Return	增益
	//********************************************************************
	double GetAntennaGain();

	//********************************************************************
	/// 设置 是否使用天线极化
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Input
	/// @Param	bEnablePolarization  是否使用天线极化
	/// @Return	
	//********************************************************************
	void SetEnablePolarization(bool bEnablePolarization);

	//********************************************************************
	/// 获得 是否使用天线极化
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Return	是否使用天线极化
	//********************************************************************
	bool GetEnablePolarization();

	//********************************************************************
	/// 设置 天线极化类型 (子类之间类型转换，子类成员值不变)
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Input
	/// @Param	ePolarizationType  天线极化类型
	/// @Return	
	//********************************************************************
	void SetPolarizationType(EPolarizationType ePolarizationType);

	//********************************************************************
	/// 获得 天线极化类型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Return	天线极化类型
	//********************************************************************
	EPolarizationType GetPolarizationType();

	//********************************************************************
	/// 获得 天线极化
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Return	天线极化
	//********************************************************************
	IPolarization* GetPolarization() { return m_Polarization; }
	
	//********************************************************************
	/// 获得 支持的调制器模型名称
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Return 支持的调制器模型名称
	//********************************************************************
	std::vector<std::string> GetSupportedModulators();

	//********************************************************************
	/// 按名称设置 调制器模型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Input
	/// @Param	strModulatorName  调制器模型名称
	/// @Return	
	//********************************************************************
	void SetModulator(std::string strModulatorName);

	//********************************************************************
	/// 设置 调制器模型类型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Input
	/// @Param	eModulatorModelType  调制器模型类型
	/// @Return	
	//********************************************************************
	void SetModulatorModelType(EModulatorModelType eModulatorModelType);

	//********************************************************************
	/// 获得 调制器模型类型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Return	调制器模型类型
	//********************************************************************
	EModulatorModelType GetModulatorModelType();

	//********************************************************************
	/// 获得 调制器模型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Return	调制器模型
	//********************************************************************
	IModulatorModel* GetModulator() { return m_Modulator; }
	
	//********************************************************************
	/// 设置 是否使用滤波器
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Input
	/// @Param	bEnableFilter  是否使用滤波器
	/// @Return	
	//********************************************************************
	void SetEnableFilter(bool bEnableFilter);

	//********************************************************************
	/// 获得 是否使用滤波器
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Return	是否使用滤波器
	//********************************************************************
	bool GetEnableFilter();

	//********************************************************************
	/// 获得 支持的滤波器类型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Return 支持的滤波器类型
	//********************************************************************
	std::vector<std::string> GetSupportedFilters();

	//********************************************************************
	/// 按名称设置当前滤波器类型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Input
	/// @Param	strFilterName  滤波器类型名称
	/// @Return	
	//********************************************************************
	void SetFilter(std::string strFilterName);

	//********************************************************************
	/// 设置 滤波器类型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Input
	/// @Param	eRFFilterModelType  滤波器类型
	/// @Return	
	//********************************************************************
	void SetFilterModelType(ERFFilterModelType eRFFilterModelType);

	//********************************************************************
	/// 获得 滤波器类型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Return	滤波器类型
	//********************************************************************
	ERFFilterModelType GetFilterModelType();

	//********************************************************************
	/// 获得  滤波器
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Return	 滤波器
	//********************************************************************
	IRFFilterModel* GetFilter() { return m_Filter; }

	//********************************************************************
	/// 设置  发射机额外损益dB
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Input 
	/// @Param	dPostTransmitGainsLosses 发射机额外损益dB
	/// @Return	
	//********************************************************************
	void SetPostTransmitGainsLosses(double dPostTransmitGainsLosses);

	//********************************************************************
	/// 获得 发射机额外损益dB
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Return	发射机额外损益dB
	//********************************************************************
	double GetPostTransmitGainsLosses();

private:
	IPolarization*				m_Polarization;			//天线极化
	IModulatorModel*			m_Modulator;				//调制器
	IRFFilterModel*			m_Filter;						//滤波器

	CTransmitter*				m_CTransmitter;
};
