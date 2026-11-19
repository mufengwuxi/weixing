#pragma once
#include <string>
#include "../IAtkObject.h"

class CObjManage;
class ISatellite;
class IAtkObjectRoot;
struct CObjData;
struct Conste_SatInfo;
struct Conste_SeedSat;
struct CTleElem;

///
/// 星座设计
///
class IATKOBJECT_API IConstellDesign
{
public:
    IConstellDesign(const IConstellDesign&) = delete;
    IConstellDesign& operator=(const IConstellDesign&) = delete;

	IConstellDesign(IAtkObjectRoot* pRoot, CObjManage* IAsGObjManage);
	~IConstellDesign();

	//********************************************************************
	/// 星座设计 WalkerDelta
	/// @Author	Gan Shu Long
	/// @Date	2025-7-1
	/// @Input
	/// @Param	sMajAx  半长轴
	/// @Param	ecc  偏心率
	/// @Param	inc  轨道倾角
	/// @Param	raan  升交点赤经
	/// @Param	argPeri  近拱点角
	/// @Param	trueAnomaly  真近点角
	/// @Param	numPlanes  轨道面个数
	/// @Param	numSatsPerPlane  面卫星数
	/// @Param	interPlaneSpacing  相位因子
	/// @Param	raanSpread  RAAN分布范围
	/// @Return	
	//********************************************************************
	std::vector<ISatellite*> ConstellDesignWalkerDelta(double sMajAx, double ecc, double inc, double raan, double argPeri, double trueAnomaly, int numPlanes, int numSatsPerPlane, int interPlaneSpacing, double raanSpread);

	//********************************************************************
	/// 星座设计 WalkerDelta(种子)
	/// @Author	Gan Shu Long
	/// @Date	2025-7-1
	/// @Input
	/// @Param	seedSatPath  种子卫星路径
	/// @Param	numPlanes  轨道面个数
	/// @Param	numSatsPerPlane  面卫星数
	/// @Param	interPlaneSpacing  相位因子
	/// @Param	raanSpread  RAAN分布范围
	/// @Return	
	//********************************************************************
	std::vector<ISatellite*> ConstellDesignWalkerDeltaBySeed(std::string seedSatPath, int numPlanes, int numSatsPerPlane, int interPlaneSpacing, double raanSpread);

	//********************************************************************
	/// 星座设计 Walker-自定义
	/// @Author	Gan Shu Long
	/// @Date	2025-7-1
	/// @Input
	/// @Param	sMajAx  半长轴
	/// @Param	ecc  偏心率
	/// @Param	inc  轨道倾角
	/// @Param	raan  升交点赤经
	/// @Param	argPeri  近拱点角
	/// @Param	trueAnomaly  真近点角
	/// @Param	numPlanes  轨道面个数
	/// @Param	numSatsPerPlane  面卫星数
	/// @Param	trueAnomalyPhasing  相邻平面真近点角增量
	/// @Param	raanIncrement  相邻平面RAAN差值
	/// @Return	
	//********************************************************************
	std::vector<ISatellite*> ConstellDesignWalkerCustom(double sMajAx, double ecc, double inc, double raan, double argPeri, double trueAnomaly, int numPlanes, int numSatsPerPlane, double trueAnomalyPhasing, double raanIncrement);

	//********************************************************************
	/// 星座设计 Walker-自定义(种子)
	/// @Author	Gan Shu Long
	/// @Date	2025-7-1
	/// @Input
	/// @Param	seedSatPath  种子卫星路径
	/// @Param	numPlanes  轨道面个数
	/// @Param	numSatsPerPlane  面卫星数
	/// @Param	trueAnomalyPhasing  相邻平面真近点角增量
	/// @Param	raanIncrement  相邻平面RAAN差值
	/// @Return	
	//********************************************************************
	std::vector<ISatellite*> ConstellDesignWalkerCustomBySeed(std::string seedSatPath, int numPlanes, int numSatsPerPlane, double trueAnomalyPhasing, double raanIncrement);

	//********************************************************************
	/// 星座设计 玫瑰星座
	/// @Author	Gan Shu Long
	/// @Date	2025-7-1
	/// @Input
	/// @Param	sMajAx  半长轴
	/// @Param	ecc  偏心率
	/// @Param	inc  轨道倾角
	/// @Param	raan  升交点赤经
	/// @Param	argPeri  近拱点角
	/// @Param	trueAnomaly  真近点角
	/// @Param	numPlanes  轨道面个数
	/// @Param	numSatsPerPlane  面卫星数
	/// @Param	molecule  协因子分子
	/// @Return	
	//********************************************************************
	std::vector<ISatellite*> ConstellDesignRosette(double sMajAx, double ecc, double inc, double raan, double argPeri, double trueAnomaly, int numPlanes, int numSatsPerPlane, int molecule);

