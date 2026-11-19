#pragma once
#include "../../IAtkObj.h"
#include "ICrdnPoint.h"
#include "../../ICEnum.h"


///
/// VGT元素 固定点
///
class IATKOBJECT_API ICrdnPointFixed :public ICrdnPoint
{
public:
    ICrdnPointFixed(const ICrdnPointFixed&) = delete;
    ICrdnPointFixed& operator=(const ICrdnPointFixed&) = delete;

	ICrdnPointFixed(CObjectBase* pCObjectBase, CObjManage* IAsGObjManage, CVGTBase* pCVGTBase);
	virtual ~ICrdnPointFixed();

	//********************************************************************
	/// 设置 坐标类型
	/// @Author	Gan Shu Long
	/// @Date	2026-06-08
	/// @Input
	/// @Param	eCrdnPointCoordType  坐标类型
	/// @Return
	//********************************************************************
	void SetCoordType(ECrdnPointCoordType eCrdnPointCoordType);
	
	//********************************************************************
	/// 获得 坐标类型
	/// @Author	Gan Shu Long
	/// @Date	2026-06-08
	/// @Return	坐标类型
	//********************************************************************
	ECrdnPointCoordType GetCoordType();

	//********************************************************************
	/// 设置 坐标类型
	/// @Author	Gan Shu Long
	/// @Date	2026-06-08
	/// @Input
	/// @Param	dX X(m)/Lat(deg)
	/// @Param	dY Y(m)/Lon(deg)
	/// @Param	dZ Z(m)/Rad(m)
	/// @Return
	//********************************************************************
	void SetCoordValue(double dX, double dY, double dZ);

	//********************************************************************
	/// 获得 坐标类型
	/// @Author	Gan Shu Long
	/// @Date	2026-06-08
	/// @Input
	/// @Param	dX X(m)/Lat(deg)
	/// @Param	dY Y(m)/Lon(deg)
	/// @Param	dZ Z(m)/Rad(m)
	/// @Return	
	//********************************************************************
	void GetCoordValue(double& dX, double& dY, double& dZ);

	IATKOBJECT_VGT_SETGET_VGT_DECL(ReferenceSystem);
};
