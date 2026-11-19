#pragma once
#include "../IAtkObj.h"
#include <string>
#include "../ICEnum.h"
#include "../IClassInfo.h"
class CTransmitter;

///
/// 调制器：基类
///
class IATKOBJECT_API IModulatorModel :public IClassInfo
{
public:
    IModulatorModel(const IModulatorModel&) = delete;
    IModulatorModel& operator=(const IModulatorModel&) = delete;

	IModulatorModel(CTransmitter* pCTransmitte);
	virtual ~IModulatorModel();
	
	//********************************************************************
	/// 获得 调制器名称
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Return	调制器名称
	//********************************************************************
	std::string GetName() { return m_Name; }

	//********************************************************************
	/// 获得 调制器类型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Return	调制器类型
	//********************************************************************
	EModulatorModelType GetType() { return m_Type; }
	
	//********************************************************************
	/// 设置 是否选择信号PSD
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Input
	/// @Param	bEnableSignalPSD  是否选择信号
	/// @Return	
	//********************************************************************
	void SetEnableSignalPSD(bool bEnableSignalPSD);

	//********************************************************************
	/// 获得 是否选择信号
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Return	是否选择信号
	//********************************************************************
	bool GetEnableSignalPSD();

	//********************************************************************
	/// 设置  空频谱数量
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Input 
	/// @Param	nPSDLimitMultiplier 空频谱数量
	/// @Return	
	//********************************************************************
	void SetPSDLimitMultiplier(int nPSDLimitMultiplier);

	//********************************************************************
	/// 获得 空频谱数量
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Return	空频谱数量
	//********************************************************************
	int GetPSDLimitMultiplier();

	//********************************************************************
	/// 设置 是否使用自动调整带宽标识
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Input
	/// @Param	bAutoScaleBandwidth  是否使用自动调整带宽标识
	/// @Return	
	//********************************************************************
	void SetAutoScaleBandwidth(bool bAutoScaleBandwidth);

	//********************************************************************
	/// 获得 是否使用自动调整带宽标识
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Return	是否使用自动调整带宽标识
	//********************************************************************
	bool GetAutoScaleBandwidth();

	//********************************************************************
	/// 设置 是否使用对称调节标识
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Input
	/// @Param	bSymmetricBandwidth  是否使用对称调节标识
	/// @Return	
	//********************************************************************
	void SetSymmetricBandwidth(bool bSymmetricBandwidth);

	//********************************************************************
	/// 获得 是否使用对称调节标识
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Return	是否使用对称调节标识
	//********************************************************************
	bool GetSymmetricBandwidth();

	//********************************************************************
	/// 设置  带宽上限单位MHz
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Input 
	/// @Param	dUpperBandwidthLimit 带宽上限单位MHz
	/// @Return	
	//********************************************************************
	void SetUpperBandwidthLimit(double dUpperBandwidthLimit);

	//********************************************************************
	/// 获得 带宽上限单位MHz
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Return	带宽上限单位MHz
	//********************************************************************
	double GetUpperBandwidthLimit();

	//********************************************************************
	/// 设置  带宽下限单位MHz
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Input 
	/// @Param	dLowerBandwidthLimit 带宽下限单位MHz
	/// @Return	
	//********************************************************************
	void SetLowerBandwidthLimit(double dLowerBandwidthLimit);

	//********************************************************************
	/// 获得 带宽下限单位MHz
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Return	带宽下限单位MHz
	//********************************************************************
	double GetLowerBandwidthLimit();

	//********************************************************************
	/// 设置  带宽单位MHz
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Input 
	/// @Param	dBandwidth  带宽单位MHz
	/// @Return	
	//********************************************************************
	void SetBandwidth(double dBandwidth);

	//********************************************************************
	/// 获得  带宽单位MHz
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Return	 带宽单位MHz
	//********************************************************************
	double GetBandwidth();

protected:
	std::string						m_Name;			//模型名称
	EModulatorModelType		m_Type;				//模型类型

private:
	CTransmitter*					m_CTransmitter;
};
