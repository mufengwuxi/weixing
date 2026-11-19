#pragma once
#include "IAtkObject.h"

class CObjManage;
struct CObjData;
class IAnimation;
class IScenario;
class IATKBatchCrdnTransform;
class ICat;
class IConstellDesign;
class IAtkObjectCoverage;

///
/// 根对象
///
class IATKOBJECT_API  IAtkObjectRoot :public IAtkObject
{
public:
    IAtkObjectRoot(const IAtkObjectRoot&) = delete;
    IAtkObjectRoot& operator=(const IAtkObjectRoot&) = delete;

	IAtkObjectRoot();
	IAtkObjectRoot(CObjManage* pIAsGObjManage);
	~IAtkObjectRoot();

	//********************************************************************
	/// 新建场景
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	strSceName  场景名称
	/// @Param	strCentralBodyName  中心天体名称
	/// @Return	
	//********************************************************************
	void NewScenario(const std::string& strSceName, std::string strCentralBodyName = "");

	//********************************************************************
	/// 关闭场景
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	
	//********************************************************************
	void CloseScenario();

	//********************************************************************
	/// 加载场景文件
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	
	//********************************************************************
	void LoadScenario(const std::string& strFilePath);

        //********************************************************************
	/// 通过路径得到当前对象
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	strTruncPath  截断路径，例："Satellite/Satelite1"
	/// @Return	对象属性
	//********************************************************************
	IAtkObject* GetObjectFromPath(const std::string& strTruncPath);

        //********************************************************************
	/// 判断对象是否存在
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	strTruncPath  截断路径，例："Satellite/Satelite1"
	/// @Return	true-对象存在
	//********************************************************************
	bool ObjectExists(const std::string& strTruncPath);

        //********************************************************************
	/// 保存场景
	/// @Author	Gan Shu Long
	/// @Date	2023-12-6
	/// @Input
	/// @Param
	/// @Return	
	//********************************************************************
	void SaveScenario();
	void SaveScenario(std::string strFilePath);

	//********************************************************************
	/// 输出数据报告
	/// @Author	Gan Shu Long
	/// @Date	2023-12-11
	/// @Input
	/// @Param 	pIAtkObject  对象指针
	/// @Param 	strStyle     报告类型
	/// @Param 	strStartUTC  开始时间
	/// @Param 	strStopUTC   结束时间
	/// @Param 	fileNamePrefix   附加后缀名
	/// @Return	输出报告路径
	//********************************************************************
	std::string OutputDataReport(IAtkObject* pIAtkObject, std::string strStyle, const std::string& strStartUTC, const std::string& strStopUTC, std::string fileNamePrefix = "");

        //********************************************************************
	/// 输出数据报告
	/// @Author	Gan Shu Long
	/// @Date	2023-12-11
	/// @Input
	/// @Param 	pIAtkObject  对象指针
	/// @Param 	strStartUTC  开始时间
	/// @Param 	strStopUTC   结束时间
	/// @Param 	pos          报告位置
	/// @Param 	vel          报告速度
	/// @Return	返回数据是否成功
	//********************************************************************
	//bool OutputJ2000Report(IAtkObject* pIAtkObject, std::vector<std::string>& dateTime, std::vector<std::vector<double>>& pos, std::vector<std::vector<double>>& vel);

	//********************************************************************
	/// 根据给定时间输出数据报告
	/// @Author	Dong Min
	/// @Date	2025-05-12
	/// @Input
	/// @Param 	pIAtkObject  对象指针
	/// @Param 	dateTime  时间
	/// @Param 	posCBI/posCBF   惯性系位置/固体系位置
	/// @Param 	velCBI/velCBF   惯性系速度/固体系速度
	/// @Return	返回数据是否成功
	//********************************************************************
	bool OutputPosVelCBI(IAtkObject* pIAtkObject, std::string dateTime, std::vector<double>& posCBI, std::vector<double>& velCBI);
	bool OutputPosVelCBF(IAtkObject* pIAtkObject, std::string dateTime, std::vector<double>& posCBF, std::vector<double>& velCBF);
	bool OutputPosVelJ2000(IAtkObject* pIAtkObject, std::string dateTime, std::vector<double>& pos, std::vector<double>& vel);

