#pragma once
#include "../../IAtkObj.h"
class CSatellite;
class IVeInertia;

///
/// 质量
///
class IATKOBJECT_API IVeMassProperties
{
public:
    IVeMassProperties(const IVeMassProperties&) = delete;
    IVeMassProperties& operator=(const IVeMassProperties&) = delete;

	IVeMassProperties(CSatellite* pSate);
	~IVeMassProperties();

	//********************************************************************
	/// 获得转动惯量属性
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	转动惯量属性指针	
	//********************************************************************
	IVeInertia* GetInertia();

	//********************************************************************
	/// 设置质量参数
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dMass  质量参数(kg)
	/// @Return	
	//********************************************************************
	void SetMass(double dMass);
	//********************************************************************
	/// 获得质量参数
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	质量参数(kg)
	//********************************************************************
	double GetMass();
	//********************************************************************
	/// 获得当前时刻质量参数
	/// @Author	Dong Min
	/// @Date	2025-06-16
	/// @Input
	/// @Param	strTime  UTCG格式时间
	/// @Return	质量参数(kg)
	//********************************************************************
	double GetMass(std::string strTime);

	//********************************************************************
	/// 设置质心位置X
	/// @Author	Gan Shu Long
	/// @Date	2026-10-20
	/// @Input 
	/// @Param	dSaturatedEirp 质心位置X(m)
	/// @Return	
	//********************************************************************
	void SetCenterMassPositionX(double dX);

	//********************************************************************
	/// 获得质心位置X
	/// @Author	Gan Shu Long
	/// @Date	2026-10-20
	/// @Return	质心位置X(m)
	//********************************************************************
	double GetCenterMassPositionX();

	//********************************************************************
	/// 设置质心位置Y
	/// @Author	Gan Shu Long
	/// @Date	2026-10-20
	/// @Input 
	/// @Param	dSaturatedEirp 质心位置Y(m)
	/// @Return	
	//********************************************************************
	void SetCenterMassPositionY(double dY);

	//********************************************************************
	/// 获得质心位置Y
	/// @Author	Gan Shu Long
	/// @Date	2026-10-20
	/// @Return	质心位置Y(m)
	//********************************************************************
	double GetCenterMassPositionY();

	//********************************************************************
	/// 设置质心位置Z
	/// @Author	Gan Shu Long
	/// @Date	2026-10-20
	/// @Input 
	/// @Param	dSaturatedEirp 质心位置Z(m)
	/// @Return	
	//********************************************************************
	void SetCenterMassPositionZ(double dZ);

	//********************************************************************
	/// 获得质心位置Z
	/// @Author	Gan Shu Long
	/// @Date	2026-10-20
	/// @Return	质心位置Z(m)
	//********************************************************************
	double GetCenterMassPositionZ();

private:
	CSatellite*     m_pSate;

	IVeInertia*     m_Inertia;   //转动惯量属性

};
