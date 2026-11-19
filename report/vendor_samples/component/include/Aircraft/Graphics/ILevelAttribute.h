#pragma once
#include "../../IAtkObj.h"
#include "../../ICEnum.h"
struct CGfxContourAttri;

///
/// 二维范围分级元素
///
class IATKOBJECT_API ILevelAttribute
{
public:
    ILevelAttribute(const ILevelAttribute&) = delete;
    ILevelAttribute& operator=(const ILevelAttribute&) = delete;

	ILevelAttribute(CGfxContourAttri* pCGfxContourAttri);
	~ILevelAttribute();

	//********************************************************************
	/// 设置分级deg
	/// @Author	Gan Shu Long
	/// @Date	2026-01-06
	/// @Input
	/// @Param	dLevel  分级
	/// @Return
	//********************************************************************
	void SetLevel(double dLevel);

	//********************************************************************
	/// 获取分级deg
	/// @Author	Gan Shu Long
	/// @Date	2026-01-06
	/// @Return 分级deg
	//********************************************************************
	double GetLevel();

	//********************************************************************
	/// 设置颜色
	/// @Author	Gan Shu Long
	/// @Date	2026-01-06
	/// @Input
	/// @Param	nColor  颜色
	/// @Return
	//********************************************************************
	void SetColor(int nColor);

	//********************************************************************
	/// 获取设置颜色
	/// @Author	Gan Shu Long
	/// @Date	2026-01-06
	/// @Return 颜色
	//********************************************************************
	int GetColor();

	//********************************************************************
	/// 设置线型
	/// @Author	Gan Shu Long
	/// @Date	2026-01-06
	/// @Input
	/// @Param	eLineStyle  线型
	/// @Return		
	//********************************************************************
	void SetLineStyle(ELineStyle eLineStyle);

	//********************************************************************
	/// 获得线型
	/// @Author	Gan Shu Long
	/// @Date	2026-01-06
	/// @Return	线型	
	//********************************************************************
	ELineStyle GetLineStyle();

	//********************************************************************
	/// 设置线宽
	/// @Author	Gan Shu Long
	/// @Date	2026-01-06
	/// @Input
	/// @Param	eLineWidth  线宽
	/// @Return	
	//********************************************************************
	void SetLineWidth(ELineWidth eLineWidth);

	//********************************************************************
	/// 获得线宽
	/// @Author	Gan Shu Long
	/// @Date	2026-01-06
	/// @Return	线宽	
	//********************************************************************
	ELineWidth GetLineWidth();

	//********************************************************************
	/// 设置是否显示标签
	/// @Author	Gan Shu Long
	/// @Date	2026-01-06
	/// @Input
	/// @Param	bUserTextVisible  是否显示标签
	/// @Return	
	//********************************************************************
	void SetUserTextVisible(bool bUserTextVisible);

	//********************************************************************
	/// 获得是否显示标签
	/// @Author	Gan Shu Long
	/// @Date	2026-01-06
	/// @Return	是否显示标签
	//********************************************************************
	bool GetUserTextVisible();

private:
	CGfxContourAttri*			m_pCGfxContourAttri;	
};
