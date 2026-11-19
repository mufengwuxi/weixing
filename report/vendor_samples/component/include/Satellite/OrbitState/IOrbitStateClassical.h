#pragma once
#include "../../ICEnum.h"
#include <vector>
#include <string>
#include "IOrbitState.h"
class CObjManage;
class CSatellite;
class CPreOrbitParam;
class IClassicalLocation;
class IClassicalOrientation;
class IClassicalSizeShape;

///
/// 预报器轨道根数设置
///
class IATKOBJECT_API IOrbitStateClassical : public IOrbitState
{
public:
    IOrbitStateClassical(const IOrbitStateClassical&) = delete;
    IOrbitStateClassical& operator=(const IOrbitStateClassical&) = delete;

	IOrbitStateClassical(CSatellite* pSate, CObjManage* IAsGObjManage, CPreOrbitParam* pPreOrbitParam);
	~IOrbitStateClassical();

	//********************************************************************
	/// 获得Location类型属性
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	Location类型属性
	//********************************************************************
	IClassicalLocation* GetLocation();

	//********************************************************************
	/// 设置Location类型
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	eLType Location类型
	/// @Return	
	//********************************************************************
	void SetLocationType(EClassicalLocation eLType);
	//********************************************************************
	/// 获得Location类型
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	Location类型
	//********************************************************************
	EClassicalLocation GetLocationType();

	//********************************************************************
	/// 获得方向属性
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	方向属性
	//********************************************************************
	IClassicalOrientation* GetOrientation();

	//********************************************************************
	/// 获得SizeShape类型属性
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	SizeShape类型属性指针
	//********************************************************************
	IClassicalSizeShape* GetSizeShape();

	//********************************************************************
	/// 设置SizeShape类型
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	eSSType SizeShape类型
	/// @Return	
	//********************************************************************
	void SetSizeShapeType(EClassicalSizeShape eSSType);
	//********************************************************************
	/// 获得SizeShape类型
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	SizeShape类型
	//********************************************************************
	EClassicalSizeShape GetSizeShapeType();

private:
	CSatellite*                  m_pSate;
	CPreOrbitParam*              m_pPreOrbitParam;

	IClassicalLocation*          m_Location;                         //Location类型属性
	IClassicalOrientation*       m_Orientation;                      //方向属性
	IClassicalSizeShape*         m_SizeShape;                        //SizeShape类型属性
};
