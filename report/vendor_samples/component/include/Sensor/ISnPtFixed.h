#pragma once
#include "../IAtkObj.h"
#include "ISnPointing.h"
struct CObjData;
class IOrientation;

///
/// 指向：固定
///
class IATKOBJECT_API ISnPtFixed : public ISnPointing
{
public:
    ISnPtFixed(const ISnPtFixed&) = delete;
    ISnPtFixed& operator=(const ISnPtFixed&) = delete;

	ISnPtFixed(CObjData* pCObjData);
	~ISnPtFixed();
	
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

