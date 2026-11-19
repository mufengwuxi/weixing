#pragma once
#include "IVeLeadTrailData.h"
struct CGfxVehPass;

///
/// 二维轨迹时间类型
///
class IATKOBJECT_API IVeLeadTrailDataTime : public IVeLeadTrailData
{
public:
    IVeLeadTrailDataTime(const IVeLeadTrailDataTime&) = delete;
    IVeLeadTrailDataTime& operator=(const IVeLeadTrailDataTime&) = delete;

	IVeLeadTrailDataTime(CGfxVehPass* pCGfxVehPass);
	~IVeLeadTrailDataTime();

	//********************************************************************
	/// 设置轨迹时间属性值
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dTime  轨迹时间属性值
	/// @Return	
	//********************************************************************
	void SetTime(double dTime);

	//********************************************************************
	/// 获得轨迹时间属性值
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	轨迹时间属性值	
	//********************************************************************
	double GetTime();

private:
	CGfxVehPass*	m_pCGfxVehPass;		//C对象二维轨迹指针
};

