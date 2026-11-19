#pragma once
#include <vector>
#include <string>
#include <map>
#include "../../ICEnum.h"
#include "../../IAtkObj.h"

class CMdHistroyTLE;
class CObjManage;


///
/// 机动分析
///
class IATKOBJECT_API IMnvTLEDataBaseDetection
{
public:
    IMnvTLEDataBaseDetection(const IMnvTLEDataBaseDetection&) = delete;
    IMnvTLEDataBaseDetection& operator=(const IMnvTLEDataBaseDetection&) = delete;

	IMnvTLEDataBaseDetection(CObjManage* IAsGObjManage);
	~IMnvTLEDataBaseDetection();

	//********************************************************************
	/// 设置待检测卫星TLE数据
	/// @Author	Dong Min
	/// @Date	2025-09-20
	/// @Input 
	/// @Param	strFilePath 待检测卫星TLE数据文件路径
	/// @Return	
	//********************************************************************
	void SetTLEData(std::string strFilePath);
	//********************************************************************
	/// 获取待检测卫星TLE数据
	/// @Author	Dong Min
	/// @Date	2025-09-20
	/// @Return 待检测卫星TLE数据文件路径
	//********************************************************************
	std::string GetTLEData();

	//********************************************************************
	/// 设置判断方法
	/// @Author	Dong Min
	/// @Date	2025-09-20
	/// @Input 
	/// @Param	eMethodType 判断方法
	/// @Return	
	//********************************************************************
	void SetJudMethodType(EJudMethodType eMethodType);
	//********************************************************************
	/// 获取判断方法
	/// @Author	Dong Min
	/// @Date	2025-09-20
	/// @Return	判断方法
	//********************************************************************
	EJudMethodType GetJudMethodType();

	//********************************************************************
	/// 设置判断方法值
	/// @Author	Dong Min
	/// @Date	2025-09-20
	/// @Input 
	/// @Param	dValue 判断方法值
	///			       EJudMethodType取eConfidenceLevel时，用于判断某历史数据点是否异常的置信概率
	///			       EJudMethodType取eMahalanobisDistar时，利用n-sigma法则进行机动检测的设定值n
	/// @Return	
	//********************************************************************
	void SetJudMethodID(double dValue);
	//********************************************************************
	/// 获取判断方法值
	/// @Author	Dong Min
	/// @Date	2025-09-20
	/// @Return	判断方法值
	//********************************************************************
	double GetJudMethodID();

	//********************************************************************
	/// 设置检测参数
	/// @Author	Dong Min
	/// @Date	2025-09-20
	/// @Input 
	/// @Param	eType 检测参数
	/// @Return	
	//********************************************************************
	void SetOrbitalElem(EOrbitalElemType eType);
	//********************************************************************
	/// 获取检测参数
	/// @Author	Dong Min
	/// @Date	2025-09-20
	/// @Return	检测参数
	//********************************************************************
	EOrbitalElemType GetOrbitalElem();

	//********************************************************************
	/// 设置机动量估计方法
	/// @Author	Dong Min
	/// @Date	2025-09-20
	/// @Input 
	/// @Param	eType 机动量估计方法
	/// @Return	
	//********************************************************************
	void SetEstMethodType(EEstMethodType eType);
	//********************************************************************
	/// 获取机动量估计方法
	/// @Author	Dong Min
	/// @Date	2025-09-20
	/// @Return	机动量估计方法
	//********************************************************************
	EEstMethodType GetEstMethodType();

	//********************************************************************
	/// 设置平滑方法
	/// @Author	Dong Min
	/// @Date	2025-09-20
	/// @Input 
	/// @Param	eType 平滑方法
	/// @Return	
	//********************************************************************
	void SetSmoothMethod(ESmoothMethodType eType);
	//********************************************************************
	/// 获取平滑方法
	/// @Author	Dong Min
	/// @Date	2025-09-20
	/// @Return	平滑方法
	//********************************************************************
	ESmoothMethodType GetSmoothMethod();

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
	/// 计算
	/// @Author	Dong Min
	/// @Date	2025-09-20
	/// @Output 
	/// @Param	strTime 机动时间
	/// @Param	dOutliers 异常值
	/// @Param	dSemiimajorAxisDev 半长轴偏差
	/// @Param	dRadial 径向机动
	/// @Param	dTransverse 迹向机动
	/// @Param	dNormal 法向机动
	/// @Return	
	//********************************************************************
	void Compute(std::vector<std::string>& strTime,
		std::vector<double>& dOutliers,
		std::vector<double>& dSemiimajorAxisDev,
		std::vector<double>& dRadial,
		std::vector<double>& dTransverse,
		std::vector<double>& dNormal);

private:
	CObjManage*                   m_IAsGObjManage;
	CMdHistroyTLE*                m_CMdHistroyTLE;


};
