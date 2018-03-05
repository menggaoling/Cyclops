#ifndef __TIMER_H_
#define __TIMER_H_

#include "HAL_BSP.h"

#define T_LOOP 0
#define T_STEP 1
#define Time_AllClose 0xff
#define Time_TotalBuffer 16

#define _Time_ERP                10
#define _Time_HJSCheck           11 //Phone Jack Sensor
#define _Time_USBProtectionCheck 12 
#define _Time_RSCUCheck          13 
#define _Time_KeepDelay          14
#define _Time_Back               15  

void Timer_Initial(void);
void Timer_Flag_Int(void);
void Timer_Counter_Clear(UCHAR by_Index);
UCHAR Timer_Counter(UCHAR by_Mode,UCHAR by_Who,USHORT w_Dat);
void Timer_Close(UCHAR by_Dat);
void Timer_Delay(UCHAR delayTime);

#endif // __TIMER_H_
