#pragma once
#include "../../IAtkObj.h"
struct CObjData;
struct CGfxRange;

///
/// 二维范围
///
class IATKOBJECT_API IVORangeContours
{
public:
    IVORangeContours(const IVORangeContours&) = delete;
    IVORangeContours& operator=(const IVORangeContours&) = delete;

	IVORangeContours(CGfxRange* pCGfxRange);
	~IVORangeContours();

	//********************************************************************
	/// 设置是否显示
	/// @Author	Gan Shu Long
	/// @Date	2026-01-09
	/// @Input
	/// @Param	bIsVisible  是否显示
	/// @Return		
	//********************************************************************
	void SetIsVisible(bool bIsVisible);

	//********************************************************************
	/// 获得是否显示
	/// @Author	Gan Shu Long
	/// @Date	2026-01-09
	/// @Return	是否显示
	//********************************************************************
	bool GetIsVisible();

	//********************************************************************
	/// 设置是否显示半透明线条
	/// @Author	Gan Shu Long
	/// @Date	2026-01-09
	/// @Input
	/// @Param	bTranslucentLines  是否显示半透明线条
	/// @Return		
	//********************************************************************
	void SetTranslucentLines(bool bTranslucentLines);

	//********************************************************************
	/// 获得是否显示半透明线条
	/// @Author	Gan Shu Long
	/// @Date	2026-01-09
	/// @Return	是否显示半透明线条
	//********************************************************************
	bool GetTranslucentLines();

	//********************************************************************
	/// 设置填充透明度
	/// @Author	Gan Shu Long
	/// @Date	2026-01-09
	/// @Input
	/// @Param	bPercentTranslucency  填充透明度
	/// @Return		
	//********************************************************************
	void SetPercentTranslucency(double dPercentTranslucency);

	//********************************************************************
	/// 获得填充透明度
	/// @Author	Gan Shu Long
	/// @Date	2026-01-09
	/// @Return	填充透明度
	//********************************************************************
	double GetPercentTranslucency();

private:
	CGfxRange*							m_pCGfxRange;			//C对象范围指针
};

