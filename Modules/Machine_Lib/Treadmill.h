#ifndef __TREADMILL_H_
#define __TREADMILL_H_


#include "HAL_BSP.h"
#include "General.h"
#include "Digital.h"
#include "Timer.h"
#include "Formula.h"
#include "C_safe.h"

#define I_NONE    0
#define I_COMP    1
#define I_OFFSET  2
#define I_NO_MOV  3
#define P_NONE    0
#define P_COMP    1

// Info 这些设定项与 General.h 的设定(各流程参数设定状态代码)是相关联的
#define Info_TIME         0
#define Info_WEIGHT       1
#define Info_HEIGHT       2
#define Info_StartIncline 3
#define Info_AGE          4
#define Info_GENDER       5
#define Info_LEVEL        6
#define Info_PERCENT      7 
#define Info_StartSpeed   8
#define Info_WorkoutSpeed 9
#define Info_WorkoutIncline 10
//
enum{
    Info_WarmUpAccTime = 1,
    Info_WorkoutAccTime,
    Info_CoolDwonAccTime,
    Info_AccumulatedTime,
    Info_FitTestACCTime
};
//
USHORT Console_Information(UCHAR by_Item,UCHAR by_Handle,USHORT by_Dat);
USHORT Console_Parameter(char by_Item);

USHORT Console_GetVolumeNumber(UCHAR by_D);
void Console_AddUserVolume(void);
void Console_DecUserVolume(void);
UCHAR Console_GetUserVolume(void);
void Console_SetDefaultVolume(UCHAR by_Dat);
UCHAR Console_GetDefaultVolume(void);
void Console_SetMaxVolume(UCHAR by_Dat);
UCHAR Console_GetMaxVolume(void);
void Console_SetOutputVolume(UCHAR by_Dat);
UCHAR Console_GetOutputVolume(void);

void Console_ChangeTimeCul(void);
void Console_BMI(void);
USHORT Console_Get_BMI(void);
void Console_VO2(void);
USHORT Console_Get_VO2(void);
void Console_Set_FitTestTimeAcc(UCHAR by_Dat);
//USHORT Console_Get_FitTestAccTime(void);
USHORT Console_Get_CsafeAccTime(void);
USHORT Console_Get_AvgPace(void);
u32 Console_Get_AvgSpeed(void);
USHORT Console_Get_METs(void);
void Console_ClearDistance(void);
UCHAR Console_Time_Counter_Proce(void);
// Time Seg (Dotmatrix Line)
UCHAR Console_Get_Time_Seg(void);
// Calories
UCHAR Console_Calories_Counter_Proce(void);
USHORT Console_Get_Calories(void);
USHORT Console_Get_Calories_Hour(void);
// Distance
UCHAR Console_Distance_Counter_Proce(void);
USHORT Console_Get_Distance(void);
//
//USHORT Console_Get_Time_Accumulated(void);
void Console_Time_Cul(void);
void Console_Set_StartAccumulated(UCHAR by_Dat);
//
UCHAR Console_DistanceGoal_Over(void);
USHORT Console_Get_GoalDistance(void);
void Console_GetInclineInformation(void);
UCHAR Console_Time_Seg_Proce(void);
UCHAR Console_5K10K_Seg_Proce(void);
void Console_Initial(void);
void Console_SEC_Counter_Int(void);
UCHAR Console_Get_TimeDIR(void);
UCHAR Check_SafeKey(void);
UCHAR Console_GetInclinePfofile(UCHAR by_Dat,UCHAR by_Dat1,UCHAR by_Dat2);
// New Time
USHORT Console_NewTime(UCHAR by_D);
// User Time
USHORT Console_UserTime(UCHAR by_D,USHORT by_D1);
// Max Time
USHORT Console_MaxTime(UCHAR by_D,USHORT by_D1);
// Time Start
void Console_TimeStart(UCHAR by_D);
// Time Over
UCHAR Console_TimeOver(UCHAR by_D,UCHAR by_D1);
// Distance Show target
UCHAR Console_ShowDistanceTarget(UCHAR by_D,UCHAR by_D1);
// Language
UCHAR Console_Language(UCHAR by_D,UCHAR by_D1);
// Display Speed Change
UCHAR Console_DisplaySpeedChange(UCHAR by_D,UCHAR by_D1);
// Digital Error
USHORT Console_DigitalError(UCHAR by_D,UCHAR by_D1,USHORT by_D2);
// Navy Test
void Console_NavyTestMode(UCHAR by_D);
// Speed Unit
UCHAR Console_SpeedUnit(UCHAR by_D,UCHAR by_D1);
// Max Speed
UCHAR Console_MaxSpeed(UCHAR by_D,UCHAR by_D1);
// Start Speed 
UCHAR Console_StartSpeed(UCHAR by_D,USHORT by_D1);
// Sleep Time
USHORT Console_SleepTime(UCHAR by_D,USHORT by_D1);
// ADC
USHORT Console_HighLowADC(UCHAR by_D,UCHAR by_D1,USHORT by_D2);
// RPM Parameter
USHORT Console_RPM_Parameter(UCHAR by_D1,UCHAR by_D2,USHORT by_D3);
// Gender
//UCHAR Console_Gender(UCHAR by_D,UCHAR by_D1);
// Workout Status
UCHAR Console_WorkoutStatus(UCHAR by_D,UCHAR by_D1);
// Audio
UCHAR Console_Audio(UCHAR by_D,UCHAR by_D1);
// Power On 
USHORT Console_PowerOn(UCHAR by_D,USHORT by_D1);
// Program Mode
UCHAR Console_ProgramMode(UCHAR by_D,UCHAR by_D1);
// Fitness Test
UCHAR Console_GerkinConversion(void);
UCHAR Console_Gerkin(void);
USHORT Console_AirForce(void);
UCHAR Console_Navy(void);
UCHAR Console_PEB(void);
UCHAR Console_DistanceEND(void);
UCHAR Console_Army(void);
UCHAR Console_ArmyScore(USHORT by_TimeStart,USHORT by_TimeEnd);
UCHAR Console_Marine(void);
//
UCHAR Console_CsafGoalEND(void);
void Console_CsafGoalDisable(void);
//USHORT Console_ChangeDistanceBuffer(char by_D);

void Console_SetHRControlModel(UCHAR by_D);
void Console_HRControlMax(UCHAR by_D);
void Console_HRControlChange(UCHAR by_D1,USHORT by_D2);
UCHAR Console_GetHRControlMax(void);
UCHAR Console_GetHRControlMode(void);
UCHAR Console_MaxIncline(UCHAR by_D,UCHAR by_D1);
long unsigned int Console_HJS(UCHAR by_Mo,long unsigned int by_D);
UCHAR Console_AutoSoftwareUpdateStatus(UCHAR by_Mode);

USHORT Console_GetTimeSegData(void);

#endif //__TREADMILL_H_
