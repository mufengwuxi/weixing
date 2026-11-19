#pragma once
#include "../IAtkObj.h"
#include "../IAtkObject.h"
#include "../AccessConstraints/IAccessConstraintCollection.h"
#include "ISnCommonTasks.h"
#include "ISnPattern.h"
#include "ILocationData.h"
#include "ISnPointing.h"
#include "../ICEnum.h"
#include "Graphics/ISnGraphics.h"
class CObjManage;
struct CObjData;
class CSensor;
class ISnGraphics;
class ISnVO;

///
/// 传感器
///
class IATKOBJECT_API ISensor :public IAtkObject
{
public:
    ISensor(const ISensor&) = delete;
    ISensor& operator=(const ISensor&) = delete;

	ISensor(CObjData* pObjData, CObjManage* IAsGObjManage, IAtkObjectRoot* Root);
	~ISensor();

	//********************************************************************
	/// 设置 模式类型
	/// @Author	Dong Min
	/// @Date	2026-03-20
	/// @Input
	/// @Param	eSnPattern  模式类型
	/// @Return	
	//********************************************************************
	void SetPatternType(ESnPattern eSnPattern);
	
	//********************************************************************
	/// 获得 模式类型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Return	模式类型
	//********************************************************************
	ESnPattern GetPatternType();
	
	//********************************************************************
	/// 获得 模式指针
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Return	模式指针
	//********************************************************************
	ISnPattern* GetPattern() { return m_Pattern; }

	//********************************************************************
	/// 设置 位置类型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Input
	/// @Param	eSnLocation  位置类型
	/// @Return	
	//********************************************************************
	void SetLocationType(ESnLocation eSnLocation);

	//********************************************************************
	/// 获得 位置类型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Return	位置类型
	//********************************************************************
	ESnLocation GetLocationType();
	
	//********************************************************************
	/// 获得 位置指针
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Return	位置指针
	//********************************************************************
	ILocationData* GetLocationData() { return m_LocationData; }

	//********************************************************************
	/// 设置 指向类型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Input
	/// @Param	eSnPointing  指向类型
	/// @Return	
	//********************************************************************
	void SetPointingType(ESnPointing eSnPointing);

	//********************************************************************
	/// 获得 指向类型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Return	指向类型
	//********************************************************************
	ESnPointing GetPointingType();

	//********************************************************************
	/// 获得 指向指针
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Return	指向指针
	//********************************************************************
	ISnPointing* GetPointing() { return m_Pointing; }

	//********************************************************************
	/// 获得 公共任务
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Return	公共任务
	//********************************************************************
	ISnCommonTasks*	GetCommonTasks() { return m_CommonTasks; }

	//********************************************************************
	/// 获得二维属性
	/// @Author	Gan Shu Long
	/// @Date	2024-06-11
	/// @Return	二维属性指针	
	//********************************************************************
	ISnGraphics* GetGraphics() { return m_Graphics; }

	//********************************************************************
	/// 获得三维属性
	/// @Author	Gan Shu Long
	/// @Date	2024-06-11
	/// @Return	三维属性指针	
	//********************************************************************
	ISnVO* GetVO() { return m_VO; }

private:
	CObjData*								m_pCObjData;
	CSensor*								m_pCSensor;

	ISnCommonTasks*					m_CommonTasks;
	ISnPattern*								m_Pattern;					//模式
	ILocationData*							m_LocationData;			//相对位置
	ISnPointing*								m_Pointing;					//指向
	ISnGraphics*							m_Graphics;				//二维
	ISnVO*									m_VO;							//三维
};
