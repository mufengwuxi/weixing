#pragma once
#include <string>
#include <vector>
#include "../../IAtkObj.h"
#include "../../ICEnum.h"
class CObjectBase;
class CObjManage;
class ICrdnSystem;
class CVGTSystem;


///
/// VGT系统集合
///
class IATKOBJECT_API ICrdnSystemGroup
{
public:
    ICrdnSystemGroup(const ICrdnSystemGroup&) = delete;
    ICrdnSystemGroup& operator=(const ICrdnSystemGroup&) = delete;

	ICrdnSystemGroup(CObjectBase* pCObjectBase, CObjManage* IAsGObjManage);
	virtual ~ICrdnSystemGroup();
	
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
	/// @Param	ECrdnSystemType  类型
	/// @Return	是否支持类型	
	//********************************************************************
	bool IsTypeSupported(ECrdnSystemType eCrdnSystemType);

	//********************************************************************
	/// 创建VGT角
	/// @Author	Gan Shu Long
	/// @Date	2026-06-02
	/// @Input
	/// @Param	strName 名称
	/// @Param	strDescription 描述
	/// @Param	eCrdnSystemType 角类型
	/// @Return	VGT角
	//********************************************************************
	ICrdnSystem* Create(std::string strName, std::string strDescription, ECrdnSystemType eCrdnSystemType);

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
	ICrdnSystem* Item(std::string strName);

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
	ICrdnSystem* NewICrdnSystem(CVGTSystem* pCVGTSystem, ECrdnSystemType eCrdnSystemType);

private:
	CObjManage*                           m_IAsGObjManage;
	CObjectBase*                          m_pCObjectBase;

	std::vector<ICrdnSystem*>              m_VectorICrdnSystem;
};
