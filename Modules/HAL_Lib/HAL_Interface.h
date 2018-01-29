#ifndef __HAL_INTERFACE_H
#define __HAL_INTERFACE_H

#include "HAL_BSP.h"


void Hal_System_Initial(void);
void HW_Set_IO_Output(GPIO_TypeDef *Port, UINT16 Pin);
void HW_Set_IO_OutputLow(GPIO_TypeDef *Port, UINT16 Pin);
void HW_Set_IO_Input(GPIO_TypeDef *Port, UINT16 Pin);
void HW_Set_IO_InputHigh(GPIO_TypeDef *Port, UINT16 Pin);
void HW_Set_IO_InputFloat(GPIO_TypeDef *Port, UINT16 Pin);
void HW_Set_IO_AF_PP(GPIO_TypeDef *Port, UINT16 Pin);
void HW_Set_IO_High(GPIO_TypeDef *Port, UINT16 Pin);
void HW_Set_IO_Low(GPIO_TypeDef *Port, UINT16 Pin);
void HW_Toggle_IO(GPIO_TypeDef *Port, UINT16 Pin);
UCHAR HW_Get_IO(GPIO_TypeDef *Port, UINT16 Pin);
UCHAR HW_Get_IO_Filter(GPIO_TypeDef *Port, UINT16 Pin);


#endif //__HAL_INTERFACE_H  