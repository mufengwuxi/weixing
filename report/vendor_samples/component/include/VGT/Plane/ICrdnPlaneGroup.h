#pragma once
#include <string>
#include <vector>
#include "../../IAtkObj.h"
#include "../../ICEnum.h"
class CObjectBase;
class CObjManage;
class ICrdnPlane; 
class CVGTPlane;


///
/// VGT面集合
///
class IATKOBJECT_API ICrdnPlaneGroup
{
public:
    ICrdnPlaneGroup(const ICrdnPlaneGroup&) = delete;
    ICrdnPlaneGroup& operator=(const ICrdnPlaneGroup&) = delete;

	ICrdnPlaneGroup(CObjectBase* pCObjectBase, CObjManage* IAsGObjManage);
	virtual ~ICrdnPlaneGroup();
	
	//********************************************************************
	/// 获得支持的VGT元素类型
	/// @Author	Gan Shu Long
	/// @Date	2026-06-02
	/// @Return	支持的VGT元素类型
	//********************************************************************
	std::vector<std::string> GetSupportedVGTTypes();

	//********************************************************************
	/// 获得 是否支持类型
	/// @Author	Gan Shu Long
	/// @Date	2026-06-02
	/// @Input
	/// @Param	ECrdnPlaneType  类型
	/// @Return	是否支持类型	
	//********************************************************************
	bool IsTypeSupported(ECrdnPlaneType eCrdnPlaneType);

	//********************************************************************
	/// 创建VGT角
	/// @Author	Gan Shu Long
	/// @Date	2026-06-02
	/// @Input
	/// @Param	strName 名称
	/// @Param	strDescription 描述
	/// @Param	eCrdnPlaneType 角类型
	/// @Return	VGT角
	//********************************************************************
	ICrdnPlane* Create(std::string strName, std::string strDescription, ECrdnPlaneType eCrdnPlaneType);

	//********************************************************************
	/// 删除VGT元素
	/// @Author	Gan Shu Long
	/// @Date	2026-06-02
	/// @Input
	/// @Param	strName 名称
	/// @Return	是否成功
	//********************************************************************
	void Remove(std::string strName);

	//********************************************************************
	/// 获取VGT元素
	/// @Author	Gan Shu Long
	/// @Date	2026-06-02
	/// @Input
	/// @Param	strName VGT元素名称
	/// @Return	VGT元素基类指针
	//********************************************************************
	ICrdnPlane* Item(std::string strName);

	//********************************************************************
	/// 获取VGT是否已存在
	/// @Author	Gan Shu Long
	/// @Date	2026-06-02
	/// @Input
	/// @Param	strName VGT元素名称
	/// @Return	VGT是否已存在
	//********************************************************************
	bool Contains(std::string strName);

private:
	ICrdnPlane* NewICrdnPlane(CVGTPlane* pCVGTPlane, ECrdnPlaneType eCrdnPlaneType);

private:
	CObjManage*                           m_IAsGObjManage;
	CObjectBase*                          m_pCObjectBase;

	std::vector<ICrdnPlane*>              m_VectorICrdnPlane;
};
