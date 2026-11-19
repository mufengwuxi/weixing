#pragma once
#include "../../IAtkObj.h"
#include "../../ICEnum.h"
struct CGfxVeh2D;

///
/// 二维轨迹线属性
///
class IATKOBJECT_API IVeGfxLine
{
public:
    IVeGfxLine(const IVeGfxLine&) = delete;
    IVeGfxLine& operator=(const IVeGfxLine&) = delete;

	IVeGfxLine(CGfxVeh2D* pCGfxVeh2D);
	~IVeGfxLine();

	//********************************************************************
	/// 设置轨迹线型
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	eLineStyle  轨迹线型
	/// @Return	
	//********************************************************************
	void SetStyle(ELineStyle eLineStyle);
	//********************************************************************
	/// 获得轨迹线型
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	轨迹线型
	//********************************************************************
	ELineStyle GetStyle();

	//********************************************************************
	/// 设置轨迹线宽
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	eLineWidth  轨迹线宽
	/// @Return	
	//********************************************************************
	void SetWidth(ELineWidth eLineWidth);
	//********************************************************************
	/// 获得轨迹线宽
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	轨迹线宽
	//********************************************************************
	ELineWidth GetWidth(); 

private:
	CGfxVeh2D*		m_pCGfxVeh2D;	//C对象二维显示指针
};
