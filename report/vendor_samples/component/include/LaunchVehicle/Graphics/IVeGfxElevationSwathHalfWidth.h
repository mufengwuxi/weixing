#pragma once
#include "../../IAtkObj.h"
#include "IVeGfxElevation.h"
struct CGfxSwath;

///
/// 刈幅半幅宽
///
class IATKOBJECT_API IVeGfxElevationSwathHalfWidth : public IVeGfxElevation
{
public:
    IVeGfxElevationSwathHalfWidth(const IVeGfxElevationSwathHalfWidth&) = delete;
    IVeGfxElevationSwathHalfWidth& operator=(const IVeGfxElevationSwathHalfWidth&) = delete;

	IVeGfxElevationSwathHalfWidth(CGfxSwath* pCGfxSwath);
	~IVeGfxElevationSwathHalfWidth();

	//********************************************************************
	/// 设置距离
	/// @Author	Gan Shu Long
	/// @Date	2026-01-28
	/// @Input
	/// @Param	dDistance  距离
	/// @Return		
	//********************************************************************
	void SetDistance(double dDistance);

	//********************************************************************
	/// 获得距离
	/// @Author	Gan Shu Long
	/// @Date	2026-01-28
	/// @Return	距离
	//********************************************************************
	double GetDistance();

private:
	CGfxSwath*           m_pCGfxSwath;
};

