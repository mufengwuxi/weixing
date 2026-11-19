#pragma once
#include "../IAtkObj.h"
#include <string>
#include <vector>
class CSensor;
class CObjManage;
class IAtkObject;
class ISnTarget;

///
/// 目标定向已选对象列表
///
class IATKOBJECT_API ISnTargetCollection
{
public:
    ISnTargetCollection(const ISnTargetCollection&) = delete;
    ISnTargetCollection& operator=(const ISnTargetCollection&) = delete;

	ISnTargetCollection(CSensor* pCSensor, CObjManage* IAsGObjManage);
	~ISnTargetCollection();

	//********************************************************************
	/// 增加目标对象通过对象路径名称
	/// @Author	Gan Shu Long
	/// @Date	2026-03-24
	/// @Input
	/// @Param	strObjPathName  对象路径名称
	/// @Return	空
	//********************************************************************
	void Add(std::string strObjPathName);

	//********************************************************************
	/// 增加目标对象通过对象指针
	/// @Author	Gan Shu Long
	/// @Date	2026-03-24
	/// @Input
	/// @Param	IAtkObject*  对象指针
	/// @Return	空
	//********************************************************************
	void AddObject(IAtkObject* pIAtkObject);

	//********************************************************************
	/// 通过索引移除目标对象
	/// @Author	Gan Shu Long
	/// @Date	2026-03-24
	/// @Input
	/// @Param	int  对象索引
	/// @Return	空
	//********************************************************************
	void Remove(int index);

	//********************************************************************
	/// 移除所有目标对象
	/// @Author	Gan Shu Long
	/// @Date	2026-03-24
	/// @Input
	/// @Param	
	/// @Return	空
	//********************************************************************
	void RemoveAll();

	//********************************************************************
	/// 通过对象指针移除目标对象
	/// @Author	Gan Shu Long
	/// @Date	2026-03-24
	/// @Input
	/// @Param	IAtkObject*  对象指针
	/// @Return	空
	//********************************************************************
	void RemoveObject(IAtkObject* pIAtkObject);

	//********************************************************************
	/// 通过对象路径名称移除目标对象
	/// @Author	Gan Shu Long
	/// @Date	2026-03-24
	/// @Input
	/// @Param	strObjPathName  对象路径名称
	/// @Return	空
	//********************************************************************
	void RemoveTarget(std::string strObjPathName);

	//********************************************************************
	/// 获得目标对象数量
	/// @Author	Gan Shu Long
	/// @Date	2026-03-24
	/// @Input
	/// @Param	
	/// @Return	目标对象数量
	//********************************************************************
	int GetCount();

	//********************************************************************
	/// 通过索引获得目标对象
	/// @Author	Gan Shu Long
	/// @Date	2026-03-24
	/// @Input
	/// @Param	int  对象索引
	/// @Return	目标对象
	//********************************************************************
	ISnTarget* Item(int index);
	
private:
	CObjManage*						m_IAsGObjManage;
	CSensor*							m_pCSensor;

	std::vector<ISnTarget*>		m_ISnTargetVector;			//已选对象列表
};
