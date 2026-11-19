#pragma once
#include "../../IAtkObj.h"
#include "../../ICEnum.h"
#include <string>
struct CGfxAttSphPlanetProj;

///
/// 三维姿态球行星投影
///
class IATKOBJECT_API IPlanetProjection
{
public:
    IPlanetProjection(const IPlanetProjection&) = delete;
    IPlanetProjection& operator=(const IPlanetProjection&) = delete;

	IPlanetProjection(CGfxAttSphPlanetProj* pCGfxAttSphPlanetProj);
	~IPlanetProjection();

	//********************************************************************
	/// 获得名称
	/// @Author	Gan Shu Long
	/// @Date	2026-01-21
	/// @Return	名称
	//********************************************************************
	std::string GetName();

	//********************************************************************
	/// 设置是否显示
	/// @Author	Gan Shu Long
	/// @Date	2026-01-21
	/// @Input
	/// @Param	bIsVisible  是否显示
	/// @Return		
	//********************************************************************
	void SetIsVisible(bool bIsVisible);

	//********************************************************************
	/// 获得是否显示
	/// @Author	Gan Shu Long
	/// @Date	2026-01-21
	/// @Return	是否显示
	//********************************************************************
	bool GetIsVisible();

	//********************************************************************
	/// 设置是否显示标签
	/// @Author	Gan Shu Long
	/// @Date	2026-01-21
	/// @Input
	/// @Param	bIsVisibleLable  是否显示标签
	/// @Return		
	//********************************************************************
	void SetIsVisibleLable(bool bIsVisibleLable);

	//********************************************************************
	/// 获得是否显示
	/// @Author	Gan Shu Long
	/// @Date	2026-01-21
	/// @Return	是否显示
	//********************************************************************
	bool GetIsVisibleLable();

	//********************************************************************
	/// 设置颜色
	/// @Author	Gan Shu Long
	/// @Date	2026-01-21
	/// @Input
	/// @Param	nColor  颜色
	/// @Return
	//********************************************************************
	void SetColor(int nColor);

	//********************************************************************
	/// 获取设置颜色
	/// @Author	Gan Shu Long
	/// @Date	2026-01-21
	/// @Return 颜色
	//********************************************************************
	int GetColor();

	//********************************************************************
	/// 设置点大小
	/// @Author	Gan Shu Long
	/// @Date	2026-01-21
	/// @Input
	/// @Param	nColor  点大小
	/// @Return
	//********************************************************************
	void SetPointSize(int nPointSize);

	//********************************************************************
	/// 获取点大小
	/// @Author	Gan Shu Long
	/// @Date	2026-01-21
	/// @Return 点大小
	//********************************************************************
	int GetPointSize();

private:
	CGfxAttSphPlanetProj*			m_pCGfxAttSphPlanetProj;
};
