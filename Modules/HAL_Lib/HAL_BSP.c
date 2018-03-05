#include "HAL_BSP.h"

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
        
//	/* Configure one bit for preemption priority */
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
//            
//        /* Enable the TIM2 global Interrupt */
//        NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
//        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
//        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
//        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//        NVIC_Init(&NVIC_InitStructure);
        
        /* Enable the TIM3 global Interrupt */
        NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
        
//        /* Enable the EXTI9_5 Interrupt */
//        
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);		    
        
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
        TIM_TimeBaseInitTypeDef         TIM_TimeBaseStructure;
        TIM_OCInitTypeDef               TIM_OCInitStructure;        
        //EXTI_InitTypeDef 		EXTI_InitStructure;	

        
	/********************* Configure  UART *******************/
	/* Configure USART1 / USART2 Rx (PA.10/PA3) as input floating */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
        
	/* Configure USART1 / USART2 Tx (PA.09/PA2) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_2;//;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

        /* Configure USART3 Rx (PB11) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Configure USART3 Tx (PB10) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
        
        /* PORT D -> pin2:GUI_RX */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
        /* 
           PORT C -> pin12:GUI_TX 
                     pin10:TV_TX
        */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_12;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOC, &GPIO_InitStructure);
        
        /* PORT C -> pin11:TV_RX */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
        
        /* ================================================================== */
        /*  PORT A  */
        /*
            pin0 :NC
            pin1 :DIR_485
            pin2 :UART2_TX 
            pin3 :UART2_RX 
            pin4 :EEPROM CS
            pin5 :EEPROM SK
            pin6 :EEPROM DO
            pin7 :Fan RPM
            pin8 :USB_CTL1 x
            pin9 :USB_CTL2 x
            pin10:USB_CTL3 x
            pin11:USB_DM 
            pin12:USB_DP 
            pin13:TMS/SWDIO (JTAG)
            pin14:TCK/SWDIO (JTAG)
            pin15:TDI       (JTAG) 
        */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ;//| GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7 |\
                                      GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 ;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
  
        
//        GPIO_ResetBits(GPIOA,GPIO_Pin_0);//==>没用到设LOW
        /* ================================================================== */
        /* PORT B */   
        /*  
            pin0 : 
            pin1 : 
            pin2 : DQ5 (Quick key)
            pin3 : TDO (N)
            pin4 : TRST (N)
            pin5 : Safe_Key 
            pin6 : Fan (PWM) TM4_CH1
            pin7 : HP/nLINE
            pin8 : USB CURRRENT S0
            pin9 : USB CURRRENT S1
            pin10: UART3_TX (N)
            pin11: UART3_RX (N)
            pin12: LED_OE
            pin13: LED_CLK
            pin14: LED_LE
            pin15: LED_SDI
        */
//        
//        GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14|\
//                                       GPIO_Pin_15;
        GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
//        
//        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
//        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
//        GPIO_Init(GPIOB, &GPIO_InitStructure);
//        
//        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_5 | GPIO_Pin_8;
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
        
        //==>Fan PWM
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);         
              
        /* -----------------------------------------------------------------------
        TIM3 Configuration: generate 1 PWM signals with 1 different duty cycles:
        The TIM3CLK frequency is set to SystemCoreClock (Hz), to get TIM3 counter
        clock at 24 MHz the Prescaler is computed as following:
         - Prescaler = (TIM3CLK / TIM3 counter clock) - 1
        SystemCoreClock is set to 72 MHz for Low-density, Medium-density, High-density
        and Connectivity line devices and to 24 MHz for Low-Density Value line and
        Medium-Density Value line devices
      
        The TIM4 is running at 6 KHz: TIM3 Frequency = TIM3 counter clock/(ARR + 1)
                                                       = 36 MHz / 4000 = 6 KHz
        TM3 Channel4 duty cycle = (TIM3_CCR1/ TIM3_ARR)* 100 = 50%
        ----------------------------------------------------------------------- */
        /* Time base configuration */
        TIM_TimeBaseStructure.TIM_Period = 3600 - 1;//0x7ce;//12000 ;
        
        /* Compute the prescaler value */
        TIM_TimeBaseStructure.TIM_Prescaler = 0 ; 
        TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
        TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
      
        /* PWM2 Mode configuration: Channel4 */
        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
        TIM_OCInitStructure.TIM_Pulse = 100-1 ;//0
        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
        TIM_OC4Init(TIM3, &TIM_OCInitStructure);
        TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
        //
        TIM_ARRPreloadConfig(TIM3, ENABLE);
        /* TIM3 enable counter */
        TIM_Cmd(TIM3, ENABLE);
                   
          
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
//           pin0 : 
//           pin1 : 
//           pin2 : 
//           pin3 : 
//           pin4 : USB_EN
//           pin5 : NA
//           pin6 : 
//           pin7 : 
//           pin9 : 
//           pin10: 
//           pin11: 
//           pin12: 
//           pin13: NC
//           pin14: NC
//           pin15: NC
//        */
//        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6  | GPIO_Pin_8  | GPIO_Pin_9 | GPIO_Pin_10 |\
//                                      GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14 |\
//                                      GPIO_Pin_15;
//        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
//        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_Init(GPIOD, &GPIO_InitStructure);
//        
//        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_3;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//	GPIO_Init(GPIOD, &GPIO_InitStructure);
//        
//        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4  | GPIO_Pin_5 | GPIO_Pin_7 ;
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);	
        
        
//        
//        // NA
//        GPIO_SetBits(GPIOD,GPIO_Pin_4);
//        GPIO_SetBits(GPIOD,GPIO_Pin_5);
//        //
        
        /* ================================================================== */
        /* PORT E */
        /* 
           pin0 : FAN RPM
           pin1 : 
           pin2 : 
           pin3 : USB CTRL
           pin4 : 
           pin5 : Power Mode
           pin6 : 
           pin7 : 
           pin8 : 
           pin9 : 
           pin10: 
           pin11: 
           pin12:       
           pin13 : POWER_EN1
           pin14 : POWER_EN2
           pin15 : POWER_EN3 
        */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);	   
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOE, &GPIO_InitStructure);        
        
        /* Time base configuration */
        TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
        TIM_TimeBaseStructure.TIM_Prescaler = 18 - 1;
        TIM_TimeBaseStructure.TIM_ClockDivision = 0;
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
        TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
        
        TIM_ETRClockMode2Config(TIM4, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0);  
