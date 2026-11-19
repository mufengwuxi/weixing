#pragma once
#include "../../IAtkObj.h"
#include <vector>
struct CVehEllipseSet;
class IVeGfxGroundEllipsesElement;

///
/// 椭圆设置集合
///
class IATKOBJECT_API IVeGfxGroundEllipsesCollection
{
public:
    IVeGfxGroundEllipsesCollection(const IVeGfxGroundEllipsesCollection&) = delete;
    IVeGfxGroundEllipsesCollection& operator=(const IVeGfxGroundEllipsesCollection&) = delete;

	IVeGfxGroundEllipsesCollection(std::vector<CVehEllipseSet>* pVectorCVehEllipseSet);
	~IVeGfxGroundEllipsesCollection();

	//********************************************************************
	/// 获取集合个数
	/// @Author	Gan Shu Long
	/// @Date	2026-01-09
	/// @Return	集合个数
	//********************************************************************
	int GetCount();

	//********************************************************************
	///  获取指定序号椭圆设置
	/// @Author	Gan Shu Long
	/// @Date	2026-01-09
	/// @Input
	/// @Param	nIndex  指定序号
	/// @Return	指定序号椭圆设置
	//********************************************************************
	IVeGfxGroundEllipsesElement* Item(int nIndex);

private:
	std::vector<IVeGfxGroundEllipsesElement*>			m_VectorIVeGfxGroundEllipsesElement;				//i对象椭圆设置集合

	std::vector<CVehEllipseSet>*								m_pVectorCVehEllipseSet;										//C对象椭圆设置集合
};

