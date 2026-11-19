#pragma once
#include "../IAtkObject.h"
#include "../ICEnum.h"
#include <vector>
#include <string>
class CObjManage;
struct CObjData;
class CAntenna;
class IAntennaModel;
class ICROrientation;

///
/// 天线
///
class IATKOBJECT_API IAntenna :public IAtkObject
{
public:
    IAntenna(const IAntenna&) = delete;
    IAntenna& operator=(const IAntenna&) = delete;

	IAntenna(CObjData* pObjData, CObjManage* IAsGObjManage, IAtkObjectRoot* Root);
	~IAntenna();

	//********************************************************************
	/// 获得 支持的模型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-30
	/// @Return 支持的模型
	//********************************************************************
	std::vector<std::string> GetSupportedModels();

	//********************************************************************
	/// 按名称设置 模型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-30
	/// @Input
	/// @Param	strEmbeddedModel  模型
	/// [0] 'Gaussian'
	/// [1] 'Isotropic'
	/// [2] 'Parabolic'
	/// [3] 'External Antenna Pattern'
	/// @Return	
	//********************************************************************
	void SetModel(std::string strEmbeddedModel);

	//********************************************************************
	/// 获得  模型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-30
	/// @Return	 模型
	//********************************************************************
	IAntennaModel* GetModel() { return m_Model; }

	//********************************************************************
	/// 获得  指向
	/// @Author	Gan Shu Long
	/// @Date	2026-03-30
	/// @Return	 指向
	//********************************************************************
	ICROrientation* GetOrientation() { return m_Orientation; }

private:
	//********************************************************************
	/// 获得  模型类型字符串
	/// @Author	Gan Shu Long
	/// @Date	2026-03-27
	/// @Input
	/// @Param	eAntennaModelType   模型类型
	/// @Return  模型类型字符串
	//********************************************************************
	std::string ModelTypeToStr(EAntennaModelType eAntennaModelType);

private:
	CAntenna*				m_CAntenna;

	IAntennaModel*			m_Model;						//嵌入式模型
	ICROrientation*			m_Orientation;			//嵌入式模型指向
};
