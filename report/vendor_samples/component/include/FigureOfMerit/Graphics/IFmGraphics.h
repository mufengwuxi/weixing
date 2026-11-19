#pragma once
#include "../../IAtkObj.h"
class CFigureOfMerit;
class IFmGfxAttributesAnimation;
class IFmGfxAttributes;

///
/// 二维视图
///
class IATKOBJECT_API IFmGraphics
{
public:
    IFmGraphics(const IFmGraphics&) = delete;
    IFmGraphics& operator=(const IFmGraphics&) = delete;

	IFmGraphics(CFigureOfMerit* pCFigureOfMerit);
	~IFmGraphics();

	//********************************************************************
	/// 设置对象是否在二维显示
	/// @Author	Gan Shu Long
	/// @Date	2026-05-11
	/// @Input
	/// @Param	bIsObjectGraphicsVisible  是否显示
	/// @Return	
	//********************************************************************
	void SetIsObjectGraphicsVisible(bool bIsObjectGraphicsVisible);

	//********************************************************************
	/// 获得对象是否在二维显示
	/// @Author	Gan Shu Long
	/// @Date	2026-05-11
	/// @Return	true-显示
	//********************************************************************
	bool GetIsObjectGraphicsVisible();

	//********************************************************************
	/// 获得 动态属性
	/// @Author	Gan Shu Long
	/// @Date	2026-05-11
	/// @Return	动态属性
	//********************************************************************
	IFmGfxAttributesAnimation* GetAnimation() { return m_Animation; }

	//********************************************************************
	/// 获得 静态属性
	/// @Author	Gan Shu Long
	/// @Date	2026-05-11
	/// @Return	静态属性
	//********************************************************************
	IFmGfxAttributes* GetStatic() { return m_Static; }
	
private:
	CFigureOfMerit*							m_pCFigureOfMerit;

	IFmGfxAttributesAnimation*		m_Animation;
	IFmGfxAttributes*						m_Static;
};

