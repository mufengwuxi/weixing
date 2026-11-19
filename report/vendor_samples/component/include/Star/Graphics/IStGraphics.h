#pragma once
#include "../../IAtkObj.h"
#include "../../ICEnum.h"
class CStar;
class IVODataDisplayCollection;

///
/// 二维
///
class IATKOBJECT_API IStGraphics
{
public:
    IStGraphics(const IStGraphics&) = delete;
    IStGraphics& operator=(const IStGraphics&) = delete;

	IStGraphics(CStar* pCStar);
	~IStGraphics();

	//********************************************************************
	/// 设置 是否显示
	/// @Author	Gan Shu Long
	/// @Date	2024-06-12
	/// @Input
	/// @Param	bIsObjectGraphicsVisible  是否显示
	/// @Return	
	//********************************************************************
	void SetIsObjectGraphicsVisible(bool bIsObjectGraphicsVisible);

	//********************************************************************
	/// 获得 是否显示
	/// @Author	Gan Shu Long
	/// @Date	2024-06-12
	/// @Return	是否显示
	//********************************************************************
	bool GetIsObjectGraphicsVisible();

	//********************************************************************
	/// 设置 是否显示标签
	/// @Author	Gan Shu Long
	/// @Date	2026-03-09
	/// @Input
	/// @Param	bLabelVisible  是否显示标签
	/// @Return	
	//********************************************************************
	void SetLabelVisible(bool bLabelVisible);

	//********************************************************************
	/// 获得 是否显示标签
	/// @Author	Gan Shu Long
	/// @Date	2026-03-09
	/// @Return	是否显示标签
	//********************************************************************
	bool GetLabelVisible();

	//********************************************************************
	/// 设置二维显示颜色
	/// @Author	Gan Shu Long
	/// @Date	2024-06-11
	/// @Input
	/// @Param	nColor  二维显示颜色，例绿色-65280
	/// @Return	
	//********************************************************************
	void SetColor(int nColor);

	//********************************************************************
	/// 获得二维显示颜色
	/// @Author	Gan Shu Long
	/// @Date	2024-06-11
	/// @Return	二维显示颜色
	//********************************************************************
	int GetColor();

	//********************************************************************
	/// 获得三维数据显示属性
	/// @Author	Gan Shu Long
	/// @Date	2026-03-09
	/// @Return	三维数据显示指针		
	//********************************************************************
	IVODataDisplayCollection* GetDataDisplay() { return m_DataDisplay; }

private:
	CStar*										m_pCStar;
	IVODataDisplayCollection*		m_DataDisplay;				//三维数据显示指针
};

