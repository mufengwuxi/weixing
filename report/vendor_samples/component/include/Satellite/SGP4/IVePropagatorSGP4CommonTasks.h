#pragma once
#include <string>
#include "../../IAtkObj.h"
class CSatellite;
class IVePropagatorSGP4;

///
/// SGP4段添加
///
class IATKOBJECT_API IVePropagatorSGP4CommonTasks
{
public:
    IVePropagatorSGP4CommonTasks(const IVePropagatorSGP4CommonTasks&) = delete;
    IVePropagatorSGP4CommonTasks& operator=(const IVePropagatorSGP4CommonTasks&) = delete;

	IVePropagatorSGP4CommonTasks(CSatellite* pSate, IVePropagatorSGP4* pISGP4);
	~IVePropagatorSGP4CommonTasks();

	//********************************************************************
	/// 从文件中添加SGP4段
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	SSCNumber  SSC编号
	/// @Param	FileName   文件路径
	/// @Return	
	//********************************************************************
	void AddSegsFromFile(std::string strSSCNumber, std::string strFileName);


private:
	CSatellite*            m_pSate;

	IVePropagatorSGP4*     m_pISGP4;

	double ChangeValue(const std::string strInput, double nValue);

};

