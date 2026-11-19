#pragma once
#include "../IAtkObj.h"
#include <vector>
#include <string>
struct CVehEllipse;
class CObjManage;

///
/// 区域目标列表
///
class IATKOBJECT_API ICvAreaTargetsCollection
{
public:
    ICvAreaTargetsCollection(const ICvAreaTargetsCollection&) = delete;
    ICvAreaTargetsCollection& operator=(const ICvAreaTargetsCollection&) = delete;

	ICvAreaTargetsCollection(std::vector<void*>* pVectorAreaTargetList, CObjManage* IAsGObjManage);
	~ICvAreaTargetsCollection();

	//********************************************************************
	/// 添加区域目标
	/// @Author	Gan Shu Long
	/// @Date	2026-01-14
	/// @Input
	/// @Param	strTargetName  区域目标名称
	/// @Return	
	//********************************************************************
	void Add(std::string strTargetName);

	//********************************************************************
	/// 添加区域目标
	/// @Author	Gan Shu Long
	/// @Date	2026-01-14
	/// @Input
	/// @Param	strTargetName  区域名称
	/// @Return	
	//********************************************************************
	void Remove(std::string strTargetName);

	//********************************************************************
	/// 移除所有区域目标
	/// @Author	Gan Shu Long
	/// @Date	2026-01-14
	/// @Return	
	//********************************************************************
	void RemoveAll();

	//********************************************************************
	/// 移除指定序号区域目标
	/// @Author	Gan Shu Long
	/// @Date	2026-01-14
	/// @Input
	/// @Param	nIndex  指定序号
	/// @Return	
	//********************************************************************
	void RemoveAt(int nIndex);

	//********************************************************************
	/// 获取可用区域目标列表
	/// @Author	Gan Shu Long
	/// @Date	2026-01-14
	/// @Return	可用区域目标列表
	//********************************************************************
	std::vector<std::string> GetAvailableAreaTargets();

	//********************************************************************
	/// 获取区域目标个数
	/// @Author	Gan Shu Long
	/// @Date	2026-01-14
	/// @Return	区域个数
	//********************************************************************
	int GetCount();

	//********************************************************************
	///  获取指定序号区域目标
	/// @Author	Gan Shu Long
	/// @Date	2026-01-14
	/// @Input
	/// @Param	nIndex  指定序号
	/// @Return	指定序号区域目标
	//********************************************************************
	std::string Item(int nIndex);

private:
	CObjManage*											m_IAsGObjManage;

	std::vector<void*>*									m_pVectorAreaTargetList;									//C对象区域目标列表
};

