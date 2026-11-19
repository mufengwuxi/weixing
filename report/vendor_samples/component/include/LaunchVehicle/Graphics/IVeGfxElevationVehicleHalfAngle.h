#pragma once
#include "../../IAtkObj.h"
#include "IVeGfxElevation.h"
struct CGfxSwath;

///
/// 飞行器半锥角 飞行器半锥角包络
///
class IATKOBJECT_API IVeGfxElevationVehicleHalfAngle : public IVeGfxElevation
{
public:
    IVeGfxElevationVehicleHalfAngle(const IVeGfxElevationVehicleHalfAngle&) = delete;
    IVeGfxElevationVehicleHalfAngle& operator=(const IVeGfxElevationVehicleHalfAngle&) = delete;

	IVeGfxElevationVehicleHalfAngle(CGfxSwath* pCGfxSwath);
	~IVeGfxElevationVehicleHalfAngle();
	
	//********************************************************************
	/// 设置角度
	/// @Author	Gan Shu Long
	/// @Date	2026-01-28
	/// @Input
	/// @Param	dAngle  角度
	/// @Return		
	//********************************************************************
	void SetAngle(double dAngle);

	//********************************************************************
	/// 获得角度
	/// @Author	Gan Shu Long
	/// @Date	2026-01-28
	/// @Return	角度
	//********************************************************************
	double GetAngle();

private:
	CGfxSwath*           m_pCGfxSwath;
};

