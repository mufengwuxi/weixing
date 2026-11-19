#pragma once
#include "../../IAtkObj.h"
struct GfxFOMGraphics;

///
/// 分级属性列表元素
///
class IATKOBJECT_API IFmGfxLevelAttributesElement
{
public:
    IFmGfxLevelAttributesElement(const IFmGfxLevelAttributesElement&) = delete;
    IFmGfxLevelAttributesElement& operator=(const IFmGfxLevelAttributesElement&) = delete;

	IFmGfxLevelAttributesElement(GfxFOMGraphics* pGfxFOMGraphics, double dLevel);
	~IFmGfxLevelAttributesElement();

	//********************************************************************
	/// 设置分级deg
	/// @Author	Gan Shu Long
	/// @Date	2026-05-12
	/// @Input
	/// @Param	dLevel  分级
	/// @Return
	//********************************************************************
	void SetLevel(double dLevel);

	//********************************************************************
	/// 获取分级deg
	/// @Author	Gan Shu Long
	/// @Date	2026-05-12
	/// @Return 分级deg
	//********************************************************************
	double GetLevel();

	//********************************************************************
	/// 设置颜色
	/// @Author	Gan Shu Long
	/// @Date	2026-05-12
	/// @Input
	/// @Param	nColor  颜色
	/// @Return
	//********************************************************************
	void SetColor(int nColor);

	//********************************************************************
	/// 获取设置颜色
	/// @Author	Gan Shu Long
	/// @Date	2026-05-12
	/// @Return 颜色
	//********************************************************************
	int GetColor();

private:
	GfxFOMGraphics*			m_pGfxFOMGraphics;

	double								m_dLevel;
};
