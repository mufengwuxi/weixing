#pragma once
#include <vector>
#include <string>
#include "../../IAtkObj.h"
class CSatellite;
class IVeThirdBodyGravityElement;

///
/// 三体引力集合
///
class IATKOBJECT_API IVeThirdBodyGravityCollection
{
public:
    IVeThirdBodyGravityCollection(const IVeThirdBodyGravityCollection&) = delete;
    IVeThirdBodyGravityCollection& operator=(const IVeThirdBodyGravityCollection&) = delete;

	IVeThirdBodyGravityCollection(CSatellite* pSate);
	~IVeThirdBodyGravityCollection();

	//********************************************************************
	/// 添加一个新的元素到三体引力集合
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	strThirdBody   三体名称   Moon、Earth、Mars
	/// @Return	三体属性	
	//********************************************************************
	IVeThirdBodyGravityElement* AddThirdBody(std::string strThirdBody);

	//********************************************************************
	/// 清空三体引力集合里的元素
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return				
	//********************************************************************
	void RemoveAll();

	//********************************************************************
	/// 清除三体引力集合里的第n个元素
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	Index   集合序号，按添加顺序排序
	/// @Return	
	//********************************************************************
	void RemoveAt(int nIndex);

	//********************************************************************
	/// 清除三体引力集合里的指定元素
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	strThirdBody   三体名称，添加过才会删除
	/// @Return	
	//********************************************************************
	void RemoveThirdBody(std::string strThirdBody);

	//********************************************************************
	/// 获得可得到的三体引力集合
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	三体引力名称
	//********************************************************************
	std::vector<std::string> GetAvailableThirdBodyNames();

public:
	int                                          GetCount();                //三体引力集合里元素的数量
	IVeThirdBodyGravityElement*                  Item(int nIndex);          //获得三体引力集合里给定序号的元素

protected:
	IVeThirdBodyGravityElement*                  m_pIVeThirdBodyGravityElement;

private:
	CSatellite*                                  m_pSate;
	std::vector<IVeThirdBodyGravityElement*>     m_vecTBGravElem;

	bool GetOperTBGElement(std::string strThirdBody);

	int GetGravityID(std::string strThirdBody);

	void InitThirdBody();

};
