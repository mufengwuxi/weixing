# -*- coding: utf-8 -*-

#	Python脚本执行命令(linux环境若装有多个python，需要指定具体版本，避免默认版本过低)
#	python ATKConnectPythonTest.py


#1，使用内嵌式python解释器时，需要将当前路径加入Python系统环境路径
import sys
import os
curPath = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0,curPath)

#2，导入依赖的Python库
import subprocess
import time

#3，导入ATK通信接口模块中需要的接口函数
from ATKConnectPython import atkOpen
from ATKConnectPython import atkConnect
from ATKConnectPython import atkClose

#4，自配置信息
ATKIP		= '127.0.0.1'
ATKPort		= 6655
ATKAutoOpen	= False
WaitTime 	= 30
if 'ATKConnectPython' in curPath:
	# git库源码路径
	ATKPath 	= r'../../../build/windows/x64/release/bin/ATK.exe'
elif 'IntegratingWithATK' in curPath:
	# 发布资源路径
	ATKPath 	= r'../../../../ATK.exe'
else:
	# 无法定位脚本位置，改为收到打开ATK
	ATKAutoOpen = False

#5，轨道快速转移实现函数
def FastTransfer():
	#1，想定新建并设置属性
	atkConnect(conID, 'New', '/ Scenario FastTransfer');
	atkConnect(conID, 'SetAnalysisTimePeriod', '* "5 Nov 2022 00:00:00.000" "7 Nov 2022 00:00:00.000"');
	atkConnect(conID, 'Animate', '* Reset');
	#2，卫星新建与轨道预报设置为机动规划
	atkConnect(conID, 'New', '/ Satellite FastTransfer');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetProp');
	#3，机动规划添加段，新添加卫星会有默认初始段
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer InsertSegment MainSequence.SegmentList.- Propagate');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer InsertSegment MainSequence.SegmentList.- Target_Sequence');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer InsertSegment MainSequence.SegmentList.Target_Sequence.SegmentList.- Maneuver');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer InsertSegment MainSequence.SegmentList.- Propagate');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer InsertSegment MainSequence.SegmentList.- Target_Sequence');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer InsertSegment MainSequence.SegmentList.Target_Sequence1.SegmentList.- Maneuver');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer InsertSegment MainSequence.SegmentList.- Propagate');
	#4，初始段属性设置
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Initial_State.InitialState.Epoch 5 Nov 2022 00:00:00.000 UtCG');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Initial_State.CoordinateType "Modified Keplerian"');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Initial_State.InitialState.Keplerian.sma 6700000 m');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Initial_State.InitialState.Keplerian.ecc 0');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Initial_State.InitialState.Keplerian.inc 0');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Initial_State.InitialState.Keplerian.RAAN 0');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Initial_State.InitialState.Keplerian.w 0');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Initial_State.InitialState.Keplerian.ta 0');
	#5，第一个预报段属性设置
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Propagate.SegmentColor -16776961');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Propagate.StoppingConditions Duration');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Propagate.StoppingConditions.Duration.TripValue 7200 sec');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Propagate.StoppingConditions.Duration.Tolerance 0.0001 sec');
	#6，第一个瞄准段中机动段属性设置
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Target_Sequence.SegmentList.Maneuver.SegmentColor 4278212095');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Target_Sequence.SegmentList.Maneuver.ImpulsiveMnvr.ThrustAxes "Satellite/FastTransfer VNC(Earth)"');
	#7，第一个瞄准段添加属性页
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer AddMCSSegmentControl MainSequence.SegmentList.Target_Sequence.SegmentList.Maneuver ImpulsiveMnvr.Cartesian.X');
	#8，设置属性页中控制变量属性
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Target_Sequence.Profiles Differential_Corrector');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetMCSControlValue MainSequence.SegmentList.Target_Sequence.Profiles.Differential_Corrector Maneuver ImpulsiveMnvr.Cartesian.X Active true');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetMCSControlValue MainSequence.SegmentList.Target_Sequence.Profiles.Differential_Corrector Maneuver ImpulsiveMnvr.Cartesian.X MaxStep 1000');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetMCSControlValue MainSequence.SegmentList.Target_Sequence.Profiles.Differential_Corrector Maneuver ImpulsiveMnvr.Cartesian.X Pert 0.1');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetMCSControlValue MainSequence.SegmentList.Target_Sequence.Profiles.Differential_Corrector Maneuver ImpulsiveMnvr.Cartesian.X Scale 1');
	#9，设置属性页中约束条件属性
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Target_Sequence.SegmentList.Maneuver.Results "Radius Of Apoapsis"');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetMCSConstraintValue MainSequence.SegmentList.Target_Sequence.Profiles.Differential_Corrector Maneuver StateCalcRadiusOfApoapsis Active true');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetMCSConstraintValue MainSequence.SegmentList.Target_Sequence.Profiles.Differential_Corrector Maneuver StateCalcRadiusOfApoapsis Tolerance 0.1 m');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetMCSConstraintValue MainSequence.SegmentList.Target_Sequence.Profiles.Differential_Corrector Maneuver StateCalcRadiusOfApoapsis Scale 1 m');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetMCSConstraintValue MainSequence.SegmentList.Target_Sequence.Profiles.Differential_Corrector Maneuver StateCalcRadiusOfApoapsis Weight 1');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetMCSConstraintValue MainSequence.SegmentList.Target_Sequence.Profiles.Differential_Corrector Maneuver StateCalcRadiusOfApoapsis Desired 84328394 m');
	#10，第二个预报段属性设置
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Propagate1.SegmentColor -16711936');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Propagate1.StoppingConditions R_Magnitude');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Propagate1.StoppingConditions.R_Magnitude.TripValue 42164197 m');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Propagate1.StoppingConditions.R_Magnitude.Tolerance 1e-6 m');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Propagate1.StoppingConditions.R_Magnitude.RepeatCount 1');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Propagate1.StoppingConditions.R_Magnitude.Condition "Cross Either (Inc.or Dec.)"');
	#11，第二个瞄准段中机动段属性设置
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Target_Sequence1.SegmentList.Maneuver.SegmentColor -16711681');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Target_Sequence1.SegmentList.Maneuver.ImpulsiveMnvr.ThrustAxes "Satellite VNC(Earth)"');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer AddMCSSegmentControl MainSequence.SegmentList.Target_Sequence1.SegmentList.Maneuver ImpulsiveMnvr.Cartesian.X');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer AddMCSSegmentControl MainSequence.SegmentList.Target_Sequence1.SegmentList.Maneuver ImpulsiveMnvr.Cartesian.Z');
	#12，第二个瞄准段添加属性页
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Target_Sequence1.Profiles Differential_Corrector');
	#13，设置属性页中控制变量属性
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetMCSControlValue MainSequence.SegmentList.Target_Sequence1.Profiles.Differential_Corrector Maneuver ImpulsiveMnvr.Cartesian.X Active true');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetMCSControlValue MainSequence.SegmentList.Target_Sequence1.Profiles.Differential_Corrector Maneuver ImpulsiveMnvr.Cartesian.X MaxStep 1000');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetMCSControlValue MainSequence.SegmentList.Target_Sequence1.Profiles.Differential_Corrector Maneuver ImpulsiveMnvr.Cartesian.X Pert 0.1');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetMCSControlValue MainSequence.SegmentList.Target_Sequence1.Profiles.Differential_Corrector Maneuver ImpulsiveMnvr.Cartesian.X Scale 1');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetMCSControlValue MainSequence.SegmentList.Target_Sequence1.Profiles.Differential_Corrector Maneuver ImpulsiveMnvr.Cartesian.Z Active true');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetMCSControlValue MainSequence.SegmentList.Target_Sequence1.Profiles.Differential_Corrector Maneuver ImpulsiveMnvr.Cartesian.Z MaxStep 1000');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetMCSControlValue MainSequence.SegmentList.Target_Sequence1.Profiles.Differential_Corrector Maneuver ImpulsiveMnvr.Cartesian.Z Pert 0.1');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetMCSControlValue MainSequence.SegmentList.Target_Sequence1.Profiles.Differential_Corrector Maneuver ImpulsiveMnvr.Cartesian.Z Scale 1');
	#14，设置属性页中约束条件属性
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Target_Sequence1.SegmentList.Maneuver.Results Eccentricity');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetMCSConstraintValue MainSequence.SegmentList.Target_Sequence1.Profiles.Differential_Corrector Maneuver StateCalcEccentricity Active true');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetMCSConstraintValue MainSequence.SegmentList.Target_Sequence1.Profiles.Differential_Corrector Maneuver StateCalcEccentricity Desired 0');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetMCSConstraintValue MainSequence.SegmentList.Target_Sequence1.Profiles.Differential_Corrector Maneuver StateCalcEccentricity Scale 1');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetMCSConstraintValue MainSequence.SegmentList.Target_Sequence1.Profiles.Differential_Corrector Maneuver StateCalcEccentricity tolerance 0.001');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetMCSConstraintValue MainSequence.SegmentList.Target_Sequence1.Profiles.Differential_Corrector Maneuver StateCalcEccentricity Weight 1');
	#15，第三个预报段属性设置
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Propagate2.SegmentColor -65536');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Propagate2.StoppingConditions Duration');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Propagate2.StoppingConditions.Duration.TripValue 86400 sec');
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Propagate2.StoppingConditions.Duration.Tolerance 0.0001 sec');
	#16，机动规划运行
	atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer RunMCS');
	#17，查看报告数据
	print(atkConnect(conID, 'Report_RM', '*/Satellite/FastTransfer Style "Position" TimePeriod "5 Nov 2022 00:00:00.000" "7 Nov 2022 00:00:00.000"'))
	#18，想定保存
	atkConnect(conID, 'Save', '/ *');

