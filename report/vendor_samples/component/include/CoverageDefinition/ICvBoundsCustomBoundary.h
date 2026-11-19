#pragma once
#include "../IAtkObj.h"
#include "ICvBounds.h"
class CCoverageDefinition;
class ICvAreaTargetsCollection;
class CObjManage;

///
/// 自定义边界
///
class IATKOBJECT_API ICvBoundsCustomBoundary :public ICvBounds
{
public:
    ICvBoundsCustomBoundary(const ICvBoundsCustomBoundary&) = delete;
    ICvBoundsCustomBoundary& operator=(const ICvBoundsCustomBoundary&) = delete;

	ICvBoundsCustomBoundary(CCoverageDefinition* pCCoverageDefinition, CObjManage* IAsGObjManage);
	~ICvBoundsCustomBoundary();

	//********************************************************************
	/// 获得区域目标列表
	/// @Author	Gan Shu Long
	/// @Date	2026-01-14
	/// @Return	区域目标列表
	//********************************************************************
	ICvAreaTargetsCollection* GetAreaTargets() { return m_AreaTargets; }

private:
	ICvAreaTargetsCollection*				m_AreaTargets;						//区域目标列表

};
