#include  "stm32f10x_conf.h"
// Bit Mask
//#define BIT0      0x01
//#define BIT1      0x02
//#define BIT2      0x04
//#define BIT3      0x08
//#define BIT4      0x10
//#define BIT5      0x20
//#define BIT6      0x40
//#define BIT7      0x80

//==>> 特殊功能机制开关 
//#define _DebugClose //==>取消设定后其自我测试侦错时可关闭特定程序用
#define _EUPs  //==> EUPs启动开关
#define _TreadSense //==>人员侦测机制启动开关
// <<======
extern const unsigned char Boot_Version[20] ;
//##################################################################################
//###                           各系统设定状态代码                               ###
//##################################################################################
// 版本设定项(升版时需要同步修改)
// T3x-05-C，TM523D-1US-C
#define Console_Version "UCB : V2.001"
//#define Console_Version "UCB:A2-20150826"
#define UCB_Software_Ver 2    // vX.x   ==> X
#define UCB_Software_Rel 1    // vX.x   ==> x
#define UCB_Software_Build 0  // vX.x.B ==> B  "

#define D_MACHINE_MODEL  		24
#define D_CONSOLE_TYPE      		18

#define CSAFE_Model 0x80
#define _Lang_UcbMachineType_ "2015-T3x"
#define CCR1_Val_Close  0    //==>for Volume PWM 0-> min 12000-> max (12000参考PinDefine)
#define CCR1_Val_Open   1400 //==>for Volume PWM 0-> min 12000-> max (12000参考PinDefine)
 
// User Weight
#define Param_UserWeight_Min  50   // Lbs
#define Param_UserWeight_Max  400  // Lbs
#define Param_UserWeight_DF   150  // Lbs
#define Param_UserWeight_Min_Kg  23  
#define Param_UserWeight_Max_Kg  182 
#define Param_UserWeight_DF_Kg   68 
// User Height
#define _Height_KM_DF 183
#define _Height_KM_H 244
#define _Height_KM_L 91
#define _Height_Mile_DF 72
#define _Height_Mile_H 96
#define _Height_Mile_L 36
// ErP Time (minutes)
#define Param_ErPTime_Min 0    
#define Param_ErPTime_Max 30 
#define Param_ErPTime_DF 0
// User Age
#define Param_UserAge_Min  10  
#define Param_UserAge_Max  100
#define Param_UserAge_DF   30
// Pause Time
#define Param_PauseTime_Min  30
#define Param_PauseTime_Max  300
#define Param_PauseTime_DF   300
// Program Level
#define Param_ProgramLv_Min 1
#define Param_ProgramLv_Max 10
#define Param_ProgramLv_DF  1
// Time
#define Param_Time_Min   4
#define Param_Time_Max   99
#define Param_Time_DF    30
// Max Speed
#define Param_MaxSpeed_MPH_Min 20
#define Param_MaxSpeed_MPH_Max 150
#define Param_MaxSpeed_MPH_DF  120
#define Param_MaxSpeed_KPH_Min 32
#define Param_MaxSpeed_KPH_Max 241
#define Param_MaxSpeed_KPH_DF  200
// Start Speed
#define Param_StartSpeed_MPH_Min 5
#define Param_StartSpeed_MPH_Max 14
#define Param_StartSpeed_MPH_DF  5
#define Param_StartSpeed_KPH_Min 8
#define Param_StartSpeed_KPH_Max 23
#define Param_StartSpeed_KPH_DF  8
// Heart Rate %
#define _THRPercent_DF 65
#define _THRPercent_H 85
#define _THRPercent_L 50
// 最大速度值
#define D_MaxSpeed_MPH 150
#define D_MaxSpeed_KPH 241
// 高低点RPM参数
#define D_MinRPM 178
#define D_MaxRPM 5205
#define TOPTEK_MinRPM 205
#define TOPTEK_MaxRPM 6100
#define DELTA_MinRPM 136
#define DELTA_MaxRPM 4425
//
#define D_Disable   0
#define D_Enable    1
//
#define _OFF 0
#define _ON  1
//
#define D_ADD       0
#define D_DEC       1
//
#define C_FAIL      0
#define C_PASS      1

#define Logo_MATRIX 0

