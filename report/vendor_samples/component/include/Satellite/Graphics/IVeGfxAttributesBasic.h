#pragma once
#include "IVeGfxAttributes.h"
struct CGfxVeh2D;
class IVeGfxLine;

///
/// 二维显示属性
///
class IATKOBJECT_API IVeGfxAttributesBasic : public IVeGfxAttributes
{
public:
    IVeGfxAttributesBasic(const IVeGfxAttributesBasic&) = delete;
    IVeGfxAttributesBasic& operator=(const IVeGfxAttributesBasic&) = delete;

	IVeGfxAttributesBasic(CGfxVeh2D* pCGfxVeh2D);
	virtual ~IVeGfxAttributesBasic();

	//********************************************************************
	/// 设置二维显示颜色
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	nColor  二维显示颜色，例绿色-65280
	/// @Return	
	//********************************************************************
	void SetColor(int nColor);
	//********************************************************************
	/// 获得二维显示颜色
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	二维显示颜色
	//********************************************************************
	int GetColor();

	//********************************************************************
	/// 设置对象是否在二维显示
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	bIsVisible  是否显示
	/// @Return	
	//********************************************************************
	void SetIsVisible(bool bIsVisible);
	//********************************************************************
	/// 获得对象是否在二维显示
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	true-显示
	//********************************************************************
	bool GetIsVisible();

	//********************************************************************
	/// 设置对象标签是否在二维显示
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	bLabelVisible  二维标签显示
	/// @Return	
	//********************************************************************
	void SetLabelVisible(bool bLabelVisible);
	//********************************************************************
	/// 获得对象标签是否在二维显示
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	true-二维标签显示
	//********************************************************************
	bool GetLabelVisible();

	//********************************************************************
	/// 设置二维显示标识类型
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	strMakStyle  标识类型
	/// @Return	
	//********************************************************************
	//void SetMarkerStyle(std::string strMakStyle);
	//********************************************************************
	/// 获得二维显示标识类型
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return 标识类型
	//********************************************************************
	//std::string GetMarkerStyle() { return m_MarkerStyle; }

	//********************************************************************
	/// 获得二维显示轨迹线属性
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return 二维显示轨迹线属性指针
	//********************************************************************
	IVeGfxLine* GetLine();

private:
	CGfxVeh2D*		m_pCGfxVeh2D;		//C对象二维显示指针

	IVeGfxLine*     m_Line;				//轨迹线属性
};
