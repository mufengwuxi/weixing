#pragma once
#include "../IAtkObj.h"
#include "IAreaTypeData.h"
#include <vector>
class IAreaTypePattern;
struct AsTAtPos;

///
/// 区域模式(边界坐标列表)
///
class IATKOBJECT_API IAreaTypePatternCollection :public IAreaTypeData
{
public:
    IAreaTypePatternCollection(const IAreaTypePatternCollection&) = delete;
    IAreaTypePatternCollection& operator=(const IAreaTypePatternCollection&) = delete;

	IAreaTypePatternCollection(AsTAtPos* pAsTAtPos);
	~IAreaTypePatternCollection();

	//********************************************************************
	/// 添加区域边界坐标
	/// @Author	Gan Shu Long
	/// @Date	2026-01-15
	/// @Input
	/// @Param	dLat  维度
	/// @Param	dLon  经度
	/// @Return 区域边界坐标
	//********************************************************************
	IAreaTypePattern* Add(double dLat, double dLon);

	//********************************************************************
	/// 插入区域边界坐标
	/// @Author	Gan Shu Long
	/// @Date	2026-01-15
	/// @Input
	/// @Param	dLat  维度
	/// @Param	dLon  经度
	/// @Param	nIndex  指定序号
	/// @Return 区域边界坐标
	//********************************************************************
	IAreaTypePattern* Insert(double dLat, double dLon, int nIndex);

	//********************************************************************
	/// 移除指定序号区域边界坐标
	/// @Author	Gan Shu Long
	/// @Date	2026-01-15
	/// @Input
	/// @Param	nIndex  指定序号
	/// @Return	
	//********************************************************************
	void Remove(int nIndex);

	//********************************************************************
	/// 移除所有区域边界坐标
	/// @Author	Gan Shu Long
	/// @Date	2026-01-15
	/// @Return	
	//********************************************************************
	void RemoveAll();

	//********************************************************************
	/// 返回数组
	/// @Author	Gan Shu Long
	/// @Date	2026-01-15
	/// @Input
	/// @Param	
	/// @Return	std::vector<IAreaTypePattern*> 区域边界坐标Vector
	//********************************************************************
	std::vector<IAreaTypePattern*> ToArray();

	//********************************************************************
	/// 获取集合个数
	/// @Author	Gan Shu Long
	/// @Date	2026-01-15
	/// @Return	集合个数
	//********************************************************************
	int GetCount();

	//********************************************************************
	///  获取指定序号区域边界坐标
	/// @Author	Gan Shu Long
	/// @Date	2026-01-15
	/// @Input
	/// @Param	nIndex  指定序号
	/// @Return	指定序号区域边界坐标
	//********************************************************************
	IAreaTypePattern* Item(int nIndex);

private:
	AsTAtPos*										m_pAsTAtPos;							//C对象边界坐标列表
	std::vector<IAreaTypePattern*>		m_VectorIAreaTypePattern;
};
