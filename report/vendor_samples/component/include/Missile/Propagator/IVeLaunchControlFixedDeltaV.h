#pragma once
#include "../../IAtkObj.h"
#include "IVeLaunchControl.h"

///
/// 增量：固定速度
///
class IATKOBJECT_API IVeLaunchControlFixedDeltaV : public IVeLaunchControl
{
public:
    IVeLaunchControlFixedDeltaV(const IVeLaunchControlFixedDeltaV&) = delete;
    IVeLaunchControlFixedDeltaV& operator=(const IVeLaunchControlFixedDeltaV&) = delete;

	IVeLaunchControlFixedDeltaV(double* pDeltaV);
	~IVeLaunchControlFixedDeltaV();

	//********************************************************************
	/// 获得速度增量
	/// @Author	Gan Shu Long
	/// @Date	2026-03-05
	/// @Return	速度增量
	//********************************************************************
	double GetDeltaV();

	//********************************************************************
	/// 设置速度增量
	/// @Author	Gan Shu Long
	/// @Date	2026-03-05
	/// @Input
	/// @Param	dDeltaV 速度增量
	/// @Return	
	//********************************************************************
	void SetDeltaV(double dDeltaV);

private:
	double*			m_pDeltaV;
};
