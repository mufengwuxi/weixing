#pragma once
#include "../IAtkObj.h"
#include "../ICEnum.h"
#include <vector>
#include <string>

class CObjManage;
struct CObjData;
struct AsTAdvCatObj;
class CAdvCat;
class ISatellite;
class IAtkObject;

///
/// 高级接近分析主配置
///
class IATKOBJECT_API IAdvCATMain
{
public:
    IAdvCATMain(const IAdvCATMain&) = delete;
    IAdvCATMain& operator=(const IAdvCATMain&) = delete;

	IAdvCATMain(CObjData* pObjData, CObjManage* IAsGObjManage);
	~IAdvCATMain();

	//********************************************************************
	/// 设置主目标序列
	/// @Author	Dong Min
	/// @Date	2025-04-28
	/// @Input
	/// @Param	pPrimSate  主目标对象
	/// @Return	
	//********************************************************************
	void SetPrimaryList(ISatellite* pPrimSate);

	//********************************************************************
	/// 获取主目标序列
	/// @Author	Dong Min
	/// @Date	2025-04-28
	/// @Return	主目标列表
	//********************************************************************
	std::vector<ISatellite*> GetPrimaryList();

	//********************************************************************
	/// 从主目标列表移除指定目标对象
	/// @Author	Dong Min
	/// @Date	2025-04-28
	/// @Input
	/// @Param	pPrimSate  主目标对象
	/// @Return	
	//********************************************************************
	void RemovePrimary(ISatellite* pPrimSate);

	//********************************************************************
	/// 清空主目标列表
	/// @Author	Dong Min
	/// @Date	2025-04-28
	/// @Return	
	//********************************************************************
	void RemoveAllPrimary();

	//********************************************************************
	/// 设置椭球三轴切向
	/// @Author	Dong Min
	/// @Date	2025-04-28
	/// @Input
	/// @Param	pPrimSate  主目标对象
	/// @Param	tangential  椭球三轴切向，单位m
	/// @Return	
	//********************************************************************
	void SetPriTangential(ISatellite* pPrimSate, double dTangential);

	//********************************************************************
	/// 获取椭球三轴切向
	/// @Author	Dong Min
	/// @Date	2025-04-28
	/// @Input
	/// @Param	pPrimSate  主目标对象
	/// @Return	椭球三轴切向，单位m
	//********************************************************************
	double GetPriTangential(ISatellite* pPrimSate);

	//********************************************************************
	/// 设置椭球三轴横向
	/// @Author	Dong Min
	/// @Date	2025-04-28
	/// @Input
	/// @Param	pPrimSate  主目标对象
	/// @Param	crossTrack  椭球三轴横向，单位m
	/// @Return	
	//********************************************************************
	void SetPriCrossTrack(ISatellite* pPrimSate, double dCrossTrack);

	//********************************************************************
	/// 获取椭球三轴横向
	/// @Author	Dong Min
	/// @Date	2025-04-28
	/// @Input
	/// @Param	pPrimSate  主目标对象
	/// @Return	椭球三轴横向，单位m
	//********************************************************************
	double GetPriCrossTrack(ISatellite* pPrimSate);

	//********************************************************************
	/// 设置椭球三轴法向
	/// @Author	Dong Min
	/// @Date	2025-04-28
	/// @Input
	/// @Param	pPrimSate  主目标对象
	/// @Param	normal  椭球三轴法向，单位m
	/// @Return	
	//********************************************************************
	void SetPriNormal(ISatellite* pPrimSate, double dNormal);

	//********************************************************************
	/// 获取椭球三轴法向
	/// @Author	Dong Min
	/// @Date	2025-04-28
	/// @Input
	/// @Param	pPrimSate  主目标对象
	/// @Return	椭球三轴法向，单位m
	//********************************************************************
	double GetPriNormal(ISatellite* pPrimSate);

	//********************************************************************
	/// 设置包络球半径
	/// @Author	Dong Min
	/// @Date	2025-04-28
	/// @Input
	/// @Param	pPrimSate  主目标对象
	/// @Param	radius  包络球半径，单位m
	/// @Return	
	//********************************************************************
	void SetPriRadius(ISatellite* pPrimSate, double dRadius);

	//********************************************************************
	/// 获取包络球半径
	/// @Author	Dong Min
	/// @Date	2025-04-28
	/// @Input
	/// @Param	pPrimSate  主目标对象
	/// @Return	包络球半径
	//********************************************************************
	double GetPriRadius(ISatellite* pPrimSate);

