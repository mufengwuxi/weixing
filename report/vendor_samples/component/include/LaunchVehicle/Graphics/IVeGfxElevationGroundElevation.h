#pragma once
#include "../../IAtkObj.h"
#include "IVeGfxElevation.h"
struct CGfxSwath;

///
/// 地面仰角 地面仰角包络
///
class IATKOBJECT_API IVeGfxElevationGroundElevation : public IVeGfxElevation
{
public:
    IVeGfxElevationGroundElevation(const IVeGfxElevationGroundElevation&) = delete;
    IVeGfxElevationGroundElevation& operator=(const IVeGfxElevationGroundElevation&) = delete;

	IVeGfxElevationGroundElevation(CGfxSwath* pCGfxSwath);
	~IVeGfxElevationGroundElevation();

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

