#pragma once
#include "../IAtkObj.h"
#include "IAntennaModel.h" 
#include "../ICEnum.h"
struct CCRAntenna;

///
/// 天线模型 Gaussian
///
class IATKOBJECT_API IAntennaModelGaussian :public IAntennaModel
{
public:
    IAntennaModelGaussian(const IAntennaModelGaussian&) = delete;
    IAntennaModelGaussian& operator=(const IAntennaModelGaussian&) = delete;

	IAntennaModelGaussian(CCRAntenna* pCCRAntenna);
	virtual ~IAntennaModelGaussian();

	//********************************************************************
	/// 获得 天线模型名称
	/// @Author	Gan Shu Long
	/// @Date	2026-03-16
	/// @Return	天线模型名称
	//********************************************************************
	std::string GetName() { return "Gaussian"; }

	//********************************************************************
	/// 获得 天线模型类型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-16
	/// @Return	天线模型类型
	//********************************************************************
	EAntennaModelType GetType() { return eAntennaModelTypeGaussian; }

	//********************************************************************
	/// 设置  天线效率%
	/// @Author	Gan Shu Long
	/// @Date	2026-03-16
	/// @Input 
	/// @Param	dEfficiency 天线效率%
	/// @Return	
	//********************************************************************
	void SetEfficiency(double dEfficiency);

	//********************************************************************
	/// 获得 天线效率%
	/// @Author	Gan Shu Long
	/// @Date	2026-03-16
	/// @Return	天线效率%
	//********************************************************************
	double GetEfficiency();

	//********************************************************************
	/// 设置  后波瓣增益
	/// @Author	Gan Shu Long
	/// @Date	2026-03-16
	/// @Input 
	/// @Param	dBacklobeGain 后波瓣增益
	/// @Return	
	//********************************************************************
	void SetBacklobeGain(double dBacklobeGain);

	//********************************************************************
	/// 获得 后波瓣增益
	/// @Author	Gan Shu Long
	/// @Date	2026-03-16
	/// @Return	后波瓣增益
	//********************************************************************
	double GetBacklobeGain();

	//********************************************************************
	/// 设置  天线直径单位m
	/// @Author	Gan Shu Long
	/// @Date	2026-03-16
	/// @Input 
	/// @Param	dDiameter 天线直径单位m
	/// @Return	
	//********************************************************************
	void SetDiameter(double dDiameter);

	//********************************************************************
	/// 获得 天线直径单位m
	/// @Author	Gan Shu Long
	/// @Date	2026-03-16
	/// @Return	天线直径单位m
	//********************************************************************
	double GetDiameter();


private:
	CCRAntenna*					m_pCCRAntenna;					//天线结构体
};