	//********************************************************************
	/// 设置椭球类别
	/// @Author	Dong Min
	/// @Date	2025-04-28
	/// @Input
	/// @Param	pPrimSate  主目标对象
	/// @Param	classType  椭球类别，单位m
	/// @Return	
	//********************************************************************
	void SetPriClass(ISatellite* pPrimSate, EAdvCatEllipType eClassType);

	//********************************************************************
	/// 获取椭球类别
	/// @Author	Dong Min
	/// @Date	2025-04-28
	/// @Input
	/// @Param	pPrimSate  主目标对象
	/// @Return	椭球类别
	//********************************************************************
	EAdvCatEllipType GetPriClass(ISatellite* pPrimSate);

	//********************************************************************
	/// 设置次目标序列
	/// @Author	Dong Min
	/// @Date	2025-04-28
	/// @Input
	/// @Param	pScdSate  次目标对象
	/// @Return	
	//********************************************************************
	void SetSecondList(ISatellite* pScdSate);

	//********************************************************************
	/// 获取次目标序列
	/// @Author	Dong Min
	/// @Date	2025-04-28
	/// @Return	次目标列表
	//********************************************************************
	std::vector<ISatellite*> GetSecondList();

	//********************************************************************
	/// 从次目标列表移除指定目标对象
	/// @Author	Dong Min
	/// @Date	2025-04-28
	/// @Input
	/// @Param	pScdSate  次目标对象
	/// @Return	
	//********************************************************************
	void RemoveSecond(ISatellite* pScdSate);

	//********************************************************************
	/// 清空次目标列表
	/// @Author	Dong Min
	/// @Date	2025-04-28
	/// @Return	
	//********************************************************************
	void RemoveAllSecond();

	//********************************************************************
	/// 设置椭球三轴切向
	/// @Author	Dong Min
	/// @Date	2025-04-28
	/// @Input
	/// @Param	pScdSate  次目标对象
	/// @Param	tangential  椭球三轴切向，单位m
	/// @Return	
	//********************************************************************
	void SetScdTangential(ISatellite* pScdSate, double dTangential);

	//********************************************************************
	/// 获取椭球三轴切向
	/// @Author	Dong Min
	/// @Date	2025-04-28
	/// @Input
	/// @Param	pScdSate  次目标对象
	/// @Return	椭球三轴切向，单位m
	//********************************************************************
	double GetScdTangential(ISatellite* pScdSate);

	//********************************************************************
	/// 设置椭球三轴横向
	/// @Author	Dong Min
	/// @Date	2025-04-28
	/// @Input
	/// @Param	pScdSate  次目标对象
	/// @Param	crossTrack  椭球三轴横向，单位m
	/// @Return	
	//********************************************************************
	void SetScdCrossTrack(ISatellite* pScdSate, double dCrossTrack);

	//********************************************************************
	/// 获取椭球三轴横向
	/// @Author	Dong Min
	/// @Date	2025-04-28
	/// @Input
	/// @Param	pScdSate  次目标对象
	/// @Return	椭球三轴横向，单位m
	//********************************************************************
	double GetScdCrossTrack(ISatellite* pScdSate);

	//********************************************************************
	/// 设置椭球三轴法向
	/// @Author	Dong Min
	/// @Date	2025-04-28
	/// @Input
	/// @Param	pScdSate  次目标对象
	/// @Param	normal  椭球三轴法向，单位m
	/// @Return	
	//********************************************************************
	void SetScdNormal(ISatellite* pScdSate, double dNormal);

	//********************************************************************
	/// 获取椭球三轴法向
	/// @Author	Dong Min
	/// @Date	2025-04-28
	/// @Input
	/// @Param	pScdSate  次目标对象
	/// @Return	椭球三轴法向，单位m
	//********************************************************************
	double GetScdNormal(ISatellite* pScdSate);

	//********************************************************************
	/// 设置包络球半径
	/// @Author	Dong Min
	/// @Date	2025-04-28
	/// @Input
	/// @Param	pScdSate  次目标对象
	/// @Param	radius  包络球半径，单位m
	/// @Return	
	//********************************************************************
	void SetScdRadius(ISatellite* pScdSate, double dRadius);

