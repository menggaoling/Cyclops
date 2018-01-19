#ifndef __FAN_H
#define __FAN_H
#include "HAL_BSP.h"


//************************************FAN***********************************

#define FAN_IO_PWM_PORT             gpioPortA
#define FAN_IO_PWM_PIN              2
#define FAN_TIMER                   TIMER0
#define FAN_CCP                     2
#define FAN_TIMER_ROUTE_CCPEN       TIMER_ROUTE_CC2PEN
#define FAM_TIMER_ROUTE_LOCATION    TIMER_ROUTE_LOCATION_LOC1
#define FAN_IO_Sel_Low()            FAN_TIMER->ROUTE &= ~(FAN_TIMER_ROUTE_CCPEN | FAM_TIMER_ROUTE_LOCATION)
#define FAN_IO_Sel_High()           FAN_TIMER->ROUTE |=  (FAN_TIMER_ROUTE_CCPEN | FAM_TIMER_ROUTE_LOCATION)
#define FAN_TIMER_TOP               11764
#define FAN_SPEED_MIN			    6000//8000
#define FAN_SPEED_MID			    7000//9500
#define FAN_SPEED_MAX			    10587//10943

void Fan_Set_Speed(unsigned char by_Dat);

#endif //__FAN_H  