#pragma once
#include "../../IAtkObj.h"
#include "../ICrdn.h"


///
/// VGT元素基类 系统
///
class IATKOBJECT_API ICrdnSystem :public ICrdn
{
public:
    ICrdnSystem(const ICrdnSystem&) = delete;
    ICrdnSystem& operator=(const ICrdnSystem&) = delete;

	ICrdnSystem(CObjectBase* pCObjectBase, CObjManage* IAsGObjManage, CVGTBase* pCVGTBase);
	virtual ~ICrdnSystem();
	
	//********************************************************************
	/// 获得 VGT元素类型
	/// @Author	Gan Shu Long
	/// @Date	2026-06-03
	/// @Return	VGT元素类型
	//********************************************************************
	ECrdnSystemType GetCrdnType();

	static ECrdnSystemType GetVGTTypeFromStr(std::string strType);
};
