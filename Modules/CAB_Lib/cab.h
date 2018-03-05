#include "HAL_Interface.h"
#ifndef __CAB_H
#define __CAB_H

#define _CAB_DIGIT_0        0
#define _CAB_DIGIT_1        1
#define _CAB_DIGIT_2        2
#define _CAB_DIGIT_3        3
#define _CAB_DIGIT_4        4
#define _CAB_DIGIT_5        5
#define _CAB_DIGIT_6        6
#define _CAB_DIGIT_7        7
#define _CAB_DIGIT_8        8
#define _CAB_DIGIT_9        9
#define _CAB_VOL_DOWN       10
#define _CAB_VOL_UP         11
#define _CAB_CH_DOWN        12
#define _CAB_CH_UP          13
#define _CAB_NOUSE1         14
#define _CAB_POWER_TOGGLE   15
#define _CAB_NOUSE2         16
#define _CAB_PREVIOUS_CH    17
#define _CAB_MUTE           18
#define _CAB_NOUSE3         19
#define _CAB_NOUSE4         20
#define _CAB_NOUSE5         21
#define _CAB_CC             22
#define _CAB_ENTER          23
#define _CAB_SOURCE         24
#define _CAB_DASH           25
//
#define _CAB_TotalCommand_ 26
//
#define _CAB_START          'A'
#define _CAB_END            0x0D

void CAB_Initial(void) ;
//void CAB_TxRx_Information(void) ;
void CAB_SendCommand(unsigned char _Cmd_) ;
void CAB_CommandToBuffer(unsigned char cmd) ;
unsigned char CAB_CommandProcess(void) ;
unsigned char CAB_SendDataCheck(void);
unsigned char CAB_SendData(void);
#endif /* __CAB_H */