#5，北斗3实现函数
def BeiDou3():
	atkConnect(conID, 'New', '/ Scenario BeiDou3');
	atkConnect(conID, 'SetAnalysisTimePeriod', '* "14 Jul 2021 04:00:00.00" "15 Jul 2021 04:00:00.00"');
	atkConnect(conID, 'Animate', '* Reset');
	atkConnect(conID, 'NewMulti', '/ Satellite 30 M1 M2 M3 M4 M5 M6 M7 M8 M9 M10 M11 M12 M13 M14 M15 M16 M17 M18 M19 M20 M21 M22 M23 M24 G1 G2 G3 IGSO-1 IGSO-2 IGSO-3', 'CentralBody Earth');
	atkConnect(conID, 'SetState', '*/Satellite/M1 SGP4 NoProp 60.0 43001 TLESource DefineElements Source Elements OrbitEpochYYDDD 2021193.899 ElementSet 1.86231812 0.0011522 55.3739 293.8842 123.0806 178.195 0 IntlDesignator 17069A_ MeanMotionDot 1.5e-07 RevNumber 2507 Classification U');
	atkConnect(conID, 'SetState', '*/Satellite/M2 SGP4 NoProp 60.0 43002 TLESource DefineElements Source Elements OrbitEpochYYDDD 2021193.837 ElementSet 1.86231373 0.0010026 55.3743 313.3839 123.1183 160.6398 0 IntlDesignator 17069B_ MeanMotionDot 1.4e-7 RevNumber 2506 Classification U');
	atkConnect(conID, 'SetState', '*/Satellite/M3 SGP4 NoProp 60.0 43107 TLESource DefineElements Source Elements OrbitEpochYYDDD 2021193.746 ElementSet 1.86232545 0.0005582 55.3813 57.895 4.2652 161.2004 0 IntlDesignator 18003A_ MeanMotionDot -7.1e-7 RevNumber 2382 Classification U');
	atkConnect(conID, 'SetState', '*/Satellite/M4 SGP4 NoProp 60.0 43108 TLESource DefineElements Source Elements OrbitEpochYYDDD 2021193.616 ElementSet 1.86232531 0.000207 55.3757 228.2601 4.2584 308.6749 0 IntlDesignator 18003B_ MeanMotionDot -7.199999999999999e-07 RevNumber 2382 Classification U');
	atkConnect(conID, 'SetState', '*/Satellite/M5 SGP4 NoProp 60.0 43207 TLESource DefineElements Source Elements OrbitEpochYYDDD 2021193.263 ElementSet 1.86231721 0.0007739 55.33280000000001 330.9078 123.2279 29.0384 0 IntlDesignator 18018A_ MeanMotionDot 1e-07 RevNumber 2320 Classification U');
	atkConnect(conID, 'SetState', '*/Satellite/M6 SGP4 NoProp 60.0 43208 TLESource DefineElements Source Elements OrbitEpochYYDDD 2021192.792 ElementSet 1.86231608 0.0009783000000000001 55.33280000000001 304.4366 123.2488 55.4571 0 IntlDesignator 18018B_ MeanMotionDot 5.999999999999999e-08 RevNumber 2319 Classification U');
	atkConnect(conID, 'SetState', '*/Satellite/M7 SGP4 NoProp 60.0 43245 TLESource DefineElements Source Elements OrbitEpochYYDDD 2021193.602 ElementSet 1.86232781 0.0001279 55.30910000000001 86.8387 2.059 305.6236 0 IntlDesignator 18029A_ MeanMotionDot -7.199999999999999e-07 RevNumber 2236 Classification U');
	atkConnect(conID, 'SetState', '*/Satellite/M8 SGP4 NoProp 60.0 43246 TLESource DefineElements Source Elements OrbitEpochYYDDD 2021193.814 ElementSet 1.86233242 0.000376 55.308 8.3653 2.083200000000001 212.7044 0 IntlDesignator 18029B_ MeanMotionDot -7.199999999999999e-07 RevNumber 2235 Classification U');
	atkConnect(conID, 'SetState', '*/Satellite/M9 SGP4 NoProp 60.0 43581 TLESource DefineElements Source Elements OrbitEpochYYDDD 2021193.311 ElementSet 1.86231886 0.0005243 54.3911 262.3791 243.7451 190.9816 0 IntlDesignator 18062A_ MeanMotionDot 5.8e-07 RevNumber 2009 Classification U');
	atkConnect(conID, 'SetState', '*/Satellite/M10 SGP4 NoProp 60.0 43582 TLESource DefineElements Source Elements OrbitEpochYYDDD 2021193.709 ElementSet 1.86232339 0.0004247 54.3905 330.6636 243.7211 121.1487 0 IntlDesignator 18062B_ MeanMotionDot 5.6e-07 RevNumber 2010 Classification U');
	atkConnect(conID, 'SetState', '*/Satellite/M11 SGP4 NoProp 60.0 43602 TLESource DefineElements Source Elements OrbitEpochYYDDD 2021193.709 ElementSet 1.8623169 0.0006347 54.4671 352.5083 242.3057 141.6904 0 IntlDesignator 18067A_ MeanMotionDot 5.8e-07 RevNumber 1959 Classification U');
	atkConnect(conID, 'SetState', '*/Satellite/M12 SGP4 NoProp 60.0 43603 TLESource DefineElements Source Elements OrbitEpochYYDDD 2021193.241 ElementSet 1.86231647 0.0003917 54.4648 334.6925 242.2882 115.7957 0 IntlDesignator 18067B_ MeanMotionDot 6e-07 RevNumber 1960 Classification U');
	atkConnect(conID, 'SetState', '*/Satellite/M13 SGP4 NoProp 60.0 43622 TLESource DefineElements Source Elements OrbitEpochYYDDD 2021193.332 ElementSet 1.86231873 0.0007935 55.2982 292.0966 122.7925 249.7309 0 IntlDesignator 18072A_ MeanMotionDot 1e-07 RevNumber 1911 Classification U');
	atkConnect(conID, 'SetState', '*/Satellite/M14 SGP4 NoProp 60.0 43623 TLESource DefineElements Source Elements OrbitEpochYYDDD 2021193.76 ElementSet 1.86231887 0.0006366 55.2994 313.5419 122.7688 245.5682 0 IntlDesignator 18072B_ MeanMotionDot 1.3e-07 RevNumber 1912 Classification U');
	atkConnect(conID, 'SetState', '*/Satellite/M15 SGP4 NoProp 60.0 43647 TLESource DefineElements Source Elements OrbitEpochYYDDD 2021190.181 ElementSet 1.86232758 0.0006537 55.2035 9.1 2.3644 205.9087 0 IntlDesignator 18078A_ MeanMotionDot -8.499999999999999e-07 RevNumber 1856 Classification U');
	atkConnect(conID, 'SetState', '*/Satellite/M16 SGP4 NoProp 60.0 43648 TLESource DefineElements Source Elements OrbitEpochYYDDD 2021193.775 ElementSet 1.86232633 0.0004398 55.2014 24.3801 2.2485 348.1599 0 IntlDesignator 18078B_ MeanMotionDot -7.199999999999999e-07 RevNumber 1865 Classification U');
	atkConnect(conID, 'SetState', '*/Satellite/M17 SGP4 NoProp 60.0 43706 TLESource DefineElements Source Elements OrbitEpochYYDDD 2021192.836 ElementSet 1.86231926 0.0011321 54.4724 266.5938 243.6213 93.3725 0 IntlDesignator 18093A_ MeanMotionDot 6e-07 RevNumber 1799 Classification U');
	atkConnect(conID, 'SetState', '*/Satellite/M18 SGP4 NoProp 60.0 43707 TLESource DefineElements Source Elements OrbitEpochYYDDD 2021193.137 ElementSet 1.86232189 0.0009158 54.4725 287.2459 243.6014 5.6998 0 IntlDesignator 18093B_ MeanMotionDot 5.9e-07 RevNumber 1798 Classification U');
	atkConnect(conID, 'SetState', '*/Satellite/M19 SGP4 NoProp 60.0 44864 TLESource DefineElements Source Elements OrbitEpochYYDDD 2021193.412 ElementSet 1.86231929 0.0017275 55.23250000000001 270.4753 123.0187 10.5842 0 IntlDesignator 19090A_ MeanMotionDot 1.1e-07 RevNumber 1069 Classification U');
	atkConnect(conID, 'SetState', '*/Satellite/M20 SGP4 NoProp 60.0 44865 TLESource DefineElements Source Elements OrbitEpochYYDDD 2021193.551 ElementSet 1.86231947 0.0016138 55.2355 284.9972 123.0454 179.3814 0 IntlDesignator 19090B_ MeanMotionDot 1.2e-07 RevNumber 1067 Classification U');
	atkConnect(conID, 'SetState', '*/Satellite/M21 SGP4 NoProp 60.0 44793 TLESource DefineElements Source Elements OrbitEpochYYDDD 2021193.415 ElementSet 1.8623249 0.000636 55.0537 15.7226 2.3161 161.625 0 IntlDesignator 19078A_ MeanMotionDot -7.3e-07 RevNumber 1111 Classification U');
	atkConnect(conID, 'SetState', '*/Satellite/M22 SGP4 NoProp 60.0 44794 TLESource DefineElements Source Elements OrbitEpochYYDDD 2021193.549 ElementSet 1.86232875 0.0004516 55.0274 19.952 2.3109 158.3043 0 IntlDesignator 19078B_ MeanMotionDot -7.3e-07 RevNumber 1111 Classification U');
	atkConnect(conID, 'SetState', '*/Satellite/M23 SGP4 NoProp 60.0 44542 TLESource DefineElements Source Elements OrbitEpochYYDDD 2021189.967 ElementSet 1.86231295 0.0008074999999999999 54.6982 317.908 243.7944 322.7966 0 IntlDesignator 19061A_ MeanMotionDot 6.7e-07 RevNumber 1218 Classification U');
	atkConnect(conID, 'SetState', '*/Satellite/M24 SGP4 NoProp 60.0 44543 TLESource DefineElements Source Elements OrbitEpochYYDDD 2021193.819 ElementSet 1.86231721 0.0005749 54.6952 301.4509 243.6322 312.6179 0 IntlDesignator 19061B_ MeanMotionDot 5.7e-07 RevNumber 1225 Classification U');
	atkConnect(conID, 'SetState', '*/Satellite/G1 SGP4 NoProp 60.0 43683 TLESource DefineElements Source Elements OrbitEpochYYDDD 2021193.71 ElementSet 1.00275341 0.0002815 1.0196 6.3665 224.1125 95.7497 0 IntlDesignator 18085A_ MeanMotionDot -2.73e-06 RevNumber 1000 Classification U');
	atkConnect(conID, 'SetState', '*/Satellite/G2 SGP4 NoProp 60.0 45344 TLESource DefineElements Source Elements OrbitEpochYYDDD 2021193.882 ElementSet 1.00271795 0.0004114 2.3956 135.7595 337.4421 215.3278 0 IntlDesignator 20017A_ MeanMotionDot -1.34e-06 RevNumber 521 Classification U');
	atkConnect(conID, 'SetState', '*/Satellite/G3 SGP4 NoProp 60.0 45807 TLESource DefineElements Source Elements OrbitEpochYYDDD 2021193.728 ElementSet 1.00272294 0.0004559 2.2238 348.4824 298.6231 16.4122 0 IntlDesignator 20040A_ MeanMotionDot -3.47e-06 RevNumber 403 Classification U'); 
	atkConnect(conID, 'SetState', '*/Satellite/IGSO-1 SGP4 NoProp 60.0 44204 TLESource DefineElements Source Elements OrbitEpochYYDDD 2021193.289 ElementSet 1.00288161 0.0018678 56.0526 190.8953 56.6488 260.1822 0 IntlDesignator 19023A_ MeanMotionDot -2.04e-06 RevNumber 830 Classification U');
	atkConnect(conID, 'SetState', '*/Satellite/IGSO-2 SGP4 NoProp 60.0 44337 TLESource DefineElements Source Elements OrbitEpochYYDDD 2021193.381 ElementSet 1.00274279 0.0020165 55.0957 194.422 174.7703 166.2423 0 IntlDesignator 19035A_ MeanMotionDot -8.6e-07 RevNumber 764 Classification U');
	atkConnect(conID, 'SetState', '*/Satellite/IGSO-3 SGP4 NoProp 60.0 44709 TLESource DefineElements Source Elements OrbitEpochYYDDD 2021193.73 ElementSet 1.00269456 0.0020755 57.8019 200.4847 299.8988 160.1392 0 IntlDesignator 19073A_ MeanMotionDot -1.65e-06 RevNumber 640 Classification U');
	atkConnect(conID, 'Animate', '* Reset');
	atkConnect(conID, 'AllInstanceNames', '/');
	atkConnect(conID, 'Animate', '* Start');
	atkConnect(conID, 'Save', '/ *');

