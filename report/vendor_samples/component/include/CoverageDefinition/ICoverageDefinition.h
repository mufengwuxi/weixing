#pragma once
#include "../IAtkObj.h"
#include "../IAtkObject.h"
#include "ICvGrid.h"
class CObjManage;
struct CObjData;
class ICvGraphics;

///
/// 覆盖定义
///
class IATKOBJECT_API ICoverageDefinition :public IAtkObject
{
public:
    ICoverageDefinition(const ICoverageDefinition&) = delete;
    ICoverageDefinition& operator=(const ICoverageDefinition&) = delete;

	ICoverageDefinition(CObjData* pObjData, CObjManage* IAsGObjManage, IAtkObjectRoot* Root);
	~ICoverageDefinition();

	//********************************************************************
	/// 获得覆盖网格
	/// @Author	Gan Shu Long
	/// @Date	2024-06-06
	/// @Return	覆盖网格
	//********************************************************************
	ICvGrid* GetGrid() { return m_Grid; }

	//********************************************************************
	/// 获得二维属性
	/// @Author	Gan Shu Long
	/// @Date	2024-06-06
	/// @Return	二维属性指针	
	//********************************************************************
	ICvGraphics* GetGraphics() { return m_Graphics; }

private:
	ICvGrid*			m_Grid;			//覆盖网格的定义。
	ICvGraphics*		m_Graphics;		//二维
};
