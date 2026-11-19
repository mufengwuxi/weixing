#pragma once
#include <vector>
#include <string>

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
#	pragma comment(lib, "ws2_32.lib")
#	ifdef ATKCONNECTOR_SHAREDLIB
#    	ifdef ATKCONNECTOR_EXPORT
#			define ATKCONNECTOR_API __declspec(dllexport)
#    	else
#    	    define ATKCONNECTOR_API __declspec(dllimport)
#    	endif // ASTROOBJ_EXPORT
#	else
#		define ATKCONNECTOR_API
#	endif
#	ifndef ATKCONNECTOR_EXPORT
#		ifdef ATKCONNECTOR_SHAREDLIB
#			if defined(_DEBUG)
#				pragma comment(lib, "ATKConnectorDllD.lib")
#			else
#				pragma comment(lib, "ATKConnectorDll.lib")
#			endif
#		else
#			if defined(_DEBUG)
#				pragma comment(lib, "ATKConnectorlibD.lib")
#			else
#				pragma comment(lib, "ATKConnectorlib.lib")
#			endif
#		endif
#	endif
#else // swig封装java，无法解析处理导出宏，需要设为空
#	ifdef _ATKCONNECTOR_SWIG
#		define ATKCONNECTOR_API
#	else
#		define ATKCONNECTOR_API __attribute__((visibility("default")))
#	endif
#endif // WIN32




#ifdef __cplusplus
extern "C" {
#endif

typedef struct CmdResult
{
	// 必须使用成员初始化列表
	CmdResult() : m_vectData() {}   // m_vectData 被默认构造为空 vector

	std::vector<std::string> m_vectData;

	// 为了安全，建议把 Item 加上 const 并保证所有路径都有返回值
	std::string Item(int nIndex) const
	{
		if (nIndex >= 0 && nIndex < static_cast<int>(m_vectData.size()))
			return m_vectData[nIndex];
		return {};                    // 越界返回空字符串
	}
};


ATKCONNECTOR_API void InitConnector();

ATKCONNECTOR_API int atkOpen(const char* szIP = "127.0.0.1", const unsigned short& nPort = 6655);

ATKCONNECTOR_API std::string atkConnect(const int conID, const std::string& strCommand, const std::string& strObjPath, const std::string& strCMDParam="");

ATKCONNECTOR_API void atkClose(const int conID);

ATKCONNECTOR_API std::string atkConnectEx(const int conID, const std::string& strCommand, const std::string& striInput);

ATKCONNECTOR_API int atkExecuteScript(std::string& striInput, std::string& strOutput);

ATKCONNECTOR_API CmdResult atkExecuteCommand(const int conID, const std::string& strCommand, const std::string& strObjPath, const std::string& strCMDParam = "");


#ifdef __cplusplus
}
#endif

