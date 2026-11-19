#pragma once
#include "../../IAtkObj.h"
#include "../../Aircraft/VO/IGreatArcVO.h"
struct CObjData;
class IVeVORouteProximity;

///
/// 舰船三维属性
///
class IATKOBJECT_API IShVO :public IGreatArcVO
{
public:
    IShVO(const IShVO&) = delete;
    IShVO& operator=(const IShVO&) = delete;

	IShVO(CObjData* pObjData);
	~IShVO();

	//********************************************************************
	/// 获得三维抵近属性
	/// @Author	Gan Shu Long
	/// @Date	2026-01-05
	/// @Return	三维抵近属性指针		
	//********************************************************************
	IVeVORouteProximity* GetProximity() { return m_Proximity; }

private:
	IVeVORouteProximity*		m_Proximity;			 //三维抵近属性
};
