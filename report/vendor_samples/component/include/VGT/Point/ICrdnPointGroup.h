#pragma once
#include <string>
#include <vector>
#include "../../IAtkObj.h"
#include "../../ICEnum.h"
class CObjectBase;
class CObjManage;
class ICrdnPoint;
class CVGTPoint;


///
/// VGT点集合
///
class IATKOBJECT_API ICrdnPointGroup
{
public:
    ICrdnPointGroup(const ICrdnPointGroup&) = delete;
    ICrdnPointGroup& operator=(const ICrdnPointGroup&) = delete;

	ICrdnPointGroup(CObjectBase* pCObjectBase, CObjManage* IAsGObjManage);
	virtual ~ICrdnPointGroup();
	
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
	/// @Param	ECrdnPointType  类型
	/// @Return	是否支持类型	
	//********************************************************************
	bool IsTypeSupported(ECrdnPointType eCrdnPointType);

	//********************************************************************
	/// 创建VGT角
	/// @Author	Gan Shu Long
	/// @Date	2026-06-02
	/// @Input
	/// @Param	strName 名称
	/// @Param	strDescription 描述
	/// @Param	eCrdnPointType 角类型
	/// @Return	VGT角
	//********************************************************************
	ICrdnPoint* Create(std::string strName, std::string strDescription, ECrdnPointType eCrdnPointType);

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
	ICrdnPoint* Item(std::string strName);

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
	ICrdnPoint* NewICrdnPoint(CVGTPoint* pCVGTPoint, ECrdnPointType eCrdnPointType);

private:
	CObjManage*                           m_IAsGObjManage;
	CObjectBase*                          m_pCObjectBase;

	std::vector<ICrdnPoint*>              m_VectorICrdnPoint;
};
