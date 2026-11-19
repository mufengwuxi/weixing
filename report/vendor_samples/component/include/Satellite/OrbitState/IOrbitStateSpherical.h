#pragma once
#include "../../ICEnum.h"
#include <vector>
#include <string>
#include "IOrbitState.h"
class CObjManage;
class CSatellite;
class CSphElem;
class CPreOrbitParam;
class IFlightPathAngle;

///
/// 预报器球坐标设置
///
class IATKOBJECT_API IOrbitStateSpherical : public IOrbitState
{
public:
    IOrbitStateSpherical(const IOrbitStateSpherical&) = delete;
    IOrbitStateSpherical& operator=(const IOrbitStateSpherical&) = delete;

	IOrbitStateSpherical(CSatellite* pSate, CObjManage* IAsGObjManage, CPreOrbitParam* pPreOrbitParam);
	~IOrbitStateSpherical();

	//********************************************************************
	/// 设置方位角
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dAzi 方位角
	/// @Return	
	//********************************************************************
	void SetAzimuth(double dAzi);
	//********************************************************************
	/// 获得方位角
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	方位角
	//********************************************************************
	double GetAzimuth();

	//********************************************************************
	/// 设置赤纬
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dDec 赤纬
	/// @Return	
	//********************************************************************
	void SetDeclination(double dDec);
	//********************************************************************
	/// 获得赤纬
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	赤纬
	//********************************************************************
	double GetDeclination();

	//********************************************************************
	/// 获得航迹角属性
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	航迹角属性指针
	//********************************************************************
	IFlightPathAngle* GetFPA();

	//********************************************************************
	/// 设置航迹角类型
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	eFPAType 航迹角类型
	/// @Return	
	//********************************************************************
	void SetFPAType(ESphericalFPA eFPAType);
	//********************************************************************
	/// 获得航迹角类型
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	航迹角类型
	//********************************************************************
	ESphericalFPA GetFPAType();

	//********************************************************************
	/// 设置半径
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dRadius 半径
	/// @Return	
	//********************************************************************
	void SetRadius(double dRadius);
	//********************************************************************
	/// 获得半径
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	半径
	//********************************************************************
	double GetRadius();

	//********************************************************************
	/// 设置赤经
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dRA 赤经
	/// @Return	
	//********************************************************************
	void SetRightAscension(double dRA);
	//********************************************************************
	/// 获得赤经
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	赤经
	//********************************************************************
	double GetRightAscension();

	//********************************************************************
	/// 设置速度
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dVel 速度
	/// @Return	
	//********************************************************************
	void SetVelocity(double dVel);
	//********************************************************************
	/// 获得速度
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	速度
	//********************************************************************
	double GetVelocity();

private:
	CObjManage*                  m_IAsGObjManage;
	CSatellite*                  m_pSate;
	CPreOrbitParam*              m_pPreOrbitParam;

	IFlightPathAngle*            m_FPA;                              //航迹角属性

	void SetSphElem(CSphElem cSE);

};
