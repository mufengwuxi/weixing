#pragma once
#include "IVeHPOPDragModel.h"
class CSatellite;

///
/// 大气阻力摄动
///
class IATKOBJECT_API IVeHPOPDragModelSpherical : public IVeHPOPDragModel
{
public:
    IVeHPOPDragModelSpherical(const IVeHPOPDragModelSpherical&) = delete;
    IVeHPOPDragModelSpherical& operator=(const IVeHPOPDragModelSpherical&) = delete;

	IVeHPOPDragModelSpherical(CSatellite* pSate);
	~IVeHPOPDragModelSpherical();

	//********************************************************************
	/// 设置阻力面积
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dAMR 阻力面积
	/// @Return	
	//********************************************************************
	void SetArea(double dArea);
	//********************************************************************
	/// 获得阻力面积
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	阻力面积
	//********************************************************************
	double GetArea();

	//********************************************************************
	/// 设置阻力系数
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dCd 阻力系数
	/// @Return	
	//********************************************************************
	void SetCd(double dCd);
	//********************************************************************
	/// 获得阻力系数
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	阻力系数
	//********************************************************************
	double GetCd();

private:
	CSatellite*     m_pSate;

};
