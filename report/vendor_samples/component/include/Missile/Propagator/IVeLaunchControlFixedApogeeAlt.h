#pragma once
#include "../../IAtkObj.h"
#include "IVeLaunchControl.h"

///
/// 增量：固定远地点距离
///
class IATKOBJECT_API IVeLaunchControlFixedApogeeAlt : public IVeLaunchControl
{
public:
    IVeLaunchControlFixedApogeeAlt(const IVeLaunchControlFixedApogeeAlt&) = delete;
    IVeLaunchControlFixedApogeeAlt& operator=(const IVeLaunchControlFixedApogeeAlt&) = delete;

	IVeLaunchControlFixedApogeeAlt(double* pApogeeAlt);
	~IVeLaunchControlFixedApogeeAlt();

	//********************************************************************
	/// 获得距离增量
	/// @Author	Gan Shu Long
	/// @Date	2026-03-05
	/// @Return	距离增量
	//********************************************************************
	double GetApogeeAlt();

	//********************************************************************
	/// 设置距离增量
	/// @Author	Gan Shu Long
	/// @Date	2026-03-05
	/// @Input
	/// @Param	dApogeeAlt 距离增量
	/// @Return	
	//********************************************************************
	void SetApogeeAlt(double dApogeeAlt);

private:
	double*			m_pApogeeAlt;
};
