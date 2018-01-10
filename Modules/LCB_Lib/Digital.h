#ifndef _Digital_H_
#define _Digital_H_

#include "LCB_Support.h"
#include "HAL_Interface.h"

#define LCB_DCI      0xC0
#define LCB_TopTek   0xC1
#define LCB_DELTA    0xC2
#define LCB_DELTA_C3 0xC3

#define CmdInitial              0x70 //设定下板初始化
#define CmdGetStatus            0x71 //状态取得
#define CmdGetErrorCode         0x72 //错误代码取得
#define CmdGetVersion           0x73 //取得MCB版本
#define CmdSkipCurrentError     0x76 //移除目前错误码
#define CmdGetDCI_Version       0x78 //DCI专用command

//==> Digital for TM
#define CmdSetMotorCounter      0xf1 //设定马达转速
#define CmdSetPwmAddStep        0xf2 //设定加速速率
#define CmdSetPwmDecStep        0xf3 //设定减速速率
#define CmdSetPemStopStep       0xf4 //设定停止速率
#define CmdSetInclineAction     0xf5 //设定升降控制
#define CmdSetInclineLocation   0xf6 //设定升降位置
#define CmdSetWorkStatus        0xf7 //设定运动状态
#define CmdGetRollerRpm         0xf8 //取得滚轮速度
#define CmdGetMotorRpm          0xf9 //取得马达速度
#define CmdGetInclineLocation   0xfa //取得升降准位
#define CmdGetDriverType        0x91 //取得马达驱动器类型
#define CmdDCI_GetMaxDataLength 0x92 //DCI专用command

#define CmdGetTreadmillInUsed   0x95 //人员侦测
#define CmdGetMainMotorInformation 0x96 //取下控信息

#define CmdDCI_SetInclineRange  0xa1 //DCI专用command,用来设定自动更正时的升降范围
#define CmdDCI_GetInclineRange  0xa0 //DCI专用command,
#define CmdDCI_MANUAL_CALIBRATION 0xab //DCI专用command,
#define CmdDCI_FORCE_INCLINE_OPERATION 0x94 // DCI专用command,强迫上升下降停止用
#define CmdDCI_GET_RPM_SPEED_RANGE 0xa6 //DCI专用command,用来取得RPM/SPEED范围
#define CmdDCI_SET_RPM_SPEED_RANGE 0xa7 //DCI专用command,用来设定RPM/SPEED范围

//==> Sinkyo
#define CmdEUPsMode                   0x7A // 省电模式
#define CmdLCBDeviceData              0x50 // 取得LCB Device 相关资料 
#define SecCmdGetEEPromMemorySizes    0x01 // CmdLCBDeviceData底下的第二道Command // 取得LCB EEProm已经储存的容量 unit:byte
#define SecCmdGetEEPromMemoryContent  0x02 // CmdLCBDeviceData底下的第二道Command // 取得LCB EEProm已经储存的内容
#define SecCmdGetECBCurrent           0x03 // CmdLCBDeviceData底下的第二道Command // 取得LCB 2个ECB的电流值 unit:mA
#define SecCmdGetEStopVale            0x04
// <==
#define InUserMotoCheckTime 6 // 马达稳定时间参数值
//

u8 Digital_GetMachineType(void);
void Digital_SetMachineType(u8 by_D);
u8 Digital_PowerON_MachineTypeCheck(void);

u8 Digital_CheckSafeKeyStatus(void);
void Digital_ClearSafeKeyStatus(void);
u8 Digital_eSTOPKeyDelay(void);
void Digital_eSTOPKeyDelayClear(void);
void Digital_CommandBufferReset(void);


void Digital_Initial(void);//====> 数字通讯参数默认
void Digital_RX(void);//====> 数字通讯 RX 数据处理
u8 Digital_Command(u8 by_Cmd,u32 by_Dat);//====> 数字通讯 command 设定
u8 Digital_CRC8(u8 *ptr,u8 DATALENGTH);//====> 数字通讯 CRC code
void Digital_AutoReply(void);//====> 数字通讯封包处理
u16 Digtial_Get_MCBVersion(void);//====> MCB版本取得
u16 Digtial_Get_InclineLocation(void);//====> 升降准位取得
u16 Digtial_Get_RollerRpm(void);//====> 滚轮RPM取得
u16 Digtial_Get_MotorRpm(void);//====> 马达RPM取得
void Digital_Set_Work(u8 by_Dat);//====> 数字通讯升降切换使用指针 (作为使用者是否开始进行运动判断)
u8 Digital_Get_RPMStatus(void);//====> 回传马达的状态
u16 Digtial_Get_ErrorCode(void);//====> 错误码
void Digtial_Set_ErrorCode(u16 by_Dat);
//void Digital_Clear_ErrorCode(void);//====> 将所取得的错误码进行位移的动作,到储存位置
u8 Digital_Check_Buffer(void);//====> 数字通讯输出缓存器是否已满
void Digital_Set_InclineCheck(u8 by_Dat);//====> 设定升降参数
void Digital_Clear_ElevationOld(void);//====> 升降目标Incline值清除
u16 Digital_Get_ElevationOld(void);//====> 升降目标Incline值取得
void Digital_Set_DigitalSpeedTarget(u16 by_Dat);//====> 马达速度追击目标值设定
void Digital_TargetSpeed_To_DigitalCommand(void);//====> 马达速度追击主程序
void Digital_Set_TargetSpeedStart(u8 by_Dat);//====> 马达速度追击与否设定
u8 Digtial_ErrorCode_Level(u16 by_Dat);//====> 错误码的等级
void Digital_Set_SpeedTargetDelay(u16 by_Dat);//====> 马达加速度速率延迟时间
void Digital_Set_AutoCheck(u8 by_Dat);//====> 机台是否进入自动更正模式
u8 Digital_Get_TargetSpeed(void);//====> 取得速度追击旗标
void Digital_UartTxRx_Information(void);//====> TX / RX 数据处理
u8 Digtial_ElevationVariationCheck(u8 by_D);//====>升降马达实际AD值与目标AD值是否误差过大
//u32 Digtial_Get_DCIVersion(void);
void Digital_ErrorCode_ChangToAddress(void);
//u8 Digital_CheckInclineRange(void);
u8 Digital_CheckError(void);

u8 Digital_CheckClassCError(void);
void Digital_ErroeCodeUpdate(void);
u8 Digital_InclineProtection(u8 by_Dat);// S003-01
u8 Digital_GetStatus(void);// S003-01
void Digital_Clear_ErrorCount(void);
u16 Digital_GetMCB_Information(u8 by_D);
//void Digital_CommunicationStart(u8 by_D);
//u8 Digital_CommunicationSave(void);
//u32 Digital_CommunicationTotal(u8 by_D);
void Digital_CommandStart(char by_D);
void Digital_MotorFunctionCheck(void);

#endif // _Digital_H_
