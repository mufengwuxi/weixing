#pragma once
#include "../../IAtkObj.h"
class CFacility;
class IGfxRangeContours;

///
/// 二维
///
class IATKOBJECT_API IFaGraphics
{
public:
    IFaGraphics(const IFaGraphics&) = delete;
    IFaGraphics& operator=(const IFaGraphics&) = delete;

	IFaGraphics(CFacility* pCFacilit);
	~IFaGraphics();

	//********************************************************************
	/// 设置对象是否在二维显示
	/// @Author	Gan Shu Long
	/// @Date	2024-06-12
	/// @Input
	/// @Param	bIsObjectGraphicsVisible  是否显示
	/// @Return	
	//********************************************************************
	void SetIsObjectGraphicsVisible(bool bIsObjectGraphicsVisible);

	//********************************************************************
	/// 获得对象是否在二维显示
	/// @Author	Gan Shu Long
	/// @Date	2024-06-12
	/// @Return	true-显示
	//********************************************************************
	bool GetIsObjectGraphicsVisible();

	//********************************************************************
	/// 设置是否显示标签
	/// @Author	Gan Shu Long
	/// @Date	2026-01-20
	/// @Input
	/// @Param	bLabelVisible  是否显示标签
	/// @Return	
	//********************************************************************
	void SetLabelVisible(bool bLabelVisible);

	//********************************************************************
	/// 获得是否显示标签
	/// @Author	Gan Shu Long
	/// @Date	2026-01-20
	/// @Return	是否显示标签
	//********************************************************************
	bool GetLabelVisible();

	//********************************************************************
	/// 设置二维显示颜色
	/// @Author	Gan Shu Long
	/// @Date	2024-05-21
	/// @Input
	/// @Param	nColor  二维显示颜色，例绿色-65280
	/// @Return	
	//********************************************************************
	void SetColor(int nColor);

	//********************************************************************
	/// 获得二维显示颜色
	/// @Author	Gan Shu Long
	/// @Date	2024-05-21
	/// @Return	二维显示颜色
	//********************************************************************
	int GetColor();

	//********************************************************************
	/// 设置二维显示标识类型
	/// @Author	Gan Shu Long
	/// @Date	2024-05-21
	/// @Input
	/// @Param	strMakStyle  标识类型
	/// @Return	
	//********************************************************************
	//void SetMarkerStyle(std::string strMakStyle);

	//********************************************************************
	/// 获得二维显示标识类型
	/// @Author	Gan Shu Long
	/// @Date	2024-05-21
	/// @Return 标识类型
	//********************************************************************
	//std::string GetMarkerStyle() { return m_MarkerStyle; }

	//********************************************************************
	/// 获得二维范围
	/// @Author	Gan Shu Long
	/// @Date	2026-01-20
	/// @Return	二维范围
	//********************************************************************
	IGfxRangeContours* GetContours() { return m_Contours; }
	
private:
	CFacility*							m_pCFacility;
	IGfxRangeContours*		m_Contours;
};

