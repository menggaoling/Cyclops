#include "HAL_ERP.h"


static struct
{
    unsigned SleepMode : 1;
    unsigned Sleeping : 1;
    unsigned LCBNoSleepMode : 1;
    unsigned WakeUpRetry : 1;
    unsigned LEDSwitch : 1;
}lcb;

unsigned short LCB_Delay[3];
unsigned char LCB_Timer[3];

// LCB EEPROM ERROR Content
unsigned short LCB_EEPROM_Memory_Size = 0;
//unsigned char GetLCBDeviceState = 0; //==> �õ�LCB Device ��Ϣ״̬
                         //==> 0 : Clear  1: Get OK  2: No Supply
unsigned char LCB_DeviceDataBuffer[20] = {0};
unsigned char LCB_GetingEEPRomFlag = 0;// ==>LCB EEPROM ����ȡ�����

// LCB EuPs
unsigned char LCB_EUPs_State = 0;
unsigned char LowPower_State = 0;
unsigned char SleepCount = 0,SleepCount_2 = 6, EnterSleepStep = 0, WakeUpRetryCount = 0;


//==============================================================================
//==> ʱ�䴦��
//==============================================================================
//�ر��ض�������
void LCB_TimerClose(unsigned char by_Dat)
{
  LCB_Timer[by_Dat]=0;
}
//ÿ1ms�����
void LCB_Timer_Int(void)
{
  static unsigned short LCB_Loop = 0;
  char i;
  
  if(LCB_Loop >= 1000)
  {// 1000 ms
      LCB_Loop = 0;
      for(i=0;i<3;i++)
      {
          if((LCB_Timer[i] == 1)&&(LCB_Delay[i] < 65000)) LCB_Delay[i]++;//==>�����ۼ���С��6500��ʱ
      }
  }
  else
      LCB_Loop ++;
}
//�������ĳһ������
void LCB_TimerCounterClear(unsigned char by_Index)
{
  LCB_Delay[by_Index]=0;
  LCB_Timer[by_Index]=1;
}
//�趨������ 1.LOOP or STEP 2.��?������ 3.����TIME
unsigned char LCB_TimerCounter(unsigned char by_Mode,unsigned char by_Who,unsigned short w_Dat)
{
  unsigned char b_Out;
  b_Out=0;
  if(LCB_Timer[by_Who]==1 && LCB_Delay[by_Who]>=w_Dat)
  {
        b_Out=1;
        if(by_Mode==T_LOOP) LCB_Delay[by_Who]=0;
        else LCB_Timer[by_Who]=0;
  }
  return(b_Out);
}

