#pragma once
#include <vector>
#include <string>
#include "IOrbitState.h"
class CObjManage;
class CSatellite;
class CPreOrbitParam;

///
/// 预报器位置速度设置
///
class IATKOBJECT_API IOrbitStateCartesian : public IOrbitState
{
public:
    IOrbitStateCartesian(const IOrbitStateCartesian&) = delete;
    IOrbitStateCartesian& operator=(const IOrbitStateCartesian&) = delete;

	IOrbitStateCartesian(CSatellite* pSate, CObjManage* IAsGObjManage, CPreOrbitParam* pPreOrbitParam);
	~IOrbitStateCartesian();

	//********************************************************************
	/// 设置位置X
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dXPos 位置X
	/// @Return	
	//********************************************************************
	void SetXPosition(double dXPos);
	//********************************************************************
	/// 获得位置X
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	位置X
	//********************************************************************
	double GetXPosition();

	//********************************************************************
	/// 设置速度X
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dXVel 速度X
	/// @Return	
	//********************************************************************
	void SetXVelocity(double dXVel);
	//********************************************************************
	/// 获得速度X
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	速度X
	//********************************************************************
	double GetXVelocity();

	//********************************************************************
	/// 设置位置Y
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dYPos 位置Y
	/// @Return	
	//********************************************************************
	void SetYPosition(double dYPos);
	//********************************************************************
	/// 获得位置Y
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	位置Y
	//********************************************************************
	double GetYPosition();

	//********************************************************************
	/// 设置速度Y
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dYVel 速度Y
	/// @Return	
	//********************************************************************
	void SetYVelocity(double dYVel);
	//********************************************************************
	/// 获得速度Y
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	速度Y
	//********************************************************************
	double GetYVelocity();

	//********************************************************************
	/// 设置位置Z
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dZPos 位置Z
	/// @Return	
	//********************************************************************
	void SetZPosition(double dZPos);
	//********************************************************************
	/// 获得位置Z
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	位置Z
	//********************************************************************
	double GetZPosition();

	//********************************************************************
	/// 设置速度Z
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dZVel 速度Z
	/// @Return	
	//********************************************************************
	void SetZVelocity(double dZVel);
	//********************************************************************
	/// 获得速度Z
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	速度Z
	//********************************************************************
	double GetZVelocity();

private:
	CSatellite*                     m_pSate;
	CPreOrbitParam*                 m_pPreOrbitParam;

};
