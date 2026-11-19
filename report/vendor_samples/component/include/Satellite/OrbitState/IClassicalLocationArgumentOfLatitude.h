#pragma once
#include "IClassicalLocation.h"
class CSatellite;

///
/// 轨道根数纬度辐角
///
class IATKOBJECT_API IClassicalLocationArgumentOfLatitude : public IClassicalLocation
{
public:
    IClassicalLocationArgumentOfLatitude(const IClassicalLocationArgumentOfLatitude&) = delete;
    IClassicalLocationArgumentOfLatitude& operator=(const IClassicalLocationArgumentOfLatitude&) = delete;

	IClassicalLocationArgumentOfLatitude(CSatellite* pSate);
	~IClassicalLocationArgumentOfLatitude();

	//********************************************************************
	/// 设置纬度辐角
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dArgOfLat 纬度辐角
	/// @Return	
	//********************************************************************
	void SetValue(double dArgOfLat);
	//********************************************************************
	/// 获得纬度辐角
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	纬度辐角
	//********************************************************************
	double GetValue();

private:
	CSatellite*     m_pSate;

};
