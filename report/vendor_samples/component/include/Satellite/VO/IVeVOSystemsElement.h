#pragma once
#include <string>
#include "IVeVOSystemsElementBase.h"
struct CGfxOrbitSystem;

///
/// 相对轨迹属性
///
class IATKOBJECT_API IVeVOSystemsElement : public IVeVOSystemsElementBase
{
public:
    IVeVOSystemsElement(const IVeVOSystemsElement&) = delete;
    IVeVOSystemsElement& operator=(const IVeVOSystemsElement&) = delete;

	IVeVOSystemsElement(CGfxOrbitSystem* vvlh, std::string strName);
	~IVeVOSystemsElement();

	//********************************************************************
	/// 相对轨迹是否显示
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	bVisible  是否显示
	/// @Return		
	//********************************************************************
	void SetIsVisible(bool bVisible);
	//********************************************************************
	/// 获得相对轨迹是否显示
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	相对轨迹是否显示，true-显示
	//********************************************************************
	bool GetIsVisible();

	//********************************************************************
	/// 相对轨迹名称
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	相对轨迹名称
	//********************************************************************
	std::string GetName();

private:
	CGfxOrbitSystem*     m_vvlh;
	std::string          m_Name;
};

