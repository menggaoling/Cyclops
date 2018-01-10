#ifndef _C_safe_H_
#define _C_safe_H_

#include "HAL_Interface.h"

//v4 2012.09.12
#define Audio_OFF   0
#define Audio_TV    1
#define Audio_PCTV  2
#define Audio_RF900 3
#define Audio_CAB   4
//================================================================
//==> mode
#define Csafe_Ready    0x01//0x81
#define Csafe_Offline  0x09//0x89
#define Csafe_IDle     0x02//0x82
#define Csafe_Manual   0x08
#define Csafe_HaveID   0x03//0x83
#define Csafe_In_Use   0x05//0x85
#define Csafe_Paused   0x06//0x86
#define Csafe_Finished 0x07//0x87
//================================================================
//==> 對應 command
//================================================================
#define cmdUpList         0x02

#define cmdIDDigits       0x10
#define cmdSetTime        0x11
#define cmdSetDate        0x12
#define cmdSetTimeout     0x13

#define cmdSetTWork       0x20
#define cmdSetHorizontal  0x21
#define cmdSetVertical    0x22
#define cmdSetCalories    0x23
#define cmdSetProgram     0x24
#define cmdSetSpeed       0x25
#define cmdSetGrade       0x28 // Incline
#define cmdSetGear        0x29 // Resistance
#define cmdSetUserInfo    0x2b
#define cmdSetTorque      0x2c
#define cmdSetLevel       0x2d

#define cmdSetTargetHR    0x30
#define cmdSetMETs        0x33 // METS goal
#define cmdSetPower       0x34 
#define cmdSetHRZone      0x35
#define cmdSetHRMAX       0x36

#define cmdSetChannelRange 0x40
#define cmdSetVolumeRange  0x41
#define cmdSetAudioMute    0x42
#define cmdSetAudioChannel 0x43
#define cmdSetAudioVolume  0x44

#define cmdGetStatus      0x80              
#define cmdReset          0x81                
#define cmdGoIdle         0x82                
#define cmdGoHavelID      0x83                
#define cmdGoInUse        0x85                
#define cmdGoFinished     0x86                
#define cmdGoReady        0x87                
#define cmdBadID          0x88

#define cmdGetVersion     0x91
#define cmdGetID          0x92
#define cmdGetUnits       0x93
#define cmdGetSerial      0x94
#define cmdGetList        0x98
#define cmdGetUtilization 0x99
#define cmdGetMotorCurrent 0x9a
#define cmdGetOdometer    0x9b
#define cmdGetErrorCode   0x9c
#define cmdGetServiceCode 0x9d

#define cmdGetTWork       0xa0
#define cmdGetHorizontal  0xa1
#define cmdGetVertical    0xa2
#define cmdGetCalories    0xa3
#define cmdGetProgram     0xa4
#define cmdGetSpeed       0xa5
#define cmdGetPace        0xa6
#define cmdGetCadence     0xa7  // Current Cadence
#define cmdGetGrade       0xa8  //(Incline)
#define cmdGetGear        0xa9
#define cmdGetUpList      0xaa                
#define cmdGetUserInFo    0xab  
#define cmdGetTorque      0xac

#define cmdGetHRCur       0xb0                
#define cmdGetHRTZone     0xb2
#define cmdGetMETs        0xb3
#define cmdGetPower       0xb4
#define cmdGetHRAvg       0xb5
#define cmdGetHRMax       0xb6

