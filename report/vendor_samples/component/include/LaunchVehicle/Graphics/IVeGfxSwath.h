#pragma once
#include "../../IAtkObj.h"
#include "../../ICEnum.h"
#include "../../IClassInfo.h"
#include <vector>
struct CGfxSwath;
class IVeGfxElevation;

///
/// 刈幅
///
class IATKOBJECT_API IVeGfxSwath :public IClassInfo
{
public:
    IVeGfxSwath(const IVeGfxSwath&) = delete;
    IVeGfxSwath& operator=(const IVeGfxSwath&) = delete;

	IVeGfxSwath(CGfxSwath* pCGfxSwath);
	virtual ~IVeGfxSwath();

	//********************************************************************
	/// 是否支持类型
	/// @Author	Gan Shu Long
	/// @Date	2026-01-28
	/// @Input
	/// @Param	eElevationType  类型
	/// @Return	是否支持类型
	//********************************************************************
	bool IsElevationTypeSupported(EVeGfxElevation eElevationType);
	
	//********************************************************************
	/// 设置类型
	/// @Author	Gan Shu Long
	/// @Date	2026-01-28
	/// @Input
	/// @Param	eElevationType  类型
	/// @Return		
	//********************************************************************
	void SetElevationType(EVeGfxElevation eElevationType);

	//********************************************************************
	/// 获得类型
	/// @Author	Gan Shu Long
	/// @Date	2026-01-08
	/// @Return	类型
	//********************************************************************
	EVeGfxElevation GetElevationType();

	//********************************************************************
	/// 获得支持类型
	/// @Author	Gan Shu Long
	/// @Date	2026-01-28
	/// @Return	支持类型
	//********************************************************************
	std::vector<std::string> GetElevationSupportedTypes();

	//********************************************************************
	/// 获得夹角
	/// @Author	Gan Shu Long
	/// @Date	2026-01-28
	/// @Return	夹角
	//********************************************************************
	IVeGfxElevation* GetElevation() { return m_Elevation; }
	
	//********************************************************************
	/// 设置显示设置类型
	/// @Author	Gan Shu Long
	/// @Date	2026-01-28
	/// @Input
	/// @Param	eElevationType  类型
	/// @Return		
	//********************************************************************
	void SetOptions(EVeGfxOptions eElevationType);

	//********************************************************************
	/// 获得显示设置类型
	/// @Author	Gan Shu Long
	/// @Date	2026-01-08
	/// @Return	显示设置类型
	//********************************************************************
	EVeGfxOptions GetOptions();
		
private:
	CGfxSwath*								m_pCGfxSwath;							//C对象航线

	IVeGfxElevation*							m_Elevation;								//夹角
};



