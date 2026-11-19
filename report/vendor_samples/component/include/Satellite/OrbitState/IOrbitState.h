#pragma once
#include <iostream>
#include "../../IAtkObj.h"
#include "../../IClassInfo.h"
#include "../../ICEnum.h"
class CObjManage;
class CPreOrbitParam;
class CCoord3;
class CSatellite;
class CCoordAxes;
class IOrbitStateCartesian;
class IOrbitStateClassical;
class IOrbitStateSpherical;
class IOrbitStateCoordinateSystem;

#define CCoord	CCoord3

///
/// 预报器坐标设置
///
class IATKOBJECT_API IOrbitState :public IClassInfo
{
public:
    IOrbitState(const IOrbitState&) = delete;
    IOrbitState& operator=(const IOrbitState&) = delete;

	IOrbitState(CSatellite* pSate, bool bIsSubClass, CObjManage* IAsGObjManage, CPreOrbitParam* pPreOrbitParam);
	virtual ~IOrbitState();

	//********************************************************************
	/// 将新的轨道状态设置到计算库
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	pOrbitState    设置好的轨道状态
	/// @Return	
	//********************************************************************
	void Assign(IOrbitState* pOrbitState);

	//********************************************************************
	/// 使用位置速度方式设置一个新的轨道状态
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	eCoordinateSystem    坐标系
	/// @Param	dXPosition           位置X
	/// @Param	dYPosition           位置Y
	/// @Param	dZPosition           位置Z
	/// @Param	dXVelocity           速度X
	/// @Param	dYVelocity           速度Y
	/// @Param	dZVelocity           速度Z
	/// @Return	
	//********************************************************************
	void AssignCartesian(ECoordinateSystem eCoordinateSystem, double dXPosition, double dYPosition, double dZPosition,
		double dXVelocity, double dYVelocity, double dZVelocity);

	//********************************************************************
	/// 使用轨道根数方式设置一个新的轨道状态
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	eCoordinateSystem    坐标系
	/// @Param	dSemiMajorAxis       半长轴
	/// @Param	dEccentricity        偏心率
	/// @Param	dInclination         轨道倾角
	/// @Param	dArgOfPerigee        近拱点角距
	/// @Param	dRAAN                升交点赤经
	/// @Param	dMeanAnomaly         平近点角
	/// @Return	
	//********************************************************************
	void AssignClassical(ECoordinateSystem eCoordinateSystem, double dSemiMajorAxis, double dEccentricity, double dInclination,
		double dArgOfPerigee, double dRAAN, double dMeanAnomaly);

	//********************************************************************
	/// 使用球坐标方式设置一个新的轨道状态
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	eCoordinateSystem    坐标系
	/// @Param	dRightAscension      赤经
	/// @Param	dDeclination         赤纬
	/// @Param	dRadius              半径
	/// @Param	dHorFlightPathAngle  水平航迹角
	/// @Param	dFlightPathAzimuth   方位角
	/// @Param	dVelocity            速度
	/// @Return	
	//********************************************************************
	void AssignSpherical(ECoordinateSystem eCoordinateSystem, double dRightAscension, double dDeclination, double dRadius,
		double dHorFlightPathAngle, double dFlightPathAzimuth, double dVelocity);

	//********************************************************************
	/// 修改坐标类型
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	eConvertType    坐标类型
	/// @Return	坐标属性
	//********************************************************************
	IOrbitState* ConvertTo(EOrbitStateType eConvertType);

	//********************************************************************
	/// 获得坐标类型
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	坐标类型
	//********************************************************************
	EOrbitStateType GetOrbitStateType();

	//********************************************************************
	/// 设置轨道历元
	/// @Author	Dong Min
	/// @Date	2026-01-26
	/// @Input
	/// @Param	strEpoch    轨道历元(UTCG格式)
	/// @Return	
	//********************************************************************
	void SetEpoch(std::string strEpoch);

	//********************************************************************
	/// 获取轨道历元
	/// @Author	Dong Min
	/// @Date	2026-01-26
	/// @Return	轨道历元(UTCG格式)
	//********************************************************************
	std::string GetEpoch();
	
	//********************************************************************
	/// 获得支持的坐标类型
	/// @Author	Gan Shu Long
	/// @Date	2026-04-23
	/// @Return	支持的坐标类型
	//********************************************************************
	std::vector<std::string> GetSupportedCoordinateSystemTypes();

	//********************************************************************
	/// 判断是否支持坐标类型
	/// @Author	Gan Shu Long
	/// @Date	2026-04-23
	/// @Return 是否支持坐标类型
	//********************************************************************
	bool IsCoordinateSystemTypeSupported(ECoordinateSystem eCSType);

	//********************************************************************
	/// 设置坐标类型
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	eCSType 坐标类型
	/// @Return	
	//********************************************************************
	void SetCoordinateSystemType(ECoordinateSystem eCSType);

	//********************************************************************
	/// 获得坐标类型
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	坐标类型
	//********************************************************************
	ECoordinateSystem GetCoordinateSystemType();

	//********************************************************************
	/// 获取 中心天体名称
	/// @Author	Gan Shu Long
	/// @Date	2026-10-22
	/// @Input
	/// @Return	中心天体名称
	//********************************************************************
	std::string	GetCentralBodyName();

	//********************************************************************
	/// 获取 轨道坐标系统
	/// @Author	Gan Shu Long
	/// @Date	2026-10-21
	/// @Input
	/// @Return	轨道坐标系统
	//********************************************************************
	IOrbitStateCoordinateSystem* GetCoordinateSystem() { return m_CoordinateSystem; }

protected:
	IOrbitStateCartesian*     m_pIOrbitStateCartesian;
	IOrbitStateClassical*     m_pIOrbitStateClassical;
	IOrbitStateSpherical*     m_pIOrbitStateSpherical;
	EOrbitStateType           m_eOrbitStateType;

	void GetAxesByName(std::vector<CCoordAxes*> axesList, std::string strAxesName, CCoordAxes* &pAxes);

private:
	CObjManage*                     m_IAsGObjManage;
	CSatellite*                     m_pSate;
	CPreOrbitParam*                 m_pPreOrbitParam;
	IOrbitStateCoordinateSystem*	m_CoordinateSystem;		//轨道坐标系统

	bool SetCoord(EOrbitStateType eToType, double dValue[6]);
	int GetIntValueByCoordinateSystemType(ECoordinateSystem eCSType);
};
