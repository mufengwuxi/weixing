#pragma once
#include "../../IAtkObj.h"
struct CGfxVeh2DCustom;
class IVeGfxAttributesBasic;

///
/// 二维显示属性
///
class IATKOBJECT_API IVeGfxInterval
{
public:
    IVeGfxInterval(const IVeGfxInterval&) = delete;
    IVeGfxInterval& operator=(const IVeGfxInterval&) = delete;

	IVeGfxInterval(CGfxVeh2DCustom* pGfxVeh2DCustom);
	~IVeGfxInterval();

	//********************************************************************
	/// 设置二维自定义显示属性
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	属性参数指针
	//********************************************************************
	IVeGfxAttributesBasic* GetGfxAttributes();

	//********************************************************************
	/// 设置二维自定义显示开始时间属性
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	strStartTime  开始时间，UTCG格式
	/// @Return	
	//********************************************************************
	void SetStartTime(std::string strStartTime);
	//********************************************************************
	/// 获得二维自定义显示开始时间属性
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	开始时间
	//********************************************************************
	std::string GetStartTime();

	//********************************************************************
	/// 设置二维自定义显示结束时间属性
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	strStopTime  结束时间，UTCG格式
	/// @Return	
	//********************************************************************
	void SetStopTime(std::string strStopTime);
	//********************************************************************
	/// 获得二维自定义显示结束时间属性
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	结束时间
	//********************************************************************
	std::string GetStopTime();

private:
	CGfxVeh2DCustom*           m_pGfxVeh2DCustom;

	IVeGfxAttributesBasic*     m_GfxAttributes;   //二维自定义显示属性
};
