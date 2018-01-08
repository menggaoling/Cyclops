#include "HAL_Lib.h"


/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void HAL_NVIC_Configuration(void)
{
        NVIC_InitTypeDef 		NVIC_InitStructure;
	//#ifdef  VECT_TAB_RAM
	/* Set the Vector Table base location at 0x20000000 */
	//NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
	//#else  /* VECT_TAB_FLASH  */
	/* Set the Vector Table base location at 0x08000000 */
	//NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0xD000);
	//#endif
        
	/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
            
        /* Enable the TIM2 global Interrupt */
        NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
        
        /* Enable the EXTI9_5 Interrupt */
        
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);		    
        
        /* Enable ADC IRQChannel */
        /*
        NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
        
        NVIC_InitStructure.NVIC_IRQChannel = OTG_FS_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
        */
}

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void HAL_GPIO_Configuration(void)
{
	GPIO_InitTypeDef 		GPIO_InitStructure;
        //EXTI_InitTypeDef 		EXTI_InitStructure;	

//        
//	/********************* Configure  UART *******************/
//	/* Configure USART1 / USART2 Rx (PA.10/PA3) as input floating */
//        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_3;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//        
//	/* Configure USART1 / USART2 Tx (PA.09/PA2) as alternate function push-pull */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_2;//;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//        /* Configure USART3 Rx (PB11) as input floating */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//
//	/* Configure USART3 Tx (PB10) as alternate function push-pull */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);	
//        
//        /* PORT D -> pin2:RF_RX */
//        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_Init(GPIOD, &GPIO_InitStructure);
//        /* 
//           PORT C -> pin12:RF_TX 
//                     pin10:TV_TX
//        */
//        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_12;
//        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//        GPIO_Init(GPIOC, &GPIO_InitStructure);
//        
//        /* PORT C -> pin11:TV_RX */
//        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_Init(GPIOC, &GPIO_InitStructure);
//        
//        /* ================================================================== */
//        /*  PORT A  */
//        /*
//            pin0 :NC
//            pin1 :DIR_485
//            pin2 :UART2_TX 
//            pin3 :UART2_RX 
//            pin4 :EEPROM CS
//            pin5 :EEPROM SK
//            pin6 :EEPROM DO
//            pin7 :EEPROM DI
//            pin8 :USB_CTL1 x
//            pin9 :USB_CTL2 x
//            pin10:USB_CTL3 x
//            pin11:USB_DM 
//            pin12:USB_DP 
//            pin13:TMS/SWDIO (JTAG)
//            pin14:TCK/SWDIO (JTAG)
//            pin15:TDI       (JTAG) 
//        */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7 |\
//                                      GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 ;
//        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);	
//        
//        GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//        
//        GPIO_ResetBits(GPIOA,GPIO_Pin_0);//==>没用到设LOW
//        /* ================================================================== */
//        /* PORT B */   
//        /*  
//            pin0 : USB_IAD
//            pin1 : NC
//            pin2 : DQ5 (Quick key)
//            pin3 : TDO (N)
//            pin4 : TRST (N)
//            pin5 : Safe_Key 
//            pin6 : VOL_CTL (PWM) TM4_CH1
//            pin7 : HP/nLINE
//            pin8 : HR ( Interrupt )
//            pin9 : NC
//            pin10: UART3_TX (N)
//            pin11: UART3_RX (N)
//            pin12: LED_OE
//            pin13: LED_CLK
//            pin14: LED_LE
//            pin15: LED_SDI
//        */
//        
//        GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14|\
//                                       GPIO_Pin_15;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);	
//        
//        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
//        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
//        GPIO_Init(GPIOB, &GPIO_InitStructure);
//        
//        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_5 | GPIO_Pin_8;
//        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//        GPIO_Init(GPIOB, &GPIO_InitStructure);
//        
//        //==>PWM
//        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
//        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_Init(GPIOB, &GPIO_InitStructure); 
 
        
        /* ==================================================================*/
        /* PORT C */ 
        /*    
            pin0 : RST_1381
            pin1 : CSAFE_CTS
            pin2 : SCK_1381
            pin3 : I/O 1381
            pin4 : DQ1 (Quick key)
            pin5 : DQ2 (Quick key)
            pin6 : DQ3 (Quick key)
            pin7 : DQ4 (Quick key)
            pin8 : BURN_IN
            pin9 : nSHUTDOWN
            pin10: TV_RX (N)
            pin11: TV_TX (N)
            pin12: RF_TX (N)
            pin13: NC
            pin14: NC
            pin15: NC
        */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
//       
//        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//	GPIO_Init(GPIOC, &GPIO_InitStructure);
//        
//        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 |\
//                                      GPIO_Pin_7;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_Init(GPIOC, &GPIO_InitStructure);
//        
//        // NA
//        GPIO_ResetBits(GPIOC,GPIO_Pin_1);
//        GPIO_ResetBits(GPIOC,GPIO_Pin_13);
//        GPIO_ResetBits(GPIOC,GPIO_Pin_15);
//        GPIO_ResetBits(GPIOC,GPIO_Pin_14);
//        //
//        
//        /* ==================================================================*/
//        /* PORT D */
//        /* 
//           pin0 : WiFi_nREADY
//           pin1 : WiFi_nRESET
//           pin2 : WiFi_RX (N)
//           pin3 : WiFi_RTS
//           pin4 : NA
//           pin5 : NA
//           pin6 : Safe_key
//           pin7 : BEEP
//           pin8 ~ pin15 : Key Pad Data DA1 ~ DA8 
//        */
//        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6  | GPIO_Pin_8  | GPIO_Pin_9 | GPIO_Pin_10 |\
//                                      GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14 |\
//                                      GPIO_Pin_15;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_Init(GPIOD, &GPIO_InitStructure);
//        
//        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_3;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//	GPIO_Init(GPIOD, &GPIO_InitStructure);
//        
//        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4  | GPIO_Pin_5 | GPIO_Pin_7 ;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_Init(GPIOD, &GPIO_InitStructure);	
//        
//        // NA
//        GPIO_SetBits(GPIOD,GPIO_Pin_4);
//        GPIO_SetBits(GPIOD,GPIO_Pin_5);
//        //
        
        /* ================================================================== */
        /* PORT E */
        /* 
           pin0 : 3.3_Power
           pin1 : +12V Power
           pin2 : +5V Power
           pin3 : NA
           pin4 : USB_ILIM
           pin5 : USB_EN
           pin6 : USB_LOAD
           pin7 : USB_ERR
           pin8 : 
           pin9 : 
           pin10: 
           pin11: 
           pin12:       
           pin13 : POWER_EN1
           pin14 : POWER_EN2
           pin15 : POWER_EN3 
        */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);	
        

}