	//********************************************************************
	/// 获取包络球半径
	/// @Author	Dong Min
	/// @Date	2025-04-28
	/// @Input
	/// @Param	pScdSate  次目标对象
	/// @Return	包络球半径
	//********************************************************************
	double GetScdRadius(ISatellite* pScdSate);

	//********************************************************************
	/// 设置椭球类别
	/// @Author	Dong Min
	/// @Date	2025-04-28
	/// @Input
	/// @Param	pScdSate  次目标对象
	/// @Param	classType  椭球类别，单位m
	/// @Return	
	//********************************************************************
	void SetScdClass(ISatellite* pScdSate, EAdvCatEllipType eClassType);

	//********************************************************************
	/// 获取椭球类别
	/// @Author	Dong Min
	/// @Date	2025-04-28
	/// @Input
	/// @Param	pScdSate  次目标对象
	/// @Return	椭球类别
	//********************************************************************
	EAdvCatEllipType GetScdClass(ISatellite* pScdSate);

	//********************************************************************
	/// 设置是否使用场景时间
	/// @Author	Dong Min
	/// @Date	2025-04-28
	/// @Input
	/// @Param	bUseSceInterval  是否使用场景时间，true-使用场景分析时间
	/// @Return		
	//********************************************************************
	void SetUseSceInterval(bool bUseSceInterval);

	//********************************************************************
	/// 获取是否使用场景时间
	/// @Author	Dong Min
	/// @Date	2025-04-28
	/// @Return	是否使用场景时间，true-使用场景分析时间
	//********************************************************************
	bool GetUseSceInterval();

	//********************************************************************
	/// 设置分析时间段
	/// @Author	Dong Min
	/// @Date	2025-04-28
	/// @Input
	/// @Param	strStartTime  开始分析时间
	/// @Param	strEndTime 结束分析时间
	/// @Return	姿态属性指针		
	//********************************************************************
	void SetTimePeriod(std::string strStartTime, std::string strEndTime);

	//********************************************************************
	/// 获取分析时间段
	/// @Author	Gan Shu Long
	/// @Date	2026-09-22
	/// @Output
	/// @Return	开始分析时间		
	//********************************************************************
	std::string GetStartTime();

	//********************************************************************
	/// 获取分析时间段
	/// @Author	Gan Shu Long
	/// @Date	2026-09-22
	/// @Output
	/// @Return	结束分析时间		
	//********************************************************************
	std::string GetStopTime();

	//********************************************************************
	/// 设置阈值
	/// @Author	Dong Min
	/// @Date	2025-04-28
	/// @Input
	/// @Param	threshold  距离阈值，单位m
	/// @Return			
	//********************************************************************
	void SetThreshold(double dThreshold);

	//********************************************************************
	/// 获取阈值
	/// @Author	Dong Min
	/// @Date	2025-04-28
	/// @Return  距离阈值，单位m
	//********************************************************************
	double GetThreshold();

	//********************************************************************
	/// 设置是否使用距离测量
	/// @Author	Dong Min
	/// @Date	2025-04-28
	/// @Input
	/// @Param	bUseRangeMeasure  true = 使用点距离；false = 使用椭距
	/// @Return			
	//********************************************************************
	void SetUseRangeMeasure(bool bUseRangeMeasure);

	//********************************************************************
	/// 获取是否使用距离测量
	/// @Author	Dong Min
	/// @Date	2025-04-28
	/// @Return  true = 使用点距离；false = 使用椭距
	//********************************************************************
	bool GetUseRangeMeasure();

	//********************************************************************
	/// 计算
	/// @Author	Dong Min
	/// @Date	2025-04-28
	/// @Return  
	//********************************************************************
	void Compute();

	//********************************************************************
	/// 获取结果数据大小
	/// @Author	Dong Min
	/// @Date	2025-05-09
	/// @Input
	/// @Param	primID  
	/// @Param	secID  
	/// @Param	tca  
	/// @Param	caMinRange  
	/// @Param	caMinVel  
	/// @Return  
	//********************************************************************
	void GetDetail(std::vector<std::string>& primID,
		std::vector<std::string>& secID,
		std::vector<std::string>& tca,
		std::vector<double>& caMinRange,
		std::vector<double>& caMinVel);

private:
	CObjManage* m_IAsGObjManage;
	std::vector<ISatellite*> vecPriISate;
	std::vector<ISatellite*> vecSecISate;

	CAdvCat* m_pAdvCAT;

};

