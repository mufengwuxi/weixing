#pragma once
#include "../../ICEnum.h"
#include <vector>
struct CObjData;
class IScenario;
struct CVehAttSegment;

///
/// 单段姿态列表
///
class IATKOBJECT_API IVeMultiSegmentBasic
{
public:
    IVeMultiSegmentBasic(const IVeMultiSegmentBasic&) = delete;
    IVeMultiSegmentBasic& operator=(const IVeMultiSegmentBasic&) = delete;

	IVeMultiSegmentBasic(CObjData* pObjData, IScenario* pIScenario);
	~IVeMultiSegmentBasic();

	//********************************************************************
	/// 添加姿态段，默认开始时间为当前最后一条姿态段开始时间+1小时
	/// @Author	Gan Shu Long
	/// @Date	2026-02-28
	/// @Input
	/// @Param	
	/// @Param	strStartTime 开始时间
	/// @Param	eVeProfile 姿态类型
	/// @Param	dqx
	/// @Param	dqy
	/// @Param	dqz
	/// @Param	dqs
	/// @Return	是否添加成功
	//********************************************************************
	bool Add(std::string strStartTime, EVeProfile eVeProfile, double dqx, double dqy, double dqz, double dqs);

	//********************************************************************
	/// 删除姿态段
	/// @Author	Gan Shu Long
	/// @Date	2026-02-28
	/// @Input
	/// @Param	index 删除索引
	/// @Return	是否删除成功
	//********************************************************************
	bool Remove(int index);

	//********************************************************************
	///姿态段数量
	/// @Author	Gan Shu Long
	/// @Date	2026-03-01
	/// @Input
	/// @Param	
	/// @Return	int 姿态段数量
	//********************************************************************
	int GetCount();

private:
	CObjData*												m_pObjData;
	IScenario*												m_pIScenario;
	std::vector<CVehAttSegment>*				m_pVectorCVehAttSegment;
};


