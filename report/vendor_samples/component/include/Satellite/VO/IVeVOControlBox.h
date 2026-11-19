#pragma once
#include "IVeVOProximityAreaObject.h"
struct CObjData;
struct CGfxControlBox;

///
/// 三维抵近控制盒
///
class IATKOBJECT_API IVeVOControlBox : public IVeVOProximityAreaObject
{
public:
    IVeVOControlBox(const IVeVOControlBox&) = delete;
    IVeVOControlBox& operator=(const IVeVOControlBox&) = delete;

	IVeVOControlBox(CGfxControlBox* pCGfxControlBox);
	~IVeVOControlBox();

	//********************************************************************
	/// 设置X轴长度
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dXAxisLen  X轴长度
	/// @Return	
	//********************************************************************
	void SetXAxisLength(double dXAxisLen);
	//********************************************************************
	/// 获得X轴长度
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	X轴长度		
	//********************************************************************
	double GetXAxisLength();

	//********************************************************************
	/// 设置Y轴长度
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dYAxisLen  Y轴长度
	/// @Return	
	//********************************************************************
	void SetYAxisLength(double dYAxisLen);
	//********************************************************************
	/// 获得Y轴长度
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	Y轴长度	
	//********************************************************************
	double GetYAxisLength();

	//********************************************************************
	/// 设置Z轴长度
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dZAxisLen  Z轴长度
	/// @Return	
	//********************************************************************
	void SetZAxisLength(double dZAxisLen);
	//********************************************************************
	/// 获得Z轴长度
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	Z轴长度	
	//********************************************************************
	double GetZAxisLength();

	//********************************************************************
	/// 设置X轴偏移量
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dXOffset  X轴偏移量
	/// @Return	
	//********************************************************************
	void SetXOffset(double dXOffset);
	//********************************************************************
	/// 获取X轴偏移量
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	X轴偏移量
	//********************************************************************
	double GetXOffset();

	//********************************************************************
	/// 设置Y轴偏移量
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dYOffset  Y轴偏移量
	/// @Return	
	//********************************************************************
	void SetYOffset(double dYOffset);
	//********************************************************************
	/// 获取Y轴偏移量
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	Y轴偏移量
	//********************************************************************
	double GetYOffset();

	//********************************************************************
	/// 设置Z轴偏移量
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dZOffset  Z轴偏移量
	/// @Return	
	//********************************************************************
	void SetZOffset(double dZOffset);
	//********************************************************************
	/// 获取Z轴偏移量
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	Z轴偏移量
	//********************************************************************
	double GetZOffset();

private:
	CGfxControlBox*              m_pCGfxControlBox;			//C对象控制盒指针
};
