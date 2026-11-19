#pragma once
#include "../../IAtkObj.h"
#include "ICrdnVector.h"


///
/// VGT元素 固定向量
///
class IATKOBJECT_API ICrdnVectorFixed :public ICrdnVector
{
public:
    ICrdnVectorFixed(const ICrdnVectorFixed&) = delete;
    ICrdnVectorFixed& operator=(const ICrdnVectorFixed&) = delete;

	ICrdnVectorFixed(CObjectBase* pCObjectBase, CObjManage* IAsGObjManage, CVGTBase* pCVGTBase);
	virtual ~ICrdnVectorFixed();

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

	IATKOBJECT_VGT_SETGET_VGT_DECL(ReferenceAxes);
};
