#pragma once
#include "../IAtkObj.h"
#include "IObjectLink.h"
#include <string>
#include <vector>
class CChain;
class CObjManage;
class IAtkObject;

///
/// 链路对象列表
///
class IATKOBJECT_API IObjectLinkCollection
{
public:
    IObjectLinkCollection(const IObjectLinkCollection&) = delete;
    IObjectLinkCollection& operator=(const IObjectLinkCollection&) = delete;

	IObjectLinkCollection(CChain* pCChain, CObjManage* IAsGObjManage, IAtkObjectRoot* Root);
	~IObjectLinkCollection();

	//********************************************************************
	/// 增加链接对象通过对象路径名称
	/// @Author	Gan Shu Long
	/// @Date	2024-01-26
	/// @Input
	/// @Param	std::string  对象名称
	/// @Return	空
	//********************************************************************
	void Add(std::string strObjPathName);

	//********************************************************************
	/// 增加链接对象通过对象指针
	/// @Author	Gan Shu Long
	/// @Date	2024-01-26
	/// @Input
	/// @Param	IAtkObject*  对象指针
	/// @Return	空
	//********************************************************************
	void AddObject(IAtkObject* pIAtkObject);

	//********************************************************************
	/// 搜索链接对象路径名称是否存在
	/// @Author	Gan Shu Long
	/// @Date	2024-01-26
	/// @Input
	/// @Param	std::string  对象名称
	/// @Return	是否存在
	//********************************************************************
	bool Contains(std::string strObjPathName);

	//********************************************************************
	/// 获得链接对象索引
	/// @Author	Gan Shu Long
	/// @Date	2024-01-26
	/// @Input
	/// @Param	std::string  对象名称
	/// @Return	对象索引 -1表示不存在
	//********************************************************************
	int IndexOf(std::string strObjPathName);

	//********************************************************************
	/// 通过索引移除链接对象
	/// @Author	Gan Shu Long
	/// @Date	2024-01-26
	/// @Input
	/// @Param	int  对象索引
	/// @Return	空
	//********************************************************************
	void Remove(int index);

	//********************************************************************
	/// 移除所有链接对象
	/// @Author	Gan Shu Long
	/// @Date	2024-01-26
	/// @Input
	/// @Param	
	/// @Return	空
	//********************************************************************
	void RemoveAll();

	//********************************************************************
	/// 通过对象路径名称移除链接对象
	/// @Author	Gan Shu Long
	/// @Date	2024-01-26
	/// @Input
	/// @Param	std::string  对象名称
	/// @Return	空
	//********************************************************************
	void RemoveName(std::string strObjPathName);

	//********************************************************************
	/// 通过对象指针移除链接对象
	/// @Author	Gan Shu Long
	/// @Date	2024-01-26
	/// @Input
	/// @Param	IAtkObject*  对象指针
	/// @Return	空
	//********************************************************************
	void RemoveObject(IAtkObject* pIAtkObject);

	//********************************************************************
	/// 获取所有链接对象
	/// @Author	Gan Shu Long
	/// @Date	2024-01-26
	/// @Input
	/// @Param	
	/// @Return	链接对象列表
	//********************************************************************
	std::vector<IObjectLink*> AvailableObjects();

	//********************************************************************
	/// 通过索引获得链接对象
	/// @Author	Gan Shu Long
	/// @Date	2024-01-26
	/// @Input
	/// @Param	int  对象索引
	/// @Return	链接对象
	//********************************************************************
	IObjectLink* Item(int index);

	//********************************************************************
	/// 获得链接对象数量
	/// @Author	Gan Shu Long
	/// @Date	2024-01-26
	/// @Input
	/// @Param	
	/// @Return	链接对象数量
	//********************************************************************
	int GetCount()
	{
		return static_cast<int>(m_IObjectLinkVector.size());
	};
	
private:
	CObjManage*                 m_IAsGObjManage;
	CChain*						m_pCChain;

	IAtkObjectRoot*                   m_Root;                ///<根对象信息
	std::vector<IObjectLink*>	m_IObjectLinkVector;		//链中的对象集合
};
