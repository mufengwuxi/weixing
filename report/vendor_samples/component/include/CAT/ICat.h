#pragma once
#include <vector>
#include <string>
#include <map>
#include "../IAtkObj.h"
#include "../ICEnum.h"
struct CObjData;
class CObjManage;
class IAtkObjectRoot;
class IScenario;
struct CatSpaceStruct;
class ICATAdvance;
class CCatSpaceObj;
class CCatSimulation;
class CCatConjunction;

typedef struct AsTCatRes
{
	std::string		catName{};			///< 名字，ssc
	std::string		catTime{};			///< 最近距离时间
	double			catRange{0.0};			///< 最近距离, km
	double			catVel{0.0};				///< 最近距离对应相对速度, km/s
} AsTCatRes;

///
/// 接近分析
///
class IATKOBJECT_API ICat
{
public:
    ICat(const ICat&) = delete;
    ICat& operator=(const ICat&) = delete;

	ICat(IAtkObjectRoot* pRoot, CObjManage* IAsGObjManage);
	~ICat();

	//********************************************************************
	/// 根据卫星与TLE数据库对象计算
	/// @Author	Dong Min
	/// @Date	2025-09-20
	/// @Input 
	/// @Param	vecSatelliteName 卫星对象名称，例Satellite1
	/// @Param	strTLEFilePath TLE数据文件路径
	/// @Param	vecExcSSCNum 排除SSC编号
	/// @Return	
	//********************************************************************
	void SetSatelliteFromScenario(std::vector<std::string> vecSatelliteName, std::string strTLEFilePath, std::vector<std::string> vecExcSSCNum = std::vector<std::string>());

	//********************************************************************
	/// 根据导弹/火箭与TLE数据库对象计算
	/// @Author	Dong Min
	/// @Date	2025-09-20
	/// @Input 
	/// @Param	vecObjName 导弹/火箭对象名称，例Missile
	/// @Param	strTLEFilePath TLE数据文件路径
	/// @Param	vecExcSSCNum 排除SSC编号
	/// @Return	
	//********************************************************************
	void SetMissOrLauVehFromScenario(std::vector<std::string> vecObjName, std::string strTLEFilePath, std::vector<std::string> vecExcSSCNum = std::vector<std::string>());

	//********************************************************************
	/// 输入目标国际编号
	/// @Author	Dong Min
	/// @Date	2025-09-20
	/// @Input 
	/// @Param	vecSSCNumber 库文件SSC编号，例00005
	/// @Param	strTLEFilePath TLE数据文件路径
	/// @Param	vecExcSSCNum 排除SSC编号
	/// @Return	
	//********************************************************************
	void SetInputSSCNumObject(std::vector<std::string> vecSSCNumber, std::string strTLEFilePath, std::vector<std::string> vecExcSSCNum = std::vector<std::string>());

	//********************************************************************
	/// 数据库目标遍历计算
	/// @Author	Dong Min
	/// @Date	2025-09-20
	/// @Input 
	/// @Param	strTLEFilePath 库文件路径
	/// @Param	vecExcSSCNum 排除SSC编号
	/// @Return	
	//********************************************************************
	void SetAllTLEObjects(std::string strTLEFilePath, std::vector<std::string> vecExcSSCNum = std::vector<std::string>());

	//********************************************************************
	/// 是否使用场景时间
	/// @Author	Dong Min
	/// @Date	2025-09-20
	/// @Input 
	/// @Param	bUse true-使用场景区间
	/// @Return	
	//********************************************************************
	void SetUseScenarioTimePeriod(bool bUse);
	//********************************************************************
	/// 获取是否使用场景时间
	/// @Author	Dong Min
	/// @Date	2025-09-20
	/// @Return	true-使用场景区间
	//********************************************************************
	bool GetUseScenarioTimePeriod();

	//********************************************************************
	/// 设置仿真分析时间
	/// @Author	Dong Min
	/// @Date	2025-09-20
	/// @Input 
	/// @Param	strStartTime 仿真开始历元
	/// @Param	strStopTime 仿真结束历元
	/// @Return	
	//********************************************************************
	void SetTimePeriod(std::string strStartTime, std::string strStopTime);

	//********************************************************************
	/// 获取仿真分析时间
	/// @Author	Gan Shu Long
	/// @Date	2026-09-22
	/// @Output
	/// @Return	开始分析时间		
	//********************************************************************
	std::string GetStartTime();

