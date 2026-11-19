#pragma once
#include "IClassicalLocation.h"
class CSatellite;

///
/// 轨道根数过近地点时间
///
class IATKOBJECT_API IClassicalLocationTimePastPerigee : public IClassicalLocation
{
public:
    IClassicalLocationTimePastPerigee(const IClassicalLocationTimePastPerigee&) = delete;
    IClassicalLocationTimePastPerigee& operator=(const IClassicalLocationTimePastPerigee&) = delete;

	IClassicalLocationTimePastPerigee(CSatellite* pSate);
	~IClassicalLocationTimePastPerigee();

	//********************************************************************
	/// 设置过近地点时间
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dTPP 过近地点时间
	/// @Return	
	//********************************************************************
	void SetValue(double dTPP);
	//********************************************************************
	/// 获得过近地点时间
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	过近地点时间
	//********************************************************************
	double GetValue();

private:
	CSatellite*     m_pSate;

};
