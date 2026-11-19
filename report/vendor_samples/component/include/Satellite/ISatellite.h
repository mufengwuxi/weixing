#pragma once
#include <string>
#include <vector>
#include "../IAtkObject.h"
class CObjManage;
struct CObjData;
class CSatellite;
class IVePassBreak;
class IVeAttitude;
class IVePropagator;
class IVeMassProperties;
class ISaVO;
class ISaGraphics;
class IManeuverDetection;
class IVeGroundEllipsesCollection;

///
/// 卫星
///
class IATKOBJECT_API ISatellite :public IAtkObject
{
public:
    ISatellite(const ISatellite&) = delete;
    ISatellite& operator=(const ISatellite&) = delete;

	ISatellite(CObjData* pObjData, CObjManage* IAsGObjManage, IAtkObjectRoot* Root);
	~ISatellite();

	//********************************************************************
	/// 判断是否支持此姿态模式
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	eAttitudeType  姿态模式
	/// @Return	true-支持此姿态模式
	//********************************************************************
	bool IsAttitudeTypeSupported(EVeAttitude eAttitudeType);

	//********************************************************************
	/// 判断是否支持此预报器类型
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	ePropagator  轨道预报器类型
	/// @Return	true-支持此预报器类型
	//********************************************************************
	bool IsPropagatorTypeSupported(EVePropagatorType ePropagator);

	//********************************************************************
	/// 设置姿态模式
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	eAttitudeType  姿态类型
	/// @Return	
	//********************************************************************
	void SetAttitudeType(EVeAttitude eAttitudeType);

	//********************************************************************
	/// 设置预报器类型
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	ePropagator  轨道预报器类型
	/// @Return	
	//********************************************************************
	void SetPropagatorType(EVePropagatorType ePropagator);

	//********************************************************************
	/// 获得姿态属性
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	姿态属性指针		
	//********************************************************************
	IVeAttitude* GetAttitude();

	//********************************************************************
	/// 获得支持的姿态模式
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	支持的姿态类型			
	//********************************************************************
	std::vector<std::string> GetAttitudeSupportedTypes();

	//********************************************************************
	/// 获得姿态模式
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	姿态类型		
	//********************************************************************
	EVeAttitude GetAttitudeType();

	//********************************************************************
	/// 获得轨道圈数
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	轨道圈数		
	//********************************************************************
	IVePassBreak* GetPassBreak();

	//********************************************************************
	/// 获得轨道预报器属性
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	轨道预报器属性指针		
	//********************************************************************
	IVePropagator* GetPropagator();

	//********************************************************************
	/// 获得支持的轨道预报器类型
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	支持的轨道预报器类型		
	//********************************************************************
	std::vector<std::string> GetPropagatorSupportedTypes();

	//********************************************************************
	/// 获得轨道预报器类型
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	轨道预报器类型		
	//********************************************************************
	EVePropagatorType GetPropagatorType();

	//********************************************************************
	/// 获得质量属性
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	质量属性指针		
	//********************************************************************
	IVeMassProperties* GetMassProperties();

	//********************************************************************
	/// 获得三维属性
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	三维属性指针	
	//********************************************************************
	ISaVO* GetVO();

	//********************************************************************
	/// 获得二维属性
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	二维属性指针	
	//********************************************************************
	ISaGraphics* GetGraphics();

	//********************************************************************
	/// 获得机动分析工具
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	机动分析工具指针	
	//********************************************************************
	IManeuverDetection* GetManeuverDetection();

	//********************************************************************
	/// 获得地面椭圆设置集合
	/// @Author	Gan Shu Long
	/// @Date	2026-10-19
	/// @Return	地面椭圆设置集合指针
	//********************************************************************
	IVeGroundEllipsesCollection* GetGroundEllipses();

private:
	IVeAttitude*                     m_Attitude;                   //姿态
	ISaGraphics*                     m_Graphics;                   //二维
	IVeMassProperties*               m_MassProperties;             //质量属性
	IVePassBreak*                    m_PassBreak;                  //轨道圈数
	IVePropagator*                   m_Propagator;                 //轨道预报器
	ISaVO*                           m_VO;                         //三维属性
	IManeuverDetection*              m_ManeuverDetection;          //机动分析工具
	IVeGroundEllipsesCollection*	 m_GroundEllipses;			   //地面椭圆设置集合

private:
	CSatellite* m_pSate;

	//I对象类型转化C类型
	std::string GetCObjTypeByI(EATKObjectType eType);

};
