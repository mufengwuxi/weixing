#pragma once
#include "../../IAtkObj.h"
#include "../ICrdn.h"


///
/// VGT元素基类 角
///
class IATKOBJECT_API ICrdnAngle :public ICrdn
{
public:
    ICrdnAngle(const ICrdnAngle&) = delete;
    ICrdnAngle& operator=(const ICrdnAngle&) = delete;

	ICrdnAngle(CObjectBase* pCObjectBase, CObjManage* IAsGObjManage, CVGTBase* pCVGTBase);
	virtual ~ICrdnAngle();
	
	//********************************************************************
	/// 获得 VGT元素类型
	/// @Author	Gan Shu Long
	/// @Date	2026-06-03
	/// @Return	VGT元素类型
	//********************************************************************
	ECrdnAngleType GetCrdnType();

	static ECrdnAngleType GetVGTTypeFromStr(std::string strType);
};
