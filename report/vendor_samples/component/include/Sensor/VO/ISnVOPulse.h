#pragma once
#include "../../IAtkObj.h"
#include "../../ICEnum.h"
class CSensor;

///
/// 传感器三维脉冲
///
class IATKOBJECT_API ISnVOPulse
{
public:
    ISnVOPulse(const ISnVOPulse&) = delete;
    ISnVOPulse& operator=(const ISnVOPulse&) = delete;

	ISnVOPulse(CSensor* pCSensor);
	~ISnVOPulse();

	//********************************************************************
	/// 设置是否显示
	/// @Author	Gan Shu Long
	/// @Date	2024-06-12
	/// @Input
	/// @Param	bool  是否显示
	/// @Return	
	//********************************************************************
	void SetPulseVisible(bool bPulseVisible);

	//********************************************************************
	/// 获得是否显示
	/// @Author	Gan Shu Long
	/// @Date	2024-06-12
	/// @Return	是否显示
	//********************************************************************
	bool GetPulseVisible();

	//********************************************************************
	/// 设置振幅
	/// @Author	Gan Shu Long
	/// @Date	2024-06-12
	/// @Input
	/// @Param	double  振幅
	/// @Return	
	//********************************************************************
	void SetAmplitude(double dAmplitude);

	//********************************************************************
	/// 获得振幅
	/// @Author	Gan Shu Long
	/// @Date	2024-06-12
	/// @Return	振幅
	//********************************************************************
	double GetAmplitude();

	//********************************************************************
	/// 设置脉冲长度
	/// @Author	Gan Shu Long
	/// @Date	2024-06-12
	/// @Input
	/// @Param	double  脉冲长度
	/// @Return	
	//********************************************************************
	void SetLength(double dLength);

	//********************************************************************
	/// 获得脉冲长度
	/// @Author	Gan Shu Long
	/// @Date	2024-06-12
	/// @Return	脉冲长度
	//********************************************************************
	double GetLength();

	//********************************************************************
	/// 设置频率类型
	/// @Author	Gan Shu Long
	/// @Date	2024-06-12
	/// @Input
	/// @Param	ESnVOPulseFrequencyPreset  频率类型
	/// @Return	
	//********************************************************************
	void SetPreselFreq(ESnVOPulseFrequencyPreset ePreselFreq);

	//********************************************************************
	/// 获得频率类型
	/// @Author	Gan Shu Long
	/// @Date	2024-06-12
	/// @Return	频率类型
	//********************************************************************
	ESnVOPulseFrequencyPreset GetPreselFreq();

	//********************************************************************
	/// 设置频率
	/// @Author	Gan Shu Long
	/// @Date	2024-06-12
	/// @Input
	/// @Param	double  频率
	/// @Return	
	//********************************************************************
	void SetFreqValue(double dFreqValue);

	//********************************************************************
	/// 获得频率
	/// @Author	Gan Shu Long
	/// @Date	2024-06-12
	/// @Return	频率
	//********************************************************************
	double GetFreqValue();

	//********************************************************************
	/// 设置是否反向
	/// @Author	Gan Shu Long
	/// @Date	2024-06-12
	/// @Input
	/// @Param	bool  是否反向
	/// @Return	
	//********************************************************************
	void SetFreqReverseDir(bool bFreqReverseDir);

	//********************************************************************
	/// 获得是否反向
	/// @Author	Gan Shu Long
	/// @Date	2024-06-12
	/// @Return	是否反向
	//********************************************************************
	bool GetFreqReverseDir();

	//********************************************************************
	/// 重置为缺省值
	/// @Author	Gan Shu Long
	/// @Date	2024-06-12
	/// @Input 
	/// @Param	
	/// @Return	
	//********************************************************************
	void ResetToDefaults();

private:
	CSensor* m_pCSensor;
	
	//bool	m_PulseVisible;						//是否显示
	//double	m_Amplitude;						//振幅
	//double	m_Length;							//脉冲长度
	ESnVOPulseFrequencyPreset	m_PreselFreq;	//频率类型	//C对象没有该值，默认为中速
	//double	m_FreqValue;						//频率
	//bool	m_FreqReverseDir;					//是否反向

	double	m_d_Amplitude;						//默认振幅
	double	m_d_Length;							//默认脉冲长度
	ESnVOPulseFrequencyPreset	m_d_PreselFreq;	//默认频率类型
	double	m_d_FreqValue;						//默认频率
	bool	m_d_FreqReverseDir;					//默认是否反向
	
};

