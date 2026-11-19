#pragma once
#include <string>
#include <vector>
#include "../../IAtkObj.h"
class CSatellite;
class IVeSGP4Segment;

///
/// SGP4段集合
///
class IATKOBJECT_API IVeSGP4SegmentCollection
{
public:
    IVeSGP4SegmentCollection(const IVeSGP4SegmentCollection&) = delete;
    IVeSGP4SegmentCollection& operator=(const IVeSGP4SegmentCollection&) = delete;

	IVeSGP4SegmentCollection(CSatellite* pSate);
	~IVeSGP4SegmentCollection();

	//********************************************************************
	/// 添加SGP4段到集合
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	
	//********************************************************************
	IVeSGP4Segment* AddSeg();

	//********************************************************************
	/// 移除集合中所有SGP4段
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	
	//********************************************************************
	void RemoveAllSegs();

	//********************************************************************
	/// 移除集合中指定SGP4段
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	nIndex  按添加顺序排序
	/// @Return	
	//********************************************************************
	void RemoveSeg(int nIndex);

	//********************************************************************
	/// 设置SSC编号
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	strSSCNumber  SSC编号
	/// @Return	
	//********************************************************************
	void SetSSCNumber(std::string strSSCNumber);
	//********************************************************************
	/// 获得SSC编号
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	SSC编号
	//********************************************************************
	std::string GetSSCNumber() { return m_SSCNumber; }

	int                 GetCount();         //SGP4段集合里元素的数量
	IVeSGP4Segment*     Item(int nIndex);   //获得SGP4段集合里元素

private:
	std::string         m_SSCNumber;


	CSatellite* m_pSate;
	std::vector<IVeSGP4Segment*> m_vecIVeSGP4Segment;

};