void Main_TimeDelay(unsigned char DelayTime)
{//==> DelayTime  unit: 1 second
    //LCB_Set_GetLCBEEPRomFlag(0);
    LCB_TimerCounterClear(0);
    while(!LCB_TimerCounter(T_STEP,0,DelayTime));
    LCB_TimerClose(0);
    return;
}
/*
//==============================================================================
//==>EuPs(Sleep Mode)����
// Return : 0 ==> δ����
//          1 ==> ׼������
//          2 ==> �� Sleep Mode ����
//==============================================================================
unsigned char LCB_SleepMode(unsigned short by_SystemStatus, unsigned char by_KeyState, unsigned char by_NowStatus, unsigned short LCB_SleepTime)
{//==> by_SystemStatus  1:AC  0:Self-Powered
    unsigned char by_Retn = 0;
    
    //==>20111219 EUPS test
    //if(by_KeyState == K_NONE && by_NowStatus == M_NOVE && by_SystemStatus == 1 && LCB_SleepTime != 0)
    if(EnterSleepStep == 2 ||(by_KeyState == K_NONE && by_NowStatus == M_NONE && by_SystemStatus == 1 && LCB_SleepTime != 0))
    {
         if(lcb.SleepMode)
         {// Sleep Mode
              by_Retn = 1;
              switch(EnterSleepStep)
              {
                  case 0: Show_Message(Message_EnterSleepMode,0,0,Show_Auto);
                          if(Led_Get_String_End())
                          {
                              Show_Message(Message_Blank,0,0,Show_Blank);
                              EnterSleepStep = 1;
                              LCB_TimerCounterClear(2);
                              Led_Set_Target(Display_OFF,0);
                              // 2014.01.21-4
                              if(EEPROM_ReadInformation(EE_TVPower) == _OFF && TV_GetPowe_OnOff() == 1)
                              {//==>��TV ON ʱ�� OFF
                                  TV_SetCommand(TVCom_POWER,TVData_PowerOFF);
                              }
                              //
                              
                          }
                          break;
                  case 1: 
                          Led_NumberSite(55,SleepCount_2-1);
                          if(LCB_TimerCounter(T_LOOP,2,1))
                              SleepCount_2 -= 1;
                          if(SleepCount_2 == 0)
                          {
                              if(!Check_SafeKey())
                              {// �������ذ���
                                  return 2;
                              }
                              EnterSleepStep = 2;
                              Digital_Command(CmdEUPsMode,LCB_EUPs_Implement);
                              SleepCount += 1;
                              LCB_TimerCounterClear(2);
                              //Led_StringParagraph(255,0);//==>������� 
                              Show_Message(Message_Blank,0,0,Show_Blank);
                              Led_Set_Target(Display_OFF,0);
                              //Led_FanControlReset();
                          }
                          break;  
                  case 2: 
                          if(LCB_EUPs_State == 1 || lcb.LCBNoSleepMode == 1)
                          {
                              lcb.Sleeping = 0;
                              while(!Low_PowerMode());
                              by_Retn = 2;
                              LCB_TimerClose(2);
                          }
                          else if(LCB_TimerCounter(T_STEP,2,2))
                          {
                              if(SleepCount >= 3 || LCB_EUPs_State == 0xff)
                              {
                                  SleepCount = 0;
                                  lcb.LCBNoSleepMode = 1;
                              }
                              else
                              {
                                  LCB_TimerCounterClear(2);
                                  Digital_Command(CmdEUPsMode,LCB_EUPs_Implement);
                                  SleepCount += 1;
                              }
                          } 
                          break;
              }      
         }
         else if(LCB_TimerCounter(T_STEP,2,LCB_SleepTime))
         {
              lcb.SleepMode = 1;
              Show_Message(Message_Blank,0,0,Show_Blank);
              Led_Set_Target(Display_OFF,0);
         }    
    }
    else
    { 
        lcb.LCBNoSleepMode = 0;
        LCB_TimerCounterClear(2);
        if(lcb.SleepMode)
            by_Retn = 1;
        lcb.SleepMode = 0;
        SleepCount = 0;
        EnterSleepStep = 0;
        SleepCount_2 = 6;    
        WakeUpRetryCount = 0;
        lcb.WakeUpRetry = 0;
    }
    return by_Retn;
}
*/
void LCB_SleepMode_Initial(void)
{
    LowPower_State = 0;
    SleepCount = 0;
    SleepCount_2 = 6;
    EnterSleepStep = 0;
    lcb.SleepMode = 0;
    lcb.LCBNoSleepMode = 0;
    lcb.Sleeping = 0;
    WakeUpRetryCount = 0;
    lcb.WakeUpRetry = 0;    
    lcb.LEDSwitch = 0;
    LCB_TimerCounterClear(2);
}

