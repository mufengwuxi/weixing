#pragma once
#include <vector>
#include <string>
#include <map>
#include "../../ICEnum.h"
#include "../../IAtkObj.h"

class CMdOrbit2Orbit;
class CObjManage;


///
/// 机动分析
///
class IATKOBJECT_API IMnvPODDataBaseDetection
{
public:
    IMnvPODDataBaseDetection(const IMnvPODDataBaseDetection&) = delete;
    IMnvPODDataBaseDetection& operator=(const IMnvPODDataBaseDetection&) = delete;

	IMnvPODDataBaseDetection(CObjManage* IAsGObjManage);
	~IMnvPODDataBaseDetection();

	//********************************************************************
	/// 设置预报模型
	/// @Author	Dong Min
	/// @Date	2025-09-20
	/// @Input 
	/// @Param	eType 预报模型，包括二体，J2，Vinti
	/// @Return	
	//********************************************************************
	void SetOrbitPropagator(EVePropagatorType eType);
	//********************************************************************
	/// 获取预报模型
	/// @Author	Dong Min
	/// @Date	2025-09-20
	/// @Return	预报模型
	//********************************************************************
	EVePropagatorType GetOrbitPropagator();

	//********************************************************************
	/// 设置交叉弧段预报步长
	/// @Author	Dong Min
	/// @Date	2025-09-20
	/// @Input 
	/// @Param	dValue 交叉弧段预报步长
	/// @Return	
	//********************************************************************
	void SetStepOfCrossPropagation(double dValue);
	//********************************************************************
	/// 获取交叉弧段预报步长
	/// @Author	Dong Min
	/// @Date	2025-09-20
	/// @Return	交叉弧段预报步长
	//********************************************************************
	double GetStepOfCrossPropagation();

	//********************************************************************
	/// 设置A目标定轨数据历元
	/// @Author	Dong Min
	/// @Date	2025-09-20
	/// @Input 
	/// @Param	strTime 历元时刻
	/// @Return	
	//********************************************************************
	void SetObjectATime(std::string strTime);
	//********************************************************************
	/// 获取A目标定轨数据历元
	/// @Author	Dong Min
	/// @Date	2025-09-20
	/// @Return	历元时刻
	//********************************************************************
	std::string GetObjectATime();

	//********************************************************************
	/// 设置A目标J2000坐标系
	/// @Author	Dong Min
	/// @Date	2025-09-20
	/// @Input 
	/// @Param	dX x轴
	/// @Param	dY y轴
	/// @Param	dZ z轴
	/// @Param	dVx x轴速度
	/// @Param	dVy y轴速度
	/// @Param	dVz z轴速度
	/// @Return	
	//********************************************************************
	void SetObjectAJ2000Frame(double dX,
		double dY, 
		double dZ, 
		double dVx, 
		double dVy,
		double dVz);
	//********************************************************************
	/// 获取A目标J2000坐标系
	/// @Author	Dong Min
	/// @Date	2025-09-20
	/// @Output 
	/// @Param	dX x轴
	/// @Param	dY y轴
	/// @Param	dZ z轴
	/// @Param	dVx x轴速度
	/// @Param	dVy y轴速度
	/// @Param	dVz z轴速度
	/// @Return	
	//********************************************************************
	void GetObjectAJ2000Frame(double& dX,
		double& dY,
		double& dZ,
		double& dVx,
		double& dVy,
		double& dVz);

	//********************************************************************
	/// 设置A目标LVLH坐标系
	/// @Author	Dong Min
	/// @Date	2025-09-20
	/// @Input 
	/// @Param	dox x轴距离均方差
	/// @Param	doy y轴距离均方差
	/// @Param	doz z轴距离均方差
	/// @Param	dovx x轴速度均方差
	/// @Param	dovy y轴速度均方差
	/// @Param	dovz z轴速度均方差
	/// @Return	
	//********************************************************************
	void SetObjectAVVLHFrame(double dox,
		double doy,
		double doz,
		double dovx,
		double dovy,
		double dovz);
	//********************************************************************
	/// 获取A目标LVLH坐标系
	/// @Author	Dong Min
	/// @Date	2025-09-20
	/// @Output 
	/// @Param	dox x轴距离均方差
	/// @Param	doy y轴距离均方差
	/// @Param	doz z轴距离均方差
	/// @Param	dovx x轴速度均方差
	/// @Param	dovy y轴速度均方差
	/// @Param	dovz z轴速度均方差
	/// @Return	
	//********************************************************************
	void GetObjectAVVLHFrame(double& dox,
		double& doy,
		double& doz,
		double& dovx,
		double& dovy,
		double& dovz);

