#ifndef _LCBSupport_H_
#define _LCBSupport_H_


#define T_LOOP 0
#define T_STEP 1
// Set EUPs Action
#define LCB_EUPs_Implement  0xFF
#define LCB_EUPs_Leave      0x00

void Main_TimeDelay(unsigned char DelayTime);
/*
void LCB_Get_LCBEEPromMemoryData(void); // 读取LCB EEPROM 内容
void LCB_Set_EEPromMemorySize(unsigned short by_Dat);
void LCB_Clear_EEPromMemorySize(void);
unsigned char LCB_Get_EEPromGetState(void);
void LCB_Set_EEPromGetState(unsigned char by_Dat);
unsigned short LCB_Get_EEPromMemoryContent(unsigned char by_Dat);
void LCB_Set_LCBEEPromMemoryContent(unsigned char by_Dat, unsigned char by_Dat2);
void LCB_Set_GetLCBEEPRomFlag(unsigned char by_Dat);
unsigned char LCB_Get_GetLCBEEPRomFlag(void);

*/
void LCB_TimerClose(unsigned char by_Dat);
void LCB_Timer_Int(void); // 实时时间计数
void LCB_TimerCounterClear(unsigned char by_Index);
unsigned char LCB_TimerCounter(unsigned char by_Mode,unsigned char by_Who,unsigned short w_Dat);

//unsigned short Digital_Get_LCBEEPromMemorySize(void);  // 取得下板EEPROM已使用的容量, 单位: Byte

// EuPs(Sleep Mode) 
unsigned char Low_PowerMode(void);
unsigned char LCB_Get_LowPower_State(void);
unsigned char LCB_SleepMode(unsigned short by_SystemStatus, unsigned char by_KeyState, unsigned char by_NowStatus, unsigned short LCB_SleepTime);
void LCB_SleepMode_Initial(void);
void Digital_Set_LCBEUPsState(unsigned char by_Dat);//==> EUPs(省电模式)
unsigned char LCB_GetSleepModeState(void);

//
extern void System_Initial(void); 
extern void CSAFE_Initial(void);
extern void TV_Initial(void);
extern void RF_Initial(void); 
extern void GPIO_Reset(void);
extern void Open_UCB_PSW(void);
unsigned char LCB_Get_LEDSwitch(void);


//void LCB_Set_LowPower_State(u8 by_D);
unsigned char LCB_Get_EUPsState(void);
// start v2.001-1
unsigned char LCB_QuickERP(unsigned char _SleepTime);
// end
#endif // _LCBSupport_H_
