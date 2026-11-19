#pragma once
#include "../../IAtkObj.h"
struct CGfxContour;
class IVeGfxElevationsCollection;

///
/// 二维等高线
///
class IATKOBJECT_API IVeGfxElevContours
{
public:
    IVeGfxElevContours(const IVeGfxElevContours&) = delete;
    IVeGfxElevContours& operator=(const IVeGfxElevContours&) = delete;

	IVeGfxElevContours(CGfxContour* pCGfxContour);
	~IVeGfxElevContours();

	//********************************************************************
	/// 设置是否显示
	/// @Author	Gan Shu Long
	/// @Date	2026-01-06
	/// @Input
	/// @Param	bIsVisible  是否显示
	/// @Return		
	//********************************************************************
	void SetIsVisible(bool bIsVisible);

	//********************************************************************
	/// 获得是否显示
	/// @Author	Gan Shu Long
	/// @Date	2026-01-06
	/// @Return	是否显示
	//********************************************************************
	bool GetIsVisible();

	//********************************************************************
	/// 设置是否显示填充
	/// @Author	Gan Shu Long
	/// @Date	2026-01-06
	/// @Input
	/// @Param	bIsVisible  是否显示填充
	/// @Return		
	//********************************************************************
	void SetIsFillVisible(bool bIsVisible);

	//********************************************************************
	/// 获得是否显示填充
	/// @Author	Gan Shu Long
	/// @Date	2026-01-06
	/// @Return	是否显示填充
	//********************************************************************
	bool GetIsFillVisible();

	//********************************************************************
	/// 设置填充透明度
	/// @Author	Gan Shu Long
	/// @Date	2026-01-06
	/// @Input
	/// @Param	bIsVisible  填充透明度
	/// @Return		
	//********************************************************************
	void SetFillTranslucency(double dFillTranslucency);

	//********************************************************************
	/// 获得填充透明度
	/// @Author	Gan Shu Long
	/// @Date	2026-01-06
	/// @Return	填充透明度
	//********************************************************************
	double GetFillTranslucency();

	//********************************************************************
	/// 获得分级列表
	/// @Author	Gan Shu Long
	/// @Date	2026-01-06
	/// @Return	分级列表
	//********************************************************************
	IVeGfxElevationsCollection* GetElevations() { return m_Elevations; }

private:
	IVeGfxElevationsCollection*	m_Elevations;				//分级

	CGfxContour*							m_pCGfxContour;		//C对象等高线指针
};

