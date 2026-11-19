#pragma once
#include <vector>
#include <string>
#include "../IAtkObj.h"
#include "../ICEnum.h"
class CCrdnTfm;

///
/// 批量坐标转换
///
class IATKOBJECT_API IATKBatchCrdnTransform
{
public:
    IATKBatchCrdnTransform(const IATKBatchCrdnTransform&) = delete;
    IATKBatchCrdnTransform& operator=(const IATKBatchCrdnTransform&) = delete;

	IATKBatchCrdnTransform();
	~IATKBatchCrdnTransform();

	//********************************************************************
	/// 设置 时间类型
	/// @Author	Gan Shu Long
	/// @Date	2026-07-03
	/// @Input
	/// @Param	eTimeType  时间类型
	///	    eUTCTIME 历元时间
	///	    eRELATIVETIME 相对时间
	/// @Return	
	//********************************************************************
	void SetTimeType(ETimeType eTimeType);

	//********************************************************************
	/// 获取 时间类型
	/// @Author	Gan Shu Long
	/// @Date	2026-07-03
	/// @Return	时间类型
	//********************************************************************
	ETimeType GetTimeType();

	//********************************************************************
	/// 设置 历元时间格式类型
	/// @Author	Gan Shu Long
	/// @Date	2026-07-03
	/// @Input
	/// @Param	strUTCTimeType  历元时间格式类型
	///     "UTCG_MM"
	///     "UTCG"
	///     "BJT"
	///     "BDT"
	///     "BDTG"
	///     "GPS"
	///     "GPSG"
	///     "TDTG"
	///     "JDate"
	///     "ModJDate"
	///     "EpSec"
	/// @Return	
	//********************************************************************
	void SetUTCTimeType(std::string strUTCTimeType);

	//********************************************************************
	/// 获取 历元时间格式类型
	/// @Author	Gan Shu Long
	/// @Date	2026-07-03
	/// @Return	历元时间格式类型
	//********************************************************************
	std::string GetUTCTimeType();

	//********************************************************************
	/// 设置 相对时间初试时间
	/// @Author	Gan Shu Long
	/// @Date	2026-07-03
	/// @Input
	/// @Param	strRelativeTimeStart  相对时间初试时间
	/// @Return	RELATIVE
	//********************************************************************
	void SetRelativeTimeStart(std::string strRelativeTimeStart);

	//********************************************************************
	/// 获取 相对时间初试时间
	/// @Author	Gan Shu Long
	/// @Date	2026-07-03
	/// @Return	相对时间初试时间
	//********************************************************************
	std::string GetRelativeTimeStart();

	//********************************************************************
	/// 设置 转换参数列表
	/// @Author	Gan Shu Long
	/// @Date	2026-07-03
	/// @Input
	/// @Param	vectorParams  转换参数列表，{ePosition，eVelocity} =>（时间，位置，速度）
	///     时间必须第一列，不需要设置
	///     ePOSITION	位置
	///     eVELOCITY	速度
	/// @Return	
	//********************************************************************
	bool SetCrdnTransformParam(std::vector<EDataSequence> vectorParams);

	//********************************************************************
	/// 获取 转换参数列表
	/// @Author	Gan Shu Long
	/// @Date	2026-07-03
	/// @Return	转换参数列表
	//********************************************************************
	std::vector<EDataSequence> GetCrdnTransformParam();

	//********************************************************************
	/// 设置 长度单位
	/// @Author	Gan Shu Long
	/// @Date	2026-07-04
	/// @Input
	/// @Param	strUnit  长度单位
	/// @Return	
	//********************************************************************
	void SetDistanceUnit(std::string strUnit);

	//********************************************************************
	/// 获取 长度单位
	/// @Author	Gan Shu Long
	/// @Date	2026-07-07
	/// @Return	长度单位
	//********************************************************************
	std::string GetDistanceUnit();

	//********************************************************************
	/// 设置 时间单位
	/// @Author	Gan Shu Long
	/// @Date	2026-07-04
	/// @Input
	/// @Param	strUnit  时间单位
	/// @Return	
	//********************************************************************
	void SetTimeUnit(std::string strUnit);

	//********************************************************************
	/// 获取 时间单位
	/// @Author	Gan Shu Long
	/// @Date	2026-07-07
	/// @Return	时间单位
	//********************************************************************
	std::string GetTimeUnit();

