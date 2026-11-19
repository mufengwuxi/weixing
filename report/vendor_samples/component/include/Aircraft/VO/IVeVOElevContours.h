#pragma once
#include "../../IAtkObj.h"
struct CObjData;
struct CGfxContour;

///
/// 三维等高线
///
class IATKOBJECT_API IVeVOElevContours
{
public:
    IVeVOElevContours(const IVeVOElevContours&) = delete;
    IVeVOElevContours& operator=(const IVeVOElevContours&) = delete;

	IVeVOElevContours(CGfxContour* pCGfxContour);
	~IVeVOElevContours();

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
	/// 设置是否显示Cones
	/// @Author	Gan Shu Long
	/// @Date	2026-01-06
	/// @Input
	/// @Param	bIsConesVisible  是否显示Cones
	/// @Return		
	//********************************************************************
	void SetIsConesVisible(bool bIsConesVisible);

	//********************************************************************
	/// 获得是否显示Cones
	/// @Author	Gan Shu Long
	/// @Date	2026-01-06
	/// @Return	是否显示Cones
	//********************************************************************
	bool GetIsConesVisible();

	//********************************************************************
	/// 设置透明度
	/// @Author	Gan Shu Long
	/// @Date	2026-01-06
	/// @Input
	/// @Param	bIsVisible  透明度
	/// @Return		
	//********************************************************************
	void SetTranslucency(double dTranslucency);

	//********************************************************************
	/// 获得填充透明度
	/// @Author	Gan Shu Long
	/// @Date	2026-01-06
	/// @Return	填充透明度
	//********************************************************************
	double GetTranslucency();

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

private:
	CGfxContour*							m_pCGfxContour;		//C对象等高线指针
};

