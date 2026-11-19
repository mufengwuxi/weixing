#pragma once
#include "IVeAttitudeStandard.h"
struct CObjData;
class IScenario;
class IVeMultiSegmentBasic;

///
/// 多段姿态类型
///
class IATKOBJECT_API IVeOrbitAttitudeMultiSegment : public IVeAttitudeStandard
{
public:
    IVeOrbitAttitudeMultiSegment(const IVeOrbitAttitudeMultiSegment&) = delete;
    IVeOrbitAttitudeMultiSegment& operator=(const IVeOrbitAttitudeMultiSegment&) = delete;

	IVeOrbitAttitudeMultiSegment(CObjData* pObjData, IScenario* pIScenario);
	~IVeOrbitAttitudeMultiSegment();

	//********************************************************************
	/// 获得多段姿态属性
	/// @Author	Gan Shu Long
	/// @Date	2026-03-01
	/// @Return	多段姿态属性指针
	//********************************************************************
	IVeMultiSegmentBasic* GetBasic() { return m_Basic; }

private:
	CObjData*										m_pObjData;
	IScenario*										m_pIScenario;
	IVeMultiSegmentBasic*					m_Basic; 
};


