#pragma once
#include <string>
#include "../IAtkObj.h"
#include "../ICEnum.h"
struct CObjData;
class CObjManage;
class ICrdnAngleGroup;
class ICrdnAxesGroup;
class ICrdnPlaneGroup;
class ICrdnPointGroup;
class ICrdnSystemGroup;
class ICrdnVectorGroup;


///
/// VGT集合
///
class IATKOBJECT_API ICrdnProvider
{
public:
    ICrdnProvider(const ICrdnProvider&) = delete;
    ICrdnProvider& operator=(const ICrdnProvider&) = delete;

	ICrdnProvider(CObjData* pObjData, CObjManage* IAsGObjManage);
	~ICrdnProvider();

	//********************************************************************
	/// VGT角集合
	/// @Author	Gan Shu Long
	/// @Date	2026-06-02
	/// @Return	VGT角集合
	//********************************************************************
	ICrdnAngleGroup* GetAngles() { return m_Angles; }

	//********************************************************************
	/// VGT轴集合
	/// @Author	Gan Shu Long
	/// @Date	2026-06-02
	/// @Return	VGT轴集合
	//********************************************************************
	ICrdnAxesGroup* GetAxes() { return m_Axes; }

	//********************************************************************
	/// VGT平面集合
	/// @Author	Gan Shu Long
	/// @Date	2026-06-02
	/// @Return	VGT平面集合
	//********************************************************************
	ICrdnPlaneGroup* GetPlanes() { return m_Planes; }

	//********************************************************************
	/// VGT点集合
	/// @Author	Gan Shu Long
	/// @Date	2026-06-02
	/// @Return	VGT点集合
	//********************************************************************
	ICrdnPointGroup* GetPoints() { return m_Points; }

	//********************************************************************
	/// VGT系统集合
	/// @Author	Gan Shu Long
	/// @Date	2026-06-02
	/// @Return	VGT系统集合
	//********************************************************************
	ICrdnSystemGroup* GetSystems() { return m_Systems; }

	//********************************************************************
	/// VGT向量集合
	/// @Author	Gan Shu Long
	/// @Date	2026-06-02
	/// @Return	VGT向量集合
	//********************************************************************
	ICrdnVectorGroup* GetVectors() { return m_Vectors; }

private:
	ICrdnAngleGroup*			m_Angles;
	ICrdnAxesGroup*				m_Axes;
	ICrdnPlaneGroup*			m_Planes;
	ICrdnPointGroup*			m_Points;
	ICrdnSystemGroup*			m_Systems;
	ICrdnVectorGroup*			m_Vectors;
};