#define L_English   0
#define L_Japanese  1
#define L_German    2
#define L_Italian   3
#define L_Spanish   4
#define L_Dutch     5
#define L_Swedish   6
#define L_Finnish   7   
#define L_Portugese 8
#define L_French    9 
#define L_Turkish   10 
//
#define C_KM        1
#define C_MILE      0

#define D_Male      0
#define D_Female    1

#define Time_DOWN    0
#define Time_UP      1
#define Time_Program 2

//#define Fan_Close   0
//#define Fan_LSP     1
//#define Fan_MSP     2
//#define Fan_HSP     3

#define Keypad_Upper 1
#define Keypad_Lower 2
#define Keypad_Quick 3

#define Data_Dec  0
#define Data_Add  1
#define Data_Set 2
#define Data_Get  3
#define Data_Total 4
//##################################################################################
//###                          各个PROGRAM名称代码                               ###
//##################################################################################
// 此Program排列与led.h中的排列有对等关系
#define TM_QuickStart  0
#define TM_Manual      1
// Training Workout program
#define TM_5K          2
#define TM_10K         3
//
#define TM_TargetHR    4  
#define TM_FatBurn     5 
#define TM_Rolling     6   
// Fitness test program
#define TM_Gerkin      7  
#define TM_PEB         8  
#define TM_Army        9  
#define TM_Navy        10 
#define TM_Airforce    11 
#define TM_Marine      12 
#define TM_WFI         13 
//
//##################################################################################
//###                             各流程状态代码                                 ###
//##################################################################################
#define M_NONE           0xa0
#define M_RESET          0xa1
#define M_START          0xa2
#define M_NEXT           0xa3
#define M_PROGRAM        0xa4
#define M_ENGINEER       0xa5
#define M_ReWork         0xa6
#define M_CSAFE          0xa7
#define M_SP1            0xa8 // 特殊模式1
#define M_SP2            0xa9 // 特殊模式2
//##################################################################################
//###                       各流程参数设定状态代码                               ###
//##################################################################################
#define DF_TIME          1
#define DF_WEIGHT        2
#define DF_HEIGHT        3
#define DF_INCLINE       4
#define DF_AGE           5
#define DF_GENDER        6
#define DF_LEVEL         7
#define DF_PERCENT       8
#define DF_SPEED         9
#define DF_PROGRAM       0xb0
#define DF_START         0xb5
#define DF_WorkSpeed     0xb9
#define DF_THR           0xba
#define DF_HRControl     0xca
#define DF_TRAINING      0xcb
#define DF_FITNESS       0xcc
//##################################################################################
//###                         显示字符串代码                                       ###
//##################################################################################
#define Message_Blank 0
#define Message_BeginYourWorkoutByButton 1
#define Message_EnterTime 2
#define Message_EnterWeight 3
#define Message_EnterLevel 4
#define Message_EnterAge 5
#define Message_EnterTHRPercentage 6
#define Message_EnterGender 7
#define Message_EnterStartToBegin 8
#define Message_StartingSpeedGrade 9
#define Message_StartingSpeedGrade_KM 10
#define Message_THRIs 11
// Program name
#define Message_Manual 12
#define Message_5k 13
#define Message_10k 14
#define Message_TargetHR 15
#define Message_FatBurn 16
#define Message_RollingHills 17
#define Message_Gerkin 18
#define Message_PEB 19
#define Message_Amry 20
#define Message_Navy 21
#define Message_Airforce 22
#define Message_Marine 23
#define Message_WFI 24
//
#define Message_Weight 25
#define Message_Level 26
#define Message_Age 27
#define Message_Male 28
#define Message_Female 29
#define Message_THRPercent 30
#define Message_Time 31
#define Message_Speed 32
#define Message_3 33
#define Message_2 34
#define Message_1 35
#define Message_Begin 36
#define Message_PressToRelease 37
#define Message_ProgramEND 38
#define Message_PressStartToResume 39
#define Message_ExceedingMaxHR 40
#define Message_TotalTime 41
#define Message_StageNumber 42
#define Message_CaloriesBurned 43
#define Message_SUPERIOR 44
#define Message_EXCELLENT 45
#define Message_GOOD 46
#define Message_FAIR 47
#define Message_POOR 48
#define Message_VERY_POOR 49
#define Message_Score 50
#define Message_OUTSTANDING_HIGH 51
#define Message_OUTSTANDING_MEDIUM 52
#define Message_OUTSTANDING_LOW 53
#define Message_EXCELLENT_HIGH 54
#define Message_EXCELLENT_MEDIUM 55
#define Message_EXCELLENT_LOW 56
#define Message_GOOD_HIGH 57
#define Message_GOOD_MEDIUM 58
#define Message_GOOD_LOW 59
#define Message_SATISFACTORY_HIGH 60
#define Message_SATISFACTORY_MEDIUM 61
#define Message_SATISFACTORY_MARGINAL 62
#define Message_OutOfOrder 63
#define Message_EnterTheSpeed 64
#define Message_EnterTheIncline 65
#define Message_Incline 66
#define Message_EnterUserID 67
#define Message_SubmittingID 68
#define Message_IDAccepted 69
#define Message_BadID 70
#define Message_NetworkMiss 71
#define Message_PleaseHold 72
#define Message_CoolDown 73
#define Message_ResetForFitTest 74
#define Message_ResetForHR 75
#define Message_Paused 76
#define Message_TotalTime_2 77
#define Message_Distance 78
#define Message_AveragePace 79
#define Message_TestTerminated 80
#define Message_GraspHRSensors 81
#define Message_WATTs 82
#define Message_DetectingHR 83
#define Message_Restarting 84
#define Message_HeartRateDetected 85
#define Message_ResetForManual 86
#define Message_ResetForRolling 87
#define Message_ResetForFatBurn 88
#define Message_ResetForTrainingWorkouts 89
#define Message_EnterHeight 90
#define Message_Height 91
#define Message_BMI 92
#define Message_GoalAchieved 93
#define Message_Points 94
#define Message_DoYouWouldUpdate 95
#define Message_PleaseChooseUpdateName 96
#define Message_NotHaveHandleFile 97
#define Message_NotUSB 98
#define Message_WorkoutSaved 99
#define Message_WorkoutSaveFailed 100
#define Message_EnterSleepMode 101
#define Message_LCBNoWakeUp 102
#define Message_WiFiDoYouWouldUpdate 103
#define Message_Channel 104
#define Message_Volume 105
#define Message_Mute 106
#define Messgae_ChooseHRcontrol 107
#define Message_SetInclineMaximumValue 108
#define Message_NoUserDetected 109 
#define Message_BeltWillBeStoppedIn10Second 110
// Language and Pattern update
#define Message_AddMultipleLanguagesFromUSB 111 
#define Message_ChoosePatternName 112
#define Message_NoFileForUpdate 113
#define Message_LoadingLanguages 114
#define Message_LoadingPattern 115
#define Message_ThisFileCanNotBeUsedInThisConsole 116
#define Message_UpdateSuccessful 117
#define Message_UpdatedFailed 118
// WiFi
#define Message_LonginXID 119
#define Message_Enter4DigitsPin 120
#define Message_Login 121
#define Message_PinIsWrong 122
#define Message_Activated 123
#define Message_Enter414DigitsAccount 124
// Headphone Jack
#define Message_HJWillRequireReplacementSoon 125
#define Message_ReplaceEarphoneBoard 126
// Bad Keypad 
#define Message_ReplaceUpToggleOverlay 127
#define Message_ReplaceLowerToggleOverlay 128
#define Message_ReplaceQuickToggleOverlay 129
#define Message_TraningWorkouts 130
#define Message_FitnessTest 131
#define Message_Welcome 132
#define Message_SoftwareUpdateIn 133
#define Message_xIDworkoutTrackingAvailable 134
#define Message_NoInternetconnection 135
//

