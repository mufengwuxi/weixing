#pragma once
#include <string>
#include <vector>
#include "../../IAtkObj.h"
#include "../../ICEnum.h"
class CObjectBase;
class CObjManage;
class ICrdnAxes;
class CVGTAxes;


///
/// VGT轴集合
///
class IATKOBJECT_API ICrdnAxesGroup
{
public:
    ICrdnAxesGroup(const ICrdnAxesGroup&) = delete;
    ICrdnAxesGroup& operator=(const ICrdnAxesGroup&) = delete;

	ICrdnAxesGroup(CObjectBase* pCObjectBase, CObjManage* IAsGObjManage);
	virtual ~ICrdnAxesGroup();
	
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
	/// @Param	ECrdnAxesType  类型
	/// @Return	是否支持类型	
	//********************************************************************
	bool IsTypeSupported(ECrdnAxesType eCrdnAxesType);

	//********************************************************************
	/// 创建VGT角
	/// @Author	Gan Shu Long
	/// @Date	2026-06-02
	/// @Input
	/// @Param	strName 名称
	/// @Param	strDescription 描述
	/// @Param	eCrdnAxesType 角类型
	/// @Return	VGT角
	//********************************************************************
	ICrdnAxes* Create(std::string strName, std::string strDescription, ECrdnAxesType eCrdnAxesType);

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
	ICrdnAxes* Item(std::string strName);

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
	ICrdnAxes* NewICrdnAxes(CVGTAxes* pCVGTAxes, ECrdnAxesType eCrdnAxesType);

private:
	CObjManage*                           m_IAsGObjManage;
	CObjectBase*                          m_pCObjectBase;

	std::vector<ICrdnAxes*>              m_VectorICrdnAxes;
};
