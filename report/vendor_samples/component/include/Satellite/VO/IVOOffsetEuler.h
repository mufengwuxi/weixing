#pragma once
#include "../../IAtkObj.h"
#include "../../ICEnum.h"
struct CGfxModel;

///
/// 欧拉角
///
class IATKOBJECT_API IVOOffsetEuler
{
public:
    IVOOffsetEuler(const IVOOffsetEuler&) = delete;
    IVOOffsetEuler& operator=(const IVOOffsetEuler&) = delete;

	IVOOffsetEuler(CGfxModel* pGfxModel);
	~IVOOffsetEuler();

	//********************************************************************
	/// 设置三维模型欧拉角1
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dEuler1  欧拉角1
	/// @Return	
	//********************************************************************
	void SetEuler1(double dEuler1);
	//********************************************************************
	/// 设置三维模型欧拉角1
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	欧拉角1
	//********************************************************************
	double GetEuler1();

	//********************************************************************
	/// 设置三维模型欧拉角2
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dEuler2  欧拉角2
	/// @Return	
	//********************************************************************
	void SetEuler2(double dEuler2);
	//********************************************************************
	/// 设置三维模型欧拉角2
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	欧拉角2
	//********************************************************************
	double GetEuler2();

	//********************************************************************
	/// 设置三维模型欧拉角3
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	dEuler3  欧拉角3
	/// @Return	
	//********************************************************************
	void SetEuler3(double dEuler3);
	//********************************************************************
	/// 设置三维模型欧拉角3
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	欧拉角3
	//********************************************************************
	double GetEuler3();

private:
	CGfxModel*					m_pGfxModel;		//C对象三维模型指针

};

