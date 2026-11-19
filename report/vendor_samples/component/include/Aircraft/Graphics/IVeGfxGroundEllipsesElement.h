#pragma once
#include "../../IAtkObj.h"
#include "../../ICEnum.h"
struct CVehEllipseSet;

///
/// 二维地面椭圆设置元素
///
class IATKOBJECT_API IVeGfxGroundEllipsesElement
{
public:
    IVeGfxGroundEllipsesElement(const IVeGfxGroundEllipsesElement&) = delete;
    IVeGfxGroundEllipsesElement& operator=(const IVeGfxGroundEllipsesElement&) = delete;

	IVeGfxGroundEllipsesElement(CVehEllipseSet* pCVehEllipseSet);
	~IVeGfxGroundEllipsesElement();

	//********************************************************************
	/// 设置是否显示名称
	/// @Author	Gan Shu Long
	/// @Date	2026-01-09
	/// @Input
	/// @Param	bIsNameVisible  是否显示名称
	/// @Return
	//********************************************************************
	void SetIsNameVisible(bool bIsNameVisible);

	//********************************************************************
	/// 获取是否显示名称
	/// @Author	Gan Shu Long
	/// @Date	2026-01-09
	/// @Return 是否显示名称
	//********************************************************************
	bool GetIsNameVisible();

	//********************************************************************
	/// 设置是否显示中心点
	/// @Author	Gan Shu Long
	/// @Date	2026-01-09
	/// @Input
	/// @Param	bIsCenterVisible  是否显示中心点
	/// @Return
	//********************************************************************
	void SetIsCenterVisible(bool bIsCenterVisible);

	//********************************************************************
	/// 获取是否显示中心点
	/// @Author	Gan Shu Long
	/// @Date	2026-01-09
	/// @Return 是否显示中心点
	//********************************************************************
	bool GetIsCenterVisible();

	//********************************************************************
	/// 设置颜色
	/// @Author	Gan Shu Long
	/// @Date	2026-01-09
	/// @Input
	/// @Param	nColor  颜色
	/// @Return
	//********************************************************************
	void SetColor(int nColor);

	//********************************************************************
	/// 获取设置颜色
	/// @Author	Gan Shu Long
	/// @Date	2026-01-09
	/// @Return 颜色
	//********************************************************************
	int GetColor();

	//********************************************************************
	/// 设置线型
	/// @Author	Gan Shu Long
	/// @Date	2026-01-09
	/// @Input
	/// @Param	eLineStyle  线型
	/// @Return		
	//********************************************************************
	void SetLineStyle(ELineStyle eLineStyle);

	//********************************************************************
	/// 获得线型
	/// @Author	Gan Shu Long
	/// @Date	2026-01-09
	/// @Return	线型	
	//********************************************************************
	ELineStyle GetLineStyle();

	//********************************************************************
	/// 设置线宽
	/// @Author	Gan Shu Long
	/// @Date	2026-01-09
	/// @Input
	/// @Param	eLineWidth  线宽
	/// @Return	
	//********************************************************************
	void SetLineWidth(ELineWidth eLineWidth);

	//********************************************************************
	/// 获得线宽
	/// @Author	Gan Shu Long
	/// @Date	2026-01-09
	/// @Return	线宽	
	//********************************************************************
	ELineWidth GetLineWidth();

private:
	CVehEllipseSet*			m_pCVehEllipseSet;
};
