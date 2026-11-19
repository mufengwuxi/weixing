#pragma once
#include "IOrientationAscNode.h"
class CSatellite;

///
/// 轨道根数升交点赤经
///
class IATKOBJECT_API IOrientationAscNodeRAAN : public IOrientationAscNode
{ 
public:
    IOrientationAscNodeRAAN(const IOrientationAscNodeRAAN&) = delete;
    IOrientationAscNodeRAAN& operator=(const IOrientationAscNodeRAAN&) = delete;

	IOrientationAscNodeRAAN(CSatellite* pSate);
	~IOrientationAscNodeRAAN();

	//********************************************************************
	/// 设置升交点赤经
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dRAAN 升交点赤经
	/// @Return	
	//********************************************************************
	void SetValue(double dRAAN);
	//********************************************************************
	/// 获得升交点赤经
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	升交点赤经
	//********************************************************************
	double GetValue();

private:
	CSatellite* m_pSate;

};
