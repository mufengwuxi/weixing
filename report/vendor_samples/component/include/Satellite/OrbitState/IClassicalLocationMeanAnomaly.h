#pragma once
#include "IClassicalLocation.h"
class CSatellite;

///
/// 轨道根数平近点角
///
class IATKOBJECT_API IClassicalLocationMeanAnomaly : public IClassicalLocation
{
public:
    IClassicalLocationMeanAnomaly(const IClassicalLocationMeanAnomaly&) = delete;
    IClassicalLocationMeanAnomaly& operator=(const IClassicalLocationMeanAnomaly&) = delete;

	IClassicalLocationMeanAnomaly(CSatellite* pSate);
	~IClassicalLocationMeanAnomaly();

	//********************************************************************
	/// 设置平近点角
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dMeanAno 平近点角
	/// @Return	
	//********************************************************************
	void SetValue(double dMeanAno);
	//********************************************************************
	/// 获得平近点角
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	平近点角
	//********************************************************************
	double GetValue();

public:
	CSatellite*     m_pSate;
};
