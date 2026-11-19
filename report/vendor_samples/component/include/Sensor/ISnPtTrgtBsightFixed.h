#pragma once
#include "ISnPtTrgtBsight.h"
struct CObjData;
class IOrientation;

///
/// 瞄准类型：固定指向
///
class IATKOBJECT_API ISnPtTrgtBsightFixed : public ISnPtTrgtBsight
{
public:
    ISnPtTrgtBsightFixed(const ISnPtTrgtBsightFixed&) = delete;
    ISnPtTrgtBsightFixed& operator=(const ISnPtTrgtBsightFixed&) = delete;

	ISnPtTrgtBsightFixed(CObjData* pCObjData);
	~ISnPtTrgtBsightFixed();

	//********************************************************************
	/// 获得 指向固定属性指针
	/// @Author	Gan Shu Long
	/// @Date	2026-03-23
	/// @Return	指向固定属性指针
	//********************************************************************
	IOrientation* GetOrientation() { return m_Orientation; }

private:
	IOrientation*			m_Orientation;
};

