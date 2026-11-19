#pragma once
#include "../IAtkObject.h"
#include "../ICEnum.h"
#include "../AccessConstraints/IAccessConstraintCollection.h"
class CObjManage;
struct CObjData;
class CFacility;
class IPosition;
class IFaGraphics;
class IFaVO;

///
/// 地面站
///
class IATKOBJECT_API IFacility :public IAtkObject
{
public:
    IFacility(const IFacility&) = delete;
    IFacility& operator=(const IFacility&) = delete;

	IFacility(CObjData* pObjData, CObjManage* IAsGObjManage, IAtkObjectRoot* Root);
	~IFacility();

	//********************************************************************
	/// 获得位置属性
	/// @Author	Gan Shu Long
	/// @Date	2024-05-21
	/// @Return	位置指针	
	//********************************************************************
	IPosition* GetPosition() { return m_Position; }

	//********************************************************************
	/// 设置 是否使用地形数据
	/// @Author	Gan Shu Long
	/// @Date	2026-10-21
	/// @Input
	/// @Param	bUseTerrain  是否使用地形数据
	/// @Return	
	//********************************************************************
	void SetUseTerrain(bool bUseTerrain);

	//********************************************************************
	/// 获得 是否使用地形数据
	/// @Author	Gan Shu Long
	/// @Date	2026-10-21
	/// @Return	是否使用地形数据
	//********************************************************************
	bool GetUseTerrain();

	//********************************************************************
	/// 设置 高度参考类型
	/// @Author	Gan Shu Long
	/// @Date	2026-10-21
	/// @Input
	/// @Param	eAltRefType  高度参考类型
	/// @Return	
	//********************************************************************
	void SetAltRef(EAltRefType eAltRefType);

	//********************************************************************
	/// 获得 高度参考类型
	/// @Author	Gan Shu Long
	/// @Date	2026-10-21
	/// @Return	高度参考类型
	//********************************************************************
	EAltRefType GetAltRef();

	//********************************************************************
	/// 设置 离地高度
	/// @Author	Gan Shu Long
	/// @Date	2026-10-21
	/// @Input
	/// @Param	dHeightAboveGround  离地高度
	/// @Return	
	//********************************************************************
	void SetHeightAboveGround(double dHeightAboveGround);

	//********************************************************************
	/// 获得 离地高度
	/// @Author	Gan Shu Long
	/// @Date	2026-10-21
	/// @Return	离地高度
	//********************************************************************
	double GetHeightAboveGround();

	//********************************************************************
	/// 获得二维属性
	/// @Author	Gan Shu Long
	/// @Date	2024-05-21
	/// @Return	二维属性指针	
	//********************************************************************
	IFaGraphics* GetGraphics() { return m_Graphics; }

	//********************************************************************
	/// 获得三维属性
	/// @Author	Gan Shu Long
	/// @Date	2024-05-21
	/// @Return	三维属性指针	
	//********************************************************************
	IFaVO* GetVO() { return m_VO; }

private:
	CFacility* m_pCFacility;

	IPosition*						m_Position;			//位置
	IFaGraphics*					m_Graphics;			//二维
	IFaVO*							m_VO;				//三维
};
