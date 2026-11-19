#pragma once
#include "../IAtkObject.h"
#include "../AccessConstraints/IAccessConstraintCollection.h"
#include "IPlCommonTasks.h"
#include "IPositionSourceData.h"
class CObjManage;
struct CObjData;
class CPlanet;
class IPlVO;


///
/// 行星
///
class IATKOBJECT_API IPlanet :public IAtkObject
{
public:
    IPlanet(const IPlanet&) = delete;
    IPlanet& operator=(const IPlanet&) = delete;

	IPlanet(CObjData* pObjData, CObjManage* IAsGObjManage, IAtkObjectRoot* Root);
	~IPlanet();

	//********************************************************************
	/// 获得 公共任务
	/// @Author	Gan Shu Long
	/// @Date	2026-03-09
	/// @Return	公共任务
	//********************************************************************
	IPlCommonTasks* GetCommonTasks() { return m_CommonTasks; }

	//********************************************************************
	/// 设置 行星位置资源类型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-09
	/// @Input
	/// @Param	type  行星位置资源类型
	/// @Return	
	//********************************************************************
	void SetPositionSource(EPlPositionSourceType type);

	//********************************************************************
	/// 获得行星位置资源类型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-09
	/// @Return	行星位置资源类型
	//********************************************************************
	EPlPositionSourceType GetPositionSource();

	//********************************************************************
	/// 获得行星位置资源
	/// @Author	Gan Shu Long
	/// @Date	2026-03-09
	/// @Return	行星位置资源
	//********************************************************************
	IPositionSourceData* GetPositionSourceData() { return m_PositionSourceData; }

	//********************************************************************
	/// 获得三维属性
	/// @Author	Gan Shu Long
	/// @Date	2026-03-09
	/// @Return	三维属性指针	
	//********************************************************************
	IPlVO* GetVO() { return m_VO; }

private:
	CPlanet* m_pCPlanet;

	IPlCommonTasks*			m_CommonTasks;
	IPositionSourceData*		m_PositionSourceData;	//位置资源数据
	IPlVO*								m_VO;											//三维
};
