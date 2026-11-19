#pragma once
#include "IAtkObject.h"


//********************************************************************
/// 设置调用语言类型 
/// @Author	ganshulong
/// @Date	2024-07-22
/// @Input	调用语言类型
/// @Return	空
IATKOBJECT_API void SetCallCodeType(std::string strCallCodeType);

//********************************************************************
/// 获取调用语言类型 
/// @Author	ganshulong
/// @Date	2024-07-22
/// @Input	空
/// @Return	调用语言类型 
//********************************************************************
IATKOBJECT_API std::string GetCallCodeType();

//********************************************************************
/// 设置保存文件的基础路径，用来兼容处理不同系统/模式/语言情况下，输出位置不统一的问题
/// @Author	ganshulong
/// @Date	2024-04-25
/// @Input	保存文件的基础路径
/// @Return	空
//********************************************************************
IATKOBJECT_API void SetSaveFileBasePath(std::string strPath);

//********************************************************************
/// 获取保存文件的基础路径，用来兼容处理不同系统/模式/语言情况下，输出位置不统一的问题
/// @Author	ganshulong
/// @Date	2024-04-25
/// @Input	保存文件的基础路径
/// @Return	空
//********************************************************************
IATKOBJECT_API std::string GetSaveFileBasePath();
