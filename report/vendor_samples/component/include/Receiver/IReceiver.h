#pragma once
#include "../IAtkObject.h"
#include "IReceiverModel.h"
#include "../ICEnum.h"
class CObjManage;
struct CObjData;
class CReceiver;


///
/// 接收器
///
class IATKOBJECT_API IReceiver :public IAtkObject
{
public:
    IReceiver(const IReceiver&) = delete;
    IReceiver& operator=(const IReceiver&) = delete;

	IReceiver(CObjData* pObjData, CObjManage* IAsGObjManage, IAtkObjectRoot* Root);
	~IReceiver();

	//********************************************************************
	/// 按名称设置当前接收模型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-10
	/// @Input
	/// @Param	modelName  接收模型名称	
	/// @"Simple Receiver Model"  简单类型
	/// @"Medium Receiver Model"  中等类型
	/// @"Complex Receiver Model"  复杂类型
	/// @Return	
	//********************************************************************
	void SetModel(std::string modelName);

	//********************************************************************
	/// 设置接收模型类型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-10
	/// @Input
	/// @Param	eReceiverModelType  接收模型类型	
	/// @Return	
	//********************************************************************
	void SetModelType(EReceiverModelType eReceiverModelType);

	//********************************************************************
	/// 获得 接收模型类型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-10
	/// @Return	接收模型类型
	//********************************************************************
	EReceiverModelType GetModelType();
	
	//********************************************************************
	/// 获得 模型
	/// @Author	Gan Shu Long
	/// @Date	2026-03-10
	/// @Return	模型
	//********************************************************************
	IReceiverModel* GetModel() { return m_Model; }

private:
	CReceiver*				m_CReceiver;		//保存一份C对象实例，省的每次改动时都需要重新便利寻找
	IReceiverModel*		m_Model;				//模型
};
