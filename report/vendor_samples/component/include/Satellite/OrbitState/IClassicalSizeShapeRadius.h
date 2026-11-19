#pragma once
#include "../../ICEnum.h"
#include "IClassicalSizeShape.h"
class CSatellite;

///
/// 轨道根数远拱点半径和近拱点半径
///
class IATKOBJECT_API IClassicalSizeShapeRadius : public IClassicalSizeShape
{
public:
    IClassicalSizeShapeRadius(const IClassicalSizeShapeRadius&) = delete;
    IClassicalSizeShapeRadius& operator=(const IClassicalSizeShapeRadius&) = delete;

	IClassicalSizeShapeRadius(CSatellite* pSate);
	~IClassicalSizeShapeRadius();

	//********************************************************************
	/// 设置轨道根数方式半径属性值
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dApogeeRadius    远拱点半径
	/// @Param	dPerigeeRadius   近拱点半径
	/// @Return	
	//********************************************************************
	void SetSizeShapeRadius(double dApogeeRadius, double dPerigeeRadius);

	//********************************************************************
	/// 设置远拱点半径
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dApoRad 远拱点半径
	/// @Return	
	//********************************************************************
	void SetApogeeRadius(double dApoRad);
	//********************************************************************
	/// 获得远拱点半径
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	远拱点半径
	//********************************************************************
	double GetApogeeRadius();

	//********************************************************************
	/// 设置近拱点半径
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dPeriRad 近拱点半径
	/// @Return	
	//********************************************************************
	void SetPerigeeRadius(double dPeriRad);
	//********************************************************************
	/// 获得近拱点半径
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	近拱点半径
	//********************************************************************
	double GetPerigeeRadius();

private:
	CSatellite* m_pSate;

};