	//********************************************************************
	/// 星座设计 玫瑰星座(种子)
	/// @Author	Gan Shu Long
	/// @Date	2025-7-1
	/// @Input
	/// @Param	seedSatPath  种子卫星路径
	/// @Param	numPlanes  轨道面个数
	/// @Param	numSatsPerPlane  面卫星数
	/// @Param	molecule  协因子分子
	/// @Return	
	//********************************************************************
	std::vector<ISatellite*> ConstellDesignRosetteBySeed(std::string seedSatPath, int numPlanes, int numSatsPerPlane, int molecule);

	//********************************************************************
	/// 星座设计 花星座
	/// @Author	Gan Shu Long
	/// @Date	2025-7-1
	/// @Input
	/// @Param	sMajAx  半长轴
	/// @Param	ecc  偏心率
	/// @Param	inc  轨道倾角
	/// @Param	raan  升交点赤经
	/// @Param	argPeri  近拱点角
	/// @Param	trueAnomaly  真近点角
	/// @Param	returnCircle  回归圈数
	/// @Param	returnDay  回归天数
	/// @Param	phaseDensity  相位密度
	/// @Param	interPlaneSpacing  相邻平面相位因子
	/// @Param	numSats  总卫星数
	/// @Param	raanSpread  分布范围
	/// @Return	
	//********************************************************************
	std::vector<ISatellite*> ConstellDesignFlower(double sMajAx, double ecc, double inc, double raan, double argPeri, double trueAnomaly, int returnCircle, int returnDay, int phaseDensity, int interPlaneSpacing, int numSats, double raanSpread);

	//********************************************************************
	/// 星座设计 花星座(种子)
	/// @Author	Gan Shu Long
	/// @Date	2025-7-1
	/// @Input
	/// @Param	seedSatPath  种子卫星路径
	/// @Param	returnCircle  回归圈数
	/// @Param	returnDay  回归天数
	/// @Param	phaseDensity  相位密度
	/// @Param	interPlaneSpacing  相邻平面相位因子
	/// @Param	numSats  总卫星数
	/// @Param	raanSpread  分布范围
	/// @Return	
	//********************************************************************
	std::vector<ISatellite*> ConstellDesignFlowerBySeed(std::string seedSatPath, int returnCircle, int returnDay, int phaseDensity, int interPlaneSpacing, int numSats, double raanSpread);

	//********************************************************************
	/// 星座设计 非对称花星座
	/// @Author	Gan Shu Long
	/// @Date	2025-7-1
	/// @Input
	/// @Param	sMajAx  半长轴
	/// @Param	ecc  偏心率
	/// @Param	inc  轨道倾角
	/// @Param	raan  升交点赤经
	/// @Param	argPeri  近拱点角
	/// @Param	trueAnomaly  真近点角
	/// @Param	returnCircle  回归圈数
	/// @Param	returnDay  回归天数
	/// @Param	numSats  总卫星数
	/// @Param	raanIncrement  升交点赤经差
	/// @Return	
	//********************************************************************
	std::vector<ISatellite*> ConstellDesignNonSymFlower(double sMajAx, double ecc, double inc, double raan, double argPeri, double trueAnomaly, int returnCircle, int returnDay, int numSats, double raanIncrement);

	//********************************************************************
	/// 星座设计 非对称花星座(种子)
	/// @Author	Gan Shu Long
	/// @Date	2025-7-1
	/// @Input
	/// @Param	seedSatPath  种子卫星路径
	/// @Param	returnCircle  回归圈数
	/// @Param	returnDay  回归天数
	/// @Param	numSats  总卫星数
	/// @Param	raanIncrement  升交点赤经差
	/// @Return	
	//********************************************************************
	std::vector<ISatellite*> ConstellDesignNonSymFlowerBySeed(std::string seedSatPath, int returnCircle, int returnDay, int numSats, double raanIncrement);

private:
	bool GetSeedSatInfo(std::string seedSatPath, Conste_SeedSat& conste, std::string& parentObjPath, std::string&  strRefObjName, CObjData *pSeedData);
	void SetConsteSeedSatOrb(int seedSatType, double sMajAx, double ecc, double inc, double raan, double argPeri, double trueAnomaly, Conste_SeedSat& conste);
	void SetConsteSeedSatTLE(int seedSatType, CTleElem tleElem, Conste_SeedSat& conste);
	void CreateCSatellites(CObjData *pSeedData, int nConsType, std::string parentObjPath, std::string strRefObjName, std::vector<Conste_SatInfo> vecResultElem, std::vector<ISatellite*>& objID);

private:
	IAtkObjectRoot*		m_Root;
	CObjManage*			m_IAsGObjManage;
};
