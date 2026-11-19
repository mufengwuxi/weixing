#pragma once
#include <string>
#include <vector>
#include "../../ICEnum.h"
#include "../../IAtkObj.h"
struct CObjData;
struct CVehAttSegment;
class IVeAttProfile;

///
/// 标准姿态类型属性
///
class IATKOBJECT_API IVeStandardBasic
{
public:
    IVeStandardBasic(const IVeStandardBasic&) = delete;
    IVeStandardBasic& operator=(const IVeStandardBasic&) = delete;

	IVeStandardBasic(CObjData* pObjData);
	~IVeStandardBasic();

	//********************************************************************
	/// 判断是否支持输入类型
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	EVeProfile   姿态类型
	/// @Return	true-对象支持此姿态类型	
	//********************************************************************
	bool IsProfileTypeSupported(EVeProfile eProfile);

	//********************************************************************
	/// 设置姿态类型
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	EVeProfile   姿态类型
	/// @Return	
	//********************************************************************
	void SetProfileType(EVeProfile eProfile);

	//********************************************************************
	/// 获得姿态类型属性
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	姿态类型属性
	//********************************************************************
	IVeAttProfile* GetProfile() { return m_Profile; }

	//********************************************************************
	/// 获得支持的姿态类型
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	支持的姿态类型
	//********************************************************************
	std::vector<std::string> GetProfileSupportedTypes();

	//********************************************************************
	/// 获得姿态类型
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	姿态类型
	//********************************************************************
	EVeProfile GetProfileType();

private:
	CObjData*					m_pObjData;
	CVehAttSegment*		m_pCVehAttSegment;

	IVeAttProfile*               m_Profile;                 //姿态类型属性

	std::string ProTypeToString(EVeProfile eProfileType);   //姿态类型转字符串

};



