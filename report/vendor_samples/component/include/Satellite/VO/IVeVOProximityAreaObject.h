#pragma once
#include <string>
#include "../../IAtkObj.h"
#include "../../IClassInfo.h"
#include "../../ICEnum.h"
struct CObjData;
struct CGfxControlBox;

///
/// 三维抵近
///
class IATKOBJECT_API IVeVOProximityAreaObject :public IClassInfo
{
public:
    IVeVOProximityAreaObject(const IVeVOProximityAreaObject&) = delete;
    IVeVOProximityAreaObject& operator=(const IVeVOProximityAreaObject&) = delete;

	IVeVOProximityAreaObject(CGfxControlBox* pCGfxControlBox);
	virtual ~IVeVOProximityAreaObject();

	//********************************************************************
	/// 设置三维抵近颜色
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	nColor  颜色
	/// @Return		
	//********************************************************************
	void SetColor(int nColor);
	//********************************************************************
	/// 获得三维抵近颜色
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	颜色
	//********************************************************************
	int GetColor();

	//********************************************************************
	/// 设置三维抵近是否显示文字
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	bIsTextVisible  是否显示文字
	/// @Return	
	//********************************************************************
	void SetIsTextVisible(bool bIsTextVisible);
	//********************************************************************
	/// 获得三维抵近是否显示文字
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	是否显示文字	
	//********************************************************************
	bool GetIsTextVisible();

	//********************************************************************
	/// 设置三维抵近是否显示
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	bIsVisible  是否显示
	/// @Return		
	//********************************************************************
	void SetIsVisible(bool bIsVisible);
	//********************************************************************
	/// 获得三维抵近是否显示
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	是否显示	
	//********************************************************************
	bool GetIsVisible();

	//********************************************************************
	/// 设置三维抵近线型
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	eLineStyle  线型
	/// @Return		
	//********************************************************************
	void SetLineStyle(ELineStyle eLineStyle);
	//********************************************************************
	/// 获得三维抵近线型
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	线型	
	//********************************************************************
	ELineStyle GetLineStyle();

	//********************************************************************
	/// 设置三维抵近线宽
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	eLineWidth  线宽
	/// @Return	
	//********************************************************************
	void SetLineWidth(ELineWidth eLineWidth);
	//********************************************************************
	/// 获得三维抵近线宽
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	线宽	
	//********************************************************************
	ELineWidth GetLineWidth();

	//********************************************************************
	/// 设置三维抵近文字
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	strText  文字
	/// @Return	
	//********************************************************************
	void SetText(std::string strText);
	//********************************************************************
	/// 获得三维抵近文字
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	文字	
	//********************************************************************
	std::string GetText();

private:
	CGfxControlBox*              m_pCGfxControlBox;			//C对象控制盒指针
};

