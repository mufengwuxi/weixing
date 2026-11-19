#pragma once
#include "../IAtkObject.h"
#include "IObjectLinkCollection.h"
class CObjManage;
struct CObjData;
class CChain;
class IChGraphics;

///
/// 链路
///
class IATKOBJECT_API IChain :public IAtkObject
{
public:
    IChain(const IChain&) = delete;
    IChain& operator=(const IChain&) = delete;

	IChain(CObjData* pObjData, CObjManage* IAsGObjManage, IAtkObjectRoot* Root);
	~IChain();

	//********************************************************************
	/// 获得链路对象列表
	/// @Author	Gan Shu Long
	/// @Date	2026-01-12
	/// @Return	链路对象列表指针	
	//********************************************************************
	IObjectLinkCollection* GetObjects() { return m_Objects; }

	//********************************************************************
	/// 获得二维属性
	/// @Author	Gan Shu Long
	/// @Date	2024-06-06
	/// @Return	二维属性指针	
	//********************************************************************
	IChGraphics* GetGraphics() { return m_Graphics; }

private:
	CChain* m_pCChain;

	IObjectLinkCollection*	m_Objects;		//链中的对象集合
	IChGraphics*		m_Graphics;			//二维
};
