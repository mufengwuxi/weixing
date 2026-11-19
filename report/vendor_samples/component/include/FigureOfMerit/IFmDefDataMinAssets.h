#pragma once
#include "../IAtkObj.h"
#include "IFmDefinitionData.h" 

///
/// 品质因子定义数据：最小重数
///
class IATKOBJECT_API IFmDefDataMinAssets :public IFmDefinitionData
{
public:
    IFmDefDataMinAssets(const IFmDefDataMinAssets&) = delete;
    IFmDefDataMinAssets& operator=(const IFmDefDataMinAssets&) = delete;

	IFmDefDataMinAssets(AsCvFmDefn* pAsCvFmDefn);
	~IFmDefDataMinAssets();

	//********************************************************************
	/// 设置  最小重数
	/// @Author	Gan Shu Long
	/// @Date	2026-05-07
	/// @Input 
	/// @Param	nMinAssets 最小重数
	/// @Return	
	//********************************************************************
	void SetMinAssets(int nMinAssets);

	//********************************************************************
	/// 获得 最小重数
	/// @Author	Gan Shu Long
	/// @Date	2026-05-07
	/// @Return	最小重数
	//********************************************************************
	int GetMinAssets();
};
