#pragma once
#include "../IAtkObj.h"
#include "../IClassInfo.h"
#include "../ICEnum.h"
struct CObjData;
class CQuaternion;

///
/// 指向固定属性
///
class IATKOBJECT_API IOrientation :public IClassInfo
{
public:
    IOrientation(const IOrientation&) = delete;
    IOrientation& operator=(const IOrientation&) = delete;

	IOrientation(CObjData* pCObjData);
	virtual ~IOrientation();

	//********************************************************************
	/// 按方位高度角设置值
	/// @Author	Gan Shu Long
	/// @Date	2026-03-17
	/// @Input
	/// @Param	dAzimuth		方位
	/// @Param	dElevation	高度角
	/// @Param	eAzElAboutBoresight	视轴类型
	/// @Return	
	//********************************************************************
	void AssignAzEl(double dAzimuth, double dElevation, EAzElAboutBoresight eAzElAboutBoresight = eAzElAboutBoresightUnknown);

	//********************************************************************
	/// 按欧拉角设置值
	/// @Author	Gan Shu Long
	/// @Date	2026-03-17
	/// @Input
	/// @Param	eEulerOrientationSequence  欧拉角序列
	/// @Param	a	欧拉角a
	/// @Param	b	欧拉角b
	/// @Param	c	欧拉角c
	/// @Return	
	//********************************************************************
	void AssignEulerAngles(EEulerOrientationSequence eEulerOrientationSequence, double a, double b, double c);

	//********************************************************************
	/// 按四元数设置值
	/// @Author	Gan Shu Long
	/// @Date	2026-03-17
	/// @Input
	/// @Param	x  四元数x
	/// @Param	y  四元数y
	/// @Param	z  四元数z
	/// @Param	s  四元数s
	/// @Return	
	//********************************************************************
	void AssignQuaternion(double x, double y, double z, double s);

	//********************************************************************
	/// 按 YPR设置值
	/// @Author	Gan Shu Long
	/// @Date	2026-03-17
	/// @Input
	/// @Param	eYPRAnglesSequence  序列
	/// @Param	dYaw
	/// @Param	dPitch
	/// @Param	dRoll
	/// @Return	
	//********************************************************************
	void AssignYPRAngles(EYPRAnglesSequence eYPRAnglesSequence, double dYaw, double dPitch, double dRoll);
	
	//********************************************************************
	/// 按 矩阵设置值
	/// @Author	Gan Shu Long
	/// @Date	2026-03-23
	/// @Input
	/// @Param	d11,d12,d13
	/// @Param	d21,d22,d23
	/// @Param	d31,d32,d33
	/// @Return	
	//********************************************************************
	void AssignMatrix3d(
		double d11, double d12, double d13,
		double d21, double d22, double d23,
		double d31, double d32, double d33);

	//********************************************************************
	/// 获得 方位高度角设置值
	/// @Author	Gan Shu Long
	/// @Date	2026-03-18
	/// @Input
	/// @Param	dAzimuth		方位
	/// @Param	dElevation	高度角
	/// @Param	eAzElAboutBoresight	视轴类型
	/// @Return	
	//********************************************************************
	void QueryAzEl(double &dAzimuth, double &dElevation, EAzElAboutBoresight &eAzElAboutBoresight);

	//********************************************************************
	/// 获得 按欧拉角设置值
	/// @Author	Gan Shu Long
	/// @Date	2026-03-18
	/// @Input
	/// @Param	eEulerOrientationSequence  欧拉角序列 （根据该值确定获取对应序列的a/b/c值）
	/// @Param	& a	欧拉角a
	/// @Param	& b	欧拉角b
	/// @Param	& c	欧拉角c
	/// @Return	
	//********************************************************************
	void QueryEulerAngles(EEulerOrientationSequence &eEulerOrientationSequence, double &a, double &b, double &c);

	//********************************************************************
	/// 获得 按四元数设置值
	/// @Author	Gan Shu Long
	/// @Date	2026-03-18
	/// @Input
	/// @Param	& x  四元数x
	/// @Param	& y  四元数y
	/// @Param	& z  四元数z
	/// @Param	& s  四元数s
	/// @Return	
	//********************************************************************
	void QueryQuaternion(double &x, double &y, double &z, double &s);

	//********************************************************************
	/// 获得 YPR设置值
	/// @Author	Gan Shu Long
	/// @Date	2026-03-17
	/// @Input
	/// @Param	eYPRAnglesSequence  序列
	/// @Param	dYaw
	/// @Param	dPitch
	/// @Param	dRoll
	/// @Return	
	//********************************************************************
	void QueryYPRAngles(EYPRAnglesSequence &eYPRAnglesSequence, double &dYaw, double &dPitch, double &dRoll);
	
	//********************************************************************
	/// 获得 矩阵设置值
	/// @Author	Gan Shu Long
	/// @Date	2026-03-23
	/// @Input
	/// @Param	d11,d12,d13
	/// @Param	d21,d22,d23
	/// @Param	d31,d32,d33
	/// @Return	
	//********************************************************************
	void QueryMatrix3d(
		double &d11, double &d12, double &d13,
		double &d21, double &d22, double &d23,
		double &d31, double &d32, double &d33);

	//********************************************************************
	/// 获得 固定指向偏置姿态类型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-17
	/// @Return	固定指向偏置姿态类型
	//********************************************************************
	EOrientationType GetOrientationType();

private:
	int EEulerOrientationSequenceToInt(EEulerOrientationSequence eEulerOrientationSequence);
	int EYPRAnglesSequenceToInt(EYPRAnglesSequence eYPRAnglesSequence);

private:
	CObjData*								m_pObjData; 
};

