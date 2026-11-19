#pragma once
#include "IFlightPathAngle.h"
class CSatellite;

///
/// 球坐标水平航迹角
///
class IATKOBJECT_API ISphericalFPAHorizontal : public IFlightPathAngle
{
public:
    ISphericalFPAHorizontal(const ISphericalFPAHorizontal&) = delete;
    ISphericalFPAHorizontal& operator=(const ISphericalFPAHorizontal&) = delete;

	ISphericalFPAHorizontal(CSatellite* pSate);
	~ISphericalFPAHorizontal();

	//********************************************************************
	/// 设置水平航迹角
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dFPA 水平航迹角
	/// @Return	
	//********************************************************************
	void SetFPA(double dFPA);
	//********************************************************************
	/// 获得水平航迹角
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	水平航迹角
	//********************************************************************
	double GetFPA();

private:
	CSatellite* m_pSate;

};
