#pragma once
#include "../../IAtkObj.h"
#include "../../ICEnum.h"
class CSatellite;
class IOrientationAscNode;

///
/// 轨道根数方向参数
///
class IATKOBJECT_API IClassicalOrientation
{
public:
    IClassicalOrientation(const IClassicalOrientation&) = delete;
    IClassicalOrientation& operator=(const IClassicalOrientation&) = delete;

	IClassicalOrientation(CSatellite* pSate);
	~IClassicalOrientation();

	//********************************************************************
	/// 获得升交点赤经或升交点经度属性
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	升交点赤经或升交点经度属性
	//********************************************************************
	IOrientationAscNode* GetAscNode();

	//********************************************************************
	/// 设置升交点类型
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	eOAN 升交点类型
	/// @Return	
	//********************************************************************
	void SetAscNodeType(EOrientationAscNode eOAN);
	//********************************************************************
	/// 获得升交点类型
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	升交点类型
	//********************************************************************
	EOrientationAscNode GetAscNodeType();

	//********************************************************************
	/// 设置轨道倾角
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dInclination 轨道倾角
	/// @Return	
	//********************************************************************
	void SetInclination(double dInclination);
	//********************************************************************
	/// 获得轨道倾角
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	轨道倾角
	//********************************************************************
	double GetInclination();

	//********************************************************************
	/// 设置近拱点角距
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dArgOfPerigee 近拱点角距
	/// @Return	
	//********************************************************************
	void SetArgOfPerigee(double dArgOfPerigee);
	//********************************************************************
	/// 获得近拱点角距
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	近拱点角距
	//********************************************************************
	double GetArgOfPerigee();


private:
	CSatellite*              m_pSate;

	IOrientationAscNode*     m_AscNode;        ///<升交点赤经或升交点经度属性
	
};
