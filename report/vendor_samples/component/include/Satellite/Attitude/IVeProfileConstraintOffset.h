#pragma once
#include "../../IAtkObj.h"
#include "IVeAttProfile.h"
#include "../../ICEnum.h"
#include <vector>
#include <string>
struct CObjData;
struct CVehAttSegment;

///
/// 偏置姿态
///
class IATKOBJECT_API IVeProfileConstraintOffset : public IVeAttProfile
{
public:
    IVeProfileConstraintOffset(const IVeProfileConstraintOffset&) = delete;
    IVeProfileConstraintOffset& operator=(const IVeProfileConstraintOffset&) = delete;

	IVeProfileConstraintOffset(CObjData* pObjData);
	~IVeProfileConstraintOffset();

	//********************************************************************
	/// 获得支持的偏置姿态类型
	/// @Author	Gan Shu Long
	/// @Date	2026-02-04
	/// @Return	支持的偏置姿态类型
	//********************************************************************
	std::vector<std::string> GetSupportedTypes();

	//********************************************************************
	/// 判断是否支持输入偏置姿态类型
	/// @Author	Gan Shu Long
	/// @Date	2026-02-04
	/// @Input
	/// @Param	eProfileOffset   偏置姿态类型
	/// @Return	是否支持输入偏置类型
	//********************************************************************
	bool IsProfileOffsetTypeSupported(EVeProfileOffset eProfileOffset);

	//********************************************************************
	/// 设置偏置姿态类型
	/// @Author	Gan Shu Long
	/// @Date	2026-02-04
	/// @Input
	/// @Param	eProfileOffset   偏置姿态类型
	/// @Return	
	//********************************************************************
	void SetProfileOffsetType(EVeProfileOffset eProfileOffset);
	
	//********************************************************************
	/// 获得偏置姿态类型
	/// @Author	Gan Shu Long
	/// @Date	2026-02-04
	/// @Return	偏置姿态类型
	//********************************************************************
	EVeProfileOffset GetProfileOffsetType();

	//********************************************************************
	/// 设置四元数
	/// @Author	Gan Shu Long
	/// @Date	2026-02-04
	/// @Input
	/// @Param	dqx
	/// @Param	dqy
	/// @Param	dqz
	/// @Param	dqs
	/// @Return	
	//********************************************************************
	void AssignQuaternion(double dqx, double dqy, double dqz, double dqs);

	//********************************************************************
	/// 获取四元数
	/// @Author	Gan Shu Long
	/// @Date	2026-02-04
	/// @Input
	/// @Param	dqx
	/// @Param	dqy
	/// @Param	dqz
	/// @Param	dqs
	/// @Return	
	//********************************************************************
	void QueryQuaternion(double& dqx, double& dqy, double& dqz, double& dqs);

	//********************************************************************
	/// 设置欧拉角
	/// @Author	Gan Shu Long
	/// @Date	2026-02-04
	/// @Input
	/// @Param	dEuler1
	/// @Param	dEuler2
	/// @Param	dEuler3
	/// @Param	iSeq		转序序列
	/// @Return	
	//********************************************************************
	void AssignEuler(double dEuler1, double dEuler2, double dEuler3, int iSeq);

	//********************************************************************
	/// 获取欧拉角
	/// @Author	Gan Shu Long
	/// @Date	2026-02-04
	/// @Input
	/// @Param	dEuler1
	/// @Param	dEuler2
	/// @Param	dEuler3
	/// @Param	iSeq		转序序列
	/// @Return	
	//********************************************************************
	void QueryEuler(double& dEuler1, double& dEuler2, double& dEuler3, int iSeq);

	//********************************************************************
	/// 设置YPR角
	/// @Author	Gan Shu Long
	/// @Date	2026-02-04
	/// @Input
	/// @Param	dAngle1	偏航角
	/// @Param	dAngle2	俯仰角
	/// @Param	dAngle3	滚转角
	/// @Param	eEYPRSequence		转序序列
	/// @Return	
	//********************************************************************
	void AssignYPR(double dAngle1, double dAngle2, double dAngle3, EYPRSequence eYPRSequence);

	//********************************************************************
	/// 获取YPR角
	/// @Author	Gan Shu Long
	/// @Date	2026-02-04
	/// @Input
	/// @Param	dAngle1	偏航角
	/// @Param	dAngle2	俯仰角
	/// @Param	dAngle3	滚转角
	/// @Param	eYPRSequence		转序序列
	/// @Return	
	//********************************************************************
	void QueryYPR(double& dAngle1, double& dAngle2, double& dAngle3, EYPRSequence eYPRSequence);

private:
	//********************************************************************
	/// 设置四元数到底层对象	底层只保存四元数值
	/// @Author	Gan Shu Long
	/// @Date	2026-02-04
	/// @Input
	/// @Param	dqx
	/// @Param	dqy
	/// @Param	dqz
	/// @Param	dqs
	/// @Return	
	//********************************************************************
	void SetQuaternionToCobj(double dqx, double dqy, double dqz, double dqs);

	//********************************************************************
	/// 设置转序序列到底层对象
	/// @Author	Gan Shu Long
	/// @Date	2026-02-04
	/// @Input
	/// @Param	iSeq 转序序列
	/// @Return	
	//********************************************************************
	void SetSeqToCobj(int iSeq);

private:
	CObjData*									m_pObjData;
};
