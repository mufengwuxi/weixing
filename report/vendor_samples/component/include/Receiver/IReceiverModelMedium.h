#pragma once
#include "../IAtkObj.h"
#include "IReceiverModel.h"
#include <vector>
#include <string>
class CReceiver;
class IPolarization;
class ISystemNoiseTemperature;
class IDemodulatorModel;
class IRFFilterModel;

///
/// 中等接收模型
///
class IATKOBJECT_API IReceiverModelMedium :public IReceiverModel
{
public:
    IReceiverModelMedium(const IReceiverModelMedium&) = delete;
    IReceiverModelMedium& operator=(const IReceiverModelMedium&) = delete;

	IReceiverModelMedium(CReceiver* pCReceiver);
	~IReceiverModelMedium();
	
	//********************************************************************
	/// 获得 接收类型名称
	/// @Author	Gan Shu Long
	/// @Date	2026-03-12
	/// @Return	接收类型名称
	//********************************************************************
	std::string GetName() { return "Medium Receiver Model"; }

	//********************************************************************
	/// 获得 接收类型类型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-12
	/// @Return	接收类型类型
	//********************************************************************
	EReceiverModelType GetType() { return eReceiverModelTypeMedium; }

	//********************************************************************
	/// 设置 频率
	/// @Author	Gan Shu Long
	/// @Date	2026-03-12
	/// @Input 
	/// @Param	double 新值
	/// @Return	
	//********************************************************************
	void SetFrequency(double frequency);

	//********************************************************************
	/// 获得 频率
	/// @Author	Gan Shu Long
	/// @Date	2026-03-12
	/// @Return	频率
	//********************************************************************
	double GetFrequency();

	//********************************************************************
	/// 设置 是否自动识别频率
	/// @Author	Gan Shu Long
	/// @Date	2026-03-10
	/// @Input
	/// @Param	bAutoTrackFrequency  是否自动识别频率
	/// @Return	
	//********************************************************************
	void SetAutoTrackFrequency(bool bAutoTrackFrequency);

	//********************************************************************
	/// 获得 是否自动识别频率
	/// @Author	Gan Shu Long
	/// @Date	2026-03-10
	/// @Return	是否自动识别频率
	//********************************************************************
	bool GetAutoTrackFrequency();

	//********************************************************************
	/// 设置 接收天线增益
	/// @Author	Gan Shu Long
	/// @Date	2026-03-12
	/// @Input 
	/// @Param	dAntennaGain 接收天线增益
	/// @Return	
	//********************************************************************
	void SetAntennaGain(double dAntennaGain);

	//********************************************************************
	/// 获得 接收天线增益
	/// @Author	Gan Shu Long
	/// @Date	2026-03-12
	/// @Return	接收天线增益
	//********************************************************************
	double GetAntennaGain();

	//********************************************************************
	/// 设置 天线到LNA路线损耗
	/// @Author	Gan Shu Long
	/// @Date	2026-03-12
	/// @Input 
	/// @Param	dAntennaToLnaLineLoss 天线到LNA路线损耗
	/// @Return	
	//********************************************************************
	void SetAntennaToLnaLineLoss(double dAntennaToLnaLineLoss);

	//********************************************************************
	/// 获得 天线到LNA路线损耗
	/// @Author	Gan Shu Long
	/// @Date	2026-03-12
	/// @Return	天线到LNA路线损耗
	//********************************************************************
	double GetAntennaToLnaLineLoss();

	//********************************************************************
	/// 设置 LNA增益
	/// @Author	Gan Shu Long
	/// @Date	2026-03-12
	/// @Input 
	/// @Param	dLnaGain LNA增益
	/// @Return	
	//********************************************************************
	void SetLnaGain(double dLnaGain);

	//********************************************************************
	/// 获得 LNA增益
	/// @Author	Gan Shu Long
	/// @Date	2026-03-12
	/// @Return	LNA增益
	//********************************************************************
	double GetLnaGain();

	//********************************************************************
	/// 设置 LNA到接收机馈线损耗
	/// @Author	Gan Shu Long
	/// @Date	2026-03-12
	/// @Input 
	/// @Param	dLnaToReceiverLineLoss LNA到接收机馈线损耗
	/// @Return	
	//********************************************************************
	void SetLnaToReceiverLineLoss(double dLnaToReceiverLineLoss);

	//********************************************************************
	/// 获得 LNA到接收机馈线损耗
	/// @Author	Gan Shu Long
	/// @Date	2026-03-12
	/// @Return	LNA到接收机馈线损耗
	//********************************************************************
	double GetLnaToReceiverLineLoss();

	//********************************************************************
	/// 设置 是否使用天线极化
	/// @Author	Gan Shu Long
	/// @Date	2026-03-10
	/// @Input
	/// @Param	bEnablePolarization  是否使用天线极化
	/// @Return	
	//********************************************************************
	void SetEnablePolarization(bool bEnablePolarization);

	//********************************************************************
	/// 获得 是否使用天线极化
	/// @Author	Gan Shu Long
	/// @Date	2026-03-10
	/// @Return	是否使用天线极化
	//********************************************************************
	bool GetEnablePolarization();

