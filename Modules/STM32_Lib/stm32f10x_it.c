/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void LED_Blink(void);
void SysTick_Handler(void)
{
  static UCHAR by_Time100ms = 0;
  LED_Blink();
  
  Console_SEC_Counter_Int();
  
  //LCB
  Digital_AutoReply();
  
  //Key
  KEY_Scan_Int();
  
  //C_Safe
  CSAFE_Counter_Int();
  if(CSAFE_TXE_Status(1)==1)
  {
    CSAFE_TXE_Status(0);
    USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
  }
  
  //MYE TV
  TV_AutoReply();
  if(TV_TXE_Status(1)==1)
  {// Need open TX interrupt
    TV_TXE_Status(0);
    USART_ITConfig(UART4, USART_IT_TXE, ENABLE);
  }
  
  //CAB
  if(CAB_CommandProcess() == 1)
  {// Have command,TX need enable
    USART_ITConfig(UART4, USART_IT_TXE, ENABLE);
  }
  
  
  //RFID
  if(RFID_GATRM310_Process())
  {
    USART_ITConfig(USART1,	USART_IT_RXNE, DISABLE);
    USART_ITConfig(USART1,	USART_IT_TXE, DISABLE);
    RFID_GATRM310_Initial();
    USART_ITConfig(USART1,	USART_IT_RXNE, ENABLE);
  }
  if(RFID_TXE_Status (1)==1)
  {// Need open TX interrupt
    RFID_TXE_Status (0);
    USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
  }
  
  Fan_1ms_Int();
  
  if(by_Time100ms++ >= 100)
  {
    by_Time100ms = 0;
    Timer_Flag_Int(); 
  }
  
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

/*******************************************************************************
* Function Name  : USART1_IRQHandler
* Description    : This function handles USART1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART1_IRQHandler(void)
{
  if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
  {
    // Receive
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
    {
      RFID_RxBuffer(USART_ReceiveData(USART1));
      /* Clear the Receive interrupt */
      USART_ClearITPendingBit(USART1, USART_IT_RXNE);
      //
    }
    // Transmit
    if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET) 
    {
      /* Clear the Transmit interrupt */
      USART_ClearITPendingBit(USART1, USART_IT_TXE);
      USART_SendData(USART1, RFID_TxBuffer());
      if(RFID_GATRM310_TxRxInterrupt(1) == 1)
        USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
      //
    }
  }  
}

/*******************************************************************************
* Function Name  : USART2_IRQHandler
* Description    : This function handles USART2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART2_IRQHandler(void)
{//==>485 communication
  if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)
  {
    Digital_UartTx_Int();
    USART_ClearITPendingBit(USART2, USART_IT_TXE);
  }
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) 
  {
    Digital_UartRx_Int();
    USART_ClearITPendingBit(USART2, USART_IT_RXNE);
  }
//  if(LCB_Get_LowPower_State() != 1)// Sinkyo
//  {
//      Digital_UartTxRx_Information();
//      return;
//  }
//  USART_ClearITPendingBit(USART2, USART_IT_RXNE);
//  USART_ClearITPendingBit(USART2, USART_IT_TXE);
//  USART_ClearITPendingBit(USART2, USART_IT_TC);
//  USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
//  USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
//  USART_ITConfig(USART2, USART_IT_TC, DISABLE);
}


/*******************************************************************************
* Function Name  : USART3_IRQHandler
* Description    : This function handles USART3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART3_IRQHandler(void)
{
  if(USART_GetITStatus(USART3, USART_IT_TXE) != RESET)
  {// TX
    USART_SendData(USART3,CSAFE_TxBuffer());// TX傳送Buffer資料
    USART_ClearITPendingBit(USART3, USART_IT_TXE);
    if(CSAFE_TxRx_Information(1) == 1)// TX
    {
      USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
    }
  }
  if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) 
  {// RX
    CSAFE_RxBuffer(USART_ReceiveData(USART3));// RX取資料至Buffer
    USART_ClearITPendingBit(USART3, USART_IT_RXNE);
    CSAFE_TxRx_Information(0); // RX
  }
  
}

/**
  * @brief  This function handles UART4 interrupt request.
  * @param  None
  * @retval None
  */
void UART4_IRQHandler(void)
{
//  CAB TV Function
//  if(USART_GetITStatus(UART4, USART_IT_TXE) != RESET) 
//  {
//    /* Clear the CAB_UART transmit interrupt */
//    USART_ClearITPendingBit(UART4, USART_IT_TXE);      
//    /* Write one byte to the transmit data register */
//    if(CAB_SendDataCheck() == 1)
//    {// Have data
//      USART_SendData(UART4,CAB_SendData());   
//    }
//    else
//    {// Not have data
//      USART_ITConfig(UART4, USART_IT_TXE, DISABLE);
//    }
//  } 
  
  if(USART_GetITStatus(UART4, USART_IT_TXE) != RESET)
  {// TX
    USART_SendData(UART4, TV_TxBuffer());// TX傳送Buffer資料
    USART_ClearITPendingBit(UART4, USART_IT_TXE);
    if(TV_UartTxRx_Information (1) == 1)// TX
    {
      USART_ITConfig(UART4, USART_IT_TXE, DISABLE);
      USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
    }
  }
  if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET) 
  {// RX
    TV_RxBuffer(USART_ReceiveData(UART4));// RX取資料至Buffer
    USART_ClearITPendingBit(UART4, USART_IT_RXNE);
    if(TV_UartTxRx_Information (0) == 1) // RX
    {
      USART_ITConfig(UART4, USART_IT_RXNE, DISABLE);
    }    
  }  
} 

/**
  * @brief  This function handles UART5 interrupt request.
  * @param  None
  * @retval None
  */
void UART5_IRQHandler(void)
{
  if(USART_GetITStatus(UART5, USART_IT_TXE) != RESET)
  {// TX
    USART_ClearITPendingBit(UART5, USART_IT_TXE);
    Com_Tx_Int(); 
  }
  if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET) 
  {// RX
    USART_ClearITPendingBit(UART5, USART_IT_RXNE);
    Com_Rx_Int();
  }  
} 



/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
