#ifndef __HAL_BSP_H
#define __HAL_BSP_H

#include "JIStypes.h"
#include "stm32f10x.h"


#define TXD          0
#define RXD          1
#define TXC          2  //Tx complete

void Hal_BSP_Initial(void);
void HAL_Set_ERP_Power(INT8 mode);
//digital 485
void Hal_Set_LCB_Serial_Dir(UINT8 direction);
void Hal_LCB_Serial_Send_Data(UINT8 data);
UINT8 Hal_LCB_Serial_Receive_Data(void);
void Hal_Set_LCB_Serial_Tx(void);
void Hal_Set_LCB_Serial_Rx(void);
UINT8 Hal_Get_LCB_Serial_IntFlag(void);
UINT8 Hal_Get_LCB_Serial_SendComplete_Flag(void);


#endif //__HAL_BSP_H  