#pragma once
#include "../../IAtkObj.h"
#include <vector>
struct CGfxContourAttri;
class ILevelAttribute;

///
/// 二维范围分级列表
///
class IATKOBJECT_API ILevelAttributeCollection
{
public:
    ILevelAttributeCollection(const ILevelAttributeCollection&) = delete;
    ILevelAttributeCollection& operator=(const ILevelAttributeCollection&) = delete;

	ILevelAttributeCollection(std::vector<CGfxContourAttri>* pVectorCGfxContourAttri);
	~ILevelAttributeCollection();

	//********************************************************************
	/// 添加分级deg
	/// @Author	Gan Shu Long
	/// @Date	2026-01-08
	/// @Input
	/// @Param	dLevelDeg  分级deg
	/// @Return		
	//********************************************************************
	ILevelAttribute* AddLevel(double dLevelDeg);

	//********************************************************************
	/// 根据步长添加分级deg
	/// @Author	Gan Shu Long
	/// @Date	2026-01-08
	/// @Input
	/// @Param	dStartElevation 开始Elevation
	/// @Param	dStopElevation 停止Elevation
	/// @Param	dSetpElevation 步长Elevation
	/// @Return		
	//********************************************************************
	void AddLevelRange(double dStartElevation, double dStopElevation, double dSetpElevation);

	//********************************************************************
	/// 移除所有分级
	/// @Author	Gan Shu Long
	/// @Date	2026-01-08
	/// @Return	
	//********************************************************************
	void RemoveAll();

	//********************************************************************
	/// 移除指定序号分级
	/// @Author	Gan Shu Long
	/// @Date	2026-01-08
	/// @Input
	/// @Param	nIndex  指定序号
	/// @Return	
	//********************************************************************
	void Remove(int nIndex);

	//********************************************************************
	/// 获取分级个数
	/// @Author	Gan Shu Long
	/// @Date	2026-01-08
	/// @Return	分级个数
	//********************************************************************
	int GetCount();

	//********************************************************************
	///  获取指定序号分级
	/// @Author	Gan Shu Long
	/// @Date	2026-01-08
	/// @Input
	/// @Param	nIndex  指定序号
	/// @Return	指定序号分级
	//********************************************************************
	ILevelAttribute* Item(int nIndex);

private:
	std::vector<ILevelAttribute*>			m_VectorILevelAttribute;				//i对象范围分级指针列表，按deg从小到大排序

	std::vector<CGfxContourAttri>*		m_pVectorCGfxContourAttri;		//C对象范围分级指针列表
};

