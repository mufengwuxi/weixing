#pragma once
#include "IClassicalLocation.h"
class CSatellite;

///
/// 轨道根数偏近点角
///
class IATKOBJECT_API IClassicalLocationEccentricAnomaly : public IClassicalLocation
{
public:
    IClassicalLocationEccentricAnomaly(const IClassicalLocationEccentricAnomaly&) = delete;
    IClassicalLocationEccentricAnomaly& operator=(const IClassicalLocationEccentricAnomaly&) = delete;

	IClassicalLocationEccentricAnomaly(CSatellite* pSate);
	~IClassicalLocationEccentricAnomaly();

	//********************************************************************
	/// 设置偏近点角
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dEccAno 偏近点角
	/// @Return	
	//********************************************************************
	void SetValue(double dEccAno);
	//********************************************************************
	/// 获得偏近点角
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	偏近点角
	//********************************************************************
	double GetValue();

private:
	CSatellite*     m_pSate;
};
