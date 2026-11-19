#pragma once
#include "../IAtkObj.h"
#include <vector>
struct CVehEllipse;
class IVeEllipseDataElement;

///
/// 椭圆数据集合
///
class IATKOBJECT_API IVeEllipseDataCollection
{
public:
    IVeEllipseDataCollection(const IVeEllipseDataCollection&) = delete;
    IVeEllipseDataCollection& operator=(const IVeEllipseDataCollection&) = delete;

	IVeEllipseDataCollection(std::vector<CVehEllipse>* pVectorCVehEllipse);
	~IVeEllipseDataCollection();

	//********************************************************************
	/// 添加椭圆数据
	/// @Author	Gan Shu Long
	/// @Date	2026-01-08
	/// @Return	椭圆数据
	//********************************************************************
	IVeEllipseDataElement* Add();
	
	//********************************************************************
	/// 移除所有椭圆数据
	/// @Author	Gan Shu Long
	/// @Date	2026-01-08
	/// @Return	
	//********************************************************************
	void RemoveAll();

	//********************************************************************
	/// 移除指定序号椭圆数据
	/// @Author	Gan Shu Long
	/// @Date	2026-01-08
	/// @Input
	/// @Param	nIndex  指定序号
	/// @Return	
	//********************************************************************
	void RemoveAt(int nIndex);

	//********************************************************************
	/// 获取椭圆数据个数
	/// @Author	Gan Shu Long
	/// @Date	2026-01-08
	/// @Return	椭圆数据个数
	//********************************************************************
	int GetCount();

	//********************************************************************
	///  获取指定序号椭圆数据
	/// @Author	Gan Shu Long
	/// @Date	2026-01-08
	/// @Input
	/// @Param	nIndex  指定序号
	/// @Return	指定序号椭圆数据
	//********************************************************************
	IVeEllipseDataElement* Item(int nIndex);

private:
	std::vector<IVeEllipseDataElement*>		m_VectorIVeEllipseDataElement;	//i对象椭圆数据集合

	std::vector<CVehEllipse>*				m_pVectorCVehEllipse;			//C对象椭圆数据集合
};

