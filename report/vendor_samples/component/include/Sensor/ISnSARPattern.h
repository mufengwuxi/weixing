#pragma once
#include "ISnPattern.h"
class CSensor;

///
/// 模式类型：SAR
///
class IATKOBJECT_API ISnSARPattern : public ISnPattern
{
public:
    ISnSARPattern(const ISnSARPattern&) = delete;
    ISnSARPattern& operator=(const ISnSARPattern&) = delete;

	ISnSARPattern(CSensor* pCSensor);
	~ISnSARPattern();

	//********************************************************************
	/// 设置 地面仰角最小值
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Input
	/// @Param	dMinElevationAngle  地面仰角最小值
	/// @Return	
	//********************************************************************
	void SetMinElevationAngle(double dMinElevationAngle);

	//********************************************************************
	/// 获得 地面仰角最小值
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Return	double 地面仰角最小值	
	//********************************************************************
	double GetMinElevationAngle();

	//********************************************************************
	/// 设置 地面仰角最大值
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Input
	/// @Param	dMaxElevationAngle  地面仰角最大值
	/// @Return	
	//********************************************************************
	void SetMaxElevationAngle(double dMaxElevationAngle);

	//********************************************************************
	/// 获得 地面仰角最大值
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Return	double 地面仰角最大值	
	//********************************************************************
	double GetMaxElevationAngle();

	//********************************************************************
	/// 设置 前向排除角
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Input
	/// @Param	dForeExclusionAngle  前向排除角
	/// @Return	
	//********************************************************************
	void SetForeExclusionAngle(double dForeExclusionAngle);

	//********************************************************************
	/// 获得 前向排除角
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Return	double 前向排除角	
	//********************************************************************
	double GetForeExclusionAngle();

	//********************************************************************
	/// 设置 后向排除角
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Input
	/// @Param	dAftExclusionAngle  后向排除角
	/// @Return	
	//********************************************************************
	void SetAftExclusionAngle(double dAftExclusionAngle);

	//********************************************************************
	/// 获得 后向排除角
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Return	double 后向排除角	
	//********************************************************************
	double GetAftExclusionAngle();

	//********************************************************************
	/// 设置 是否使用父对象高度
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Input
	/// @Param	bIsTrackParentAltitude  是否使用父对象高度
	/// @Return	
	//********************************************************************
	void SetIsTrackParentAltitude(bool bIsTrackParentAltitude);

	//********************************************************************
	/// 获得 是否使用父对象高度
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Return	double 是否使用父对象高度	
	//********************************************************************
	bool GetIsTrackParentAltitude();

	//********************************************************************
	/// 设置 父对象高度
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Input
	/// @Param	dParentAltitude  父对象高度
	/// @Return	
	//********************************************************************
	void SetParentAltitude(double dParentAltitude);

	//********************************************************************
	/// 获得 父对象高度
	/// @Author	Gan Shu Long
	/// @Date	2026-03-20
	/// @Return	double 父对象高度	
	//********************************************************************
	double GetParentAltitude();

private:
	CSensor*			m_pCSensor;
};

