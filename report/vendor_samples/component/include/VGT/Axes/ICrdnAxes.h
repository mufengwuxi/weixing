#pragma once
#include "../../IAtkObj.h"
#include "../ICrdn.h"


///
/// VGT元素基类 轴
///
class IATKOBJECT_API ICrdnAxes :public ICrdn
{
public:
    ICrdnAxes(const ICrdnAxes&) = delete;
    ICrdnAxes& operator=(const ICrdnAxes&) = delete;

	ICrdnAxes(CObjectBase* pCObjectBase, CObjManage* IAsGObjManage, CVGTBase* pCVGTBase);
	virtual ~ICrdnAxes();
	
	//********************************************************************
	/// 获得 VGT元素类型
	/// @Author	Gan Shu Long
	/// @Date	2026-06-03
	/// @Return	VGT元素类型
	//********************************************************************
	ECrdnAxesType GetCrdnType();

	static ECrdnAxesType GetVGTTypeFromStr(std::string strType);
};
