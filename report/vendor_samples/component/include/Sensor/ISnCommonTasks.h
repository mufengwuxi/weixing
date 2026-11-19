#pragma once
#include "../IAtkObj.h"
#include "ISnSimpleConicPattern.h"
#include "ISnRectangularPattern.h"
#include "ISnPtFixed.h"
#include "../ICEnum.h"
class ISensor;
class CSensor;
class CObjManage;

///
/// 常用任务
///
class IATKOBJECT_API ISnCommonTasks
{
public:
    ISnCommonTasks(const ISnCommonTasks&) = delete;
    ISnCommonTasks& operator=(const ISnCommonTasks&) = delete;

	ISnCommonTasks(ISensor* pISensor, CSensor* pCSensor, CObjManage* IAsGObjManage);
	~ISnCommonTasks();

	//********************************************************************
	/// 设置 圆锥传感器，返回指针，改变返回值，源数据也会跟着改变
	/// @Author	Gan Shu Long
	/// @Date	2023-11-27
	/// @Input
	/// @Param	coneAngle 半锥角
	/// @Param	angularResolution 点之间的角分离
	/// @Return	ISnSimpleConicPattern*  圆锥传感器类型指针
	//********************************************************************
	ISnSimpleConicPattern* SetPatternSimpleConic(double coneAngle, double angularResolution);

	//********************************************************************
	/// 设置 矩形传感器，返回指针，改变返回值，源数据也会跟着改变
	/// @Author	Gan Shu Long
	/// @Date	2023-11-27
	/// @Input
	/// @Param	verticalHalfAngle 垂直半推角
	/// @Param	horizontalHalfAngle 水平半推角
	/// @Return	ISnRectangularPattern*  矩形传感器类型指针
	//********************************************************************
	ISnRectangularPattern* SetPatternRectangular(double verticalHalfAngle, double horizontalHalfAngle);

	//********************************************************************
	/// 设置 固定欧拉值角度，返回指针，改变返回值，源数据也会跟着改变
	/// @Author	Gan Shu Long
	/// @Date	2023-11-27
	/// @Input
	/// @Param	sequence  欧拉角序列
	/// @Param	a	欧拉角a
	/// @Param	b	欧拉角b
	/// @Param	c	欧拉角c
	/// @Return	ISnPtFixed* 固定指向指针
	ISnPtFixed* SetPointingFixedEuler(EEulerOrientationSequence sequence, double a, double b, double c);

	//********************************************************************
	/// 设置 固定四元数角度，返回指针，改变返回值，源数据也会跟着改变
	/// @Author	Gan Shu Long
	/// @Date	2023-11-27
	/// @Input
	/// @Param	x  四元数x
	/// @Param	y  四元数y
	/// @Param	z  四元数z
	/// @Param	s  四元数s
	/// @Return	ISnPtFixed* 固定指向指针
	ISnPtFixed* SetPointingFixedQuat(double x, double y, double z, double s);

private:
	ISensor* m_pISensor;
	
	CObjManage* m_IAsGObjManage;
	CSensor* m_pCSensor;
};

