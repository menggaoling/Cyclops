#include "HAL_Interface.h"

void HAL_System_Initial(void)
{
  HAL_BSP_Initial();
  HAL_Set_ERP_Power(ON);
  HAL_Set_USB_Port_Mode(USB_DATA);
  HAL_Set_USB_Charge_Current(0);//2.1 A
}

//System Reset
void HAL_System_Reset(void)
{
  //before reset request be executed,there is a delay,
  //during this delay period,cpu still response interrupt.
  __set_FAULTMASK(1);
  NVIC_SystemReset();
  
}


//**************************** GPIO Operation ********************************//
void HAL_Set_IO_Output(GPIO_TypeDef *Port, UINT16 Pin)
{
    GPIO_InitTypeDef GPIO_InitS;
    GPIO_InitS.GPIO_Pin = Pin;
    GPIO_InitS.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitS.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(Port, &GPIO_InitS);
}

void HAL_Set_IO_OutputLow(GPIO_TypeDef *Port, UINT16 Pin)
{
    GPIO_InitTypeDef GPIO_InitS;
    GPIO_InitS.GPIO_Pin = Pin;
    GPIO_InitS.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitS.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(Port, &GPIO_InitS);
}

void HAL_Set_IO_Input(GPIO_TypeDef *Port, UINT16 Pin)
{
    GPIO_InitTypeDef GPIO_InitS;
    GPIO_InitS.GPIO_Pin = Pin;
    GPIO_InitS.GPIO_Mode = GPIO_Mode_IPD;//下拉输入,
    GPIO_Init(Port, &GPIO_InitS);
}

void HAL_Set_IO_InputHigh(GPIO_TypeDef *Port, UINT16 Pin)
{
    GPIO_InitTypeDef GPIO_InitS;
    GPIO_InitS.GPIO_Pin = Pin;
    GPIO_InitS.GPIO_Mode = GPIO_Mode_IPU;//上拉输入,
    GPIO_Init(Port, &GPIO_InitS);
}

void HAL_Set_IO_InputFloat(GPIO_TypeDef *Port, UINT16 Pin)
{
    GPIO_InitTypeDef GPIO_InitS;
    GPIO_InitS.GPIO_Pin = Pin;
    GPIO_InitS.GPIO_Mode = GPIO_Mode_IN_FLOATING;//上拉输入,
    GPIO_Init(Port, &GPIO_InitS);
}

void HAL_Set_IO_AF_PP(GPIO_TypeDef *Port, UINT16 Pin)
{
    GPIO_InitTypeDef GPIO_InitS;
    GPIO_InitS.GPIO_Pin = Pin;
    GPIO_InitS.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitS.GPIO_Mode = GPIO_Mode_AF_PP;//Alternate function output Push-pull
    GPIO_Init(Port, &GPIO_InitS);
}

//可以几个PIN一起写1
void HAL_Set_IO_High(GPIO_TypeDef *Port, UINT16 Pin)
{
    GPIO_SetBits(Port, Pin);
}

//可以几个PIN一起写0
void HAL_Set_IO_Low(GPIO_TypeDef *Port, UINT16 Pin)
{
    GPIO_ResetBits(Port, Pin);
}

//一次只可切换一个
void HAL_Toggle_IO(GPIO_TypeDef *Port, UINT16 Pin)
{
    if(GPIO_ReadOutputDataBit(Port, Pin))
        GPIO_ResetBits(Port, Pin);
    else
        GPIO_SetBits(Port, Pin);
}

//一次只可读一个IO口
UCHAR HAL_Get_IO(GPIO_TypeDef *Port, UINT16 Pin)
{
    return GPIO_ReadInputDataBit(Port, Pin);
}