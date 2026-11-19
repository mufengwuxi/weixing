#pragma once
#include "../IAtkObj.h"
#include "IPositionSourceData.h"
#include <string>
class CPlanet;


///
/// 行星位置资源类型：中心天体
///
class IATKOBJECT_API IPlPosCentralBody : public IPositionSourceData
{
public:
    IPlPosCentralBody(const IPlPosCentralBody&) = delete;
    IPlPosCentralBody& operator=(const IPlPosCentralBody&) = delete;

	IPlPosCentralBody(CPlanet* pCPlanet);
	~IPlPosCentralBody();

	//********************************************************************
	/// 获得 是否自动重命名
	/// @Author	Gan Shu Long
	/// @Date	2026-03-09
	/// @Return	是否自动重命名
	//********************************************************************
	bool GetAutoRename();

	//********************************************************************
	/// 设置 是否自动重命名
	/// @Author	Gan Shu Long
	/// @Date	2026-03-09
	/// @Input
	/// @Param	bAutoRename  是否自动重命名
	/// @Return	
	//********************************************************************
	void SetAutoRename(bool bAutoRename);

	//********************************************************************
	/// 获得 中央天体名称
	/// @Author	Gan Shu Long
	/// @Date	2026-03-09
	/// @Return	中央天体名称
	std::string GetCentralBody();

	//********************************************************************
	/// 设置 中央天体
	/// @Author	Gan Shu Long
	/// @Date	2026-03-09
	/// @Input
	/// @Param	strCentralBody  中央天体名称
	/// @Return	
	//********************************************************************
	void SetCentralBody(std::string strCentralBody);

private:
	//中央天体名称到编码
	int CentralBodyStr2Code(std::string strCentralBody);
	//中央天体编码到名称
	std::string CentralBodyCode2Str(int codeCentralBody);

	CPlanet* m_pCPlanet;
};

