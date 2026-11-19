#pragma once
#include "../IAtkObj.h"
#include "IFmDefCompute.h" 
#include <string>

///
/// 品质因子定义：系统数据时龄
///
class IATKOBJECT_API IFmDefSystemAgeOfData :public IFmDefCompute
{
public:
    IFmDefSystemAgeOfData(const IFmDefSystemAgeOfData&) = delete;
    IFmDefSystemAgeOfData& operator=(const IFmDefSystemAgeOfData&) = delete;

	IFmDefSystemAgeOfData(AsCvFmDefn* pAsCvFmDefn);
	~IFmDefSystemAgeOfData();

	//********************************************************************
	/// 获得 支持的计算类型
	/// @Author	Gan Shu Long
	/// @Date	2026-05-11
	/// @Return 支持的计算类型
	//********************************************************************
	std::vector<std::string> GetComputeSupportedTypes() override;

	//********************************************************************
	/// 获得 是否支持计算类型
	/// @Author	Gan Shu Long
	/// @Date	2026-05-11
	/// @Input
	/// @Param	eFmCompute  计算类型
	/// @Return	bool 是否支持计算类型	
	//********************************************************************
	bool IsComputeTypeSupported(EFmCompute eFmCompute) override;

	//********************************************************************
	/// 设置 指令对象路径
	/// @Author	Gan Shu Long
	/// @Date	2026-05-08
	/// @Input
	/// @Param	strCommandStationPath  指令对象路径
	/// @Return	
	//********************************************************************
	void SetCommandStationPath(std::string strCommandStationPath);

	//********************************************************************
	/// 获得  指令对象路径
	/// @Author	Gan Shu Long
	/// @Date	2026-05-08
	/// @Return	 指令对象路径
	//********************************************************************
	std::string GetCommandStationPath();

	//********************************************************************
	/// 设置 接收对象路径
	/// @Author	Gan Shu Long
	/// @Date	2026-05-08
	/// @Input
	/// @Param	strReceiveStationPath  接收对象路径
	/// @Return	
	//********************************************************************
	void SetReceiveStationPath(std::string strReceiveStationPath);

	//********************************************************************
	/// 获得  接收对象路径
	/// @Author	Gan Shu Long
	/// @Date	2026-05-08
	/// @Return	 接收对象路径
	//********************************************************************
	std::string GetReceiveStationPath();

	//********************************************************************
	/// 设置 指令准备时间
	/// @Author	Gan Shu Long
	/// @Date	2026-05-08
	/// @Input
	/// @Param	dCommandPrepTime  指令准备时间
	/// @Return	
	//********************************************************************
	void SetCommandPrepTime(double dCommandPrepTime);

	//********************************************************************
	/// 获得  指令准备时间
	/// @Author	Gan Shu Long
	/// @Date	2026-05-08
	/// @Return	 指令准备时间
	//********************************************************************
	double GetCommandPrepTime();

	//********************************************************************
	/// 设置 指令传输时间
	/// @Author	Gan Shu Long
	/// @Date	2026-05-08
	/// @Input
	/// @Param	dCommandTime  指令传输时间
	/// @Return	
	//********************************************************************
	void SetCommandTime(double dCommandTime);

	//********************************************************************
	/// 获得  指令传输时间
	/// @Author	Gan Shu Long
	/// @Date	2026-05-08
	/// @Return	 指令传输时间
	//********************************************************************
	double GetCommandTime();

	//********************************************************************
	/// 设置 数据准备时间
	/// @Author	Gan Shu Long
	/// @Date	2026-05-08
	/// @Input
	/// @Param	dPreCollectionTime  数据准备时间
	/// @Return	
	//********************************************************************
	void SetPreCollectionTime(double dPreCollectionTime);

	//********************************************************************
	/// 获得  数据准备时间
	/// @Author	Gan Shu Long
	/// @Date	2026-05-08
	/// @Return	 数据准备时间
	//********************************************************************
	double GetPreCollectionTime();

	//********************************************************************
	/// 设置 数据收集时间
	/// @Author	Gan Shu Long
	/// @Date	2026-05-08
	/// @Input
	/// @Param	dCollectionTime  数据收集时间
	/// @Return	
	//********************************************************************
	void SetCollectionTime(double dCollectionTime);

	//********************************************************************
	/// 获得  数据收集时间
	/// @Author	Gan Shu Long
	/// @Date	2026-05-08
	/// @Return	 数据收集时间
	//********************************************************************
	double GetCollectionTime();

	//********************************************************************
	/// 设置 数据处理时间
	/// @Author	Gan Shu Long
	/// @Date	2026-05-08
	/// @Input
	/// @Param	dPostCollectionTime  数据处理时间
	/// @Return	
	//********************************************************************
	void SetPostCollectionTime(double dPostCollectionTime);

	//********************************************************************
	/// 获得  数据处理时间
	/// @Author	Gan Shu Long
	/// @Date	2026-05-08
	/// @Return	 数据处理时间
	//********************************************************************
	double GetPostCollectionTime();

	//********************************************************************
	/// 设置 下传时间
	/// @Author	Gan Shu Long
	/// @Date	2026-05-08
	/// @Input
	/// @Param	dDownlinkTime  下传时间
	/// @Return	
	//********************************************************************
	void SetDownlinkTime(double dDownlinkTime);

	//********************************************************************
	/// 获得  下传时间
	/// @Author	Gan Shu Long
	/// @Date	2026-05-08
	/// @Return	 下传时间
	//********************************************************************
	double GetDownlinkTime();

	//********************************************************************
	/// 设置 是否允许交叉传输
	/// @Author	Gan Shu Long
	/// @Date	2026-05-08
	/// @Input
	/// @Param	bAllowForwardCrosslink  是否允许交叉传输
	/// @Return	
	//********************************************************************
	void SetAllowForwardCrosslink(bool bAllowForwardCrosslink);

	//********************************************************************
	/// 获得  是否允许交叉传输
	/// @Author	Gan Shu Long
	/// @Date	2026-05-08
	/// @Return	 是否允许交叉传输
	//********************************************************************
	bool GetAllowForwardCrosslink();

	//********************************************************************
	/// 设置 步长
	/// @Author	Gan Shu Long
	/// @Date	2026-05-08
	/// @Input
	/// @Param	dTimeStep  步长
	/// @Return	
	//********************************************************************
	void SetTimeStep(double dTimeStep);

	//********************************************************************
	/// 获得  步长
	/// @Author	Gan Shu Long
	/// @Date	2026-05-08
	/// @Return	 步长
	//********************************************************************
	double GetTimeStep();
};
