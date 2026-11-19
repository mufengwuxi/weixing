#pragma once
#include "../../IAtkObj.h"
#include "IVePropagatorBallistic.h"
#include "../../ICEnum.h"
struct CObjData;

///
/// 真实弹道学预报器
///
class IATKOBJECT_API IVePropagatorRealisticBallistic :public IVePropagatorBallistic
{
public:
    IVePropagatorRealisticBallistic(const IVePropagatorRealisticBallistic&) = delete;
    IVePropagatorRealisticBallistic& operator=(const IVePropagatorRealisticBallistic&) = delete;

	IVePropagatorRealisticBallistic(CObjData* pObjData);
	~IVePropagatorRealisticBallistic();

	//********************************************************************
	/// 获得支持的导弹类型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-06
	/// @Return 支持的导弹类型
	//********************************************************************
	std::vector<std::string> GetMissileSupportedTypes();

	//********************************************************************
	/// 判断是否支持导弹类型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-06
	/// @Param	eVeMissileType   导弹类型
	/// @Return 是否支持导弹类型
	//********************************************************************
	bool IsMissileTypeSupported(EVeMissileType eVeMissileType);

	//********************************************************************
	/// 设置导弹类型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-06
	/// @Input
	/// @Param	eVeMissileType   导弹类型
	/// @Return	
	//********************************************************************
	void SetMissileType(EVeMissileType eVeMissileType);

	//********************************************************************
	/// 获得导弹类型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-06
	/// @Return	导弹类型
	//********************************************************************
	EVeMissileType GetMissileType();

private:
	//真实弹道学落点只支持经纬高，屏蔽类型设置函数
	using  IVePropagatorBallistic::GetImpactLocationSupportedTypes;
	using  IVePropagatorBallistic::IsImpactLocationTypeSupported;
	using  IVePropagatorBallistic::SetImpactLocationType;
	using  IVePropagatorBallistic::GetImpactLocationType;

private:
	CMissile*												m_pCMissile;
};
