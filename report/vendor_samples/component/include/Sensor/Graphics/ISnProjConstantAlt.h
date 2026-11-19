#pragma once
#include "../../IAtkObj.h"
#include "IDisplayDistance.h"
class CSensor;

///
/// 投影距离
///
class IATKOBJECT_API ISnProjConstantAlt : public IDisplayDistance
{
public:
    ISnProjConstantAlt(const ISnProjConstantAlt&) = delete;
    ISnProjConstantAlt& operator=(const ISnProjConstantAlt&) = delete;

	ISnProjConstantAlt(CSensor* pCSensor);
	~ISnProjConstantAlt();
	
	//********************************************************************
	/// 设置 距离
	/// @Author	Gan Shu Long
	/// @Date	2023-12-8
	/// @Input 
	/// @Param	max 最大距离值
	/// @Return	
	//********************************************************************
	void SetMax(double max);

	double GetMax();

	//指定是否排除视界弧。
	double GetExcludeHorizonArcs();

private:
	CSensor* m_pCSensor;
};