#5，霍曼转移实现函数
def HohmannTransfer():
	#1，想定新建并设置属性
	atkConnect(conID, 'New', '/ Scenario HohmannTransfer');
	atkConnect(conID, 'SetAnalysisTimePeriod', '* "5 Nov 2022 00:00:00.000" "7 Nov 2022 00:00:00.000"');
	atkConnect(conID, 'Animate', '* Reset');
	#2，卫星新建，设置轨道预报类型为机动规划，以及二维视图线宽与轨迹类型显示设置
	atkConnect(conID, 'New', '/ Satellite Sat_Hohmann');
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetProp');
	atkConnect(conID, 'Graphics', '*/Satellite/Sat_Hohmann Basic LineWidth 3');
	atkConnect(conID, 'Graphics', '*/Satellite/Sat_Hohmann Pass2D GrndTrail All');
	#3，机动规划添加段，新添加卫星会有默认初始段
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann InsertSegment MainSequence.SegmentList.- Propagate');
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann InsertSegment MainSequence.SegmentList.- Target_Sequence');
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann InsertSegment MainSequence.SegmentList.Target_Sequence.SegmentList.- Maneuver');
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann InsertSegment MainSequence.SegmentList.- Propagate');
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann InsertSegment MainSequence.SegmentList.- Target_Sequence');
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann InsertSegment MainSequence.SegmentList.Target_Sequence1.SegmentList.- Maneuver');
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann InsertSegment MainSequence.SegmentList.- Propagate');
	#4，初始段属性设置
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetValue MainSequence.SegmentList.Initial_State.InitialState.Epoch 5 Nov 2022 00:00:00.000 UtCG');
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetValue MainSequence.SegmentList.Initial_State.CoordinateType "Modified Keplerian"');
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetValue MainSequence.SegmentList.Initial_State.InitialState.Keplerian.sma 6700000 m');
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetValue MainSequence.SegmentList.Initial_State.InitialState.Keplerian.ecc 0');
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetValue MainSequence.SegmentList.Initial_State.InitialState.Keplerian.inc 0');
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetValue MainSequence.SegmentList.Initial_State.InitialState.Keplerian.RAAN 0');
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetValue MainSequence.SegmentList.Initial_State.InitialState.Keplerian.w 0');
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetValue MainSequence.SegmentList.Initial_State.InitialState.Keplerian.ta 0');
	#5，第一个预报段属性设置
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetValue MainSequence.SegmentList.Propagate.SegmentColor 4278190335');
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetValue MainSequence.SegmentList.Propagate.StoppingConditions Duration');
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetValue MainSequence.SegmentList.Propagate.StoppingConditions.Duration.TripValue 7200 sec');
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetValue MainSequence.SegmentList.Propagate.StoppingConditions.Duration.Tolerance 0.0001 sec');
	#6，第一个瞄准段中机动段属性设置
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetValue MainSequence.SegmentList.Target_Sequence.SegmentList.Maneuver.SegmentColor 4278212095');
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetValue MainSequence.SegmentList.Target_Sequence.SegmentList.Maneuver.ImpulsiveMnvr.ThrustAxes "Satellite VNC(Earth)"');
	#7，第一个瞄准段添加属性页
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann AddMCSSegmentControl MainSequence.SegmentList.Target_Sequence.SegmentList.Maneuver ImpulsiveMnvr.Cartesian.X');
	#8，设置属性页中控制变量属性
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetValue MainSequence.SegmentList.Target_Sequence.Profiles Differential_Corrector');
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetMCSControlValue MainSequence.SegmentList.Target_Sequence.Profiles.Differential_Corrector Maneuver ImpulsiveMnvr.Cartesian.X Active true');
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetMCSControlValue MainSequence.SegmentList.Target_Sequence.Profiles.Differential_Corrector Maneuver ImpulsiveMnvr.Cartesian.X MaxStep 1000 m/sec');
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetMCSControlValue MainSequence.SegmentList.Target_Sequence.Profiles.Differential_Corrector Maneuver ImpulsiveMnvr.Cartesian.X Pert 0.1 m/sec');
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetMCSControlValue MainSequence.SegmentList.Target_Sequence.Profiles.Differential_Corrector Maneuver ImpulsiveMnvr.Cartesian.X Scale 1 m/sec');
	#9，设置属性页中约束条件属性
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetValue MainSequence.SegmentList.Target_Sequence.SegmentList.Maneuver.Results "Radius Of Apoapsis"');
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetMCSConstraintValue MainSequence.SegmentList.Target_Sequence.Profiles.Differential_Corrector Maneuver StateCalcRadiusOfApoapsis Active true');
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetMCSConstraintValue MainSequence.SegmentList.Target_Sequence.Profiles.Differential_Corrector Maneuver StateCalcRadiusOfApoapsis Tolerance 0.1 m');
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetMCSConstraintValue MainSequence.SegmentList.Target_Sequence.Profiles.Differential_Corrector Maneuver StateCalcRadiusOfApoapsis Scale 1 m');
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetMCSConstraintValue MainSequence.SegmentList.Target_Sequence.Profiles.Differential_Corrector Maneuver StateCalcRadiusOfApoapsis Weight 1');
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetMCSConstraintValue MainSequence.SegmentList.Target_Sequence.Profiles.Differential_Corrector Maneuver StateCalcRadiusOfApoapsis Desired 42238000 m');
	#10，第二个预报段属性设置
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetValue MainSequence.SegmentList.Propagate1.SegmentColor 4294902015');
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetValue MainSequence.SegmentList.Propagate1.StoppingConditions Apoapsis');
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetValue MainSequence.SegmentList.Propagate1.StoppingConditions.Apoapsis.RepeatCount 1');
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetValue MainSequence.SegmentList.Propagate1.StoppingConditions.Apoapsis.Tolerance 0.0001');
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetValue MainSequence.SegmentList.Propagate1.StoppingConditions.Apoapsis.CalcObjectAttributes.CentralBody Earth');
	#11，第二个瞄准段中机动段属性设置
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetValue MainSequence.SegmentList.Target_Sequence1.SegmentList.Maneuver.SegmentColor -256');
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetValue MainSequence.SegmentList.Target_Sequence1.SegmentList.Maneuver.ImpulsiveMnvr.ThrustAxes "Satellite VNC(Earth)"');
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann AddMCSSegmentControl MainSequence.SegmentList.Target_Sequence1.SegmentList.Maneuver ImpulsiveMnvr.Cartesian.X');
	#12，第二个瞄准段添加属性页
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetValue MainSequence.SegmentList.Target_Sequence1.Profiles Differential_Corrector');
	#13，设置属性页中控制变量属性
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetMCSControlValue MainSequence.SegmentList.Target_Sequence1.Profiles.Differential_Corrector Maneuver ImpulsiveMnvr.Cartesian.X Active true');
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetMCSControlValue MainSequence.SegmentList.Target_Sequence1.Profiles.Differential_Corrector Maneuver ImpulsiveMnvr.Cartesian.X MaxStep 100 m/sec');
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetMCSControlValue MainSequence.SegmentList.Target_Sequence1.Profiles.Differential_Corrector Maneuver ImpulsiveMnvr.Cartesian.X Pert 0.1 m/sec');
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetMCSControlValue MainSequence.SegmentList.Target_Sequence1.Profiles.Differential_Corrector Maneuver ImpulsiveMnvr.Cartesian.X Scale 1 m/sec');
	#14，设置属性页中约束条件属性
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetValue MainSequence.SegmentList.Target_Sequence1.SegmentList.Maneuver.Results "Eccentricity"');
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetMCSConstraintValue MainSequence.SegmentList.Target_Sequence1.Profiles.Differential_Corrector Maneuver StateCalcEccentricity Active true');
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetMCSConstraintValue MainSequence.SegmentList.Target_Sequence1.Profiles.Differential_Corrector Maneuver StateCalcEccentricity Tolerance 0.001');
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetMCSConstraintValue MainSequence.SegmentList.Target_Sequence1.Profiles.Differential_Corrector Maneuver StateCalcEccentricity Scale 1');
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetMCSConstraintValue MainSequence.SegmentList.Target_Sequence1.Profiles.Differential_Corrector Maneuver StateCalcEccentricity Weight 1');
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetMCSConstraintValue MainSequence.SegmentList.Target_Sequence1.Profiles.Differential_Corrector Maneuver StateCalcEccentricity Desired 0');
	#15，第三个预报段属性设置
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetValue MainSequence.SegmentList.Propagate2.SegmentColor 4278255360');
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetValue MainSequence.SegmentList.Propagate2.StoppingConditions Duration');
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetValue MainSequence.SegmentList.Propagate2.StoppingConditions.Duration.TripValue 86400');
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann SetValue MainSequence.SegmentList.Propagate2.StoppingConditions.Duration.Tolerance 0.0001');
	#16，机动规划运行
	atkConnect(conID, 'Astrogator', '*/Satellite/Sat_Hohmann RunMCS');
	#17，查看报告数据
	print(atkConnect(conID, 'Report_RM', '*/Satellite/Sat_Hohmann Style "Position" TimePeriod "5 Nov 2022 00:00:00.000" "7 Nov 2022 00:00:00.000"'))
	#18，想定保存
	atkConnect(conID, 'Save', '/ *');

