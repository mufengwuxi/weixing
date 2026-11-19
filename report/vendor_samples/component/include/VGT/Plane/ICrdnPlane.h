#pragma once
#include "../../IAtkObj.h"
#include "../ICrdn.h"


///
/// VGT元素基类 平面
///
class IATKOBJECT_API ICrdnPlane :public ICrdn
{
public:
    ICrdnPlane(const ICrdnPlane&) = delete;
    ICrdnPlane& operator=(const ICrdnPlane&) = delete;

	ICrdnPlane(CObjectBase* pCObjectBase, CObjManage* IAsGObjManage, CVGTBase* pCVGTBase);
	virtual ~ICrdnPlane();
	
	//********************************************************************
	/// 获得 VGT元素类型
	/// @Author	Gan Shu Long
	/// @Date	2026-06-03
	/// @Return	VGT元素类型
	//********************************************************************
	ECrdnPlaneType GetCrdnType();

	static ECrdnPlaneType GetVGTTypeFromStr(std::string strType);
};
