#pragma once
#include "../IAtkObj.h"
struct CWayPt;

///
/// 路径点
///
class IATKOBJECT_API IVeWaypointsElement
{
public:
    IVeWaypointsElement(const IVeWaypointsElement&) = delete;
    IVeWaypointsElement& operator=(const IVeWaypointsElement&) = delete;

	IVeWaypointsElement(CWayPt* pCWayPt);
	~IVeWaypointsElement();

	//********************************************************************
	/// 设置 加速度
	/// @Author	Gan Shu Long
	/// @Date	2023-12-11
	/// @Input 
	/// @Param	double 新值
	/// @Return	
	//********************************************************************
	void SetAcceleration(double acceleration);

	//********************************************************************
	/// 设置 高度
	/// @Author	Gan Shu Long
	/// @Date	2023-12-11
	/// @Input 
	/// @Param	double 新值
	/// @Return	
	//********************************************************************
	void SetAltitude(double altitude);

	//********************************************************************
	/// 设置 纬度
	/// @Author	Gan Shu Long
	/// @Date	2023-12-11
	/// @Input 
	/// @Param	double 新值
	/// @Return	
	//********************************************************************
	void SetLatitude(double latitude);

	//********************************************************************
	/// 设置 经度
	/// @Author	Gan Shu Long
	/// @Date	2023-12-11
	/// @Input 
	/// @Param	double 新值
	/// @Return	
	//********************************************************************
	void SetLongitude(double longitude);

	//********************************************************************
	/// 设置 速度
	/// @Author	Gan Shu Long
	/// @Date	2023-12-11
	/// @Input 
	/// @Param	double 新值
	/// @Return	
	//********************************************************************
	void SetSpeed(double speed);

	//********************************************************************
	/// 设置 时间
	/// @Author	Gan Shu Long
	/// @Date	2023-12-11
	/// @Input 
	/// @Param	double 新值
	/// @Return	
	//********************************************************************
	void SetTime(double time);

	//********************************************************************
	/// 设置 曲率
	/// @Author	Gan Shu Long
	/// @Date	2023-12-11
	/// @Input 
	/// @Param	double 新值
	/// @Return	
	//********************************************************************
	void SetTurnRadius(double turnRadius);

	double GetAcceleration();		//加速度
	double GetAltitude();				//高度
	double GetLatitude();			//纬度
	double GetLongitude();			//经度
	double GetSpeed();				//速度
	double GetTime();					//时间 以秒为单位，表示和想定开始时间的间隔
	double GetTurnRadius();		//转角弧的曲率

private:
	CWayPt*			m_CWayPt;	
};
