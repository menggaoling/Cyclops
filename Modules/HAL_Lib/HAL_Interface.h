#ifndef __HAL_INTERFACE_H
#define __HAL_INTERFACE_H

#include "HAL_BSP.h"


void HAL_System_Initial(void);

void HAL_System_Reset(void);

void HAL_Set_IO_Output(GPIO_TypeDef *Port, UINT16 Pin);
void HAL_Set_IO_OutputLow(GPIO_TypeDef *Port, UINT16 Pin);
void HAL_Set_IO_Input(GPIO_TypeDef *Port, UINT16 Pin);
void HAL_Set_IO_InputHigh(GPIO_TypeDef *Port, UINT16 Pin);
void HAL_Set_IO_InputFloat(GPIO_TypeDef *Port, UINT16 Pin);
void HAL_Set_IO_AF_PP(GPIO_TypeDef *Port, UINT16 Pin);
void HAL_Set_IO_High(GPIO_TypeDef *Port, UINT16 Pin);
void HAL_Set_IO_Low(GPIO_TypeDef *Port, UINT16 Pin);
void HAL_Toggle_IO(GPIO_TypeDef *Port, UINT16 Pin);
UCHAR HAL_Get_IO(GPIO_TypeDef *Port, UINT16 Pin);
UCHAR HAL_Get_IO_Filter(GPIO_TypeDef *Port, UINT16 Pin);


#endif //__HAL_INTERFACE_H  