%	前置步骤：复制ATKConnectJava.jar所在路径，设置为Octave当前路径

%加载ATK Connect模式Java接口库
if(ispc())
	jarPath = [pwd,'\ATKConnectJava.jar'];
elseif(isunix())
	jarPath = [pwd,'/ATKConnectJava.jar'];
end
if ~any(strcmp(jarPath,javaclasspath()))
	javaaddpath(jarPath);
	javaMethod('loadLibrary', 'ATKLibraryLoader');
end
ATKConnectJavaModule = javaObject('com.atk.connect.ATKConnectJavaModule');

conID = ATKConnectJavaModule.atkOpen('127.0.0.1', 6655)
if 0 >= conID
	fprintf('connect atk failed, conID:%d\n', conID);
	fprintf('1, confirm the atk.exe has opened\n');
	fprintf('2, confirm the atk.exe port, atkOpen(ip, port)\n');
end

%1，想定新建并设置属性
ATKConnectJavaModule.atkConnect(conID, 'New', '/ Scenario FastTransfer','');
ATKConnectJavaModule.atkConnect(conID, 'SetAnalysisTimePeriod', '* "5 Nov 2022 00:00:00.000" "7 Nov 2022 00:00:00.000"','');
ATKConnectJavaModule.atkConnect(conID, 'Animate', '* Reset','');
%2，卫星新建与轨道预报设置为机动规划
ATKConnectJavaModule.atkConnect(conID, 'New', '/ Satellite FastTransfer','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetProp','');
%3，机动规划添加段，新添加卫星会有默认初始段
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer InsertSegment MainSequence.SegmentList.- Propagate','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer InsertSegment MainSequence.SegmentList.- Target_Sequence','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer InsertSegment MainSequence.SegmentList.Target_Sequence.SegmentList.- Maneuver','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer InsertSegment MainSequence.SegmentList.- Propagate','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer InsertSegment MainSequence.SegmentList.- Target_Sequence','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer InsertSegment MainSequence.SegmentList.Target_Sequence1.SegmentList.- Maneuver','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer InsertSegment MainSequence.SegmentList.- Propagate','');
%4，初始段属性设置
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Initial_State.InitialState.Epoch 5 Nov 2022 00:00:00.000 UtCG','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Initial_State.CoordinateType "Modified Keplerian"','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Initial_State.InitialState.Keplerian.sma 6700000 m','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Initial_State.InitialState.Keplerian.ecc 0','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Initial_State.InitialState.Keplerian.inc 0','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Initial_State.InitialState.Keplerian.RAAN 0','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Initial_State.InitialState.Keplerian.w 0','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Initial_State.InitialState.Keplerian.ta 0','');
%5，第一个预报段属性设置
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Propagate.SegmentColor -16776961','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Propagate.StoppingConditions Duration','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Propagate.StoppingConditions.Duration.TripValue 7200 sec','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Propagate.StoppingConditions.Duration.Tolerance 0.0001 sec','');
%6，第一个瞄准段中机动段属性设置
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Target_Sequence.SegmentList.Maneuver.SegmentColor 4278212095','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Target_Sequence.SegmentList.Maneuver.ImpulsiveMnvr.ThrustAxes "Satellite/FastTransfer VNC(Earth)"','');
%7，第一个瞄准段添加属性页
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer AddMCSSegmentControl MainSequence.SegmentList.Target_Sequence.SegmentList.Maneuver ImpulsiveMnvr.Cartesian.X','');
%8，设置属性页中控制变量属性
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Target_Sequence.Profiles Differential_Corrector','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetMCSControlValue MainSequence.SegmentList.Target_Sequence.Profiles.Differential_Corrector Maneuver ImpulsiveMnvr.Cartesian.X Active true','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetMCSControlValue MainSequence.SegmentList.Target_Sequence.Profiles.Differential_Corrector Maneuver ImpulsiveMnvr.Cartesian.X MaxStep 1000','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetMCSControlValue MainSequence.SegmentList.Target_Sequence.Profiles.Differential_Corrector Maneuver ImpulsiveMnvr.Cartesian.X Pert 0.1','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetMCSControlValue MainSequence.SegmentList.Target_Sequence.Profiles.Differential_Corrector Maneuver ImpulsiveMnvr.Cartesian.X Scale 1','');
%9，设置属性页中约束条件属性
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Target_Sequence.SegmentList.Maneuver.Results "Radius Of Apoapsis"','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetMCSConstraintValue MainSequence.SegmentList.Target_Sequence.Profiles.Differential_Corrector Maneuver StateCalcRadiusOfApoapsis Active true','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetMCSConstraintValue MainSequence.SegmentList.Target_Sequence.Profiles.Differential_Corrector Maneuver StateCalcRadiusOfApoapsis tolerance 0.1 m','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetMCSConstraintValue MainSequence.SegmentList.Target_Sequence.Profiles.Differential_Corrector Maneuver StateCalcRadiusOfApoapsis Scale 1 m','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetMCSConstraintValue MainSequence.SegmentList.Target_Sequence.Profiles.Differential_Corrector Maneuver StateCalcRadiusOfApoapsis Weight 1','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetMCSConstraintValue MainSequence.SegmentList.Target_Sequence.Profiles.Differential_Corrector Maneuver StateCalcRadiusOfApoapsis Desired 84328394 m','');
%10，第二个预报段属性设置
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Propagate1.SegmentColor -16711936','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Propagate1.StoppingConditions R_Magnitude','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Propagate1.StoppingConditions.R_Magnitude.TripValue 42164197 m','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Propagate1.StoppingConditions.R_Magnitude.Tolerance 1e-6 m','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Propagate1.StoppingConditions.R_Magnitude.RepeatCount 1','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Propagate1.StoppingConditions.R_Magnitude.Condition "Cross Either (Inc.or Dec.)"','');
%11，第二个瞄准段中机动段属性设置
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Target_Sequence1.SegmentList.Maneuver.SegmentColor -16711681','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Target_Sequence1.SegmentList.Maneuver.ImpulsiveMnvr.ThrustAxes "Satellite VNC(Earth)"','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer AddMCSSegmentControl MainSequence.SegmentList.Target_Sequence1.SegmentList.Maneuver ImpulsiveMnvr.Cartesian.X','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer AddMCSSegmentControl MainSequence.SegmentList.Target_Sequence1.SegmentList.Maneuver ImpulsiveMnvr.Cartesian.Z','');
%12，第二个瞄准段添加属性页
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Target_Sequence1.Profiles Differential_Corrector','');
%13，设置属性页中控制变量属性
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetMCSControlValue MainSequence.SegmentList.Target_Sequence1.Profiles.Differential_Corrector Maneuver ImpulsiveMnvr.Cartesian.X Active true','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetMCSControlValue MainSequence.SegmentList.Target_Sequence1.Profiles.Differential_Corrector Maneuver ImpulsiveMnvr.Cartesian.X MaxStep 1000','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetMCSControlValue MainSequence.SegmentList.Target_Sequence1.Profiles.Differential_Corrector Maneuver ImpulsiveMnvr.Cartesian.X Pert 0.1','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetMCSControlValue MainSequence.SegmentList.Target_Sequence1.Profiles.Differential_Corrector Maneuver ImpulsiveMnvr.Cartesian.X Scale 1','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetMCSControlValue MainSequence.SegmentList.Target_Sequence1.Profiles.Differential_Corrector Maneuver ImpulsiveMnvr.Cartesian.Z Active true','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetMCSControlValue MainSequence.SegmentList.Target_Sequence1.Profiles.Differential_Corrector Maneuver ImpulsiveMnvr.Cartesian.Z MaxStep 1000','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetMCSControlValue MainSequence.SegmentList.Target_Sequence1.Profiles.Differential_Corrector Maneuver ImpulsiveMnvr.Cartesian.Z Pert 0.1','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetMCSControlValue MainSequence.SegmentList.Target_Sequence1.Profiles.Differential_Corrector Maneuver ImpulsiveMnvr.Cartesian.Z Scale 1','');
%14，设置属性页中约束条件属性
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Target_Sequence1.SegmentList.Maneuver.Results Eccentricity','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetMCSConstraintValue MainSequence.SegmentList.Target_Sequence1.Profiles.Differential_Corrector Maneuver StateCalcEccentricity Active true','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetMCSConstraintValue MainSequence.SegmentList.Target_Sequence1.Profiles.Differential_Corrector Maneuver StateCalcEccentricity Desired 0','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetMCSConstraintValue MainSequence.SegmentList.Target_Sequence1.Profiles.Differential_Corrector Maneuver StateCalcEccentricity Scale 1','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetMCSConstraintValue MainSequence.SegmentList.Target_Sequence1.Profiles.Differential_Corrector Maneuver StateCalcEccentricity tolerance 0.001','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetMCSConstraintValue MainSequence.SegmentList.Target_Sequence1.Profiles.Differential_Corrector Maneuver StateCalcEccentricity Weight 1','');
%15，第三个预报段属性设置
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Propagate2.SegmentColor -65536','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Propagate2.StoppingConditions Duration','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Propagate2.StoppingConditions.Duration.TripValue 86400 sec','');
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer SetValue MainSequence.SegmentList.Propagate2.StoppingConditions.Duration.Tolerance 0.0001 sec','');
%16，机动规划运行
ATKConnectJavaModule.atkConnect(conID, 'Astrogator', '*/Satellite/FastTransfer RunMCS' ,'');
%17，查看报告数据
ATKConnectJavaModule.atkConnect(conID, 'Report_RM', '*/Satellite/FastTransfer Style "Position" TimePeriod "5 Nov 2022 01:00:00.000" "5 Nov 2022 02:00:00.000"' ,'');
%18，想定保存
ATKConnectJavaModule.atkConnect(conID, 'Save', '/ *','');
%19，暂停1秒再关闭连接
pause(1)
ATKConnectJavaModule.atkClose(conID);