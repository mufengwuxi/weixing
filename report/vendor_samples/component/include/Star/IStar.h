#pragma once
#include "../IAtkObject.h"
#include "../AccessConstraints/IAccessConstraintCollection.h"
class CObjManage;
struct CObjData;
class CStar;
class IStGraphics;

///
/// 恒星
///
class IATKOBJECT_API IStar :public IAtkObject
{
public:
    IStar(const IStar&) = delete;
    IStar& operator=(const IStar&) = delete;

	IStar(CObjData* pObjData, CObjManage* IAsGObjManage, IAtkObjectRoot* Root);
	~IStar();

	//********************************************************************
	/// 设置 赤经(deg)
	/// @Author	Gan Shu Long
	/// @Date	2026-03-09
	/// @Input 
	/// @Param	locationRightAscension 赤经(deg)
	/// @Return	
	//********************************************************************
	void SetLocationRightAscension(double locationRightAscension);

	//********************************************************************
	/// 获得 赤经(deg)
	/// @Author	Gan Shu Long
	/// @Date	2026-03-09
	/// @Return	 赤经(deg)
	//********************************************************************
	double GetLocationRightAscension();

	//********************************************************************
	/// 设置 赤纬(deg)
	/// @Author	Gan Shu Long
	/// @Date	2026-03-09
	/// @Input 
	/// @Param	locationDeclination 赤纬(deg)
	/// @Return	
	//********************************************************************
	void SetLocationDeclination(double locationDeclination);

	//********************************************************************
	/// 获得 赤纬(deg)
	/// @Author	Gan Shu Long
	/// @Date	2026-03-09
	/// @Return	 赤纬(deg)
	//********************************************************************
	double GetLocationDeclination();

	//********************************************************************
	/// 设置 赤经(arc sec/year)
	/// @Author	Gan Shu Long
	/// @Date	2026-03-09
	/// @Input 
	/// @Param	properMotionRightAscension 赤经(arc sec/year)
	/// @Return	
	//********************************************************************
	void SetProperMotionRightAscension(double properMotionRightAscension);

	//********************************************************************
	/// 获得 赤经(arc sec/year)
	/// @Author	Gan Shu Long
	/// @Date	2026-03-09
	/// @Return	 赤经(arc sec/year)
	//********************************************************************
	double GetProperMotionRightAscension();

	//********************************************************************
	/// 设置 赤纬(darc sec/year)
	/// @Author	Gan Shu Long
	/// @Date	2026-03-09
	/// @Input 
	/// @Param	properMotionDeclination 赤纬(darc sec/year)
	/// @Return	
	//********************************************************************
	void SetProperMotionDeclination(double properMotionDeclination);

	//********************************************************************
	/// 获得 赤纬(darc sec/year)
	/// @Author	Gan Shu Long
	/// @Date	2026-03-09
	/// @Return	 赤纬(darc sec/year)
	//********************************************************************
	double GetProperMotionDeclination();

	//********************************************************************
	/// 设置 星等
	/// @Author	Gan Shu Long
	/// @Date	2026-03-09
	/// @Input 
	/// @Param	magnitude 星等
	/// @Return	
	//********************************************************************
	void SetMagnitude(double magnitude);

	//********************************************************************
	/// 获得 星等
	/// @Author	Gan Shu Long
	/// @Date	2026-03-09
	/// @Return	 星等
	//********************************************************************
	double GetMagnitude();

	//********************************************************************
	/// 设置 年视差
	/// @Author	Gan Shu Long
	/// @Date	2026-03-09
	/// @Input 
	/// @Param	parallax 年视差
	/// @Return	
	//********************************************************************
	void SetParallax(double parallax);

	//********************************************************************
	/// 获得 年视差
	/// @Author	Gan Shu Long
	/// @Date	2026-03-09
	/// @Return	 年视差
	//********************************************************************
	double GetParallax();

	//********************************************************************
	/// 获得二维属性
	/// @Author	Gan Shu Long
	/// @Date	2026-03-09
	/// @Return	二维属性指针	
	//********************************************************************
	IStGraphics* GetGraphics() { return m_Graphics; }

private:
	CStar*					m_pCStar;

	IStGraphics*		m_Graphics;			//二维
};