void HAL_ERP_Low_Power(void)
{//==>����ʡ��ģʽ
  EXTI_InitTypeDef   EXTI_InitStructure;
  
  
  Digital_Clear_ErrorCount();
  
  USART_DeInit(USART1) ;
  USART_DeInit(USART2) ;
  USART_DeInit(USART3) ;
  USART_DeInit(UART4) ;
  USART_DeInit(UART5) ;
  
  TIM_DeInit(TIM2);
  TIM_DeInit(TIM3);
  TIM_DeInit(TIM4);
  
  GPIO_AFIODeInit();
  HAL_ERP_GPIO_Reset(); 
  
  GPIO_DeInit(GPIOA);
  GPIO_DeInit(GPIOB);
  GPIO_DeInit(GPIOC);
  GPIO_DeInit(GPIOE);
  
  
  // MCU Enter STOP Mode
  EXTI_DeInit() ;
  PWR_ClearFlag(PWR_FLAG_WU);
  PWR_ClearFlag(PWR_FLAG_SB);

  //
  //====>> Ϊ���MCUʡ��ģʽ�趨ʹ��,��Ϊ�ⲿѶ�Ŵ���������
  // 0: PA
  // 1: PB
  // 2: PC
  // 3: PD
  // 4: PE
  // 5: PF
  // 6: PG
  //                                   
  // AFIO->EXTICR[0] 0~3      >> 0x0000xxxx
  // AFIO->EXTICR[1] 4~7      >> 0x0000xxxx
  // AFIO->EXTICR[2] 8~11     >> 0x0000xxxx
  // AFIO->EXTICR[3] 12~15    >> 0x0000xxxx
//  AFIO->EXTICR[2] = 0x00003333 ;
  AFIO->EXTICR[3] = 0x00003333 ;
  //
  // 
  /* Configure Key Button EXTI Line to generate an interrupt on falling edge */  
  EXTI_InitStructure.EXTI_Line = EXTI_Line14;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Event;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE ;
  EXTI_Init(&EXTI_InitStructure);
  
  
  /* Mode: SLEEP + Entry with WFE*/
  //__WFE();
  /* Mode: STOP + Regulator in ON + Entry with WFE*/
  PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFE);
  /* Request to enter STOP mode with regulator in low power mode*/
  //PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFE);
  EXTI_DeInit();
  
  
}

unsigned char LCB_Get_LEDSwitch(void)
{
  return lcb.LEDSwitch;
}
unsigned char LCB_Get_LowPower_State(void)
{
    return LowPower_State;
}
//===================================
// 2012.11.22
//void LCB_Set_LowPower_State(u8 by_D)
//{
//  LowPower_State = by_D;
//}
//===================================
void Digital_Set_LCBEUPsState(unsigned char by_Dat)
{
    LCB_EUPs_State = by_Dat;
}
unsigned char LCB_Get_EUPsState(void)
{
  return LCB_EUPs_State;
}

unsigned char LCB_GetSleepModeState(void)
{
    return lcb.SleepMode;
}

