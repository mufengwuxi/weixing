#pragma once
#include "../IAtkObj.h" 
#include "../ICEnum.h"
class CReceiver;

///
/// 链路裕量
///
class IATKOBJECT_API ILinkMargin
{
public:
    ILinkMargin(const ILinkMargin&) = delete;
    ILinkMargin& operator=(const ILinkMargin&) = delete;

	ILinkMargin(CReceiver* pCReceiver);
	~ILinkMargin();

	//********************************************************************
	/// 设置  链路裕量类型
	/// @Author	Gan Shu Long
	/// @Date	2023-12-11
	/// @Input 
	/// @Param	double 新值
	/// @Return	
	//********************************************************************
	void SetType(ELinkMarginType type);

	//********************************************************************
	/// 设置  阈值
	/// @Author	Gan Shu Long
	/// @Date	2023-12-11
	/// @Input 
	/// @Param	double 新值
	/// @Return	
	//********************************************************************
	void SetThreshold(double threshold);

	//链路裕量类型
	ELinkMarginType GetType();

	//阈值
	double GetThreshold();

private:
	CReceiver*		m_CReceiver;
};
