#pragma once
#include "../../IAtkObj.h"
struct CObjData;
struct CGfxRange;
class ILevelAttributeCollection;

///
/// 二维范围
///
class IATKOBJECT_API IGfxRangeContours
{
public:
    IGfxRangeContours(const IGfxRangeContours&) = delete;
    IGfxRangeContours& operator=(const IGfxRangeContours&) = delete;

	IGfxRangeContours(CGfxRange* pCGfxRange);
	~IGfxRangeContours();

	//********************************************************************
	/// 设置是否显示
	/// @Author	Gan Shu Long
	/// @Date	2026-01-08
	/// @Input
	/// @Param	bIsVisible  是否显示
	/// @Return		
	//********************************************************************
	void SetIsVisible(bool bIsVisible);

	//********************************************************************
	/// 获得是否显示
	/// @Author	Gan Shu Long
	/// @Date	2026-01-08
	/// @Return	是否显示
	//********************************************************************
	bool GetIsVisible();

	//********************************************************************
	/// 设置是否显示填充
	/// @Author	Gan Shu Long
	/// @Date	2026-01-08
	/// @Input
	/// @Param	bIsVisible  是否显示填充
	/// @Return		
	//********************************************************************
	void SetIsFillVisible(bool bIsVisible);

	//********************************************************************
	/// 获得是否显示填充
	/// @Author	Gan Shu Long
	/// @Date	2026-01-08
	/// @Return	是否显示填充
	//********************************************************************
	bool GetIsFillVisible();

	//********************************************************************
	/// 设置填充透明度
	/// @Author	Gan Shu Long
	/// @Date	2026-01-08
	/// @Input
	/// @Param	bIsVisible  填充透明度
	/// @Return		
	//********************************************************************
	void SetFillTranslucency(double dFillTranslucency);

	//********************************************************************
	/// 获得填充透明度
	/// @Author	Gan Shu Long
	/// @Date	2026-01-08
	/// @Return	填充透明度
	//********************************************************************
	double GetFillTranslucency();

	//********************************************************************
	/// 获得分级列表
	/// @Author	Gan Shu Long
	/// @Date	2026-01-06
	/// @Return	分级列表
	//********************************************************************
	ILevelAttributeCollection* GetElevations() { return m_LevelAttributes; }

private:
	ILevelAttributeCollection*		m_LevelAttributes;		//分级

	CGfxRange*							m_pCGfxRange;			//C对象范围指针
};

