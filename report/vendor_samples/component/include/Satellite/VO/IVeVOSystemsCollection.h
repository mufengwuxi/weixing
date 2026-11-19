#pragma once
#include <string>
#include <vector>
#include "../../IAtkObj.h"
#include "../../ICEnum.h"
class CObjManage;
struct CObjData;
class IVeVOSystemsElement;

///
/// 相对轨迹集合
///
class IATKOBJECT_API IVeVOSystemsCollection
{
public:
    IVeVOSystemsCollection(const IVeVOSystemsCollection&) = delete;
    IVeVOSystemsCollection& operator=(const IVeVOSystemsCollection&) = delete;

	IVeVOSystemsCollection(CObjData* pObjData, CObjManage* IAsGObjManage);
	~IVeVOSystemsCollection();

	//********************************************************************-
	/// 添加相对轨迹
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	strSystemName  对象系统名称，例"Satellite/Satellite1 VVLH System"
	/// @Return	相对轨迹指针		
	//********************************************************************
	IVeVOSystemsElement* Add(std::string strSystemName);

	//********************************************************************
	/// 是否包含相对轨迹
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	strSystemName  对象系统名称，例"Satellite/Satellite1 VVLH System"
	/// @Return	是否包含相对轨迹，true-包含
	//********************************************************************
	bool Contains(std::string strSystemName);

	//********************************************************************
	/// 移除指定相对轨迹
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	strSystemName  对象系统名称，例"Satellite/Satellite1 VVLH System"
	/// @Return	
	//********************************************************************
	void Remove(std::string strSystemName);

	//********************************************************************
	/// 移除所有相对轨迹
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	
	//********************************************************************
	void RemoveAll();

	//********************************************************************
	/// 移除指定相对轨迹
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	nIndex  集合中第nIndex个相对轨迹
	/// @Return		
	//********************************************************************
	void RemoveAt(int nIndex);

	//********************************************************************
	/// 集合中相对轨迹个数
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	集合中相对轨迹个数	
	//********************************************************************
	int GetCount();

	//********************************************************************
	/// 获得指定相对轨迹
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	nIndex  集合中第nIndex个相对轨迹
	/// @Return	相对轨迹属性指针		
	//********************************************************************
	IVeVOSystemsElement* Item(int nIndex);


private:
	CObjManage*							m_IAsGObjManage;
	CObjData*							m_pObjData;

	std::vector<IVeVOSystemsElement*>	m_vecVOSysElem;   //元素数组

};
