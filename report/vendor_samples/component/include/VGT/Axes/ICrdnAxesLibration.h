#pragma once
#include "../../IAtkObj.h"
#include "../../ICEnum.h"
#include "ICrdnAxes.h"
#include <string>
#include <vector>


///
/// VGT元素 天体平动点坐标轴
///
class IATKOBJECT_API ICrdnAxesLibration :public ICrdnAxes
{
public:
    ICrdnAxesLibration(const ICrdnAxesLibration&) = delete;
    ICrdnAxesLibration& operator=(const ICrdnAxesLibration&) = delete;

	ICrdnAxesLibration(CObjectBase* pCObjectBase, CObjManage* IAsGObjManage, CVGTBase* pCVGTBase);
	virtual ~ICrdnAxesLibration();

	IATKOBJECT_VGT_SETGET_CENTRALBODY_DECL(CelestialBody);
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
