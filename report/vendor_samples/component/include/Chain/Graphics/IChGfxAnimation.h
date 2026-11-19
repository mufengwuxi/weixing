#pragma once
#include "../../IAtkObj.h"
#include "../../ICEnum.h"
class CChain;

///
/// 动态可视化属性
///
class IATKOBJECT_API IChGfxAnimation
{
public:
    IChGfxAnimation(const IChGfxAnimation&) = delete;
    IChGfxAnimation& operator=(const IChGfxAnimation&) = delete;

	IChGfxAnimation(CChain* pCChain);
	~IChGfxAnimation();

	//********************************************************************
	/// 设置是否高亮显示
	/// @Author	Gan Shu Long
	/// @Date	2026-01-12
	/// @Input
	/// @Param	bIsHighlightVisible  是否高亮显示
	/// @Return
	//********************************************************************
	void SetIsHighlightVisible(bool bIsHighlightVisible);

	//********************************************************************
	/// 获取是否高亮显示
	/// @Author	Gan Shu Long
	/// @Date	2026-01-12
	/// @Return 是否高亮显示
	//********************************************************************
	bool GetIsHighlightVisible();

	//********************************************************************
	/// 设置是否显示线
	/// @Author	Gan Shu Long
	/// @Date	2026-01-12
	/// @Input
	/// @Param	bIsLineVisible  是否显示线
	/// @Return
	//********************************************************************
	void SetIsLineVisible(bool bIsLineVisible);

	//********************************************************************
	/// 获取是否显示线
	/// @Author	Gan Shu Long
	/// @Date	2026-01-12
	/// @Return 是否显示线
	//********************************************************************
	bool GetIsLineVisible();
	
	//********************************************************************
	/// 设置颜色
	/// @Author	Gan Shu Long
	/// @Date	2026-01-12
	/// @Input
	/// @Param	nColor  颜色
	/// @Return
	//********************************************************************
	void SetColor(int nColor);

	//********************************************************************
	/// 获取设置颜色
	/// @Author	Gan Shu Long
	/// @Date	2026-01-12
	/// @Return 颜色
	//********************************************************************
	int GetColor();

	//********************************************************************
	/// 设置线型
	/// @Author	Gan Shu Long
	/// @Date	2026-01-12
	/// @Input
	/// @Param	eLineStyle  线型
	/// @Return		
	//********************************************************************
	void SetLineStyle(ELineStyle eLineStyle);

	//********************************************************************
	/// 获得线型
	/// @Author	Gan Shu Long
	/// @Date	2026-01-12
	/// @Return	线型	
	//********************************************************************
	ELineStyle GetLineStyle();

	//********************************************************************
	/// 设置线宽
	/// @Author	Gan Shu Long
	/// @Date	2026-01-12
	/// @Input
	/// @Param	eLineWidth  线宽
	/// @Return	
	//********************************************************************
	void SetLineWidth(ELineWidth eLineWidth);

	//********************************************************************
	/// 获得线宽
	/// @Author	Gan Shu Long
	/// @Date	2026-01-12
	/// @Return	线宽	
	//********************************************************************
	ELineWidth GetLineWidth();

	//********************************************************************
	/// 设置是否显示方向
	/// @Author	Gan Shu Long
	/// @Date	2026-01-12
	/// @Input
	/// @Param	bIsDirectionVisible  是否显示方向
	/// @Return
	//********************************************************************
	void SetIsDirectionVisible(bool bIsDirectionVisible);

	//********************************************************************
	/// 获取是否显示方向
	/// @Author	Gan Shu Long
	/// @Date	2026-01-12
	/// @Return 是否显示方向
	//********************************************************************
	bool GetIsDirectionVisible();

private:
	CChain*			m_pCChain;
};