void Hal_System_Initial(void)
{
//  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//  TIM_OCInitTypeDef  TIM_OCInitStructure;
  //TIM_ICInitTypeDef  TIM_ICInitStructure;
  //ADC_InitTypeDef  ADC_InitStructure;
  
  //initial uart peripheral 
  USART_DeInit(USART1) ;
  USART_DeInit(USART2) ;
  USART_DeInit(USART3) ;
  USART_DeInit(UART4) ;
  USART_DeInit(UART5) ;

  /* System Clocks Configuration */
  SystemInit();
  
  
  /* Enable peripheral device clock */
  /*要打开外部中断需要加上 RCC_APB2Periph_AFIO 设定*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB \
    | RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1  | RCC_APB2Periph_AFIO \
      , ENABLE);//RCC_APB2Periph_ADC1 | 
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4 | RCC_APB1Periph_USART2 | RCC_APB1Periph_USART3 | \
    RCC_APB1Periph_UART4 | RCC_APB1Periph_UART5 | RCC_APB1Periph_PWR,ENABLE);
  
  
  HAL_GPIO_Configuration();
  
//  HAL_NVIC_Configuration();
  
  /* Setup SysTick Timer for 1 msec interrupts  */
  SysTick_Config(SystemCoreClock / 1000);
  //===============================================================================================================  
  
  

  
}

void LED_Blink(void)
{
   if(GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_8))
     GPIO_ResetBits(GPIOC,GPIO_Pin_8);
   else
     GPIO_SetBits(GPIOC,GPIO_Pin_8);
}

void HAL_Set_ERP_Power(INT8 mode)
{
  if( mode == ON)
  {
    GPIO_SetBits(GPIOE,GPIO_Pin_13);
    GPIO_SetBits(GPIOE,GPIO_Pin_14);
    GPIO_SetBits(GPIOE,GPIO_Pin_15);
  }
  else if( mode == OFF)
  {
    GPIO_ResetBits(GPIOE,GPIO_Pin_13);
    GPIO_ResetBits(GPIOE,GPIO_Pin_14);
    GPIO_ResetBits(GPIOE,GPIO_Pin_15);
  }
}