#pragma once
#include "IOrientationAscNode.h"
class CSatellite;

///
/// 轨道根数LAN
///
class IATKOBJECT_API IOrientationAscNodeLAN : public IOrientationAscNode
{
public:
    IOrientationAscNodeLAN(const IOrientationAscNodeLAN&) = delete;
    IOrientationAscNodeLAN& operator=(const IOrientationAscNodeLAN&) = delete;

	IOrientationAscNodeLAN(CSatellite* pSate);
	~IOrientationAscNodeLAN();

	//********************************************************************
	/// 设置LAN
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dLAN 
	/// @Return	
	//********************************************************************
	void SetValue(double dLAN);
	//********************************************************************
	/// 获得LAN
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	LAN
	//********************************************************************
	double GetValue();

private:
	CSatellite* m_pSate;

};

