#pragma once
#include "../../IAtkObj.h"
class CObjManage;
struct CObjData;
class CSensor;
class ISnVOPulse;
class IVOVector;
class IVODataDisplayCollection;

///
/// 传感器三维
///
class IATKOBJECT_API ISnVO
{
public:
    ISnVO(const ISnVO&) = delete;
    ISnVO& operator=(const ISnVO&) = delete;

	ISnVO(CObjData* pObjData);
	~ISnVO();

	//********************************************************************
	/// 设置三维空间投影属性
	/// @Author	Gan Shu Long
	/// @Date	2024-06-12
	/// @Input
	/// @Param	double  三维空间投影属性
	/// @Return	
	//********************************************************************
	void SetSpaceProjection(double nSpaceProjection);

	//********************************************************************
	/// 获得三维空间投影属性
	/// @Author	Gan Shu Long
	/// @Date	2024-06-12
	/// @Return	三维空间投影属性		
	//********************************************************************
	double GetSpaceProjection();

	//********************************************************************
	/// 设置 是否使用距离约束
	/// @Author	Gan Shu Long
	/// @Date	2026-03-24
	/// @Input
	/// @Param	bEnableRangeConstraint  是否使用距离约束
	/// @Return	
	//********************************************************************
	void SetEnableRangeConstraint(bool bEnableRangeConstraint);

	//********************************************************************
	/// 获得 是否使用距离约束
	/// @Author	Gan Shu Long
	/// @Date	2026-03-24
	/// @Return	是否使用距离约束
	//********************************************************************
	bool GetEnableRangeConstraint();

	//********************************************************************
	/// 获得三维脉冲属性
	/// @Author	Gan Shu Long
	/// @Date	2024-06-12
	/// @Return	三维脉冲属性指针		
	//********************************************************************
	ISnVOPulse* GetPulse() { return m_Pulse; }

	//********************************************************************
	/// 获得三维向量属性
	/// @Author	Gan Shu Long
	/// @Date	2024-06-12
	/// @Return	三维向量属性指针		
	//********************************************************************
	IVOVector* GetVector() { return m_Vector; }

	//********************************************************************
	/// 获得三维数据显示属性
	/// @Author	Gan Shu Long
	/// @Date	2026-01-05
	/// @Return	三维数据显示属性指针		
	//********************************************************************
	IVODataDisplayCollection* GetDataDisplay() { return m_DataDisplay; }

private:
	CSensor*		m_pCSensor;

	ISnVOPulse*							m_Pulse;						//三维脉冲属性
	IVOVector*								m_Vector;					//三维向量属性
	IVODataDisplayCollection*     m_DataDisplay;			//三维数据显示属性
};