#define Message_TotalMessage 135 // 字符串数量
//#################################################################################
//####################             IO Map                  ########################
//#################################################################################
//=================================================================
//====> BEEP 控制脚
//=================================================================
#define IO_Set_BEEP()    {GPIO_SetBits(GPIOD,GPIO_Pin_7);}
#define IO_Clear_BEEP()  {GPIO_ResetBits(GPIOD,GPIO_Pin_7);}
//=================================================================
//====> MBI5026 控制脚
//=================================================================
#define IO_Clear_OE()    {GPIO_ResetBits(GPIOB,GPIO_Pin_12);}
#define IO_Set_OE()      {GPIO_SetBits(GPIOB,GPIO_Pin_12);}
#define IO_Clear_SDI()   {GPIO_ResetBits(GPIOB,GPIO_Pin_15);}
#define IO_Set_SDI()     {GPIO_SetBits(GPIOB,GPIO_Pin_15);}
#define IO_Clear_LE()    {GPIO_ResetBits(GPIOB,GPIO_Pin_14);}
#define IO_Set_LE()      {GPIO_SetBits(GPIOB,GPIO_Pin_14);}
#define IO_Clear_CLK()   {GPIO_ResetBits(GPIOB,GPIO_Pin_13);}
#define IO_Set_CLK()     {GPIO_SetBits(GPIOB,GPIO_Pin_13);}