	//********************************************************************
	/// 根据给定信息输出数据
	/// @Author	Dong Min
	/// @Date	2025-05-12
	/// @Input
	/// @Param 	pIAtkObject/strObjPath  对象指针/对象路径，例：Satellite/Satellite1
	/// @Param 	eDataType    数据类型
	/// @Param 	strStartTime  开始时间
	/// @Param 	strStopTime   结束时间
	/// @Param 	dStepSec   步长
	/// @Output
	/// @Param 	vecEpoch  时间 
	/// @Param 	vecData  数据 
	/// @Return	返回数据是否成功
	//********************************************************************
	bool OutputOrbitDataCBF(IAtkObject* pIAtkObject,
		EDataType eDataType,
		std::string strStartTime,
		std::string strStopTime,
		double dStepSec,
		std::vector<std::string>& vecEpoch,
		std::vector<std::vector<double>>& vecData);

	bool OutputOrbitDataCBF(std::string strObjPath,
		EDataType eDataType,
		std::string strStartTime,
		std::string strStopTime,
		double dStepSec,
		std::vector<std::string>& vecEpoch,
		std::vector<std::vector<double>>& vecData);

	//********************************************************************
	/// 通过路径得到当前对象
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	strTruncPath  截断路径，例："Satellite/Satelite1"
	/// @Return	对象属性
	//********************************************************************
	IScenario* GetCurrentScenario();

	//********************************************************************
	/// 获取仿真操作指针
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	仿真操作指针
	//********************************************************************
	IAnimation* GetAnimation();

	//********************************************************************
	/// 批量坐标转换
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	批量坐标转换操作指针
	//********************************************************************
	IATKBatchCrdnTransform* GetBatchCrdnTransform();

	//********************************************************************
	/// 接近分析
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	接近分析操作指针
	//********************************************************************
	ICat* GetCat();

	//********************************************************************
	/// 星座设计
	/// @Author	Gan Shu Long
	/// @Date	2025-7-1
	/// @Return	星座设计操作指针
	//********************************************************************
	IConstellDesign* GetConstellDesign();

	//********************************************************************
	/// 覆盖性
	/// @Author	Gan Shu Long
	/// @Date	2026-09-30
	/// @Return	覆盖性操作指针
	//********************************************************************
	IAtkObjectCoverage* GetObjectCoverage();

private:
	bool                                 bNewObjManage;

	//IAtkObject*                        m_CurrentObject;
	IScenario*                           m_CurrentScenario;   //场景信息
	IAnimation*                          m_Animation;		 //多继承转成员
	IATKBatchCrdnTransform*              m_BatchCrdnTransform;   //批量坐标转换
	ICat*                                m_Cat;   //批量坐标转换
	IConstellDesign*                     m_ConstellDesign;   //星座设计
	IAtkObjectCoverage*					 m_ObjectCoverage;	//覆盖性

	// 递归加载场景对象
	bool LoadObject(CObjData* pObjData, IAtkObject* pParentIAtkObj);
	// 新建对象
	IAtkObject* NewIObject(CObjData* pObjData);
	// 删除对象
	void DeIObject(IAtkObject* pParentIAtkObj);
	// 递归查找I对象
	void InitIObject();
	// 递归查找对象
	void GetObject(std::string strObjPath, IAtkObjectCollection* pParentObj, IAtkObject* &pIObj);

	friend class IAtkObject;
};


//********************************************************************
/// 此接口仅向脚本工具提供使用，请不要随意调用
/// @Author	Dong Min
/// @Date	2025-11-25
/// @Return	保证脚本工具使用一个Root指针
//********************************************************************
IATKOBJECT_API IAtkObjectRoot* GetIAtkObjectRoot();


