#pragma once
#include "../IAtkObject.h"
#include "../Aircraft/IGreatArcVehicle.h"
class CObjManage;
struct CObjData;
class ISuGraphics;
class ISuVO;

///
/// 潜艇
///
class IATKOBJECT_API ISubmarine :public IAtkObject
{
public:
    ISubmarine(const ISubmarine&) = delete;
    ISubmarine& operator=(const ISubmarine&) = delete;

	ISubmarine(CObjData* pObjData, CObjManage* IAsGObjManage, IAtkObjectRoot* Root);
	~ISubmarine();

	//********************************************************************
	/// 获得基类成员
	/// @Author	Gan Shu Long
	/// @Date	2026-01-17
	/// @Return	基类成员指针	
	//********************************************************************
	IGreatArcVehicle* GetGreatArcVehicle() { return m_GreatArcVehicle; }

	//********************************************************************
	/// 获得二维属性
	/// @Author	Gan Shu Long
	/// @Date	2024-06-11
	/// @Return	二维属性指针	
	//********************************************************************
	ISuGraphics* GetGraphics() { return m_Graphics; }

	//********************************************************************
	/// 获得三维属性
	/// @Author	Gan Shu Long
	/// @Date	2024-06-11
	/// @Return	三维属性指针	
	//********************************************************************
	ISuVO* GetVO() { return m_VO; }

private:
	IGreatArcVehicle*	m_GreatArcVehicle;	//本来是基类，java不支持多继承，就改成了类成员

	ISuGraphics*			m_Graphics;			//二维
	ISuVO*					m_VO;				//三维
};
