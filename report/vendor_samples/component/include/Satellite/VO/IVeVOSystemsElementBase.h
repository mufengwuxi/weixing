#pragma once
#include "../../IAtkObj.h"
#include "../../IClassInfo.h"
struct CGfxOrbitSystem;

///
/// 相对轨迹基类
///
class IATKOBJECT_API IVeVOSystemsElementBase :public IClassInfo
{
public:
    IVeVOSystemsElementBase(const IVeVOSystemsElementBase&) = delete;
    IVeVOSystemsElementBase& operator=(const IVeVOSystemsElementBase&) = delete;

	IVeVOSystemsElementBase(CGfxOrbitSystem* vvlh);
	virtual ~IVeVOSystemsElementBase();

	//********************************************************************
	/// 设置相对轨迹颜色
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	nColor  颜色
	/// @Return		
	//********************************************************************
	void SetColor(int nColor);
	//********************************************************************
	/// 获得相对轨迹颜色
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	相对轨迹颜色	
	//********************************************************************
	int GetColor();

	//********************************************************************
	/// 设置相对轨迹是否使用默认颜色
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	bInherit  是否使用默认颜色
	/// @Return		
	//********************************************************************
	void SetInherit(bool bInherit);
	//********************************************************************
	/// 获得相对轨迹是否使用默认颜色
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	是否使用默认颜色，true-使用默认颜色，此时自定义颜色为false
	//********************************************************************
	bool GetInherit();


private:
	CGfxOrbitSystem*     m_vvlh;

};