	//********************************************************************
	/// 设置B目标定轨数据历元
	/// @Author	Dong Min
	/// @Date	2025-09-20
	/// @Input 
	/// @Param	strTime 历元时刻
	/// @Return	
	//********************************************************************
	void SetObjectBTime(std::string strTime);
	//********************************************************************
	/// 获取B目标定轨数据历元
	/// @Author	Dong Min
	/// @Date	2025-09-20
	/// @Return	历元时刻
	//********************************************************************
	std::string GetObjectBTime();

	//********************************************************************
	/// 设置B目标J2000坐标系
	/// @Author	Dong Min
	/// @Date	2025-09-20
	/// @Input 
	/// @Param	dX x轴
	/// @Param	dY y轴
	/// @Param	dZ z轴
	/// @Param	dVx x轴速度
	/// @Param	dVy y轴速度
	/// @Param	dVz z轴速度
	/// @Return	
	//********************************************************************
	void SetObjectBJ2000Frame(double dX,
		double dY,
		double dZ,
		double dVx,
		double dVy,
		double dVz);
	//********************************************************************
	/// 获取B目标J2000坐标系
	/// @Author	Dong Min
	/// @Date	2025-09-20
	/// @Output 
	/// @Param	dX x轴
	/// @Param	dY y轴
	/// @Param	dZ z轴
	/// @Param	dVx x轴速度
	/// @Param	dVy y轴速度
	/// @Param	dVz z轴速度
	/// @Return	
	//********************************************************************
	void GetObjectBJ2000Frame(double& dX,
		double& dY,
		double& dZ,
		double& dVx,
		double& dVy,
		double& dVz);

	//********************************************************************
	/// 设置B目标LVLH坐标系
	/// @Author	Dong Min
	/// @Date	2025-09-20
	/// @Input 
	/// @Param	dox x轴距离均方差
	/// @Param	doy y轴距离均方差
	/// @Param	doz z轴距离均方差
	/// @Param	dovx x轴速度均方差
	/// @Param	dovy y轴速度均方差
	/// @Param	dovz z轴速度均方差
	/// @Return	
	//********************************************************************
	void SetObjectBVVLHFrame(double dox,
		double doy,
		double doz,
		double dovx,
		double dovy,
		double dovz);
	//********************************************************************
	/// 获取B目标LVLH坐标系
	/// @Author	Dong Min
	/// @Date	2025-09-20
	/// @Output 
	/// @Param	dox x轴距离均方差
	/// @Param	doy y轴距离均方差
	/// @Param	doz z轴距离均方差
	/// @Param	dovx x轴速度均方差
	/// @Param	dovy y轴速度均方差
	/// @Param	dovz z轴速度均方差
	/// @Return	
	//********************************************************************
	void GetObjectBVVLHFrame(double& dox,
		double& doy,
		double& doz,
		double& dovx,
		double& dovy,
		double& dovz);

	//********************************************************************
	/// 计算
	/// @Author	Dong Min
	/// @Date	2025-09-20
	/// @Output 
	/// @Param	nMnvRes 机动关联结果：0-同目标未机动，1-同目标机动，2-相异目标
	/// @Param	strDetectDVutc 预估的机动时刻
	/// @Param	dDetectDVx 预估的机动分量Vx
	/// @Param	dDetectDVy 预估的机动分量Vy
	/// @Param	dDetectDVz 预估的机动分量Vz
	/// @Param	dSma 半长轴偏差
	/// @Param	dInc 倾角偏差
	/// @Param	dEcc 离心率偏差
	/// @Param	dRAAN 升交点赤经偏差
	/// @Param	strTime UTC时间
	/// @Param	dMahDist 马氏距离
	/// @Return	
	//********************************************************************
	void Compute(
		int& nMnvRes,
		std::string& strDetectDVutc,
		double& dDetectDVx,
		double& dDetectDVy,
		double& dDetectDVz,
		double& dSma,
		double& dInc,
		double& dEcc,
		double& dRAAN,
		std::vector<std::string>& strTime,
		std::vector<double>& dMahDist);

private:
	CObjManage*        m_IAsGObjManage;
	CMdOrbit2Orbit*    m_CMdOrbit2Orbit;

};