//=================================================================
//====> POWER 控制脚
//=================================================================
#define LED_5V_Clear()   {GPIO_ResetBits(GPIOE,GPIO_Pin_2);}
#define LED_5V_Set()     {GPIO_SetBits(GPIOE,GPIO_Pin_2);}
#define LED_12V_Clear()  {GPIO_ResetBits(GPIOE,GPIO_Pin_1);}
#define LED_12V_Set()    {GPIO_SetBits(GPIOE,GPIO_Pin_1);}
//====> Sleep LED 控制脚
//=================================================================
#define IO_SleepMode_LED(s)   GPIO_WriteBit(GPIOE,GPIO_Pin_0,s)
//=================================================================
//====> Keypad按键控制脚
//=================================================================
#define IO_Clear_SK1()   {GPIO_ResetBits(GPIOE,GPIO_Pin_15);}
#define IO_Set_SK1()     {GPIO_SetBits(GPIOE,GPIO_Pin_15);}
#define IO_Clear_SK2()   {GPIO_ResetBits(GPIOE,GPIO_Pin_14);}
#define IO_Set_SK2()     {GPIO_SetBits(GPIOE,GPIO_Pin_14);}
#define IO_Clear_SK3()   {GPIO_ResetBits(GPIOE,GPIO_Pin_13);}
#define IO_Set_SK3()     {GPIO_SetBits(GPIOE,GPIO_Pin_13);}
#define IO_Clear_SK4()   {GPIO_ResetBits(GPIOE,GPIO_Pin_12);}
#define IO_Set_SK4()     {GPIO_SetBits(GPIOE,GPIO_Pin_12);}
#define IO_Clear_SK5()   {GPIO_ResetBits(GPIOE,GPIO_Pin_11);}
#define IO_Set_SK5()     {GPIO_SetBits(GPIOE,GPIO_Pin_11);}
#define IO_Clear_SK6()   {GPIO_ResetBits(GPIOE,GPIO_Pin_10);}
#define IO_Set_SK6()     {GPIO_SetBits(GPIOE,GPIO_Pin_10);}
#define IO_Clear_SK7()   {GPIO_ResetBits(GPIOE,GPIO_Pin_9);}
#define IO_Set_SK7()     {GPIO_SetBits(GPIOE,GPIO_Pin_9);}
#define IO_Clear_SK8()   {GPIO_ResetBits(GPIOE,GPIO_Pin_8);}
#define IO_Set_SK8()     {GPIO_SetBits(GPIOE,GPIO_Pin_8);}
//#define IO_Clear_SK1_8()          {GPIOE->BRR = 0x0000FF00;}
#define IO_Read_KeyboardData()    GPIO_ReadInputData(GPIOD)
//=================================================================
//====> 风扇控制脚
//=================================================================
//#define IO_FAN(s)         {TIM3->CCR4 = s ;}
//#define IO_FAN_LSP()      {TIM3->CCR4 = 50 ;}
//#define IO_FAN_MSP()      {TIM3->CCR4 = 300 ;}
//#define IO_FAN_HSP()      {TIM3->CCR4 = 1800 ;}
//=================================================================
//====> Voice 控制脚
//=================================================================
//#define IO_Set_Voice1()   {GPIO_SetBits(GPIOE,GPIO_Pin_1);}
//#define IO_Clear_Voice1() {GPIO_ResetBits(GPIOE,GPIO_Pin_1);}
//#define IO_Set_Voice2()   {GPIO_SetBits(GPIOE,GPIO_Pin_2);}
//#define IO_Clear_Voice2() {GPIO_ResetBits(GPIOE,GPIO_Pin_2);}
//#define IO_Set_Voice3()   {GPIO_SetBits(GPIOE,GPIO_Pin_3);}
//#define IO_Clear_Voice3() {GPIO_ResetBits(GPIOE,GPIO_Pin_3);}
//=================================================================
//====> RTC 控制脚
//=================================================================
//#define IO_Set_HT1381_SCK()   {GPIO_SetBits(GPIOC,GPIO_Pin_2);}
#define IO_Clear_HT1381_SCK() {GPIO_ResetBits(GPIOC,GPIO_Pin_2);}
//#define IO_Set_HT1381_Data()  {GPIO_SetBits(GPIOC,GPIO_Pin_3);}
#define IO_Clear_HT1381_Data(){GPIO_ResetBits(GPIOC,GPIO_Pin_3);}
//#define IO_Set_HT1381_RST()   {GPIO_SetBits(GPIOC,GPIO_Pin_0);}
#define IO_Clear_HT1381_RST() {GPIO_ResetBits(GPIOC,GPIO_Pin_0);}
//=================================================================
//====> 数字通讯 TX/RX 方向控制脚
//=================================================================
#define IO_Set_Digital_CTL()  {GPIO_SetBits(GPIOA,GPIO_Pin_1);}
#define IO_Clear_Digital_CTL(){GPIO_ResetBits(GPIOA,GPIO_Pin_1);}
//=================================================================
//====> USB 控制脚
//=================================================================
#define IO_USB_SetCTL1()   {GPIO_SetBits(GPIOA,GPIO_Pin_8);}   
#define IO_USB_ResetCTL1() {GPIO_ResetBits(GPIOA,GPIO_Pin_8);}      
#define IO_USB_SetCTL2()   {GPIO_SetBits(GPIOA,GPIO_Pin_9);}   
#define IO_USB_ResetCTL2() {GPIO_ResetBits(GPIOA,GPIO_Pin_9);}      
#define IO_USB_SetCTL3()   {GPIO_SetBits(GPIOA,GPIO_Pin_10);}   
#define IO_USB_ResetCTL3() {GPIO_ResetBits(GPIOA,GPIO_Pin_10);}      
#define IO_USB_SetILIM()   {GPIO_SetBits(GPIOE,GPIO_Pin_4);}   
#define IO_USB_ResetILIM() {GPIO_ResetBits(GPIOE,GPIO_Pin_4);}      
#define IO_USB_SetEN()     {GPIO_SetBits(GPIOE,GPIO_Pin_5);}   
#define IO_USB_ResetEN()   {GPIO_ResetBits(GPIOE,GPIO_Pin_5);}    
#define IO_USB_ERR()        GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_7)
#define IO_USB_LOAD()       GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6)
// Only charge
#define IO_USB_OnlyCharge()   {IO_USB_SetCTL1();IO_USB_ResetCTL2();IO_USB_SetCTL3();IO_USB_SetILIM();}  
// charge and communication
#define IO_USB_Communication(){IO_USB_SetCTL1();IO_USB_SetCTL2();IO_USB_SetCTL3();IO_USB_ResetILIM();}  
//=================================================================
// Audio
//=================================================================
#define IO_Ext_Audio()           {GPIO_SetBits(GPIOB,GPIO_Pin_7);}// for 900MHz
#define IO_TV_Audio()            {GPIO_ResetBits(GPIOB,GPIO_Pin_7);} 
#define IO_OpenAudio()           {GPIO_SetBits(GPIOC,GPIO_Pin_9);} // ShutDown pin
#define IO_CloseAudio()          {GPIO_ResetBits(GPIOC,GPIO_Pin_9);}
//=================================================================
// HR
//=================================================================
#define IO_HeartRateSignal()  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)
//=================================================================
// Safe Key
//=================================================================
#define IO_SafeKey()          GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_6)
//=================================================================
// Headphone Jack Sensor
//=================================================================
#define IO_HJS()              GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)
//=================================================================
//====> WiFi 控制脚
//=================================================================
#define IO_WiFi_RTS()         GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3)
#define IO_WiFi_Ready()       GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_0)
// WiFi Reset Pin
#define IO_WiFi_ResetLow()    GPIO_ResetBits(GPIOD,GPIO_Pin_1);
#define IO_WiFi_ResetHi()     GPIO_SetBits(GPIOD,GPIO_Pin_1);

/****************************** END OF FILE *********************************/

