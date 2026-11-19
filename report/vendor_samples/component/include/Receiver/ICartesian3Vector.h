#pragma once
#include "../IAtkObj.h"
class CAntenna;

///
/// 天线指向偏移
///
class IATKOBJECT_API ICartesian3Vector
{
public:
    ICartesian3Vector(const ICartesian3Vector&) = delete;
    ICartesian3Vector& operator=(const ICartesian3Vector&) = delete;

	ICartesian3Vector(CAntenna* pCAntenna);
	~ICartesian3Vector();

	//********************************************************************
	/// 设置  X
	/// @Author	Gan Shu Long
	/// @Date	2026-03-19
	/// @Input 
	/// @Param	dX X
	/// @Return	
	//********************************************************************
	void SetX(double dX);

	//********************************************************************
	/// 获得 X
	/// @Author	Gan Shu Long
	/// @Date	2026-03-19
	/// @Return	X
	//********************************************************************
	double GetX();

	//********************************************************************
	/// 设置  Y
	/// @Author	Gan Shu Long
	/// @Date	2026-03-19
	/// @Input 
	/// @Param	dY Y
	/// @Return	
	//********************************************************************
	void SetY(double dY);

	//********************************************************************
	/// 获得 Y
	/// @Author	Gan Shu Long
	/// @Date	2026-03-19
	/// @Return	Y
	//********************************************************************
	double GetY();

	//********************************************************************
	/// 设置  Z
	/// @Author	Gan Shu Long
	/// @Date	2026-03-19
	/// @Input 
	/// @Param	dZ Z
	/// @Return	
	//********************************************************************
	void SetZ(double dZ);

	//********************************************************************
	/// 获得 Z
	/// @Author	Gan Shu Long
	/// @Date	2026-03-19
	/// @Return	Z
	//********************************************************************
	double GetZ();

private:
	CAntenna*			m_pCAntenna;		//天线对象指针
};

