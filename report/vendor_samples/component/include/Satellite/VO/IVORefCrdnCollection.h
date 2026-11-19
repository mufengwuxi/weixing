#pragma once
#include <string>
#include "../../IAtkObj.h"
#include "../../ICEnum.h"
struct CObjData;
class CVehicleGfx3D;
class CFacilityGfx3D;
class CSensorGfx3D;
class CObjectBase;
class CVGTBase;
class IVORefCrdn;

///
/// 三维向量集合
///
class IATKOBJECT_API IVORefCrdnCollection
{
public:
    IVORefCrdnCollection(const IVORefCrdnCollection&) = delete;
    IVORefCrdnCollection& operator=(const IVORefCrdnCollection&) = delete;

	IVORefCrdnCollection(CObjData* pObjData, CVehicleGfx3D* pCVehGfx3D);
	IVORefCrdnCollection(CObjData* pObjData, CFacilityGfx3D* pCFacGfx3D);
	IVORefCrdnCollection(CObjData* pObjData, CSensorGfx3D* pCSenGfx3D);
	~IVORefCrdnCollection();

	//********************************************************************
	/// 添加三维向量元素
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	eType  向量类型
	/// @Param	strName  向量名称
	/// @Return	向量元素	属性指针
	//********************************************************************
	IVORefCrdn* Add(EGeometricElemType eType, std::string strName);

	//********************************************************************
	/// 获得指定三维向量元素
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	eType  向量类型
	/// @Param	strName  向量名称
	/// @Return	向量元素	属性指针
	//********************************************************************
	IVORefCrdn* GetCrdnByName(EGeometricElemType eType, std::string strName);

	//********************************************************************
	/// 移除指定三维向量元素
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	nIndex  第nIndex向量元素
	/// @Return		
	//********************************************************************
	void Remove(int nIndex);

	//********************************************************************
	/// 移除所有三维向量元素
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return		
	//********************************************************************
	void RemoveAll();

	//********************************************************************
	/// 移除指定三维向量元素
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	eType  向量类型
	/// @Param	strName  向量名称
	/// @Return	
	//********************************************************************
	void RemoveByName(EGeometricElemType Type, std::string strName);

	//********************************************************************
	/// 获得可支持的向量名称
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	可支持的向量名称	
	//********************************************************************
	std::vector<std::string> GetAvailableCrdns();

	//********************************************************************
	/// 获得集合个数
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Return	集合个数	
	//********************************************************************
	int GetCount();

	//********************************************************************
	/// 获得指定三维向量元素
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	nIndex  第nIndex三维向量元素
	/// @Return	指定三维向量元素
	//********************************************************************
	IVORefCrdn* Item(int nIndex);

private:
	CObjData*						m_pObjData;

	CVehicleGfx3D*              m_pCVehGfx3D;		//Vehicle三维向量指针
	CFacilityGfx3D*             m_pCFacGfx3D;		//Facility三维向量指针
	CSensorGfx3D*               m_pCSenGfx3D;		//Sensor三维向量指针


	std::vector<IVORefCrdn*>		m_vecVORefCrdn;     //元素数组


	//********************************************************************
	/// 获得输入字符串中VGT天体对象及名称
	/// @Author	Dong Min
	/// @Date	2023-11-3
	/// @Input
	/// @Param	strInput  输入特定格式字符串，例CentralBody/Earth Earth(True) Vector
	/// @Output
	/// @Param	pCB  对象指针，例Earth指针
	/// @Param	pVGTBase  VGT指针，例Earth(True)指针 
	/// @Param	type  VGT类型，例输入Vector，type类型为eVectorElem
	/// @Return	指定三维向量元素
	//********************************************************************
	bool GetVGTCBAndName(std::string strInput,
		CObjectBase* &pCB,
		CVGTBase* &pVGTBase,
		EGeometricElemType& type);

};

