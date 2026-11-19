#pragma once
#include "../../IAtkObj.h"
#include "../../IClassInfo.h" 
struct GfxFOMGraphics;
class IFmGfxContours;

///
/// 二维视图 静态
///
class IATKOBJECT_API IFmGfxAttributes :public IClassInfo
{
public:
    IFmGfxAttributes(const IFmGfxAttributes&) = delete;
    IFmGfxAttributes& operator=(const IFmGfxAttributes&) = delete;

	IFmGfxAttributes(GfxFOMGraphics* pGfxFOMGraphics);
	~IFmGfxAttributes();

	//********************************************************************
	/// 设置 是否显示静态/动态图像
	/// @Author	Gan Shu Long
	/// @Date	2026-05-11
	/// @Input
	/// @Param	bIsVisible  是否显示静态/动态图像
	/// @Return	
	//********************************************************************
	void SetIsVisible(bool bIsVisible);

	//********************************************************************
	/// 获得 是否显示静态/动态图像
	/// @Author	Gan Shu Long
	/// @Date	2026-05-11
	/// @Return 是否显示静态/动态图像
	//********************************************************************
	bool GetIsVisible();

	//********************************************************************
	/// 设置 透明度
	/// @Author	Gan Shu Long
	/// @Date	2026-05-12
	/// @Input
	/// @Param	dFillTranslucency  透明度
	/// @Return	
	//********************************************************************
	void SetFillTranslucency(double dFillTranslucency);

	//********************************************************************
	/// 获得  透明度
	/// @Author	Gan Shu Long
	/// @Date	2026-05-12
	/// @Return	 透明度
	//********************************************************************
	double GetFillTranslucency();

	//********************************************************************
	/// 设置 颜色
	/// @Author	Gan Shu Long
	/// @Date	2026-05-12
	/// @Input
	/// @Param	nColor  颜色
	/// @Return
	//********************************************************************
	void SetColor(int nColor);

	//********************************************************************
	/// 获取 颜色
	/// @Author	Gan Shu Long
	/// @Date	2026-05-12
	/// @Return 颜色
	//********************************************************************
	int GetColor();

	//********************************************************************
	/// 获得 分级轮廓
	/// @Author	Gan Shu Long
	/// @Date	2026-05-11
	/// @Return	分级轮廓
	//********************************************************************
	IFmGfxContours* GetContours() { return m_Contours; }
	
private:
	GfxFOMGraphics*			m_pGfxFOMGraphics;

	IFmGfxContours*		m_Contours;
	
};

