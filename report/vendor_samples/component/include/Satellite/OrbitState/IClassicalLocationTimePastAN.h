#pragma once
#include "IClassicalLocation.h"
class CSatellite;

///
/// 轨道根数过升交点时间
///
class IATKOBJECT_API IClassicalLocationTimePastAN : public IClassicalLocation
{
public:
    IClassicalLocationTimePastAN(const IClassicalLocationTimePastAN&) = delete;
    IClassicalLocationTimePastAN& operator=(const IClassicalLocationTimePastAN&) = delete;

	IClassicalLocationTimePastAN(CSatellite* pSate);
	~IClassicalLocationTimePastAN();

	//********************************************************************
	/// 设置过升交点时间
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dTPAN 过升交点时间
	/// @Return	
	//********************************************************************
	void SetValue(double dTPAN);
	//********************************************************************
	/// 获得过升交点时间
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	过升交点时间
	//********************************************************************
	double GetValue();

private:
	CSatellite*     m_pSate;
};

