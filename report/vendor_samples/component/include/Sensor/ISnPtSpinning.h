#pragma once
#include "../IAtkObj.h"
#include "ISnPointing.h"
#include "../ICEnum.h"
struct CObjData;
class CSensor;

///
/// 指向：旋转
///
class IATKOBJECT_API ISnPtSpinning : public ISnPointing
{
public:
    ISnPtSpinning(const ISnPtSpinning&) = delete;
    ISnPtSpinning& operator=(const ISnPtSpinning&) = delete;

	ISnPtSpinning(CObjData* pCObjData);
	~ISnPtSpinning();

	//********************************************************************
	/// 设置 扫描模式
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Input
	/// @Param	eSnScanMode  扫描模式
	/// @Return	
	//********************************************************************
	void SetScanMode(ESnScanMode eSnScanMode);

	//********************************************************************
	/// 获得 扫描模式
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Return	double 扫描模式
	//********************************************************************
	ESnScanMode GetScanMode();

	//********************************************************************
	/// 设置 开始角
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Input
	/// @Param	dClockAngleStart  开始角
	/// @Return	
	//********************************************************************
	void SetClockAngleStart(double dClockAngleStart);

	//********************************************************************
	/// 获得 开始角
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Return	double 开始角
	//********************************************************************
	double GetClockAngleStart();

	//********************************************************************
	/// 设置 结束角
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Input
	/// @Param	dClockAngleStop  结束角
	/// @Return	
	//********************************************************************
	void SetClockAngleStop(double dClockAngleStop);

	//********************************************************************
	/// 获得 结束角
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Return	double 结束角
	//********************************************************************
	double GetClockAngleStop();

	//********************************************************************
	/// 设置 选择速度
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Input
	/// @Param	dSpinRate  选择速度
	/// @Return	
	//********************************************************************
	void SetSpinRate(double dSpinRate);

	//********************************************************************
	/// 获得 选择速度m_SenPtSpin
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Return	double 选择速度
	//********************************************************************
	double GetSpinRate();

	//********************************************************************
	/// 设置 初始偏差角
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Input
	/// @Param	dOffsetAngle  初始偏差角
	/// @Return	
	//********************************************************************
	void SetOffsetAngle(double dOffsetAngle);

	//********************************************************************
	/// 获得 初始偏差角
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Return	double 初始偏差角
	//********************************************************************
	double GetOffsetAngle();
	
	//********************************************************************
	/// 设置 方位角
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Input
	/// @Param	dSpinAxisAzimuth  方位角
	/// @Return	
	//********************************************************************
	void SetSpinAxisAzimuth(double dSpinAxisAzimuth);

	//********************************************************************
	/// 获得 方位角
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Return	double 方位角
	//********************************************************************
	double GetSpinAxisAzimuth();

	//********************************************************************
	/// 设置 高度角
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Input
	/// @Param	dSpinAxisElevation  高度角
	/// @Return	
	//********************************************************************
	void SetSpinAxisElevation(double dSpinAxisElevation);

	//********************************************************************
	/// 获得 高度角
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Return	double 高度角
	//********************************************************************
	double GetSpinAxisElevation();
	
	//********************************************************************
	/// 设置 圆锥角
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Input
	/// @Param	dSpinAxisConeAngle  圆锥角
	/// @Return	
	//********************************************************************
	void SetSpinAxisConeAngle(double dSpinAxisConeAngle);

	//********************************************************************
	/// 获得 圆锥角
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Return	double 圆锥角
	//********************************************************************
	double GetSpinAxisConeAngle();

private:
	CSensor*				m_pCSensor;
};