//        TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);   
        TIM_SetCounter(TIM4, 0);    
        TIM_Cmd(TIM4, ENABLE);         

}

void  HAL_Usart_Initial(void)
{
	USART_InitTypeDef 	USART_InitStructure;
	NVIC_InitTypeDef 	NVIC_InitStructure;
        
        //initial uart peripheral 
        USART_DeInit(USART1) ;
        USART_DeInit(USART2) ;
        USART_DeInit(USART3) ;
        USART_DeInit(UART4) ;
        USART_DeInit(UART5) ;
        
        //USART_ClockInitTypeDef  USART_ClockInitStructure ;
        /*######################################################################
          #  UART 1 = RFID 
          #  UART 2 = Digital 
          #  UART 3 = CSAFE 
          #  UART 4 = TV/CAB 
          #  UART 5 = UCB 
          ######################################################################*/     
	USART_InitStructure.USART_BaudRate = 9600;//115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	/* Configure USART2,USART3 */
	
	USART_Init(USART2, &USART_InitStructure);	
        USART_Init(USART3, &USART_InitStructure);
        
        /* Configure USART1,UART5, */
        USART_InitStructure.USART_BaudRate = 115200;        
        USART_Init(USART1, &USART_InitStructure);
	USART_Init(UART5, &USART_InitStructure);	
        
	/* Enable USART1,USART2,USART3 Receive interrupt */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
        USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART1, ENABLE);	
	USART_Cmd(USART2, ENABLE);	
	USART_Cmd(USART3, ENABLE);	
	USART_Cmd(UART5, ENABLE);	
        
        /*######################################################################
          #  UART 4 default  (TV control) 
          ######################################################################*/        
        USART_InitStructure.USART_BaudRate = 38400;//115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	/* Configure UART4 */
        USART_Init(UART4, &USART_InitStructure);

	/* Enable USART1 Receive interrupt */
//        USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);

        USART_Cmd(UART4, ENABLE);	
        
        /*######################################################################
          #  UART 5 = UCB communication
          ######################################################################*/  
        /*
#ifdef  WiFi_Module
	USART_InitStructure.USART_BaudRate = 115200;
#else
        USART_InitStructure.USART_BaudRate = 9600;
#endif
        
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	// Configure USART5 
	USART_Init(UART5, &USART_InitStructure);	
	// Enable USART5 Receive interrupt 
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
#ifdef  WiFi_Module
        USART_ITConfig(UART5,USART_IT_TC,ENABLE);
#endif
	USART_Cmd(UART5, ENABLE);	
        */
        /* Enable the USART1 Interrupt */
        
        NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
        
	/* Enable the USART2 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	

	/* Enable the USART3 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;// 2014.12.11
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);		
        
        /* Enable the USART4 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;// 2014.12.11
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);		
        
        /* Enable the USART5 Interrupt */
        
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);		
        
        
}

