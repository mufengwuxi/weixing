#pragma once
#include "../IAtkObj.h" 
#include "../IClassInfo.h" 
struct AsCvFmDefn;
class IFmSatisfaction;

///
/// 品质因子定义
///
class IATKOBJECT_API IFmDefinition :public IClassInfo
{
public:
    IFmDefinition(const IFmDefinition&) = delete;
    IFmDefinition& operator=(const IFmDefinition&) = delete;

	IFmDefinition(AsCvFmDefn* pAsCvFmDefn);
	~IFmDefinition();

	//********************************************************************
	/// 获得  有效条件
	/// @Author	Gan Shu Long
	/// @Date	2026-05-07
	/// @Return	 定义
	//*******************************************************
	IFmSatisfaction* GetSatisfaction() { return m_Satisfaction; }

protected:
	AsCvFmDefn*			m_pAsCvFmDefn;

private:
	IFmSatisfaction*		m_Satisfaction;
};
