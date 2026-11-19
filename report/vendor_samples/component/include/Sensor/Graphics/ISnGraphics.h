#pragma once
#include "../../IAtkObj.h"
//#include "ISnProjection.h"
#include "../../ICEnum.h"
class CSensor;
class ISnProjection;

///
/// 传感器二维
///
class IATKOBJECT_API ISnGraphics
{
public:
    ISnGraphics(const ISnGraphics&) = delete;
    ISnGraphics& operator=(const ISnGraphics&) = delete;

	ISnGraphics(CSensor* pCSensor);
	~ISnGraphics();

	//********************************************************************
	/// 设置是否显示
	/// @Author	Gan Shu Long
	/// @Date	2024-06-12
	/// @Input
	/// @Param	bool  是否显示
	/// @Return	
	//********************************************************************
	void SetIsObjectGraphicsVisible(bool bIsObjectGraphicsVisible);

	//********************************************************************
	/// 获得是否显示
	/// @Author	Gan Shu Long
	/// @Date	2024-06-12
	/// @Return	是否显示
	//********************************************************************
	bool GetIsObjectGraphicsVisible();

	//********************************************************************
	/// 设置是否填充
	/// @Author	Gan Shu Long
	/// @Date	2024-06-12
	/// @Input
	/// @Param	bool  是否填充
	/// @Return	
	//********************************************************************
	void SetFillVisible(bool bFillVisible);

	//********************************************************************
	/// 获得是否填充
	/// @Author	Gan Shu Long
	/// @Date	2024-06-12
	/// @Return	是否填充
	//********************************************************************
	bool GetFillVisible();

	//********************************************************************
	/// 设置二维显示颜色
	/// @Author	Gan Shu Long
	/// @Date	2024-06-12
	/// @Input
	/// @Param	nColor  二维显示颜色，例绿色-65280
	/// @Return	
	//********************************************************************
	void SetColor(int nColor);

	//********************************************************************
	/// 获得二维显示颜色
	/// @Author	Gan Shu Long
	/// @Date	2024-06-12
	/// @Return	二维显示颜色
	//********************************************************************
	int GetColor();

	//********************************************************************
	/// 设置线宽
	/// @Author	Gan Shu Long
	/// @Date	2024-06-12
	/// @Input
	/// @Param	eLineWidth  轨迹线宽
	/// @Return	
	//********************************************************************
	void SetLineWidth(ELineWidth eLineWidth);

	//********************************************************************
	/// 获得线宽
	/// @Author	Gan Shu Long
	/// @Date	2024-06-12
	/// @Return	轨迹线宽
	//********************************************************************
	ELineWidth GetLineWidth();
	
	//********************************************************************
	/// 获得 投影指针
	/// @Author	Gan Shu Long
	/// @Date	2026-03-24
	/// @Return	投影指针
	//********************************************************************
	ISnProjection* GetProjection() { return m_Projection; }

private:
	CSensor*			m_pCSensor;
	ISnProjection*		m_Projection;
};

