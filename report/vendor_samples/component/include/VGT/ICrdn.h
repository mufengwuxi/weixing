#pragma once
#include "../IAtkObj.h"
#include "../ICEnum.h"
#include "../IClassInfo.h"
#include "ICrdnMacroDecl.h"
#include <string>
class CObjectBase;
class CObjManage;
class CVGTBase;
template <typename T> class TWeakPtr;

///
/// VGT元素基类
///
class IATKOBJECT_API ICrdn :public IClassInfo
{
public:
    ICrdn(const ICrdn&) = delete;
    ICrdn& operator=(const ICrdn&) = delete;

	ICrdn(CObjectBase* pCObjectBase, CObjManage* IAsGObjManage, CVGTBase* pCVGTBase);
	virtual ~ICrdn();
	
	//********************************************************************
	/// 设置 VGT元素名称
	/// @Author	Gan Shu Long
	/// @Date	2026-06-03
	/// @Input
	/// @Param	strName  VGT元素名称
	/// @Return	
	//********************************************************************
	void SetName(std::string strName);

	//********************************************************************
	/// 获得 VGT元素名称
	/// @Author	Gan Shu Long
	/// @Date	2026-06-03
	/// @Return	VGT元素名称
	//********************************************************************
	std::string GetName();

	//********************************************************************
	/// 获得 父对象路径
	/// @Author	Gan Shu Long
	/// @Date	2026-06-03
	/// @Return	父对象路径
	//********************************************************************
	std::string GetPath();
	
	//********************************************************************
	/// 设置 VGT元素描述
	/// @Author	Gan Shu Long
	/// @Date	2026-06-03
	/// @Input
	/// @Param	strDescription VGT元素描述
	/// @Return	
	//********************************************************************
	void SetDescription(std::string strDescription);

	//********************************************************************
	/// 获得 VGT元素描述
	/// @Author	Gan Shu Long
	/// @Date	2026-06-03
	/// @Return	VGT元素描述
	//********************************************************************
	std::string GetDescription();

protected:
	//********************************************************************
	/// 获得VGT对象通过路径
	/// @Author	Gan Shu Long
	/// @Date	2026-06-01
	/// @Input
	/// @Param	strVGTPath VGT对象路径 例如:"Satellite/Satellite1 VNC.X"
	/// @Param	strVGTType VGT对象类型 例如:"CVGTAngle"
	/// @Return	VGT对象
	//********************************************************************
	CVGTBase* GetVGTObjFromStrPath(std::string strVGTPath, std::string strVGTType);

protected:
	CObjectBase*        m_pCObjectBase;
	CObjManage*			m_IAsGObjManage;
	TWeakPtr<CVGTBase>*	m_pCVGTBase;			//使用弱引用指针，处理底层VGT被界面删除的情况，调用前用m_pCVGTBase->get()验证底层VGT是否还存在
};
