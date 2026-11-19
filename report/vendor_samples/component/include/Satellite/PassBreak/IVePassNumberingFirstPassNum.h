#pragma once
#include "IVePassNumbering.h"
class CSatellite;

///
/// 初始圈数
///
class IATKOBJECT_API IVePassNumberingFirstPassNum : public IVePassNumbering
{
public:
	IVePassNumberingFirstPassNum(const IVePassNumberingFirstPassNum&) = delete;
	IVePassNumberingFirstPassNum& operator=(const IVePassNumberingFirstPassNum&) = delete;

	IVePassNumberingFirstPassNum(CSatellite* pSate);
	~IVePassNumberingFirstPassNum();

	//********************************************************************
	/// 设置初始圈数
	/// @Author	Gan Shu Long
	/// @Date	2026-10-20
	/// @Input 
	/// @Param	nFirstPassNum 初始圈数
	/// @Return	
	//********************************************************************
	void SetFirstPassNum(int nFirstPassNum);

	//********************************************************************
	/// 获得初始圈数
	/// @Author	Gan Shu Long
	/// @Date	2026-10-20
	/// @Return	初始圈数
	//********************************************************************
	int GetFirstPassNum();

private:
	CSatellite* m_pSate;

};