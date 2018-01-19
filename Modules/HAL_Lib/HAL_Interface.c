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
    GPIO_InitS.GPIO_Mode = GPIO_Mode_IPD;//��������,
    GPIO_Init(Port, &GPIO_InitS);
}

void HW_Set_IO_InputHigh(GPIO_TypeDef *Port, UINT16 Pin)
{
    GPIO_InitTypeDef GPIO_InitS;
    GPIO_InitS.GPIO_Pin = Pin;
    GPIO_InitS.GPIO_Mode = GPIO_Mode_IPU;//��������,
    GPIO_Init(Port, &GPIO_InitS);
}

void HW_Set_IO_InputFloat(GPIO_TypeDef *Port, UINT16 Pin)
{
    GPIO_InitTypeDef GPIO_InitS;
    GPIO_InitS.GPIO_Pin = Pin;
    GPIO_InitS.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������,
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

//���Լ���PINһ��д1
void HW_Set_IO_High(GPIO_TypeDef *Port, UINT16 Pin)
{
    GPIO_SetBits(Port, Pin);
}

//���Լ���PINһ��д0
void HW_Set_IO_Low(GPIO_TypeDef *Port, UINT16 Pin)
{
    GPIO_ResetBits(Port, Pin);
}

//һ��ֻ���л�һ��
void HW_Toggle_IO(GPIO_TypeDef *Port, UINT16 Pin)
{
    if(GPIO_ReadOutputDataBit(Port, Pin))
        GPIO_ResetBits(Port, Pin);
    else
        GPIO_SetBits(Port, Pin);
}

//һ��ֻ�ɶ�һ��IO��
UCHAR HW_Get_IO(GPIO_TypeDef *Port, UINT16 Pin)
{
    return GPIO_ReadInputDataBit(Port, Pin);
}