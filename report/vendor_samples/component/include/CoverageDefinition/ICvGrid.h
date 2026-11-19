#pragma once
#include "../IAtkObj.h"
#include "../ICEnum.h"
class CCoverageDefinition;
class CObjManage;
class ICvResolution;
class ICvBounds;

///
/// 网格
///
class IATKOBJECT_API ICvGrid
{
public:
    ICvGrid(const ICvGrid&) = delete;
    ICvGrid& operator=(const ICvGrid&) = delete;

	ICvGrid(CCoverageDefinition* pCCoverageDefinition, CObjManage* IAsGObjManage);
	~ICvGrid();

	//********************************************************************
	/// 设置 覆盖区域类型
	/// @Author	Gan Shu Long
	/// @Date	2023-12-9
	/// @Input 
	/// @Param	ECvBounds boundsType 覆盖区域类型
	/// @Return	
	//********************************************************************
	void SetBoundsType(ECvBounds boundsType);

	//********************************************************************
	/// 获得覆盖区域类型
	/// @Author	Gan Shu Long
	/// @Date	2023-12-9
	/// @Return	覆盖区域类型
	//********************************************************************
	ECvBounds GetBoundsType();

	//********************************************************************
	/// 设置 网格分辨率类型
	/// @Author	Gan Shu Long
	/// @Date	2023-12-9
	/// @Input 
	/// @Param	ECvResolution resolutionType 网格分辨率类型
	/// @Return	
	//********************************************************************
	void SetResolutionType(ECvResolution resolutionType);

	//********************************************************************
	/// 获得 网格分辨率类型
	/// @Author	Gan Shu Long
	/// @Date	2023-12-9
	/// @Return	网格分辨率类型
	//********************************************************************
	ECvResolution GetResolutionType();	
	
	//********************************************************************
	/// 获得覆盖区域
	/// @Author	Gan Shu Long
	/// @Date	2026-01-19
	/// @Return	覆盖区域
	//********************************************************************
	ICvBounds* GetBounds() { return m_Bounds; }

	//********************************************************************
	/// 获得 网格分辨率
	/// @Author	Gan Shu Long
	/// @Date	2026-01-19
	/// @Return	网格分辨率
	//********************************************************************
	ICvResolution* GetResolution() { return m_Resolution; }

private:
	CObjManage*								m_IAsGObjManage;                ///<根对象信息
	ICvBounds* m_Bounds;				//覆盖区域
	ICvResolution* m_Resolution;		//网格分辨率

	CCoverageDefinition* m_pCCoverageDefinition;
};
