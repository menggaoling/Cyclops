#ifndef __HAL_ERP_H_
#define __HAL_ERP_H_

#include  "Digital.h"
#include "HAL_BSP.h"

#define T_LOOP 0
#define T_STEP 1
// Set EUPs Action
#define LCB_EUPs_Implement  0xFF
#define LCB_EUPs_Leave      0x00

void Main_TimeDelay(unsigned char DelayTime);
 
void LCB_TimerClose(unsigned char by_Dat);
void LCB_Timer_Int(void); // 实时时间计数
void LCB_TimerCounterClear(unsigned char by_Index);
unsigned char LCB_TimerCounter(unsigned char by_Mode,unsigned char by_Who,unsigned short w_Dat);

void HAL_ERP_GPIO_Reset(void);

// EuPs(Sleep Mode) 
void HAL_ERP_Low_Power(void);
unsigned char LCB_Get_LowPower_State(void);
unsigned char LCB_SleepMode(unsigned short by_SystemStatus, unsigned char by_KeyState, unsigned char by_NowStatus, unsigned short LCB_SleepTime);
void LCB_SleepMode_Initial(void);
void Digital_Set_LCBEUPsState(unsigned char by_Dat);//==> EUPs(省电模式)
unsigned char LCB_GetSleepModeState(void);

unsigned char LCB_Get_EUPsState(void);
unsigned char LCB_QuickERP(unsigned char _SleepTime);
// end






#endif // __HAL_ERP_H_