#define cmdGetAudioChannel 0xc0
#define cmdGetAudioVolume  0xc1
#define cmdGetAudioMute    0xc2
//================================================================
//==> 分別放入準備傳送給主機的對應資訊參數
//================================================================
enum
{
  _Set_WorkoutTime=1,       // 運動累計時間
  _Set_WorkoutDistance,     // 運動累計距離
  _Set_WorkoutCalories,     // 運動累計卡路里
  _Set_Level, 
  _Set_Speed,  
  _Set_MET,  
  _Set_Watt,  
  _Set_Program,  
  _Set_AccumulatedTime,     // 總時間累計
  _Set_Manufacturer, 
  _Set_CID , 
  _Set_Model , 
  _Set_Version, 
  _Set_Release,   
  _Set_Units,               // 0x01(英制)0x00(公制)
  _Set_DistanceUnits,       // Distance 顯示單位
  _Set_SpeedUnits,          // Speed 顯示單位
  _Set_Online,              // 連線旗標
  _Set_StartWorkout,        // 開始運動旗標
  _Set_Incline,             // Incline
  _Set_HRMax ,              // 實際取得的最大心跳值
  _Set_HRAvg ,              // 實際平均心跳值
  _Set_HRTZone,             // 實際心跳在所設定範圍內的時間計數
  _Set_Gender,              
  _Set_AGE,                 
  _Set_WeightUnits,         // Weight 顯示單位
  _Set_Weight,              
  _Set_Mode,                // CSAFE區塊模式
  _Set_HRCur,               // 即時心跳
  _Set_AccumulatedDistance ,// 總距離累計
  _Set_AvgSpeed, 
  _Set_Serial,
  _Set_MotorCurrent,
  _Set_ErrorCode,
  _Set_ServiceCode,
  _Set_Gear,
  _Set_Torque,
  _Set_AudioMute,
  _Set_AudioChannel,
  _Set_AudioVolume,
  _Set_WorkoutVertical,
  _Set_VerticalUnits,
  _Set_CommunicationMode ,   // 週邊控制通訊模式(CSAFE or RF900)
//================================================================
//==> 分別取得主機設定的對應資訊參數
//================================================================
  _Get_DFdistnce=100,      // 設定值
  _Get_TargetDistance,     // 設定旗標
  _Get_DFcalories,         // 設定值
  _Get_TargetCalories,     // 設定旗標
  _Get_DFprogram,          // 設定值
  _Get_Mode,               // CSAFE模式狀態
  _Get_Online,             // 連線狀態
  _Get_HRZoneMax,          // 主機設定心跳範圍最大值(供範圍時間計數用)
  _Get_HRZoneMin,          // 主機設定心跳範圍最小值(供範圍時間計數用)
  _Get_THR,                // 主機設定心跳目標值
  _Get_TargetHR,           // 主機設定心跳目標值旗標
  _Get_THR_Max,            // 主機設定心跳目標最大值
  _Get_TargetTime,         // 設定旗標
  _Get_Time,               // 設定值
  _Get_TargetLevel,        // 設定旗標
  _Get_Level,              // 設定值
  _Get_Gender,             // 設定值
  _Get_AGE ,               // 設定值
  _Get_Power,              // 設定值
  _Get_Weight,             // 設定值
  _Get_TargetPower,        // 設定旗標
  _Get_SetTime,            // 主機設定的時鐘時間
  _Get_SetDate,            // 主機設定的時鐘日期
  _Get_SetTimeout,         // 主機設定的Timeout值
  _Get_TargetSpeed,        // 設定旗標
  _Get_TargetGrade,        // 設定旗標
  _Get_TargetGear,         // 設定旗標
  _Get_TargetTorque,       // 設定旗標
  _Get_SetSpeed,           // 設定值
  _Get_SetGrade,           // 設定值
  _Get_SetGear,            // 設定值
  _Get_SetTorque,          // 設定值
  _Get_TargetMETs,         // 設定旗標
  _Get_TargetChannelRange, // 設定旗標
  _Get_TargetVolumeRange,  // 設定旗標
  _Get_TargetAudioMute,    // 設定旗標
  _Get_TargetAudioChannel, // 設定旗標
  _Get_TargetAudioVolume,  // 設定旗標
  _Get_SetMETs,            // 設定值 
  _Get_SetChannelRange,    // 設定值
  _Get_SetVolumeRange,     // 設定值
  _Get_SetAudioMute,       // 設定值
  _Get_SetAudioChannel,    // 設定值
  _Get_SetAudioVolume,     // 設定值
  _Get_TargetUserInfo,     // 設定旗標
  _Get_HRAvg ,             // HR平均值
  _Get_CID,
  _Get_Manufacturer,
  _Get_Model,
  
};
//================================================================
#define _NormalMode 0
#define _TestMode   1
#define _false      0
#define _true       1
//================================================================
void CSAFE_Counter_Int(void);
void CSAFE_RX_Buffer_Decision(void);
void CSAFE_Number_Check(void);
char CSAFE_TX_XOR(char by_Dat);
void CSAFE_RX_Answer_Buffer(char by_Dat);
void CSAFE_Initial(void);
void CSAFE_Power_On(void);
void CSAFE_Set_ID_Number(unsigned long int by_Dat);
void CSAFE_ClearIDNumber(void);
void CSAFE_Set_AnyKey(void);
void CSAFE_Answer(char by_Dat);
void CSAFE_Error_Answer(char by_Dat);
char CSAFE_Mode_Check_Answer(char by_Dat);
void CSAFE_Work_Exit(void);
char CSAFE_Mode_Judge(void);
void CSAFE_Reset(void);//==>將C-SAFE系統參數重置
void CSAFE_EngTest(void);
void CSAFE_CommunicationMode(char by_Dat);
char CSAFE_TestStatus(void);
void CSAFE_Write(char by_Length);
char CSAFE_TxRx_Information(char by_D);
char CSAFE_TxBuffer(void);
void CSAFE_RxBuffer(char by_D);
void CSAFE_900M_Cardio(char by_Command,char by_Num); 
void CSAFE_SetCheckAnyKeyStatus(char by_Dat);
void CSAFE_900M_E7command(void);
char CSAFE_900M_CommandStatus(void);
void CSAFE_900M_CommandStatusClear(void);
void CSAFE_900M_E7DataAnalysis(void);
char CSAFE_GetNetPulse(void);
void CSAFE_HRInformation(void);
char CSAFE_900M_GetControlBuffer(char by_D);
unsigned char CSAFE_TXE_Status(unsigned char by_D);
unsigned long int CSAFE_Information(unsigned char by_D,unsigned long int by_Data);
#endif // _C_safe_H_