void HAL_BSP_Initial(void)
{
//  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//  TIM_OCInitTypeDef  TIM_OCInitStructure;
  //TIM_ICInitTypeDef  TIM_ICInitStructure;
  //ADC_InitTypeDef  ADC_InitStructure;
  

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
  
  HAL_NVIC_Configuration();
  
  /* Setup SysTick Timer for 1 msec interrupts  */
  SysTick_Config(SystemCoreClock / 1000);
  //===============================================================================================================  
  
  
  HAL_Usart_Initial();
  
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
    GPIO_SetBits(GPIOE,GPIO_Pin_13); //ERP Power 
    GPIO_SetBits(GPIOE,GPIO_Pin_14);//power ERP 12v inner
    GPIO_SetBits(GPIOE,GPIO_Pin_15);//power ERP 12v export
    GPIO_SetBits(GPIOE,GPIO_Pin_5);//power mode pwm operation
    GPIO_SetBits(GPIOD,GPIO_Pin_4);//usb port charge chip Enable pin
  }
  else if( mode == OFF)
  {
    GPIO_ResetBits(GPIOE,GPIO_Pin_13);
    GPIO_ResetBits(GPIOE,GPIO_Pin_14);
    GPIO_ResetBits(GPIOE,GPIO_Pin_15);
    GPIO_ResetBits(GPIOD,GPIO_Pin_4);    
  }
}

void HAL_Set_USB_Port_Mode(INT8 mode)
{
  if( mode == USB_CHARGE)
  {
    GPIO_ResetBits(GPIOE,GPIO_Pin_3);// usb in charge mode
  }
  else if( mode == USB_DATA)
  {
    GPIO_SetBits(GPIOE,GPIO_Pin_3); // usb in data transmission mode
  }
}
/*
mode 0: 0.1 A
mode 1: 0.5 A
mode 2: 1.0 A
mode 3: 2.1 A
*/
void HAL_Set_USB_Charge_Current(INT8 mode)
{
   if( mode&0x01 == 0)
  {
    GPIO_ResetBits(GPIOB,GPIO_Pin_8);
  }
  else
  {
    GPIO_SetBits(GPIOB,GPIO_Pin_8); 
  }
  
  if( mode&0x02 == 0)
  {
    GPIO_ResetBits(GPIOB,GPIO_Pin_9);
  }
  else
  {
    GPIO_SetBits(GPIOB,GPIO_Pin_9); 
  }
  
}

//Digital LCB function                         
void HAL_Set_LCB_Serial_Dir(UINT8 direction)
{
  if( direction == TXD)
  {
    GPIO_SetBits(GPIOA,GPIO_Pin_1);
  }
  else if( direction == RXD)
  {
    GPIO_ResetBits(GPIOA,GPIO_Pin_1);
  }
}
    
void HAL_LCB_Serial_Send_Data(UINT8 data)
{
  USART_SendData(USART2,data);
}

UINT8 HAL_LCB_Serial_Receive_Data(void)
{
  return USART_ReceiveData(USART2);
}

void HAL_Set_LCB_Serial_Tx(void)
{
  HAL_Set_LCB_Serial_Dir(TXD);//==>封包设为输出
  USART_ITConfig(USART2, USART_IT_TXE, ENABLE);//==>USART2中断打开
  USART_ITConfig(USART2, USART_IT_RXNE,DISABLE);//==>USART2 RX中断关闭
}
void HAL_Set_LCB_Serial_Rx(void)
{
  HAL_Set_LCB_Serial_Dir(RXD);//==>封包设为输入
  USART_ITConfig(USART2,USART_IT_TXE,DISABLE);//==>USART2 TX中断关闭
  USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//==>USART2 RX中断打开
}


//UCB Communication Function
void HAL_Com_Uart_Send_Data(UCHAR data)
{
    USART_SendData(UART5,data);
}
UCHAR HAL_Com_Uart_Receive_Data(void)
{
    return USART_ReceiveData(UART5);
}
void HAL_Com_TX_INT_Enable(void)
{
  USART_ITConfig(UART5, USART_IT_TXE, ENABLE);//==>USART2中断打开
}
void HAL_Com_TX_INT_Disable(void)
{
  USART_ITConfig(UART5,USART_IT_TXE,DISABLE);//==>USART2 TX中断关闭
}

//Fan 
USHORT HAL_Fan_TIM_GetCounter(void)
{
  return TIM_GetCounter(TIM4);
}
void HAL_Fan_TIM_SetCounter(USHORT Counter)
{
  TIM_SetCounter(TIM4,0);
}

void HAL_Set_Fan_Duty( USHORT duty)
{
  if(duty > 0)//setting value less than actual value 1
    duty--;
  TIM_SetCompare4(TIM3,duty);
}