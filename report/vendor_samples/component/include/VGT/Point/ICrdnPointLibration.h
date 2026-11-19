#pragma once
#include "../../IAtkObj.h"
#include "../../ICEnum.h"
#include "ICrdnPoint.h"
#include <string>
#include <vector>


///
/// VGT元素 天平动点
///
class IATKOBJECT_API ICrdnPointLibration :public ICrdnPoint
{
public:
    ICrdnPointLibration(const ICrdnPointLibration&) = delete;
    ICrdnPointLibration& operator=(const ICrdnPointLibration&) = delete;

	ICrdnPointLibration(CObjectBase* pCObjectBase, CObjManage* IAsGObjManage, CVGTBase* pCVGTBase);
	virtual ~ICrdnPointLibration();

	IATKOBJECT_VGT_SETGET_CENTRALBODY_DECL(PrimaryBody);
	IATKOBJECT_VGT_SETGET_DECL(Point, ELibrationPoint);

	//********************************************************************
	/// 设置 次要天体列表
	/// @Author	Gan Shu Long
	/// @Date	2026-06-05
	/// @Input
	/// @Param	strCentralBodyPaths  次要天体列表
	/// @Return
	//********************************************************************
	err_t SetSecondaryBodies(std::vector<std::string> strCentralBodyPaths);

	//********************************************************************
	/// 获得 次要天体列表
	/// @Author	Gan Shu Long
	/// @Date	2026-06-05
	/// @Return	次要天体列表
	//********************************************************************
	std::vector<std::string> GetSecondaryBodies();
};
