#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "HAL_BSP.h"

#define KB_NONE           0x00
#define KB_NA             0xff
#define KB_HOLD_3S        0x80
#define KB_HOLD_5S        0x40
#define KB_HOLD_10S       0xC0
#define KB_HOLD_30S       0xE0
#define KB_LOCK           0xEE//add by ckm

#define KB_StartOrStop         		1
#define KB_Up            		2
#define KB_Down            		3
#define KB_SPEED_UP          		4
#define KB_SPEED_DOWN     		5

#define KB_Up_Down          	    40
#define KB_KB_WattsOrProfile_UP     41

#define KB_RF_SYNC           KB_KB_WattsOrProfile_UP+KB_HOLD_3S
#define KB_RESET             KB_StartOrStop+KB_HOLD_3S
#define KB_ENG               KB_Up_Down+KB_HOLD_3S
#define KB_RESET_COSTOM      KB_SPEED_UP+KB_HOLD_5S

void KB_Initial(void);
CHAR KB_Get_Code(void);
void KEY_Scan_Int(void);
void KB_Key_Process(void);
#endif



