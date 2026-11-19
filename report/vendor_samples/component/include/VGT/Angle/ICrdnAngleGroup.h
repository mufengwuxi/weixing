#pragma once
#include <string>
#include <vector>
#include "../../IAtkObj.h"
#include "../../ICEnum.h"
class CObjectBase;
class CObjManage;
class ICrdnAngle;
class CVGTAngle;


///
/// VGT角集合
///
class IATKOBJECT_API ICrdnAngleGroup
{
public:
    ICrdnAngleGroup(const ICrdnAngleGroup&) = delete;
    ICrdnAngleGroup& operator=(const ICrdnAngleGroup&) = delete;

	ICrdnAngleGroup(CObjectBase* pCObjectBase, CObjManage* IAsGObjManage);
	virtual ~ICrdnAngleGroup();
	
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
	/// @Param	ECrdnAngleType  类型
	/// @Return	是否支持类型	
	//********************************************************************
	bool IsTypeSupported(ECrdnAngleType eCrdnAngleType);

	//********************************************************************
	/// 创建VGT角
	/// @Author	Gan Shu Long
	/// @Date	2026-06-02
	/// @Input
	/// @Param	strName 名称
	/// @Param	strDescription 描述
	/// @Param	eCrdnAngleType 角类型
	/// @Return	VGT角
	//********************************************************************
	ICrdnAngle* Create(std::string strName, std::string strDescription, ECrdnAngleType eCrdnAngleType);

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
	ICrdnAngle* Item(std::string strName);

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
	ICrdnAngle* NewICrdnAngle(CVGTAngle* pCVGTAngle, ECrdnAngleType eCrdnAngleType);

private:
	CObjManage*                           m_IAsGObjManage;
	CObjectBase*                          m_pCObjectBase;

	std::vector<ICrdnAngle*>              m_VectorICrdnAngle;
};
