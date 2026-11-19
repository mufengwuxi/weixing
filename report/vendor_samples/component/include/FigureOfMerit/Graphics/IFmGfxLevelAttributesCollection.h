#pragma once
#include "../../IAtkObj.h"
#include <vector>
struct GfxFOMGraphics;
class IFmGfxLevelAttributesElement;

///
/// 分级属性列表
///
class IATKOBJECT_API IFmGfxLevelAttributesCollection
{
public:
    IFmGfxLevelAttributesCollection(const IFmGfxLevelAttributesCollection&) = delete;
    IFmGfxLevelAttributesCollection& operator=(const IFmGfxLevelAttributesCollection&) = delete;

	IFmGfxLevelAttributesCollection(GfxFOMGraphics* pGfxFOMGraphics);
	~IFmGfxLevelAttributesCollection();

	//********************************************************************
	/// 添加分级deg
	/// @Author	Gan Shu Long
	/// @Date	2026-05-12
	/// @Input
	/// @Param	dLevel  分级deg
	/// @Return		
	//********************************************************************
	IFmGfxLevelAttributesElement* AddLevel(double dLevel);

	//********************************************************************
	/// 根据步长添加分级deg
	/// @Author	Gan Shu Long
	/// @Date	2026-05-12
	/// @Input
	/// @Param	dStartLevel 开始
	/// @Param	dStopLevel 停止
	/// @Param	dSetpLevel 步长
	/// @Return		
	//********************************************************************
	void AddLevelRange(double dStartLevel, double dStopLevel, double dSetpLevel);

	//********************************************************************
	/// 移除所有分级
	/// @Author	Gan Shu Long
	/// @Date	2026-05-12
	/// @Return	
	//********************************************************************
	void RemoveAll();

	//********************************************************************
	/// 移除 指定分级
	/// @Author	Gan Shu Long
	/// @Date	2026-05-12
	/// @Input
	/// @Param	dLevel  指定分级
	/// @Return	
	//********************************************************************
	void Remove(double dLevel);

	//********************************************************************
	/// 获取分级个数
	/// @Author	Gan Shu Long
	/// @Date	2026-05-12
	/// @Return	分级个数
	//********************************************************************
	int GetCount();

	//********************************************************************
	///  获取指定分级
	/// @Author	Gan Shu Long
	/// @Date	2026-05-12
	/// @Input
	/// @Param	dLevel  指定分级
	/// @Return	指定分级
	//********************************************************************
	IFmGfxLevelAttributesElement* Item(double dLevel);

private:
	//获取分级索引
	int GetLevelIndex(double dLevel);

	GfxFOMGraphics*														m_pGfxFOMGraphics;

	std::vector<IFmGfxLevelAttributesElement*>			m_VectorILevelAttribute;
	std::vector<int>															m_VectorColor;
	int																					m_ColorIndex;
};
