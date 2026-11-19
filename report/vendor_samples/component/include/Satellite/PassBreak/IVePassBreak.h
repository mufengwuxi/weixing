#pragma once
#include "../../ICEnum.h"
class CSatellite;
class ISatellite;
class IVeDefinition;
class IVePassNumbering;

///
/// 轨道圈数
///
class IATKOBJECT_API IVePassBreak
{
public:
    IVePassBreak(const IVePassBreak&) = delete;
    IVePassBreak& operator=(const IVePassBreak&) = delete;

	IVePassBreak(CSatellite* pSate, ISatellite* pISate);
	~IVePassBreak();

	//********************************************************************
	/// 设置坐标系
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	eCoordSys 坐标系
	/// @Return	
	//********************************************************************
	void SetCoordinateSystem(EVeCoordinateSystem eCoordSys);
	//********************************************************************
	/// 获得坐标系
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	坐标系
	//********************************************************************
	EVeCoordinateSystem GetCoordinateSystem();

	//********************************************************************
	/// 获得定义属性
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	定义属性指针
	//********************************************************************
	IVeDefinition* GetDefinition();

	//********************************************************************
	/// 获得圈数属性
	/// @Author	Gan Shu Long
	/// @Date	2026-10-20
	/// @Return	定义圈数指针
	//********************************************************************
	IVePassNumbering* GetPassNumbering() { return m_PassNumbering; }

private:
	CSatellite*             m_pSate;
	ISatellite*             m_pISate;

	IVeDefinition*			m_Definition;		//定义
	IVePassNumbering*		m_PassNumbering;	//圈数
};