	//********************************************************************
	/// 设置 批量坐标转换源坐标系
	/// @Author	Gan Shu Long
	/// @Date	2026-07-03
	/// @Input
	/// @Param	strCentralBody  中心天体
	/// @Param	strCoordinateSystem  系统
	/// @Return	设置 是否成功
	//********************************************************************
	bool SetSrcCoordinate(std::string strCentralBody, std::string strCoordinateSystem);

	//********************************************************************
	/// 获取 批量坐标转换源坐标系中心天体
	/// @Author	Gan Shu Long
	/// @Date	2026-09-22
	/// @Output
	/// @Return	中心天体
	//********************************************************************
	std::string GetSrcCentralBody();

	//********************************************************************
	/// 获取 批量坐标转换源坐标系系统
	/// @Author	Gan Shu Long
	/// @Date	2026-09-22
	/// @Output
	/// @Return	系统
	//********************************************************************
	std::string GetSrcCoordinateSystem();

	//********************************************************************
	/// 设置 批量坐标转换目标坐标系
	/// @Author	Gan Shu Long
	/// @Date	2026-07-03
	/// @Input
	/// @Param	strCentralBody  中心天体
	/// @Param	strCoordinateSystem  系统
	/// @Return	
	//********************************************************************
	bool SetDstCoordinate(std::string strCentralBody, std::string strCoordinateSystem);

	//********************************************************************
	/// 获取 批量坐标转换源目标坐标系中心天体
	/// @Author	Gan Shu Long
	/// @Date	2026-09-22
	/// @Output
	/// @Return	中心天体
	//********************************************************************
	std::string GetDstCentralBody();

	//********************************************************************
	/// 获取 批量坐标转换源目标坐标系系统
	/// @Author	Gan Shu Long
	/// @Date	2026-09-22
	/// @Output
	/// @Return	系统
	//********************************************************************
	std::string GetDstCoordinateSystem();
	
	//********************************************************************
	/// 设置 导入文件类型
	/// @Author	Gan Shu Long
	/// @Date	2026-07-03
	/// @Input
	/// @Param	eImportFileType  导入文件类型
	///	    eTxt 文本
	///	    eEPHEMERIS 星历      //导入星历文件时，默认就是相对时间
	/// @Return	
	//********************************************************************
	bool SetImportFileType(EImportExportFileType eImportFileType);

	//********************************************************************
	/// 获取 导入文件类型
	/// @Author	Gan Shu Long
	/// @Date	2026-07-03
	/// @Return	导入文件类型
	//********************************************************************
	EImportExportFileType GetImportFileType();

	//********************************************************************
	/// 设置 导入文件路径 
	/// 注意：导入星历文件内的时间单位坐标系，会覆盖原本的时间单位坐标系设置
	/// @Author	Gan Shu Long
	/// @Date	2026-07-03
	/// @Input
	/// @Param	strImportFilePath  导入文件路径
	/// @Return	
	//********************************************************************
	void SetImportFilePath(std::string strImportFilePath);

	//********************************************************************
	/// 获取 导入文件路径
	/// @Author	Gan Shu Long
	/// @Date	2026-07-03
	/// @Return	导入文件路径
	//********************************************************************
	std::string GetImportFilePath();

	//********************************************************************
	/// 设置 导出文件类型
	/// @Author	Gan Shu Long
	/// @Date	2026-07-03
	/// @Input
	/// @Param	eExportFileType  导出文件类型
	///	    eTxt 文本
	///	    eEPHEMERIS 星历
	/// @Return	
	//********************************************************************
	bool SetExportFileType(EImportExportFileType eExportFileType);

	//********************************************************************
	/// 获取 导出文件类型
	/// @Author	Gan Shu Long
	/// @Date	2026-07-03
	/// @Return	导出文件类型
	//********************************************************************
	EImportExportFileType GetExportFileType();

	//********************************************************************
	/// 批量坐标转换为批量坐标转换数据保存到文件
	/// @Author	Gan Shu Long
	/// @Date	2026-07-03
	/// @Input
	/// @Param	strExportFilePath  导出文件路径
	/// @Return	
	//********************************************************************
	bool Export(std::string strExportFilePath);

private:
	CCrdnTfm*		m_pCCrdnTfm;						//计算内核
	std::string	    m_strUTCTimeType;					//暂存一份历元时间格式，切换回历元时间时使用
};
