#pragma once
#include "../../IAtkObj.h"
#include "../../ICEnum.h"
struct GfxFOMGraphics;
class IFmGfxLevelAttributesCollection;
class IFmGfxRampColor;

///
/// 分级轮廓
///
class IATKOBJECT_API IFmGfxContours
{
public:
    IFmGfxContours(const IFmGfxContours&) = delete;
    IFmGfxContours& operator=(const IFmGfxContours&) = delete;

	IFmGfxContours(GfxFOMGraphics* pGfxFOMGraphics);
	~IFmGfxContours();

	//********************************************************************
	/// 设置 是否显示分级轮廓 Contours(否则显示有效区域Satisfaction )
	/// @Author	Gan Shu Long
	/// @Date	2026-05-12
	/// @Input
	/// @Param	bIsVisible  是否显示分级轮廓
	/// @Return	
	//********************************************************************
	void SetIsVisible(bool bIsVisible);

	//********************************************************************
	/// 获得 是否显示分级轮廓
	/// @Author	Gan Shu Long
	/// @Date	2026-05-12
	/// @Return 是否显示分级轮廓
	//********************************************************************
	bool GetIsVisible();

	//********************************************************************
	/// 获得 分级属性列表
	/// @Author	Gan Shu Long
	/// @Date	2026-05-12
	/// @Return	分级属性列表
	//********************************************************************
	IFmGfxLevelAttributesCollection* GetLevelAttributes() { return m_LevelAttributes; }

	//********************************************************************
	/// 设置 是否显示品质因子值超出最大级别的区域
	/// @Author	Gan Shu Long
	/// @Date	2026-05-13
	/// @Input
	/// @Param	bIsShowUpToMaxOnly  是否显示品质因子值超出最大级别的区域
	/// @Return	
	//********************************************************************
	void SetIsShowUpToMaxOnly(bool bIsShowUpToMaxOnly);

	//********************************************************************
	/// 获得 是否显示品质因子值超出最大级别的区域
	/// @Author	Gan Shu Long
	/// @Date	2026-05-13
	/// @Return 是否显示品质因子值超出最大级别的区域
	//********************************************************************
	bool GetIsShowUpToMaxOnly();
	
	//********************************************************************
	/// 设置 取色类型
	/// @Author	Gan Shu Long
	/// @Date	2026-05-12
	/// @Input
	/// @Param	eFmGfxColorMethod  取色类型
	/// @Return	
	//********************************************************************
	void SetColorMethod(EFmGfxColorMethod eFmGfxColorMethod);

	//********************************************************************
	/// 获得  取色类型
	/// @Author	Gan Shu Long
	/// @Date	2026-05-12
	/// @Return	 取色类型
	//********************************************************************
	EFmGfxColorMethod GetColorMethod();

	//********************************************************************
	/// 获得 色阶
	/// @Author	Gan Shu Long
	/// @Date	2026-05-12
	/// @Return	色阶
	//********************************************************************
	IFmGfxRampColor* GetRampColor() { return m_RampColor; }

	//********************************************************************
	/// 设置 轮廓线插值类型
	/// @Author	Gan Shu Long
	/// @Date	2026-05-13
	/// @Input
	/// @Param	eFmGfxContourType  轮廓线插值类型
	/// @Return	
	//********************************************************************
	void SetContourType(EFmGfxContourType eFmGfxContourType);

	//********************************************************************
	/// 获得  轮廓线插值类型
	/// @Author	Gan Shu Long
	/// @Date	2026-05-13
	/// @Return	 轮廓线插值类型
	//********************************************************************
	EFmGfxContourType GetContourType();

	//********************************************************************
	/// 设置 插值采样类型
	/// @Author	Gan Shu Long
	/// @Date	2026-05-13
	/// @Input
	/// @Param	eFmGfxSampleType  插值采样类型
	/// @Return	
	//********************************************************************
	void SetSampleType(EFmGfxSampleType eFmGfxSampleType);

	//********************************************************************
	/// 获得  插值采样类型
	/// @Author	Gan Shu Long
	/// @Date	2026-05-13
	/// @Return	 插值采样类型
	//********************************************************************
	EFmGfxSampleType GetSampleType();

	//********************************************************************
	/// 设置 每个网格插值点数
	/// @Author	Gan Shu Long
	/// @Date	2026-05-13
	/// @Input
	/// @Param	dSamplePointNum  每个网格插值点数
	/// @Return	
	//********************************************************************
	void SetSamplePointNum(double dSamplePointNum);

	//********************************************************************
	/// 获得  每个网格插值点数
	/// @Author	Gan Shu Long
	/// @Date	2026-05-13
	/// @Return	 每个网格插值点数
	//********************************************************************
	double GetSamplePointNum();

	//********************************************************************
	/// 设置 是否显示等高线
	/// @Author	Gan Shu Long
	/// @Date	2026-05-13
	/// @Input
	/// @Param	bIsShowContourLines  是否显示等高线
	/// @Return	
	//********************************************************************
	void SetIsShowContourLines(bool bIsShowContourLines);

	//********************************************************************
	/// 获得 是否显示等高线
	/// @Author	Gan Shu Long
	/// @Date	2026-05-13
	/// @Return 是否显示等高线
	//********************************************************************
	bool GetIsShowContourLines();

	//********************************************************************
	/// 设置 等高线线宽
	/// @Author	Gan Shu Long
	/// @Date	2026-05-13
	/// @Input
	/// @Param	nLineWidth  等高线线宽
	/// @Return	
	//********************************************************************
	void SetLineWidth(ELineWidth eLineWidth);

	//********************************************************************
	/// 获得 等高线线宽
	/// @Author	Gan Shu Long
	/// @Date	2026-05-13
	/// @Return 等高线线宽
	//********************************************************************
	ELineWidth GetLineWidth();

	//********************************************************************
	/// 设置 是否自定义颜色
	/// @Author	Gan Shu Long
	/// @Date	2026-05-13
	/// @Input
	/// @Param	bIsCustomColor  是否自定义颜色
	/// @Return	
	//********************************************************************
	void SetIsCustomColor(bool bIsCustomColor);

	//********************************************************************
	/// 获得 是否自定义颜色
	/// @Author	Gan Shu Long
	/// @Date	2026-05-13
	/// @Return 是否自定义颜色
	//********************************************************************
	bool GetIsCustomColor();

	//********************************************************************
	/// 设置 自定义颜色
	/// @Author	Gan Shu Long
	/// @Date	2026-05-13
	/// @Input
	/// @Param	nCustomColor  自定义颜色
	/// @Return	
	//********************************************************************
	void SetCustomColor(int nCustomColor);

	//********************************************************************
	/// 获得 自定义颜色
	/// @Author	Gan Shu Long
	/// @Date	2026-05-13
	/// @Return 自定义颜色
	//********************************************************************
	int GetCustomColor();

private:
	GfxFOMGraphics*							m_pGfxFOMGraphics;

	IFmGfxLevelAttributesCollection*	m_LevelAttributes;
	IFmGfxRampColor*						m_RampColor;
};

