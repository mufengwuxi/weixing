#pragma once
#include "../IAtkObject.h"
#include "../ICEnum.h"
#include <vector>
#include <string>
class CObjManage;
struct CObjData;
class CFigureOfMerit;
class IFmDefinition;
class IFmGraphics;

///
/// 品质因子
///
class IATKOBJECT_API IFigureOfMerit :public IAtkObject
{
public:
    IFigureOfMerit(const IFigureOfMerit&) = delete;
    IFigureOfMerit& operator=(const IFigureOfMerit&) = delete;

	IFigureOfMerit(CObjData* pObjData, CObjManage* IAsGObjManage, IAtkObjectRoot* Root);
	~IFigureOfMerit();

	//********************************************************************
	/// 获得 支持的定义类型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-30
	/// @Return 支持的定义类型
	//********************************************************************
	std::vector<std::string> GetDefinitionSupportedTypes();

	//********************************************************************
	/// 获得 是否支持定义类型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-30
	/// @Input
	/// @Param	eFmDefinitionType  定义类型
	/// @Return	bool 是否支持定义类型	
	//********************************************************************
	bool IsDefinitionTypeSupported(EFmDefinitionType eFmDefinitionType);

	//********************************************************************
	/// 设置 定义类型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-30
	/// @Input
	/// @Param	eFmDefinitionType  定义类型
	/// @Return	
	//********************************************************************
	void SetDefinitionType(EFmDefinitionType eFmDefinitionType);

	//********************************************************************
	/// 获得  定义类型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-30
	/// @Return	 定义类型
	//********************************************************************
	EFmDefinitionType GetDefinitionType();

	//********************************************************************
	/// 获得  定义
	/// @Author	Gan Shu Long
	/// @Date	2026-03-30
	/// @Return	 定义
	//*******************************************************
	IFmDefinition* GetDefinition() { return m_Definition; }

	//********************************************************************
	/// 获得 二维视图
	/// @Author	Gan Shu Long
	/// @Date	2026-05-11
	/// @Return	二维视图
	//********************************************************************
	IFmGraphics* GetGraphics() { return m_Graphics; }

private:
	CFigureOfMerit*				m_pCFigureOfMerit;

	IFmDefinition*					m_Definition;			//定义
	IFmGraphics*					m_Graphics;			//二维
};
