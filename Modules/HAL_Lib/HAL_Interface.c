#include "HAL_Interface.h"

void Hal_System_Initial(void)
{
  Hal_BSP_Initial();
  HAL_Set_ERP_Power(ON);
}



//**************************** GPIO Operation ********************************//
void HW_Set_IO_Output(GPIO_TypeDef *Port, UINT16 Pin)
{
    GPIO_InitTypeDef GPIO_InitS;
    GPIO_InitS.GPIO_Pin = Pin;
    GPIO_InitS.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitS.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(Port, &GPIO_InitS);
}

void HW_Set_IO_OutputLow(GPIO_TypeDef *Port, UINT16 Pin)
{
    GPIO_InitTypeDef GPIO_InitS;
    GPIO_InitS.GPIO_Pin = Pin;
    GPIO_InitS.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitS.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(Port, &GPIO_InitS);
}

void HW_Set_IO_Input(GPIO_TypeDef *Port, UINT16 Pin)
{
    GPIO_InitTypeDef GPIO_InitS;
    GPIO_InitS.GPIO_Pin = Pin;
    GPIO_InitS.GPIO_Mode = GPIO_Mode_IPD;//下拉输入,
    GPIO_Init(Port, &GPIO_InitS);
}

void HW_Set_IO_InputHigh(GPIO_TypeDef *Port, UINT16 Pin)
{
    GPIO_InitTypeDef GPIO_InitS;
    GPIO_InitS.GPIO_Pin = Pin;
    GPIO_InitS.GPIO_Mode = GPIO_Mode_IPU;//上拉输入,
    GPIO_Init(Port, &GPIO_InitS);
}

void HW_Set_IO_InputFloat(GPIO_TypeDef *Port, UINT16 Pin)
{
    GPIO_InitTypeDef GPIO_InitS;
    GPIO_InitS.GPIO_Pin = Pin;
    GPIO_InitS.GPIO_Mode = GPIO_Mode_IN_FLOATING;//上拉输入,
    GPIO_Init(Port, &GPIO_InitS);
}

void HW_Set_IO_AF_PP(GPIO_TypeDef *Port, UINT16 Pin)
{
    GPIO_InitTypeDef GPIO_InitS;
    GPIO_InitS.GPIO_Pin = Pin;
    GPIO_InitS.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitS.GPIO_Mode = GPIO_Mode_AF_PP;//Alternate function output Push-pull
    GPIO_Init(Port, &GPIO_InitS);
}

//可以几个PIN一起写1
void HW_Set_IO_High(GPIO_TypeDef *Port, UINT16 Pin)
{
    GPIO_SetBits(Port, Pin);
}

//可以几个PIN一起写0
void HW_Set_IO_Low(GPIO_TypeDef *Port, UINT16 Pin)
{
    GPIO_ResetBits(Port, Pin);
}

//一次只可切换一个
void HW_Toggle_IO(GPIO_TypeDef *Port, UINT16 Pin)
{
    if(GPIO_ReadOutputDataBit(Port, Pin))
        GPIO_ResetBits(Port, Pin);
    else
        GPIO_SetBits(Port, Pin);
}

//一次只可读一个IO口
UCHAR HW_Get_IO(GPIO_TypeDef *Port, UINT16 Pin)
{
    return GPIO_ReadInputDataBit(Port, Pin);
}