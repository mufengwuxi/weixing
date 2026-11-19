#pragma once
#include "../../IAtkObj.h"
struct GfxFOMGraphics;

///
/// 色阶
///
class IATKOBJECT_API IFmGfxRampColor
{
public:
    IFmGfxRampColor(const IFmGfxRampColor&) = delete;
    IFmGfxRampColor& operator=(const IFmGfxRampColor&) = delete;

	IFmGfxRampColor(GfxFOMGraphics* pGfxFOMGraphics);
	~IFmGfxRampColor();

	//********************************************************************
	/// 设置 起始颜色
	/// @Author	Gan Shu Long
	/// @Date	2026-05-12
	/// @Input
	/// @Param	nStartColor  起始颜色
	/// @Return
	//********************************************************************
	void SetStartColor(int nStartColor);

	//********************************************************************
	/// 获取 起始颜色
	/// @Author	Gan Shu Long
	/// @Date	2026-05-12
	/// @Return 起始颜色
	//********************************************************************
	int GetStartColor();

	//********************************************************************
	/// 设置 结束颜色
	/// @Author	Gan Shu Long
	/// @Date	2026-05-12
	/// @Input
	/// @Param	nEndColor  结束颜色
	/// @Return
	//********************************************************************
	void SetEndColor(int nEndColor);

	//********************************************************************
	/// 获取 结束颜色
	/// @Author	Gan Shu Long
	/// @Date	2026-05-12
	/// @Return 结束颜色
	//********************************************************************
	int GetEndColor();

private:
	GfxFOMGraphics*			m_pGfxFOMGraphics;
};