void HAL_ERP_GPIO_Reset(void)// Sinkyo..EUPs
{

    //=================================================================
    //====> PORT A
    //=================================================================
    HAL_Set_IO_OutputLow(GPIOE,GPIO_Pin_1);  //485 DIR
    HAL_Set_IO_High(GPIOE,GPIO_Pin_2);       //485 Tx
    HAL_Set_IO_High(GPIOE,GPIO_Pin_3); //485 Rx
    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_4); //USB detect
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_5);
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_6);
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_7);
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_8); 
    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_9); //RFID TX
    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_10); //RFID RX
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_11);
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_12);
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_13); //DEBUG 
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_14); //DEBUG 
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_15); //DEBUG 
    //=================================================================
    //====> PORT B
    //=================================================================
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_0);
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_1);
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_2);
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_3);//DEBUG 
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_4);//DEBUG 
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_5);
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_6);
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_7);
    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_8); //USB CURRRENT S0
    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_9); //USB CURRRENT S1
    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_10); //C_SAFE TX
    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_11); //C_SAFE RX
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_12);
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_13);
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_14);
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_15); 
    //=================================================================
    //====> PORT C
    //=================================================================
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_0);
    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_1); //C_SAFE CTS
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_2);
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_3);
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_4);
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_5);
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_6);
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_7);
    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_8);
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_9);
    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_10);  //TV RX
    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_11);  //TV TX
    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_12);  //UCB TX

    //=================================================================
    //====> PORT D
    //=================================================================
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_0);
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_1);
    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_2);  //UCB RX
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_3);
    HAL_Set_IO_Low(GPIOE,GPIO_Pin_4);        //USB EN 
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_5);
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_6);
    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_7);
    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_8);
    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_9);
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_10);
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_11);
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_12);
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_13);
    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_14);
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_15); 
    //=================================================================
    //====> PORT E
    //=================================================================
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_0);
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_1);
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_2);
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_3);
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_4);
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_5);
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_6);
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_7);
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_8);
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_9);
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_10);
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_11);
//    HAL_Set_IO_InputFloat(GPIOE,GPIO_Pin_12);
    HAL_Set_IO_Low(GPIOE,GPIO_Pin_13);       //POWER_EN1
    HAL_Set_IO_Low(GPIOE,GPIO_Pin_14);       //POWER_EN2
    HAL_Set_IO_Low(GPIOE,GPIO_Pin_15);       //POWER_EN3
}
//// start v2.001-1
//unsigned char LCB_QuickERP(unsigned char _SleepTime)
//{
//  unsigned char EnterSleepStep = 1;
//  unsigned char by_String[18] = {0};
//  
//  Show_Message(Message_Blank,0,0,Show_Blank);
//  Led_Set_Target(Display_OFF,0);
//  if(EEPROM_ReadInformation(EE_TVPower) == _OFF && TV_GetPowe_OnOff() == 1)
//  {//==>��TV ON ʱ�� OFF
//      TV_SetCommand(TVCom_POWER,TVData_PowerOFF);
//  }
//  lcb.LCBNoSleepMode = 0;
//  LCB_EUPs_State=0;
//  LCB_TimerCounterClear(2);
//  while(1)
//  {
//      switch(EnterSleepStep)
//      {
//          case 1: 
//                  if(_SleepTime != 0)
//                  {
//                      sprintf((char*)by_String, "%d",_SleepTime);
//                      Led_String(by_String, 0, Show_Auto);
//                      if(LCB_TimerCounter(T_LOOP,2,1))
//                      {
//                          _SleepTime -= 1;
//                      }
//                  }
//                  else 
//                  {
//                      if(Check_SafeKey()==0)
//                      {// �������ذ���
//                          LCB_TimerCounterClear(2);
//                          while(1)
//                          {
//                              Led_String("Safe key wrong",0,Show_Auto);
//                              if(LCB_TimerCounter(T_LOOP,2,3)) return M_RESET;
//                          }
//                      }
//                      EnterSleepStep = 2;
//                      Digital_Command(CmdEUPsMode,LCB_EUPs_Implement);
//                      SleepCount += 1;
//                      LCB_TimerCounterClear(2);
////                      Led_StringParagraph(255,0);//==>�������
//                      Led_Set_Target(Display_OFF,0);
////                      Led_FanControlReset();
//                  }
//                  break;  
//          case 2: 
//                  if(LCB_EUPs_State == 1 || lcb.LCBNoSleepMode == 1)
//                  {
//                      lcb.Sleeping = 0;
//                      while(!Low_PowerMode());
//                      return M_RESET;
//                  }
//                  else if(LCB_TimerCounter(T_STEP,2,2))
//                  {
//                      if(SleepCount >= 3 || LCB_EUPs_State == 0xff)
//                      {
//                          //SleepCount = 0;
//                          //lcb.LCBNoSleepMode = 1;
//                          LCB_TimerCounterClear(2);
//                          while(1)
//                          {
//                              Led_String("MCB not support",0,Show_Auto);
//                              if(LCB_TimerCounter(T_LOOP,2,5)) return M_NONE;
//                          }
//                      }
//                      else
//                      {
//                          LCB_TimerCounterClear(2);
//                          Digital_Command(CmdEUPsMode,LCB_EUPs_Implement);
//                          SleepCount += 1;
//                      }
//                  } 
//                  break;
//      }  
//  }
//}
//// end
