#pragma once
#include "../../IAtkObj.h"
#include "../../ICEnum.h"
class CGfx3DAttitudeSphere;
class IPlanetProjectionCollection;

///
/// 三维姿态球
///
class IATKOBJECT_API IVOAttitudeSphere
{
public:
    IVOAttitudeSphere(const IVOAttitudeSphere&) = delete;
    IVOAttitudeSphere& operator=(const IVOAttitudeSphere&) = delete;

	IVOAttitudeSphere(CGfx3DAttitudeSphere* pCGfx3DAttitudeSphere);
	~IVOAttitudeSphere();

	//********************************************************************
	/// 设置是否显示
	/// @Author	Gan Shu Long
	/// @Date	2026-01-21
	/// @Input
	/// @Param	bIsVisible  是否显示
	/// @Return		
	//********************************************************************
	void SetIsVisible(bool bIsVisible);

	//********************************************************************
	/// 获得是否显示
	/// @Author	Gan Shu Long
	/// @Date	2026-01-21
	/// @Return	是否显示
	//********************************************************************
	bool GetIsVisible();

	//********************************************************************
	/// 设置颜色
	/// @Author	Gan Shu Long
	/// @Date	2026-01-21
	/// @Input
	/// @Param	nColor  颜色
	/// @Return
	//********************************************************************
	void SetColor(int nColor);

	//********************************************************************
	/// 获取设置颜色
	/// @Author	Gan Shu Long
	/// @Date	2026-01-21
	/// @Return 颜色
	//********************************************************************
	int GetColor();

	//********************************************************************
	/// 设置零度线宽
	/// @Author	Gan Shu Long
	/// @Date	2026-01-21
	/// @Input
	/// @Param	eZeroDegLineWidth  零度线宽
	/// @Return	
	//********************************************************************
	void SetZeroDegLineWidth(ELineWidth eZeroDegLineWidth);

	//********************************************************************
	/// 获得零度线宽
	/// @Author	Gan Shu Long
	/// @Date	2026-01-21
	/// @Return	零度线宽	
	//********************************************************************
	ELineWidth GetZeroDegLineWidth();
	
	//********************************************************************
	/// 设置线宽
	/// @Author	Gan Shu Long
	/// @Date	2026-01-21
	/// @Input
	/// @Param	eLineWidth  线宽
	/// @Return	
	//********************************************************************
	void SetLineWidth(ELineWidth eLineWidth);

	//********************************************************************
	/// 获得线宽
	/// @Author	Gan Shu Long
	/// @Date	2026-01-21
	/// @Return	线宽	
	//********************************************************************
	ELineWidth GetLineWidth();

	////********************************************************************
	///// 设置参考坐标系
	///// @Author	Gan Shu Long
	///// @Date	2026-01-21
	///// @Input
	///// @Param	eLineWidth  参考坐标系
	///// @Return	
	////********************************************************************
	//void SetReferenceAxes(ELineWidth eLineWidth);

	////********************************************************************
	///// 获得参考坐标系
	///// @Author	Gan Shu Long
	///// @Date	2026-01-21
	///// @Return	参考坐标系	
	////********************************************************************
	//ELineWidth GetReferenceAxes();

	//********************************************************************
	/// 设置比例
	/// @Author	Gan Shu Long
	/// @Date	2026-01-21
	/// @Input
	/// @Param	dScale  比例
	/// @Return		
	//********************************************************************
	void SetScale(double dScale);

	//********************************************************************
	/// 获得比例
	/// @Author	Gan Shu Long
	/// @Date	2026-01-21
	/// @Return	比例		
	//********************************************************************
	double GetScale();

	//********************************************************************
	/// 获得行星投影列表
	/// @Author	Gan Shu Long
	/// @Date	2026-01-21
	/// @Return	行星投影列表
	//********************************************************************
	IPlanetProjectionCollection* GetPlanetProjectionCollection() { return m_PlanetProjectionCollection; }

private:
	IPlanetProjectionCollection*			m_PlanetProjectionCollection;	//行星投影列表

	CGfx3DAttitudeSphere*					m_pCGfx3DAttitudeSphere;		//C对象姿态球
};

