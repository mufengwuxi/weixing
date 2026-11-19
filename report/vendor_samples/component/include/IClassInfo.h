#pragma once
#include <string>
#include "IAtkObj.h"
class CObjectBase;

///
/// 基本属性
///
#pragma warning(disable : 4251)
class IATKOBJECT_API  IClassInfo
{
public:
    IClassInfo(const IClassInfo&) = delete;
    IClassInfo& operator=(const IClassInfo&) = delete;

	IClassInfo(CObjectBase* pObjBase);
	virtual ~IClassInfo();

	 std::string			GetClassInfoName() { return m_ClassName; };			///<对象类名

	//********************************************************************
	/// 设置对象属性值
	/// @Author	Dong Min
	/// @Date	2025-06-24
	/// @Input
	/// @Param	strObjPath   属性设置路径
	/// @Param	strValue   属性值
	/// @Return	
	//********************************************************************
	 bool	SetAttribute(std::string strObjPath, std::string strValue);
	 
	//********************************************************************
	/// 获取对象属性值
	/// @Author	Dong Min
	/// @Date	2025-06-24
	/// @Input
	/// @Param	strObjPath   属性设置路径 
	/// @Return	 属性值
	//********************************************************************
	 std::string GetAttribute(std::string strObjPath);

	 //********************************************************************
	 /// 对象参数设置界面
	 /// @Author	Dong Min
	 /// @Date	2025-12-01
	 /// @Input
	 /// @Param	strObjPath   属性设置路径
	 /// @Param	strValue   属性值
	 /// @Return	
	 //********************************************************************
	 virtual bool	SetUiParameter();


protected:
	CObjectBase*        m_pObjBase;
	std::string			m_ClassName;

};