#5，可见性覆盖实现函数
def Access():
	atkConnect(conID, 'New', '/ Scenario Scenario1');
	atkConnect(conID, 'SetAnAlysisTimePeriod', '* "14 Mar 2024 00:00:00.000" "21 Mar 2024 00:00:00.000"');
	atkConnect(conID, 'Animate', '* Reset');
	atkConnect(conID, 'New', '/ Satellite Satellite1');
	atkConnect(conID, 'SetStAte', '*/Satellite/Satellite1 TLE "1 53239U 22085A   24073.99998228  .00036791  00000+0  39433-3 0  9991" "2 53239  41.4674 304.8696 0008909 169.2446 190.8585 15.63041866161592"');
	atkConnect(conID, 'New', '/ Satellite Satellite2');
	atkConnect(conID, 'SetStAte', '*/Satellite/Satellite2 TLE "1 54216U 22143A   24073.99998228  .00036791  00000+0  39433-3 0  9992" "2 54216  41.4674 304.8696 0008909 169.2446 190.8585 15.63041866 78067"');
	atkConnect(conID, 'New', '/ Satellite Satellite3');
	atkConnect(conID, 'SetStAte', '*/Satellite/Satellite3 TLE "1 53100U 22078A   24073.26760998  .00000061  00000+0  00000+0 0  9996" "2 53100   1.4751 279.1940 0002414  86.8508 272.2569  1.00273500 6273"');
	atkConnect(conID, 'New', '/ Satellite Satellite4');
	atkConnect(conID, 'SetStAte', '*/Satellite/Satellite4 TLE "1 49011U 21063A   24077.17755980  .00000095  00000+0  00000+0 0  9991" "2 49011   0.5397 280.1351 0002487  55.6215 280.1852  1.00272627 9828"');
	atkConnect(conID, 'New', '/ Facility Facility1');
	atkConnect(conID, 'SetPosition', '*/Facility/Facility1 Geodetic 34.5 109.51 321');
	atkConnect(conID, 'New', '/ Facility Facility2');
	atkConnect(conID, 'SetPosition', '*/Facility/Facility2 Geodetic 42.68 125.53 359');
	atkConnect(conID, 'New', '/ Facility Facility3');
	atkConnect(conID, 'SetPosition', '*/Facility/Facility3 Geodetic 37.11 79.92 1328');
	atkConnect(conID, 'New', '/ Facility Facility4');
	atkConnect(conID, 'SetPosition', '*/Facility/Facility4 Geodetic 39.51 75.93 1255');
	atkConnect(conID, 'New', '/ Facility Facility5');
	atkConnect(conID, 'SetPosition', '*/Facility/Facility5 Geodetic 24.48 118.09 15');
	atkConnect(conID, 'New', '/ Facility Facility6');
	atkConnect(conID, 'SetPosition', '*/Facility/Facility6 Geodetic 40.07 116.26 41');
	atkConnect(conID, 'New', '/ Facility Facility7');
	atkConnect(conID, 'SetPosition', '*/Facility/Facility7 Geodetic 18.31 109.31 18');
	atkConnect(conID, 'New', '/ Facility Facility8');
	atkConnect(conID, 'SetPosition', '*/Facility/Facility8 Geodetic 36.07 120.38 12');
	atkConnect(conID, 'Animate', '* Reset');
	atkConnect(conID, 'Access', '*/Satellite/Satellite1 */Satellite/Satellite3 TimePeriod "14 Mar 2024 00:00:00.000" "21 Mar 2024 00:00:00.000"')
	atkConnect(conID, 'Access', '*/Satellite/Satellite2 */Satellite/Satellite3 TimePeriod "14 Mar 2024 00:00:00.000" "21 Mar 2024 00:00:00.000"')
	atkConnect(conID, 'Access', '*/Satellite/Satellite1 */Satellite/Satellite4 TimePeriod "14 Mar 2024 00:00:00.000" "21 Mar 2024 00:00:00.000"')
	atkConnect(conID, 'Access', '*/Satellite/Satellite2 */Satellite/Satellite4 TimePeriod "14 Mar 2024 00:00:00.000" "21 Mar 2024 00:00:00.000"')
	atkConnect(conID, 'Access', '*/Facility/Facility3 */Satellite/Satellite3 TimePeriod "14 Mar 2024 00:00:00.000" "21 Mar 2024 00:00:00.000"') 
	atkConnect(conID, 'Access', '*/Facility/Facility4 */Satellite/Satellite3 TimePeriod "14 Mar 2024 00:00:00.000" "21 Mar 2024 00:00:00.000"')
	atkConnect(conID, 'Access', '*/Facility/Facility3 */Satellite/Satellite4 TimePeriod "14 Mar 2024 00:00:00.000" "21 Mar 2024 00:00:00.000"') 
	atkConnect(conID, 'Access', '*/Facility/Facility4 */Satellite/Satellite4 TimePeriod "14 Mar 2024 00:00:00.000" "21 Mar 2024 00:00:00.000"')
	atkConnect(conID, 'SAve', '/ *');

#6，判断是否自动打开ATK.exe
if ATKAutoOpen:
	subprocess.Popen([ATKPath])
	print("自动打开ATK，请等待...")
else:
	print("请手动打开ATK，等待" + str(WaitTime) + "秒")
	
conID = -1
count =  0
#7，循环检测ATK连接状态
while 0 > conID and count < WaitTime:
	#等待1秒,命令源码里会Sleep(1000)
	time.sleep(1)
	count += 1;
	print(str(count) + "/" + str(WaitTime) + "秒")
	#8，建立ATK连接
	conID = atkOpen(ATKIP, ATKPort)

if -1 < conID:
	print("connect atk success")
	#9，调用实现函数
	FastTransfer();
	#10，关闭ATK连接
	time.sleep(1);
	atkClose(conID);
else:
	print("connect atk failed, please confirm atk is open status")