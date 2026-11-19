#pragma once
#include "../../IAtkObj.h"
#include "IGreatArcVO.h"
struct CObjData;
class IVeVORouteProximity;
class IVeVOElevContours;

///
/// 飞机三维属性
///
class IATKOBJECT_API IAcVO :public IGreatArcVO
{
public:
    IAcVO(const IAcVO&) = delete;
    IAcVO& operator=(const IAcVO&) = delete;

	IAcVO(CObjData* pObjData);
	~IAcVO();

	//********************************************************************
	/// 获得三维抵近属性
	/// @Author	Gan Shu Long
	/// @Date	2026-01-05
	/// @Return	三维抵近属性指针		
	//********************************************************************
	IVeVORouteProximity* GetProximity() { return m_Proximity; }

	//********************************************************************
	/// 获得三维等高线属性
	/// @Author	Gan Shu Long
	/// @Date	2026-01-07
	/// @Return	二维等高线指针		
	//********************************************************************
	IVeVOElevContours* GetElevContours() { return m_ElevContours; }

private:
	IVeVORouteProximity*		m_Proximity;			 //三维抵近属性
	IVeVOElevContours*			m_ElevContours;	//三维等高线
};