	//********************************************************************
	/// 获取仿真分析时间
	/// @Author	Gan Shu Long
	/// @Date	2026-09-22
	/// @Output
	/// @Return	结束分析时间		
	//********************************************************************
	std::string GetStopTime();

	//********************************************************************
	/// 设置相对距离门限
	/// @Author	Dong Min
	/// @Date	2025-09-20
	/// @Input 
	/// @Param	dValue 相对距离门限
	/// @Return	
	//********************************************************************
	void SetMaxRange(double dValue);
	//********************************************************************
	/// 获取相对距离门限
	/// @Author	Dong Min
	/// @Date	2025-09-20
	/// @Return	相对距离门限
	//********************************************************************
	double GetMaxRange();

	//********************************************************************
	/// 计算
	/// @Author	Dong Min
	/// @Date	2025-09-20
	/// @Return	
	//********************************************************************
	void Compute();

	//********************************************************************
	/// 获取结果
	/// @Author	Dong Min
	/// @Date	2025-09-20
	/// @Output 
	/// @Param	PrimarySSC 主目标
	/// @Param	SecondarySSC 次目标
	/// @Param	EquivDist 等效距离 // m_MissDist/10 和 fabs(m_MissDistR)中较大者
	/// @Param	MissDistance 相对距离
	/// @Param	MissDistR 径向距离
	/// @Param	MissDistT 横向距离
	/// @Param	MissDistN 法向距离
	/// @Param	AngleCA 接近夹角
	/// @Param	RelativeVelocity 相对速度
	/// @Param	TCA 接近时刻
	/// @Param	Tin 开始时刻
	/// @Param	Tout 结束时刻
	/// @Return	
	//********************************************************************
	void GetResults(std::vector<std::string>& PrimarySSC,
		std::vector<std::string>& SecondarySSC,
		std::vector<double>& EquivDist,
		std::vector<double>& MissDistance,
		std::vector<double>& MissDistR,
		std::vector<double>& MissDistT,
		std::vector<double>& MissDistN,
		std::vector<double>& AngleCA,
		std::vector<double>& RelativeVelocity,
		std::vector<std::string>& TCA,
		std::vector<std::string>& Tin,
		std::vector<std::string>& Tout);

	//********************************************************************
	/// 高级配置指针
	/// @Author	Dong Min
	/// @Date	2025-09-20
	/// @Return	
	//********************************************************************
	ICATAdvance* GetAdvance();


	//********************************************************************
	/// 接近分析计算
	/// @Author	
	/// @Date	
	/// @Input 
	/// @Param	primElem 主目标卫星轨道根数 a, e, i, raan，argper，ma
	/// @Param	secElems 次目标卫星轨道根数 a, e, i, raan，argper，ma
	/// @Param	OrbPropType 轨道预报模型，与卫星对象对应
	/// @Param	startUTC 场景开始时间
	/// @Param	stopUTC 场景结束时间
	/// @Param	t1 分析时段开始时间UTC
	/// @Param	t2 分析时段结束时间UTC
	/// @Param	range 相对距离阈值, km
	/// @Param	vel 对速度阈值, km/s
	/// @Output 
	/// @Param	result 结果
	/// @Return	
	//********************************************************************
	void CatComponent(std::vector<double> primElem,
		std::vector<std::vector<double>> secElems,
		EVePropagatorType OrbPropType,
		std::string startUTC,
		std::string stopUTC,
		std::string t1,
		std::string t2,
		double range,
		double vel,
		std::vector<AsTCatRes>& result);

private:
	int SetObjElement(IScenario* pIScen, EVePropagatorType OrbPropType, std::vector<double> objElem, std::string strObjName, std::vector<CObjData*>& vecFrom);

private:
	IAtkObjectRoot*    m_pRoot;
	CObjManage*        m_IAsGObjManage;
	CCatSimulation*    m_pSimulationToRun;
	ICATAdvance*       m_ICATAdvance;

	int                                  m_Mode;
	bool                                 m_UseScenarioTimePeriod;
	std::vector<std::string>             m_excludeVec;
	std::vector<std::string>             m_primariesSSCs;
	std::map<std::string, CCatSpaceObj*> m_TleObj;
	std::map<std::string, CCatSpaceObj*> m_SatObj;
	std::vector<CCatConjunction>*        m_pCatFound;
};
