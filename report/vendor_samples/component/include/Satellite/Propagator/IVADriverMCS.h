#pragma once
#include "IVePropagator.h"
class CObjManage;
class CSatellite;
class CMCSSequence;
class ISatellite;
class IVAMCSSegmentCollection;

///
/// 机动规划预报器
///
class IATKOBJECT_API IVADriverMCS : public IVePropagator
{
public:
    IVADriverMCS(const IVADriverMCS&) = delete;
    IVADriverMCS& operator=(const IVADriverMCS&) = delete;

	IVADriverMCS(CSatellite* pSate, ISatellite* pISate, CObjManage* IAsGObjManage);
	~IVADriverMCS();

	//********************************************************************
	/// 应用所有属性页修改
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return				
	//********************************************************************
	void ApplyAllProfileChanges();

	//********************************************************************
	/// 重置所有属性页修改
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return				
	//********************************************************************
	void ResetAllProfiles();

	//********************************************************************
	/// 运行机动规划
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return				
	//********************************************************************
	void RunMCS();

	//********************************************************************
	/// 获得段集合
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	段集合指针			
	//********************************************************************
	IVAMCSSegmentCollection* GetMainSequence();

	//********************************************************************
	/// 设置卫星参考航天器
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return				
	//********************************************************************
	void SetObjReference(std::string strObjPath);

private:
	CObjManage*                  m_IAsGObjManage;
	CSatellite*                  m_pSate;
	ISatellite*                  m_pISate;

	IVAMCSSegmentCollection*     m_MainSequence;   //根节点

	bool SetSegmentState(CMCSSequence* pcRootSequence, CMCSSequence* pcSequence, IVAMCSSegmentCollection* pIVAMCSSegmentCollection, std::string strParentSegmentName);

};
