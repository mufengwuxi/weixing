#pragma once
#include "../../IAtkObj.h"
#include "IVeLaunchControl.h"

///
/// 增量：固定速度与最小偏心率
///
class IATKOBJECT_API IVeLaunchControlFixedDeltaVMinEcc : public IVeLaunchControl
{
public:
    IVeLaunchControlFixedDeltaVMinEcc(const IVeLaunchControlFixedDeltaVMinEcc&) = delete;
    IVeLaunchControlFixedDeltaVMinEcc& operator=(const IVeLaunchControlFixedDeltaVMinEcc&) = delete;

	IVeLaunchControlFixedDeltaVMinEcc(double* pDeltaVMin);
	~IVeLaunchControlFixedDeltaVMinEcc();

	//********************************************************************
	/// 获得速度增量
	/// @Author	Gan Shu Long
	/// @Date	2026-03-05
	/// @Return	速度增量
	//********************************************************************
	double GetDeltaVMin();

	//********************************************************************
	/// 设置速度增量
	/// @Author	Gan Shu Long
	/// @Date	2026-03-05
	/// @Input
	/// @Param	dDeltaV 速度增量
	/// @Return	
	//********************************************************************
	void SetDeltaVMin(double dDeltaVMin);

private:
	double*			m_pDeltaVMin;
};
