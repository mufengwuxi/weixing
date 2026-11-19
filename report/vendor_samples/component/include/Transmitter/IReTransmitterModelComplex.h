#pragma once
#include "../IAtkObj.h"
#include "IReTransmitterModel.h"
#include "../ICEnum.h"
#include <vector>
#include <string>
struct CObjData;
class CTransmitter;
class IAntennaControl;
class IPolarization;
class IRFFilterModel;

///
/// 转发发射器：复杂转发器
///
class IATKOBJECT_API IReTransmitterModelComplex :public IReTransmitterModel
{
public:
    IReTransmitterModelComplex(const IReTransmitterModelComplex&) = delete;
    IReTransmitterModelComplex& operator=(const IReTransmitterModelComplex&) = delete;

	IReTransmitterModelComplex(CObjData* pCObjData);
	~IReTransmitterModelComplex();

	//********************************************************************
	/// 设置  功率
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Input 
	/// @Param	dSaturatedPower 功率
	/// @Return	
	//********************************************************************
	void SetSaturatedPower(double dSaturatedPower);

	//********************************************************************
	/// 获得 功率
	/// @Author	Gan Shu Long
	/// @Date	2026-03-25
	/// @Return	功率
	//********************************************************************
	double GetSaturatedPower();

	//********************************************************************
	/// 获得 天线
	/// @Author	Gan Shu Long
	/// @Date	2026-03-13
	/// @Return	天线
	//********************************************************************
	IAntennaControl* GetAntennaControl() { return m_AntennaControl; }

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
	IAntennaControl*			m_AntennaControl;		//天线
	IPolarization*				m_Polarization;			//天线极化
	IRFFilterModel*			m_Filter;						//滤波器

	CTransmitter*				m_CTransmitter;
};
