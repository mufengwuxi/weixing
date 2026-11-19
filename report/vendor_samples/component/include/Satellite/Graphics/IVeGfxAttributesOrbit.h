#pragma once
#include "IVeGfxAttributesBasic.h"
struct CGfxVeh2D;

///
/// 二维显示轨迹属性
///
class IATKOBJECT_API IVeGfxAttributesOrbit : public IVeGfxAttributesBasic
{
public:
    IVeGfxAttributesOrbit(const IVeGfxAttributesOrbit&) = delete;
    IVeGfxAttributesOrbit& operator=(const IVeGfxAttributesOrbit&) = delete;

	IVeGfxAttributesOrbit(CGfxVeh2D* pCGfxVeh2D);
	~IVeGfxAttributesOrbit();

	//********************************************************************
	/// 设置是否显示二维轨迹
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	bIsGroundTrackVisible  是否显示二维轨迹
	/// @Return	
	//********************************************************************
	void SetIsGroundTrackVisible(bool bIsGroundTrackVisible);

	//********************************************************************
	/// 获得是否显示二维轨迹
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	true-显示
	//********************************************************************
	bool GetIsGroundTrackVisible();

	//********************************************************************
	/// 设置是否显示三维地面轨道
	/// @Author	Gan Shu Long
	/// @Date	2026-01-26
	/// @Input
	/// @Param	bIsOrbitGroundVisible  是否显示三维地面轨道
	/// @Return	
	//********************************************************************
	void SetIsOrbitGroundVisible(bool bIsOrbitGroundVisible);

	//********************************************************************
	/// 获得是否显示三维地面轨道
	/// @Author	Gan Shu Long
	/// @Date	2026-01-26
	/// @Return	true-显示
	//********************************************************************
	bool GetIsOrbitGroundVisible();

	//********************************************************************
	/// 设置是否显示三维轨道
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	bIsOrbitVisible  是否显示三维轨道
	/// @Return	
	//********************************************************************
	void SetIsOrbitVisible(bool bIsOrbitVisible);

	//********************************************************************
	/// 获得是否显示三维轨道
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	true-显示
	//********************************************************************
	bool GetIsOrbitVisible();

private:
	CGfxVeh2D*		m_pCGfxVeh2D;				//C对象二维显示指针

};
