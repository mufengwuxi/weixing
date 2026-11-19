#pragma once
#include "ISnPtTrgtBsight.h"
#include "../ICEnum.h"
struct CObjData;
class CSensor;

///
/// 瞄准类型：追踪
///
class IATKOBJECT_API ISnPtTrgtBsightTrack : public ISnPtTrgtBsight
{
public:
    ISnPtTrgtBsightTrack(const ISnPtTrgtBsightTrack&) = delete;
    ISnPtTrgtBsightTrack& operator=(const ISnPtTrgtBsightTrack&) = delete;

	ISnPtTrgtBsightTrack(CObjData* pCObjData);
	~ISnPtTrgtBsightTrack();

	//********************************************************************
	/// 设置 追踪模式
	/// @Author	Gan Shu Long
	/// @Date	2026-03-23
	/// @Input
	/// @Param	eTrackModeType  追踪模式
	/// @Return	
	//********************************************************************
	void SetTrackMode(ETrackModeType eTrackModeType);

	//********************************************************************
	/// 获得 追踪模式
	/// @Author	Gan Shu Long
	/// @Date	2026-03-23
	/// @Return	追踪模式
	//********************************************************************
	ETrackModeType GetTrackMode();

	/// 设置 视轴类型 todo
	/// 获得 视轴类型 todo

private:
	CSensor*			m_pCSensor;
};
