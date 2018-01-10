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
//==> ���� command
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
//==> ���O��J�ǳƶǰe���D����������T�Ѽ�
//================================================================
enum
{
  _Set_WorkoutTime=1,       // �B�ʲ֭p�ɶ�
  _Set_WorkoutDistance,     // �B�ʲ֭p�Z��
  _Set_WorkoutCalories,     // �B�ʲ֭p�d����
  _Set_Level, 
  _Set_Speed,  
  _Set_MET,  
  _Set_Watt,  
  _Set_Program,  
  _Set_AccumulatedTime,     // �`�ɶ��֭p
  _Set_Manufacturer, 
  _Set_CID , 
  _Set_Model , 
  _Set_Version, 
  _Set_Release,   
  _Set_Units,               // 0x01(�^��)0x00(����)
  _Set_DistanceUnits,       // Distance ��ܳ��
  _Set_SpeedUnits,          // Speed ��ܳ��
  _Set_Online,              // �s�u�X��
  _Set_StartWorkout,        // �}�l�B�ʺX��
  _Set_Incline,             // Incline
  _Set_HRMax ,              // ��ڨ��o���̤j�߸���
  _Set_HRAvg ,              // ��ڥ����߸���
  _Set_HRTZone,             // ��ڤ߸��b�ҳ]�w�d�򤺪��ɶ��p��
  _Set_Gender,              
  _Set_AGE,                 
  _Set_WeightUnits,         // Weight ��ܳ��
  _Set_Weight,              
  _Set_Mode,                // CSAFE�϶��Ҧ�
  _Set_HRCur,               // �Y�ɤ߸�
  _Set_AccumulatedDistance ,// �`�Z���֭p
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
  _Set_CommunicationMode ,   // �g�䱱��q�T�Ҧ�(CSAFE or RF900)
//================================================================
//==> ���O���o�D���]�w��������T�Ѽ�
//================================================================
  _Get_DFdistnce=100,      // �]�w��
  _Get_TargetDistance,     // �]�w�X��
  _Get_DFcalories,         // �]�w��
  _Get_TargetCalories,     // �]�w�X��
  _Get_DFprogram,          // �]�w��
  _Get_Mode,               // CSAFE�Ҧ����A
  _Get_Online,             // �s�u���A
  _Get_HRZoneMax,          // �D���]�w�߸��d��̤j��(�ѽd��ɶ��p�ƥ�)
  _Get_HRZoneMin,          // �D���]�w�߸��d��̤p��(�ѽd��ɶ��p�ƥ�)
  _Get_THR,                // �D���]�w�߸��ؼЭ�
  _Get_TargetHR,           // �D���]�w�߸��ؼЭȺX��
  _Get_THR_Max,            // �D���]�w�߸��ؼг̤j��
  _Get_TargetTime,         // �]�w�X��
  _Get_Time,               // �]�w��
  _Get_TargetLevel,        // �]�w�X��
  _Get_Level,              // �]�w��
  _Get_Gender,             // �]�w��
  _Get_AGE ,               // �]�w��
  _Get_Power,              // �]�w��
  _Get_Weight,             // �]�w��
  _Get_TargetPower,        // �]�w�X��
  _Get_SetTime,            // �D���]�w�������ɶ�
  _Get_SetDate,            // �D���]�w���������
  _Get_SetTimeout,         // �D���]�w��Timeout��
  _Get_TargetSpeed,        // �]�w�X��
  _Get_TargetGrade,        // �]�w�X��
  _Get_TargetGear,         // �]�w�X��
  _Get_TargetTorque,       // �]�w�X��
  _Get_SetSpeed,           // �]�w��
  _Get_SetGrade,           // �]�w��
  _Get_SetGear,            // �]�w��
  _Get_SetTorque,          // �]�w��
  _Get_TargetMETs,         // �]�w�X��
  _Get_TargetChannelRange, // �]�w�X��
  _Get_TargetVolumeRange,  // �]�w�X��
  _Get_TargetAudioMute,    // �]�w�X��
  _Get_TargetAudioChannel, // �]�w�X��
  _Get_TargetAudioVolume,  // �]�w�X��
  _Get_SetMETs,            // �]�w�� 
  _Get_SetChannelRange,    // �]�w��
  _Get_SetVolumeRange,     // �]�w��
  _Get_SetAudioMute,       // �]�w��
  _Get_SetAudioChannel,    // �]�w��
  _Get_SetAudioVolume,     // �]�w��
  _Get_TargetUserInfo,     // �]�w�X��
  _Get_HRAvg ,             // HR������
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
void CSAFE_Reset(void);//==>�NC-SAFE�t�ΰѼƭ��m
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
