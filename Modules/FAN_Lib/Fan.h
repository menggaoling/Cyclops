#ifndef __FAN_H
#define __FAN_H
#include "HAL_BSP.h"


//************************************FAN***********************************

#define FAN_SPEED_MIN			    360//8000
#define FAN_SPEED_MID			    1800//9500
#define FAN_SPEED_MAX			    3600//10943

#define FAN_TIMER_TOP                       3600

//Fan motor 2 pulse per turn.r/min need multiple 60.sample per 100ms excute once,need multiple 10.
#define FAN_PARAMETER                       (10*60)/2  

void Fan_Initial_Data(void);
void Fan_Set_Level(UCHAR by_Level);
void Fan_Set_Speed(USHORT by_Dat);
void Fan_Inc_Level(void);
void Fan_Dec_Level(void);
USHORT Fan_Get_Speed(void);
void Fan_1ms_Int(void);

#endif //__FAN_H  