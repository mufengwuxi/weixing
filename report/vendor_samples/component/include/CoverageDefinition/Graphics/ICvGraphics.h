#pragma once
#include "../../IAtkObj.h"
class CCoverageDefinition;

///
/// 二维
///
class IATKOBJECT_API ICvGraphics
{
public:
    ICvGraphics(const ICvGraphics&) = delete;
    ICvGraphics& operator=(const ICvGraphics&) = delete;

	ICvGraphics(CCoverageDefinition* pCCoverageDefinition);
	~ICvGraphics();

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
	/// 设置是否显示区域
	/// @Author	Gan Shu Long
	/// @Date	2024-06-11
	/// @Input
	/// @Param	bool  是否显示区域
	/// @Return	
	//********************************************************************
	void SetIsRegionVisible(bool bIsRegionVisible);

	//********************************************************************
	/// 获得是否显示区域
	/// @Author	Gan Shu Long
	/// @Date	2024-06-11
	/// @Return	是否显示区域
	//********************************************************************
	bool GetIsRegionVisible();

	//********************************************************************
	/// 设置是否显示点
	/// @Author	Gan Shu Long
	/// @Date	2024-06-11
	/// @Input
	/// @Param	bool  是否显示点
	/// @Return	
	//********************************************************************
	void SetIsPointsVisible(bool bIsPointsVisible);

	//********************************************************************
	/// 获得是否显示点
	/// @Author	Gan Shu Long
	/// @Date	2024-06-11
	/// @Return	是否显示点
	//********************************************************************
	bool GetIsPointsVisible();

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
	/// 设置点大小
	/// @Author	Gan Shu Long
	/// @Date	2024-06-11
	/// @Input
	/// @Param	double  点大小
	/// @Return	
	//********************************************************************
	void SetPointSize(double dPointSize);

	//********************************************************************
	/// 获得点大小
	/// @Author	Gan Shu Long
	/// @Date	2024-06-11
	/// @Return	点大小
	//********************************************************************
	double GetPointSize();

	//********************************************************************
	/// 设置是否填充
	/// @Author	Gan Shu Long
	/// @Date	2024-06-11
	/// @Input
	/// @Param	bool  是否填充
	/// @Return	
	//********************************************************************
	void SetFillPoints(bool bFillPoints);

	//********************************************************************
	/// 获得是否填充
	/// @Author	Gan Shu Long
	/// @Date	2024-06-11
	/// @Return	是否填充
	//********************************************************************
	bool GetFillPoints();

private:
	CCoverageDefinition*	m_pCCoverageDefinition;
};

