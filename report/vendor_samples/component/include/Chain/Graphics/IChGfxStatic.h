#pragma once
#include "../../IAtkObj.h"
#include "../../ICEnum.h"
class CChain;

///
/// 静态可视化属性
///
class IATKOBJECT_API IChGfxStatic
{
public:
    IChGfxStatic(const IChGfxStatic&) = delete;
    IChGfxStatic& operator=(const IChGfxStatic&) = delete;

	IChGfxStatic(CChain* pCChain);
	~IChGfxStatic();

	//********************************************************************
	/// 设置是否显示
	/// @Author	Gan Shu Long
	/// @Date	2026-01-12
	/// @Input
	/// @Param	bIsVisible  是否显示
	/// @Return
	//********************************************************************
	void SetIsVisible(bool bIsVisible);

	//********************************************************************
	/// 获取是否显示
	/// @Author	Gan Shu Long
	/// @Date	2026-01-12
	/// @Return 是否显示
	//********************************************************************
	bool GetIsVisible();

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

private:
	CChain*			m_pCChain;
};
