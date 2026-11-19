#pragma once
#include "../../IAtkObj.h"
#include "../../ICEnum.h"
class CAreaTarget;

///
/// 二维
///
class IATKOBJECT_API IATGraphics
{
public:
    IATGraphics(const IATGraphics&) = delete;
    IATGraphics& operator=(const IATGraphics&) = delete;

	IATGraphics(CAreaTarget* pCAreaTarget);
	~IATGraphics();
	
	//********************************************************************
	/// 设置是否显示
	/// @Author	Gan Shu Long
	/// @Date	2026-01-09
	/// @Input
	/// @Param	bIsObjectGraphicsVisible  是否显示
	/// @Return
	//********************************************************************
	void SetIsObjectGraphicsVisible(bool bIsObjectGraphicsVisible);

	//********************************************************************
	/// 获取是否显示
	/// @Author	Gan Shu Long
	/// @Date	2026-01-15
	/// @Return 是否显示
	//********************************************************************
	bool GetIsObjectGraphicsVisible();

	//********************************************************************
	/// 设置颜色
	/// @Author	Gan Shu Long
	/// @Date	2026-01-15
	/// @Input
	/// @Param	nColor  颜色
	/// @Return
	//********************************************************************
	void SetColor(int nColor);

	//********************************************************************
	/// 获取设置颜色
	/// @Author	Gan Shu Long
	/// @Date	2026-01-15
	/// @Return 颜色
	//********************************************************************
	int GetColor();

	//********************************************************************
	/// 设置线型
	/// @Author	Gan Shu Long
	/// @Date	2026-01-15
	/// @Input
	/// @Param	eBoundaryStyle  线型
	/// @Return		
	//********************************************************************
	void SetBoundaryStyle(ELineStyle eBoundaryStyle);

	//********************************************************************
	/// 获得线型
	/// @Author	Gan Shu Long
	/// @Date	2026-01-15
	/// @Return	线型	
	//********************************************************************
	ELineStyle GetBoundaryStyle();

	//********************************************************************
	/// 设置线宽
	/// @Author	Gan Shu Long
	/// @Date	2026-01-15
	/// @Input
	/// @Param	eBoundaryWidth  线宽
	/// @Return	
	//********************************************************************
	void SetBoundaryWidth(ELineWidth eLineWidth);

	//********************************************************************
	/// 获得线宽
	/// @Author	Gan Shu Long
	/// @Date	2026-01-15
	/// @Return	线宽	
	//********************************************************************
	ELineWidth GetBoundaryWidth();

	//********************************************************************
	/// 设置是否显示标签
	/// @Author	Gan Shu Long
	/// @Date	2026-01-15
	/// @Input
	/// @Param	bLabelVisible  是否显示标签
	/// @Return
	//********************************************************************
	void SetLabelVisible(bool bLabelVisible);

	//********************************************************************
	/// 获取是否显示标签
	/// @Author	Gan Shu Long
	/// @Date	2026-01-15
	/// @Return 是否显示标签
	//********************************************************************
	bool GetLabelVisible();

	//********************************************************************
	/// 设置是否显示中心点
	/// @Author	Gan Shu Long
	/// @Date	2026-01-15
	/// @Input
	/// @Param	bCentroidVisible  是否显示中心点
	/// @Return
	//********************************************************************
	void SetCentroidVisible(bool bCentroidVisible);

	//********************************************************************
	/// 获取是否显示中心点
	/// @Author	Gan Shu Long
	/// @Date	2026-01-15
	/// @Return 是否显示中心点
	//********************************************************************
	bool GetCentroidVisible();

	//********************************************************************
	/// 设置是否显示边界点
	/// @Author	Gan Shu Long
	/// @Date	2026-01-15
	/// @Input
	/// @Param	bBoundaryPtsVisible  是否显示边界点
	/// @Return
	//********************************************************************
	void SetBoundaryPtsVisible(bool bBoundaryPtsVisible);

	//********************************************************************
	/// 获取是否显示边界点
	/// @Author	Gan Shu Long
	/// @Date	2026-01-15
	/// @Return 是否显示边界点
	//********************************************************************
	bool GetBoundaryPtsVisible();

	//********************************************************************
	/// 设置是否填充
	/// @Author	Gan Shu Long
	/// @Date	2026-01-15
	/// @Input
	/// @Param	bBoundaryPtsFill  是否填充
	/// @Return
	//********************************************************************
	void SetBoundaryFill(bool bBoundaryPtsFill);

	//********************************************************************
	/// 获取是否填充
	/// @Author	Gan Shu Long
	/// @Date	2026-01-15
	/// @Return 是否填充
	//********************************************************************
	bool GetBoundaryFill();
	
	//********************************************************************
	/// 设置透明度
	/// @Author	Gan Shu Long
	/// @Date	2026-01-15
	/// @Input
	/// @Param	dBoundaryFillPercentTranslucency  透明度
	/// @Return
	//********************************************************************
	void SetBoundaryFillPercentTranslucency(double dBoundaryFillPercentTranslucency);

	//********************************************************************
	/// 获取透明度
	/// @Author	Gan Shu Long
	/// @Date	2026-01-15
	/// @Return 透明度
	//********************************************************************
	double GetBoundaryFillPercentTranslucency();

private:
	CAreaTarget*				m_pCAreaTarget;
}; 
