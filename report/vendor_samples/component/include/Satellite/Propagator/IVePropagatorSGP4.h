#pragma once
#include "IVePropagator.h"
struct CObjData;
class CSatellite;
class IVePropagatorSGP4CommonTasks;
class IVeSGP4SegmentCollection;
class ICrdnEventIntervalSmartInterval;

///
/// SGP4预报器
///
class IATKOBJECT_API IVePropagatorSGP4 : public IVePropagator
{
public:
    IVePropagatorSGP4(const IVePropagatorSGP4&) = delete;
    IVePropagatorSGP4& operator=(const IVePropagatorSGP4&) = delete;

	IVePropagatorSGP4(CObjData* pObjData);
	~IVePropagatorSGP4();

	//********************************************************************
	/// 获得操作段属性类
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	SGP4段操作指针			
	//********************************************************************
	IVePropagatorSGP4CommonTasks* GetCommonTasks();

	//********************************************************************
	/// 获得段集合类
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	段集合指针			
	//********************************************************************
	IVeSGP4SegmentCollection* GetSegments();

	//********************************************************************
	/// 设置步长
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dStep 步长
	/// @Return	
	//********************************************************************
	void SetStep(double dStep);
	//********************************************************************
	/// 获得步长
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	步长			
	//********************************************************************
	double GetStep();

	//********************************************************************
	/// 获取 历元时间间隔
	/// @Author	Gan Shu Long
	/// @Date	2026-10-21
	/// @Input
	/// @Return	历元时间间隔
	//********************************************************************
	ICrdnEventIntervalSmartInterval* GetEphemerisInterval() { return m_EphemerisInterval; }

private:
	CObjData*              m_pObjData;
	CSatellite*            m_pSate;

	IVePropagatorSGP4CommonTasks*     m_CommonTasks;   //操作段
	IVeSGP4SegmentCollection*         m_Segments;      //段集合
	ICrdnEventIntervalSmartInterval*	m_EphemerisInterval;	//历元时间间隔

};