	//********************************************************************
	/// 设置 天线极化类型 (子类之间类型转换，子类成员值不变)
	/// @Author	Gan Shu Long
	/// @Date	2026-03-10
	/// @Input
	/// @Param	EPolarizationType  天线极化类型
	/// @Return	
	//********************************************************************
	void SetPolarizationType(EPolarizationType newType);

	//********************************************************************
	/// 获得 天线极化类型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-10
	/// @Return	天线极化类型
	//********************************************************************
	EPolarizationType GetPolarizationType();

	//********************************************************************
	/// 获得 天线极化
	/// @Author	Gan Shu Long
	/// @Date	2026-03-10
	/// @Return	天线极化
	//********************************************************************
	IPolarization* GetPolarization() { return m_Polarization; }

	//********************************************************************
	/// 获得 系统噪声温度
	/// @Author	Gan Shu Long
	/// @Date	2026-03-12
	/// @Return	系统噪声温度
	//********************************************************************
	ISystemNoiseTemperature* GetSystemNoiseTemperature() { return m_SystemNoiseTemperature; }
	
	//********************************************************************
	/// 设置 是否自动设置调解
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Input
	/// @Param	bAutoSelectDemodulator  是否自动设置调解
	/// @Return	
	//********************************************************************
	void SetAutoSelectDemodulator(bool bAutoSelectDemodulator);

	//********************************************************************
	/// 获得 是否自动设置调解
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Return	是否自动设置调解
	//********************************************************************
	bool GetAutoSelectDemodulator();

	//********************************************************************
	/// 获得 支持的解调器类型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Return 支持的解调器类型
	//********************************************************************
	std::vector<std::string> GetSupportedDemodulators();

	//********************************************************************
	/// 按名称设置当前解调器类型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-10
	/// @Input
	/// @Param	demodulatorName  解调器类型名称
	/// @Return	
	//********************************************************************
	void SetDemodulator(std::string demodulatorName);

	//********************************************************************
	/// 获得  解调器
	/// @Author	Gan Shu Long
	/// @Date	2026-03-10
	/// @Return	 解调器
	//********************************************************************
	IDemodulatorModel* GetDemodulator() { return m_Demodulator; }

	//********************************************************************
	/// 设置  带宽
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Input 
	/// @Param	dBandwidth 带宽
	/// @Return	
	//********************************************************************
	void SetBandwidth(double dBandwidth);

	//********************************************************************
	/// 获得 带宽
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Return	带宽
	//********************************************************************
	double GetBandwidth();

	//********************************************************************
	/// 设置 是否自动调整带宽
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Input
	/// @Param	bAutoScaleBandwidth  是否自动调整带宽
	/// @Return	
	//********************************************************************
	void SetAutoScaleBandwidth(bool bAutoScaleBandwidth);

	//********************************************************************
	/// 获得 是否自动调整带宽
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Return	是否自动调整带宽
	//********************************************************************
	bool GetAutoScaleBandwidth();

	//********************************************************************
	/// 设置 是否使用滤波器
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Input
	/// @Param	bEnableFilter  是否使用滤波器
	/// @Return	
	//********************************************************************
	void SetEnableFilter(bool bEnableFilter);

	//********************************************************************
	/// 获得 是否使用滤波器
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Return	是否使用滤波器
	//********************************************************************
	bool GetEnableFilter();

	//********************************************************************
	/// 获得 支持的滤波器类型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Return 支持的滤波器类型
	//********************************************************************
	std::vector<std::string> GetSupportedFilters();

	//********************************************************************
	/// 按名称设置当前滤波器类型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Input
	/// @Param	demodulatorName  滤波器类型名称
	/// @Return	
	//********************************************************************
	void SetFilter(std::string strFilterName);

	//********************************************************************
	/// 获得  滤波器
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Return	 滤波器
	//********************************************************************
	IRFFilterModel* GetFilter() { return m_Filter; }

	//********************************************************************
	/// 设置  接收机额外损益
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Input 
	/// @Param	dGain 接收机额外损益
	/// @Return	
	//********************************************************************
	void SetGain(double dGain);

	//********************************************************************
	/// 获得 接收机额外损益
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Return	接收机额外损益
	//********************************************************************
	double GetGain();

private:
	//********************************************************************
	/// 获得 解调器类型字符串
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Input
	/// @Param	eDemodulatorModelType  解调器类型
	/// @Return 解调器类型字符串
	//********************************************************************
	std::string DemodulatorTypeToStr(EDemodulatorModelType eDemodulatorModelType);

	//********************************************************************
	/// 获得 滤波器类型字符串
	/// @Author	Gan Shu Long
	/// @Date	2026-03-11
	/// @Input
	/// @Param	eRFFilterModelType 滤波器类型
	/// @Return 滤波器类型字符串
	//********************************************************************
	std::string FilterTypeToStr(ERFFilterModelType eRFFilterModelType);

private:
	CReceiver*								m_CReceiver;									//保存一份C对象实例，省的每次改动时都需要重新便利寻找

	IPolarization*							m_Polarization;								//天线极化
	ISystemNoiseTemperature*	m_SystemNoiseTemperature;		//系统噪声温度
	IDemodulatorModel*				m_Demodulator;								//解调器
	IRFFilterModel*						m_Filter;											//滤波器
};
