#pragma once
#include "../../IAtkObj.h"
#include "../../ICEnum.h"
class CSatellite;
class IVeBreakAngle;

///
/// 轨道圈数定义经纬度
///
class IATKOBJECT_API IVeDefinition
{
public:
    IVeDefinition(const IVeDefinition&) = delete;
    IVeDefinition& operator=(const IVeDefinition&) = delete;

	IVeDefinition(CSatellite* pSate);
	~IVeDefinition();

	//********************************************************************
	/// 设置二维轨道圈数经纬度类型
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	eBreakAngleType   轨道圈数经纬度类型
	/// @Return				
	//********************************************************************
	void SetBreakAngleType(EVeBreakAngleType eBreakAngleType);

	//********************************************************************
	/// 获得经纬度类型指针
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	经纬度类型指针
	//********************************************************************
	IVeBreakAngle* GetBreakAngle();

	//********************************************************************
	/// 获得经纬度类型
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	经纬度类型
	//********************************************************************
	EVeBreakAngleType GetBreakAngleType();

	//********************************************************************
	/// 设置方向
	/// @Author	Gan Shu Long
	/// @Date	2026-10-20
	/// @Input 
	/// @Param	eVeDirection 方向
	/// @Return	
	//********************************************************************
	void SetDirection(EVeDirection eVeDirection);

	//********************************************************************
	/// 获得方向
	/// @Author	Gan Shu Long
	/// @Date	2026-10-20
	/// @Return	方向
	//********************************************************************
	EVeDirection GetDirection();

private:
	CSatellite*           m_pSate;

	IVeBreakAngle*        m_BreakAngle;       //经纬度类型指针
};

