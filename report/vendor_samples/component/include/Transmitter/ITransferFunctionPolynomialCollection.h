#pragma once
#include "../IAtkObj.h"
#include <vector>

///
/// 频率转换方法集合
///
class IATKOBJECT_API ITransferFunctionPolynomialCollection
{
public:
    ITransferFunctionPolynomialCollection(const ITransferFunctionPolynomialCollection&) = delete;
    ITransferFunctionPolynomialCollection& operator=(const ITransferFunctionPolynomialCollection&) = delete;

	ITransferFunctionPolynomialCollection(std::vector<double>* pPolynomialCollection);
	~ITransferFunctionPolynomialCollection();

	//********************************************************************
	/// 添加系数
	/// @Author	Gan Shu Long
	/// @Date	2026-03-26
	/// @Input 
	/// @Param	dCoefficient 添加系数
	/// @Return	
	//********************************************************************
	void Add(double dCoefficient);

	//********************************************************************
	/// 插入系数
	/// @Author	Gan Shu Long
	/// @Date	2026-03-26
	/// @Input 
	/// @Param	index 指定序号
	/// @Param	dCoefficient 添加系数
	/// @Return	
	//********************************************************************
	void InsertAt(int index, double dCoefficient);

	//********************************************************************
	/// 移除系数
	/// @Author	Gan Shu Long
	/// @Date	2026-03-26
	/// @Input 
	/// @Param	index 指定序号
	/// @Return	
	//********************************************************************
	void RemoveAt(int index);

	//********************************************************************
	/// 获取集合个数
	/// @Author	Gan Shu Long
	/// @Date	2026-03-26
	/// @Return	集合个数
	//********************************************************************
	int GetCount();

	//********************************************************************
	///  获取指定序号椭圆设置
	/// @Author	Gan Shu Long
	/// @Date	2026-03-26
	/// @Input
	/// @Param	index  指定序号
	/// @Return	系数
	//********************************************************************
	double Item(int index);
	
private:
	std::vector<double>*		m_pPolynomialCollection;
};
