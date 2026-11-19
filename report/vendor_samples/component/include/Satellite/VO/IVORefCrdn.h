#pragma once
#include <string>
#include "../../IAtkObj.h"
#include "../../ICEnum.h"
struct CObjData;
class CGfxVectorDef;
class CGfxPoint;
struct CGfxVectorAttr;

///
/// 三维向量属性
///
class IATKOBJECT_API IVORefCrdn
{
public:
    IVORefCrdn(const IVORefCrdn&) = delete;
    IVORefCrdn& operator=(const IVORefCrdn&) = delete;

	IVORefCrdn(CGfxVectorDef* pGfxVector);
	IVORefCrdn(CGfxPoint* pGfxpoint);
	~IVORefCrdn();

	//********************************************************************
	/// 设置三维向量属性颜色
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	nColor  颜色，例绿色-65280
	/// @Return		
	//********************************************************************
	void SetColor(int nColor);
	//********************************************************************
	/// 获得三维向量属性颜色
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	颜色属性值	
	//********************************************************************
	int GetColor();

	//********************************************************************
	/// 设置三维向量属性是否显示标签
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	bLabelVisible  是否显示标签
	/// @Return		
	//********************************************************************
	void SetLabelVisible(bool bLabelVisible);
	//********************************************************************
	/// 获得三维向量属性是否显示标签
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	是否显示标签	
	//********************************************************************
	bool GetLabelVisible();

	//********************************************************************
	/// 获得三维向量属性名称
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	三维向量属性名称，例"Satellite/Satellite1 J2000 Axes"	
	//********************************************************************
	std::string GetName();

	//********************************************************************
	/// 获得三维向量属性类型ID
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	类型ID
	//********************************************************************
	EGeometricElemType GetTypeID();

	//********************************************************************
	/// 设置三维向量属性是否显示
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	bVisible  是否显示
	/// @Return		
	//********************************************************************
	void SetVisible(bool bVisible);
	//********************************************************************
	/// 获得三维向量属性是否显示
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	是否显示	
	//********************************************************************
	bool GetVisible();

	//********************************************************************
	/// 设置三维向量属性粗细
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dThickness  粗细
	/// @Return		
	//********************************************************************
	void SetThickness(double dThickness);
	//********************************************************************
	/// 获得三维向量属性粗细
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	粗细	
	//********************************************************************
	double GetThickness();

private:
	CGfxVectorDef*			            m_pCGfxVector;
	CGfxPoint*                          m_pCGfxpoint;

};
