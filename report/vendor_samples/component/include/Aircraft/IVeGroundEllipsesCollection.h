#pragma once
#include "../IAtkObj.h"
#include <vector>
#include <string>
struct CVehEllipseSet;
class IVeGroundEllipseElement;

///
/// 椭圆设置集合
///
class IATKOBJECT_API IVeGroundEllipsesCollection
{
public:
    IVeGroundEllipsesCollection(const IVeGroundEllipsesCollection&) = delete;
    IVeGroundEllipsesCollection& operator=(const IVeGroundEllipsesCollection&) = delete;

	IVeGroundEllipsesCollection(std::vector<CVehEllipseSet>* pVectorCVehEllipseSet);
	~IVeGroundEllipsesCollection();

	//********************************************************************
	/// 添加椭圆设置
	/// @Author	Gan Shu Long
	/// @Date	2026-01-08
	/// @Input
	/// @Param	strEllipseSetName  椭圆设置名称
	/// @Return 椭圆设置
	//********************************************************************
	IVeGroundEllipseElement* Add(std::string strEllipseSetName);

	//********************************************************************
	///  获取指定名称椭圆设置
	/// @Author	Gan Shu Long
	/// @Date	2026-01-08
	/// @Input
	/// @Param	strEllipseSetName  椭圆设置名称
	/// @Return	指定名称椭圆设置
	//********************************************************************
	IVeGroundEllipseElement* GetEllipseSet(std::string strEllipseSetName);

	//********************************************************************
	/// 移除所有椭圆设置
	/// @Author	Gan Shu Long
	/// @Date	2026-01-08
	/// @Return	
	//********************************************************************
	void RemoveAll();

	//********************************************************************
	/// 移除指定序号椭圆设置
	/// @Author	Gan Shu Long
	/// @Date	2026-01-08
	/// @Input
	/// @Param	nIndex  指定序号
	/// @Return	
	//********************************************************************
	void RemoveAt(int nIndex);

	//********************************************************************
	/// 移除指定椭圆设置
	/// @Author	Gan Shu Long
	/// @Date	2026-01-08
	/// @Input
	/// @Param	strEllipseSetName  椭圆设置名称
	/// @Return	
	//********************************************************************
	void RemoveEllipseSet(std::string strEllipseSetName);

	//********************************************************************
	/// 获取集合个数
	/// @Author	Gan Shu Long
	/// @Date	2026-01-08
	/// @Return	集合个数
	//********************************************************************
	int GetCount();

	//********************************************************************
	///  获取指定序号椭圆设置
	/// @Author	Gan Shu Long
	/// @Date	2026-01-08
	/// @Input
	/// @Param	nIndex  指定序号
	/// @Return	指定序号椭圆设置
	//********************************************************************
	IVeGroundEllipseElement* Item(int nIndex);

private:
	std::vector<IVeGroundEllipseElement*>			m_VectorIVeGroundEllipseElement;				//i对象椭圆设置集合

	std::vector<CVehEllipseSet>*							m_pVectorCVehEllipseSet;								//C对象椭圆设置集合
};

