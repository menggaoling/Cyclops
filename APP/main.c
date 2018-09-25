#include "main.h"




UCHAR HeartRate = 0 ;
UCHAR TargetHR = 0;
USHORT Key_Value = K_NONE;
UCHAR Workout_Status = WORKOUT_RESET;



static struct
{

    UINT32 Stop      : 1;
    UINT32 ShowChang : 1;
    UINT32 WarmUp    : 1;
    UINT32 CoolDown  : 1;
    UINT32 THR_Over  : 1;
    UINT32 THR_Safe  : 1;
    UINT32 WorkEnd   : 1;
    UINT32 Enter     : 1;
    UINT32 KeyColse  : 1;
    UINT32 ErrorCode : 1;
    UINT32 GO        : 1;
    UINT32 FitTest   : 1;
    UINT32 Pause     : 1;
    UINT32 FirstTime : 1;
    UINT32 FirstHR : 1;
    UINT32 Choose    : 1;
    UINT32 ChangProgram : 1;
    UINT32 PowerON : 1;
    UINT32 ShowCoolDownStart : 1;
    UINT32 QuickStart : 1;
    UINT32 FitTest_END : 1;
    UINT32 USB_SaveSummary : 1;
    UINT32 RF900Show : 1;// 100.09.28
    UINT32 RF900ShowStart : 1;// 100.09.28
    UINT32 ShowInformation : 1;
    UINT32 HR_Avg : 1;
    UINT32 WiFi_SendSummary : 1;
    UINT32 HRT_Based : 1;
    UINT32 HR_Blink : 1;
    UINT32 UseNumKey : 1;
    // 2014.11.24
    UINT32 ErpMode : 1;
    UINT32 Check : 1; 

}bit;
  
union {
  struct {
         UCHAR Error_Code : 1;
         UCHAR CSafe_Status : 1;
         UCHAR RFID_Status : 1;
         UCHAR CSafe_Recv_Settings : 1;
         UCHAR RFID_UID : 1; 
         UCHAR Safety_Key : 1;
         UCHAR Blank0 : 1;
         UCHAR Blank1 : 1;
         //---------------------------         
         UCHAR Blank2 : 1;
         UCHAR Blank3 : 1;
         UCHAR Blank4 : 1;
         UCHAR Blank5 : 1;
         UCHAR Blank6 : 1;
         UCHAR Blank7 : 1;
         UCHAR Blank8 : 1;
         UCHAR Blank9 : 1;   
         // 2 Byte
         } Data ;
  UCHAR bytes[2] ;
} MainStatus ;

UCHAR by_Address=M_RESET;
UCHAR by_WorkoutDFNewProgram;//==>做为运动中重新设定新Program的判断基准,暂存新设定用  
UINT32 by_HoldBuffer;



void Main_UCB_CMD_Process(void)
{
  DATA_PACKAGE PackageData;
  DataUnion16 Temp_Value;
  USHORT Index = 0;

  if(Com_GetCommand(&PackageData))
  {
    switch(PackageData.Command)
    {
    case CMD_SET_MACHINE_INFO:
      Digital_GetMachineType();
      Console_MaxIncline(Data_Set,150);  
      Console_MaxSpeed(Data_Set,Param_MaxSpeed_MPH_DF);
      Console_StartSpeed(Data_Set,5);
      Console_Information(Info_StartIncline,Data_Set,0);     
      break;
    case CMD_SET_USER_INFO:
      Console_Information(Info_GENDER,Data_Set,D_Male);
      Console_Information(Info_WEIGHT,Data_Set,150);
      Console_Information(Info_AGE,Data_Set,30);  
      Console_Information(Info_HEIGHT,Data_Set,_Height_Mile_DF);
      Console_SpeedUnit(Data_Set,C_MILE);
      break;
    case CMD_WORKOUT_START:
      if(!Digital_Get_RPMStatus())
      {//wait for belt or incline motor stop
        Temp_Value.data8[0] = PackageData.Data[1];        
        Temp_Value.data8[1] = PackageData.Data[0];

        Console_Information(Info_WorkoutSpeed,Data_Set,Temp_Value.data16*10);
        
        Temp_Value.data8[0] = PackageData.Data[3];        
        Temp_Value.data8[1] = PackageData.Data[2];
        Console_Information(Info_StartIncline,Data_Set,Temp_Value.data16);       
        PackageData.Length = 0;
        Workout_Status = WORKOUT_RUNNING;
      }
      break;
    case CMD_SET_SPEED:
      if(Workout_Status == WORKOUT_RUNNING)
      {
        Temp_Value.data8[0] = PackageData.Data[1];        
        Temp_Value.data8[1] = PackageData.Data[0];
        Console_Information(Info_WorkoutSpeed,Data_Set,Temp_Value.data16*10);
        PackageData.Length = 0;
      }
      break;
    case CMD_SET_INCLINE:
       if(Workout_Status == WORKOUT_RUNNING)
       {
         Temp_Value.data8[0] = PackageData.Data[1];        
         Temp_Value.data8[1] = PackageData.Data[0];
         Console_Information(Info_WorkoutIncline,Data_Set,Temp_Value.data16/5);//(Incline /2)*10
         PackageData.Length = 0;
       }
      break;
    case CMD_SET_RESISTANCE:
      break;
    case CMD_SET_KEY_VALUE:
      Key_Value = PackageData.Data[0];
      PackageData.Length = 0;
      break;
    case CMD_GET_WORKOUT_DATA:
      if(Workout_Status == WORKOUT_RUNNING)
      {
        Index = 0;

        //time
        Temp_Value.data16 = Console_Parameter(Info_AccumulatedTime);//Console_Time(Data_Get,0);
        PackageData.Data[Index++] = Temp_Value.data8[1];
        PackageData.Data[Index++] = Temp_Value.data8[0];
        //speed
        Temp_Value.data16 = Console_Information(Info_WorkoutSpeed,Data_Get,0)/10;
        PackageData.Data[Index++] = Temp_Value.data8[1];
        PackageData.Data[Index++] = Temp_Value.data8[0];
        //incline
        Temp_Value.data16 = Console_Information(Info_WorkoutIncline,Data_Get,0)*5;//(Incline * 10)/2
        PackageData.Data[Index++] = Temp_Value.data8[1];
        PackageData.Data[Index++] = Temp_Value.data8[0];
        
        // Average Pace
        Temp_Value.data16 = 36000 / Console_Get_AvgSpeed();
        PackageData.Data[Index++] = Temp_Value.data8[1];
        PackageData.Data[Index++] = Temp_Value.data8[0];
        //Calories
        Temp_Value.data16 = Console_Get_Calories();
        PackageData.Data[Index++] = Temp_Value.data8[1];
        PackageData.Data[Index++] = Temp_Value.data8[0];
        //METs
        Temp_Value.data16 = Console_Get_METs();
        PackageData.Data[Index++] = Temp_Value.data8[1];
        PackageData.Data[Index++] = Temp_Value.data8[0];       
        //watts
        Temp_Value.data16 = (USHORT)((FLOAT)Console_Get_Calories_Hour() / 4.2);
        PackageData.Data[Index++] = Temp_Value.data8[1];
        PackageData.Data[Index++] = Temp_Value.data8[0];
        //Distance
        Temp_Value.data16 = (Console_Get_Distance()/10);
        PackageData.Data[Index++] = Temp_Value.data8[1];
        PackageData.Data[Index++] = Temp_Value.data8[0];   
        
        //RPM
        Temp_Value.data16 = 0;
        PackageData.Data[Index++] = Temp_Value.data8[1];
        PackageData.Data[Index++] = Temp_Value.data8[0];       
        
        //Resistance Level
        Temp_Value.data16 = 0;
        PackageData.Data[Index++] = Temp_Value.data8[1];
        PackageData.Data[Index++] = Temp_Value.data8[0];
        
        //SPM
        Temp_Value.data16 = 0;
        PackageData.Data[Index++] = Temp_Value.data8[1];
//        PackageData.Data[Index++] = Temp_Value.data8[0];       
        PackageData.Data[Index++] = KB_Get_Code();

//        {// Pace
//          36000 / (Console_Information(Info_WorkoutSpeed,Data_Get,0) / 10);//(Console_Speed(Data_Get,0)
//        }        
      }
      else
        memset(PackageData.Data, 0, 22); 
      PackageData.Length = 22;      
      break;
    case CMD_GET_ERROR_CODE:
      break;
    case CMD_SET_WORKOUT_STATE:
      break;
    case CMD_WORKOUT_STOP:
      {
        Index = 0;
        //time
        Temp_Value.data16 = Console_Parameter(Info_AccumulatedTime);//Console_Time(Data_Get,0);
        PackageData.Data[Index++] = Temp_Value.data8[1];
        PackageData.Data[Index++] = Temp_Value.data8[0];       
        // Average Pace
        Temp_Value.data16 = 36000 / Console_Get_AvgSpeed();
        PackageData.Data[Index++] = Temp_Value.data8[1];
        PackageData.Data[Index++] = Temp_Value.data8[0];   
        //Calories        
        Temp_Value.data16 = Console_Get_Calories();
        PackageData.Data[Index++] = Temp_Value.data8[1];
        PackageData.Data[Index++] = Temp_Value.data8[0]; 
        //METs        
        Temp_Value.data16 = Console_Get_METs();
        PackageData.Data[Index++] = Temp_Value.data8[1];
        PackageData.Data[Index++] = Temp_Value.data8[0];       
        //watts
        Temp_Value.data16 = (USHORT)((FLOAT)Console_Get_Calories_Hour() / 4.2);
        PackageData.Data[Index++] = Temp_Value.data8[1];
        PackageData.Data[Index++] = Temp_Value.data8[0];
        //Distance
        Temp_Value.data16 = Console_Get_Distance();
        PackageData.Data[Index++] = Temp_Value.data8[1];
        PackageData.Data[Index++] = Temp_Value.data8[0];
        
        PackageData.Length = 12;
        
        Workout_Status = WORKOUT_RESET;
      }
      break;
    case CMD_GET_RFID_STATE:
      break;
    case CMD_GET_CSAFE_STATUS:
      break;
    case CMD_GET_CSAFE_PROGRAM_SETTING:
      break;
    case CMD_SET_FAN_LEVEL:
      {
        Fan_Set_Level(PackageData.Data[0]);
        PackageData.Length = 0;
      }
      break;
    case CMD_SET_HEARTRATE:
      Temp_Value.data8[0] = PackageData.Data[1];        
      Temp_Value.data8[1] = PackageData.Data[0];
      HeartRate = Temp_Value.data16;
      PackageData.Length = 0;
      break;
    case CMD_CLEAR_RFID:
      break;
    case CMD_GET_VERSION:
      break;
    case CMD_ENTER_UPDATE:
      break;
    case CMD_UPDATE_PACKAGE_DATA:
      break;
    case CMD_ENTER_SLEEP:
      bit.ErpMode = TRUE;
      Timer_Counter_Clear(_Time_ERP);
      PackageData.Length = 0;
      break;
    case CMD_GET_KEY_VALUE:
      {
        PackageData.Data[0] = KB_Get_Code();
        PackageData.Length = 1;
      }
      break;      
    default:break;
    }
    
    

    PackageData.Data[PackageData.Length++] = MainStatus.bytes[1];
    PackageData.Data[PackageData.Length++] = MainStatus.bytes[0];
    
    Com_Send(PackageData.Command, PackageData.Length, PackageData.Data);
  }
}

   
/*******************************************************************************
* Function Name  : Workout_LevelChange()
* Description    : 运动期间须调整LEVEL参数
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
UCHAR Workout_LevelChange(void)
{
  UCHAR by_Status = M_NONE;
//  UCHAR by_Bu = Console_Information(Info_WorkoutSpeed,Data_Get,0)/10;
//  UCHAR _Total;
//  UINT16 by_K ;
//  
//  bit.key = 0;
//  Keyboard_NumberCount(3,K_BACK);
//  if(Console_SpeedUnit(Data_Get,0) == C_KM)
//      Console_Information(Info_WorkoutSpeed,Data_Set,80);
//  else
//      Console_Information(Info_WorkoutSpeed,Data_Set,50); 
//  Show_Message(Message_Level,0,0,Show_Auto);//==>Level
//  Led_Window(87,1,Console_Information(Info_LEVEL,Data_Get,0));
//  Timer_Counter_Clear(0);
//  
//  while(by_Status == M_NONE)
//  {
//      Led_ProfileMap(1);//单排闪
//      by_K = Key_Proce();
//      switch(by_K)
//      {
//          case K_PAUSE:
//                   return M_SP1;
//          case K_STOP:
//                   return M_SP2;
//          case K_SpeedU|K_MASK: 
//          case K_SpeedU:    
//          case K_SpeedD|K_MASK:
//          case K_SpeedD: 
//                   if(by_K == K_SpeedU||by_K == (K_SpeedU|K_MASK))
//                       Console_Information(Info_LEVEL,Data_Add,0);
//                   else
//                       Console_Information(Info_LEVEL,Data_Dec,0);
//                   Led_Window(87,1,Console_Information(Info_LEVEL,Data_Get,0));
//                   Timer_Counter_Clear(0);
//                   Keyboard_NumberCount(0,by_K);
//                   bit.key = 0;
//                   Led_ProfileMapBuffer(Console_ProgramMode(Data_Get,0),Console_Information(Info_LEVEL,Data_Get,0),0);
//                   Led_ProfileMap(0);//==>取图资秀出
//                   break;
//          case K_0:
//          case K_1:
//          case K_2:
//          case K_3:
//          case K_4:
//          case K_5:
//          case K_6:
//          case K_7:
//          case K_8:
//          case K_9:
//                   // 数字键换算累计值
//                   _Total = Keyboard_NumberCount(2,by_K);
//                   // 
//                   Led_Window(87,1,_Total);    
//                   Timer_Counter_Clear(0);
//                   bit.key = 1;
//                   break; 
//          case K_ENTER:
//                   if(bit.key == 0 || Console_Information(Info_LEVEL,Data_Set,_Total) == 1)
//                   {
//                       Led_ProfileMapBuffer(Console_ProgramMode(Data_Get,0),Console_Information(Info_LEVEL,Data_Get,0),0);
//                       Led_ProfileMap(0);//==>取图资秀出
//                       by_Status = M_NEXT;
//                       break;
//                   }
//          case K_BACK:  
//                   bit.key = 0;
//                   Led_Window(87,1,Console_Information(Info_LEVEL,Data_Get,0));
//                   Timer_Counter_Clear(0);
//                   Keyboard_NumberCount(0,by_K);
//                   break;
//      }
//      if(!Check_SafeKey()) by_Status = M_RESET;
//      if(Timer_Counter(T_STEP,0,50)) by_Status = M_NEXT;
//  }
//  if(by_Status != M_RESET && by_Status != M_NEXT)
//  {
//      Console_Information(Info_WorkoutSpeed,Data_Set,by_Bu * 10);//20150825 change program speed retard to 0.5/0.8
//  }
//  Keyboard_NumberCount(0,by_K);
  return by_Status;
}
/*******************************************************************************
* Function Name  : NavyFitnessTestStart
* Description    : Navy test开始
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NavyFitnessTestStart(void)
{
//   bit.WarmUp=0;
//   //Console_Time(Data_Set,0);
//   Console_Information(Info_TIME,Data_Set,0);
//   Console_UserTime(Data_Set,0);
//   Console_MaxTime(Data_Set,99*60);
//   //Console_Incline(Data_Set,2); 
//   Console_Information(Info_WorkoutIncline,Data_Set,2);
//   Console_Time_Cul();
//   Console_TimeOver(Data_Set,0);
//   Console_ClearDistance();
//   Console_Set_StartAccumulated(1);//==>×üê±???aê?à???
//   Console_Set_FitTestTimeAcc(1);
   
}

/*******************************************************************************
* Function Name  : Safe key check
* Input          : None
* Output         : GPIOB Pin 12 signal
* Return         : None
*******************************************************************************/
UCHAR Check_SafeKey(void)
{
    if(Digital_CheckSafeKeyStatus())
    {//==>当取得0x02B2错误码时
        Digital_ClearSafeKeyStatus();
        MainStatus.Data.Safety_Key = 1;
        by_WorkoutDFNewProgram=0;
        return 0;
    }
    if(IO_SafeKey()) 
    {//==>放开 eStop key
        if(MainStatus.Data.Safety_Key)
        {//==>当第一次放开时就下三道command进行LCB初始化,解放紧急开关动作
            MainStatus.Data.Safety_Key = 0;
            Digital_CommandBufferReset();//==>重置下控  
        }
        Digital_eSTOPKeyDelayClear();
    }
    else
    {//==>按下 eStop key
        if(!MainStatus.Data.Safety_Key)
        {//==>当第一次按下且未取得0x02B2错误码时
            if(Digital_eSTOPKeyDelay())
            {//==>持续 ms紧急开关成立
                MainStatus.Data.Safety_Key = 1;
                by_WorkoutDFNewProgram=0;
                return 0;
            }
        }
        else return 0; 
    }
    return 1; 
}


/*******************************************************************************
* Function Name  : BeltStop_StatusCheck
*                  在提示下另行判断是否结束运动模式
* Input          : None
* Output         : 是否结束 Workout
*******************************************************************************/                                            
UCHAR BeltStop_StatusCheck(void)
{
//  UCHAR by_Status = M_NONE;
  UCHAR _Out = 0;
//  UCHAR _Count = 10;
//  UCHAR _Mode = 0;
//  
//  
//  Led_Set_Reset();
//  while(by_Status == M_NONE)
//  {
//      Heart_Decoder();
//      if(_Mode == 0)
//      {
//          Show_Message(Message_BeltWillBeStoppedIn10Second,0,0,Show_Auto);
//          if(Led_Get_String_End() == 1) 
//          {
//              Show_Message(Message_Blank,0,0,Show_Auto);//Message_NoUserDetected
//              Led_NumberSite(49,1);
//              Led_NumberSite(55,0);
//              Timer_Counter_Clear(0);
//              _Mode = 2;
//          }
//      }
//      else
//      {
//          if(Timer_Counter(T_STEP,0,10))
//          {
//              if(_Count == 10)
//              {
//                  Show_Message(Message_Blank,0,0,Show_Auto);//Message_NoUserDetected
//              }
//              _Count -= 1;
//              Led_NumberSite(52,_Count);
//              Timer_Counter_Clear(0);
//              if(_Count == 0)
//              {
//                  _Out = 1;
//                  by_Status = M_NEXT;
//              }
//          }
//      }
//      switch(Key_Proce())
//      {
//          case K_STOP: 
//                      _Out = 2;
//                      by_Status = M_NEXT;
//                      break;
//          case K_PAUSE: 
//                      _Out = 3;
//                      by_Status = M_NEXT;
//                      break;            
//          case K_NONE:      
//                      break;
//          default:
//                      by_Status = M_NEXT;
//                      break;            
//      } 
//      if(Check_SafeKey() == 0)
//      {
//          _Out = 1;
//          by_Status = M_NEXT;
//      }   
//      if((Digital_CeckNonInUser == 0)||(HeartRate > 40)) by_Status = M_NEXT;//
//  }
//  by_InUserModeMotoSafeTime = 0;
//  by_NonInUserTime = 0;
//  Digital_CeckNonInUser = 0;
//  Timer_Counter_Clear(0);
//  Show_Message(Message_Blank,0,0,Show_Blank);
  return _Out;
}

/*******************************************************************************
* Function Name  : Start_CoolDown
* Description    : Cool Down模式进入初始化
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Start_CoolDown(void)
{   
  bit.WarmUp = 0;
  if(Console_ProgramMode(Data_Get,0) == TM_WFI)
  {
      by_HoldBuffer=180;
  }
  else
  {//==>Cool down Time 2 ~ 5 min
      by_HoldBuffer = (Console_Parameter(Info_AccumulatedTime) * 2) / 10 ;
      if(by_HoldBuffer > 300) by_HoldBuffer = 300;
      else if(by_HoldBuffer < 120) by_HoldBuffer = 120;  
  }
  //Console_Time(Data_Set,by_HoldBuffer);
  Console_Information(Info_TIME,Data_Set,by_HoldBuffer);
  Console_UserTime(Data_Set,by_HoldBuffer);
  Console_Time_Cul();
  Console_TimeOver(Data_Set,0);
  if(Console_ProgramMode(Data_Get,0) >= TM_Gerkin)
  {
      if(Console_ProgramMode(Data_Get,0) == TM_Navy)//==>980810
      {
          Console_Set_StartAccumulated(0);//==>总时间停止累计
      }
      Console_Set_FitTestTimeAcc(0);
  } 
  
  //Console_Incline(Data_Set,0); 
  Console_Information(Info_WorkoutIncline,Data_Set,0);
  //==> Now speed x 60%
  by_HoldBuffer=(UINT32)(Console_Information(Info_WorkoutSpeed,Data_Get,0) * 60) / 100;//Console_Speed(Data_Get,0)
  if(Console_SpeedUnit(Data_Get,0) == C_KM)
  {
      if(by_HoldBuffer < 80) by_HoldBuffer = 80;
      else if(by_HoldBuffer > 640) by_HoldBuffer = 640;
      if(Console_ProgramMode(Data_Get,0) == TM_WFI)
      {
          by_HoldBuffer = 480;
      }
  }
  else
  {
      if(by_HoldBuffer < 50) by_HoldBuffer = 50;
      else if(by_HoldBuffer > 400) by_HoldBuffer = 400;
      if(Console_ProgramMode(Data_Get,0) == TM_WFI)
      {
          by_HoldBuffer = 300;
      }
  }
  bit.KeyColse=0;//==>打开数字键功能
  //Console_Speed(Data_Set,by_HoldBuffer);
  Console_Information(Info_WorkoutSpeed,Data_Set,by_HoldBuffer);
  by_HoldBuffer=0;
  bit.ShowCoolDownStart=1;
  bit.CoolDown=1;

}  




//******************************************************************************
//==> ErrorCodeDisplay_Mode()
//==> 错误码显示模式
//==> 0:一般模式  1:运动模式
//******************************************************************************
char ErrorCodeDisplay_Mode(char by_D)
{
  char by_S = M_NONE;
//  UINT16 by_KeyInput = 0;
//  
//    // 耳机座保养提示
//  if(EEPROM_ReadInformation(EE_HJS_total) >= \
//     EEPROM_ReadInformation(EE_HJS_CheckNumber))
//  {
//      if(bit.HJS_ErrorCheckDisplay == 0)
//      {
//          bit.HJS_ErrorCheckDisplay=1;
//      Digtial_Set_ErrorCode(0x0304);
//      Digital_ErrorCode_ChangToAddress();
//      }
//      if(EEPROM_ReadInformation(EE_HJS_Switch) == D_Enable)
//      {
//          Show_Message(Message_Blank,0,0,Show_Blank);
//          Led_Set_Reset();
//          while(Led_Get_String_End() == 0)
//          {
//              Show_Message(Message_ReplaceEarphoneBoard,0,0,Show_Auto);
//          }
//      }       
//  }
//  if(by_D == 0)
//  {// Home Screen 一般模式
//  // Keypad Bad Notification 
//  if(EEPROM_ReadInformation(EE_KeyNotifiction) == D_Enable)
//  {
//      UCHAR i;
//     
//      // Have bad key
//      if(KeyScan_GetBypaddKeyNumber() != 0)
//      {
//              if(bit.KEY_ErrorCheckDisplay == 0)
//              {
//                  bit.KEY_ErrorCheckDisplay=1;
//          Digtial_Set_ErrorCode(0x0306);
//          Digital_ErrorCode_ChangToAddress();
//      }
//          }
//          else
//              bit.KEY_ErrorCheckDisplay=0;
//          // have up/lower/quick block need to check
//      for(i=Keypad_Upper;i<=Keypad_Quick;i++)
//      {
//          if(KeyScan_BadKeyRegion(i) == 1)
//          {
//              Led_Set_Reset();
//              while(Led_Get_String_End() == 0)
//              {
//                  Show_Message(Message_ReplaceUpToggleOverlay+(i-1),0,0,Show_Auto);
//              }
//          }
//      }
//  }
//  }
//  if(Digtial_Get_ErrorCode() != 0)
//  {
//      bit.ErrorCode = 1;
//      // 2014.11.24
//      if(bit.ErpMode == 1) return by_S;
//      //
//      if(Digital_CheckClassCError() == 1) 
//      {//==>Class C等级的错误码产生时
//          WiFi_OccurredErrorCode(Digtial_Get_ErrorCode());// AM system 
//          Show_Message(Message_Blank,0,0,Show_Blank);
//          Led_ClearProfileMap();
//          Led_String(" Error   :       ",0,0);
//          Led_Special(0,70,Digtial_Get_ErrorCode());
//          Digtial_Set_ErrorCode(0);//==>清除错误码暂存
//          Digital_ErroeCodeUpdate();
//          EEPROM_SaveError();//==>再将错误码存入
//          bit.ErrorCode = 0;
//          Digital_Set_DigitalSpeedTarget(0);//==>速度归零
//          Digital_Command(CmdSetInclineAction,0);//==>升降停止
//          // 2014.11.24
//          ChangeWorkoutState(WIFI_WORKOUT_STOP);
//          //
//          while(by_S == M_NONE)
//          {
//             by_KeyInput = Key_Proce();
//             by_S =  EngineerMode_SpecialMode(by_KeyInput);
//          }
//      }
//      else
//      {
//          if(by_D == 0)
//          {
//              if(EEPROM_ReadInformation(EE_ErrorCode) == D_Enable)
//              {//==>当开启Class C以下错误码显示
//                  Show_Message(Message_Blank,0,0,Show_Blank);
//                  Led_String(" Error   :       ",0,0);
//                  Led_Special(0,70,Digtial_Get_ErrorCode());
//                  Timer_Counter_Clear(0);
//                  while(!Timer_Counter(T_STEP,0,30)) 
//                  {
//                      by_KeyInput = Key_Proce();
//                      by_S =  EngineerMode_SpecialMode(by_KeyInput);
//                  }
//                  Show_Message(Message_Blank,0,0,Show_Blank);
//              }
//          }
//          Digtial_Set_ErrorCode(0);//==>清除错误码暂存
//      }
//  }
  return by_S; 
}


//==>从 Digital.c 回传资料
extern UCHAR Digital_CeckNonInUser;
extern UCHAR by_NonInUserTime;// 2014.02.12-1
extern UCHAR by_InUserModeMotoSafeTime; // 2014.02.12-1
extern UCHAR by_CheckInUserTimeNumber;//==>′ó Digital.c ??′?×êá?


void EEPROM_Read_Initial(void)
{
  UCHAR i;
 


  Console_SetDefaultVolume(15);
  Console_SetMaxVolume(32);
  Console_SetOutputVolume(13);
  //
  Console_MaxIncline(Data_Set,150);
  //Console_SetAM_Status(EEPROM_ReadInformation(EE_AM));
  by_CheckInUserTimeNumber = 30;
  if( by_CheckInUserTimeNumber == 0) by_CheckInUserTimeNumber = 0xFF;
  Console_NavyTestMode(0);//==>980810
  Console_SpeedUnit(Data_Set,C_MILE);
  //Console_Weight(Data_Set,EEPROM_ReadInformation(EE_WEIGHT));
  Console_Information(Info_HEIGHT,Data_Set,_Height_Mile_DF);
  Console_Information(Info_WEIGHT,Data_Set,150);
  Console_MaxTime(Data_Set,60*60);
  //Console_Time(Data_Set,EEPROM_ReadInformation(EE_USER_TIME)*60);
  Console_Information(Info_TIME,Data_Set,20*60);
//  if((EEPROM_ReadInformation(EE_MAX_ELEVATION) < EEPROM_ReadInformation(EE_LOW_ELEVATION))|| \
//     (EEPROM_ReadInformation(EE_MAX_ELEVATION) - EEPROM_ReadInformation(EE_LOW_ELEVATION) < (120*128)))
//  {// 当储存值跑掉时需要回默认值
//      EEPROM_WriteInformation(EE_LOW_ELEVATION,25 * 128);
//      EEPROM_WriteInformation(EE_MAX_ELEVATION,190 * 128);
//  }
  //Console_Level(Data_Set,EEPROM_ReadInformation(EE_LEVEL));
  Console_Information(Info_LEVEL,Data_Set,1);
  //Console_AGE(Data_Set,EEPROM_ReadInformation(EE_AGE));
  Console_Information(Info_AGE,Data_Set,30);
  Console_MaxSpeed(Data_Set,Param_MaxSpeed_MPH_DF);
  Console_StartSpeed(Data_Set,5);
  //Console_Gender(Data_Set,EEPROM_ReadInformation(EE_GENDER));
  Console_Information(Info_GENDER,Data_Set,D_Male);
  Console_Language(Data_Set,L_English);
  //Voice_SetSwitch(EEPROM_ReadInformation(EE_Speaker));
//  BEEP_SetSwitch(_ON);
  Console_SleepTime(Data_Set,0);
  // Incline
  Console_HighLowADC(Data_Set,0,33 * 128);
  Console_HighLowADC(Data_Set,1,207 * 128);
  // RPM
  Console_RPM_Parameter(1,1,DELTA_MaxRPM);// Set MaxRPM
  Console_RPM_Parameter(1,0,DELTA_MinRPM);// Set MinRPM
  //
  Console_HJS(Data_Set,0);
  //BLX_Set_RF_Service(EEPROM_ReadInformation(EE_RF_SERVICE));
  //BLX_Get_SerialNumber();
  //BLX_Get_ClubIDNumber();
  Console_Audio(Data_Set,Audio_OFF);
//  CSAFE_SetInformation(_Set_CommunicationMode,Console_Audio(Data_Get,0));
  for(i = 0;i < 10;i++)
  {
      Console_DigitalError(Data_Set,i,0);
  }
  
}

static UCHAR by_Status ;  
static UCHAR by_NoHRTime;//==>做为Gerkin program心跳侦测使用累计
static UCHAR by_PauseTimeShowChang;
static UCHAR by_HRSum;
static UCHAR by_Conversion;
//    UCHAR by_FitnessENDStatus = 0;
static UCHAR _EN957_HRZero ;
static UCHAR _EN957_Delay;
static UCHAR _ChangDFTime;
//
static UCHAR by_HRControlTime;
static UCHAR by_HRControlNumber;
static UINT16 by_HRBuffer;
static UINT16 by_PauseTime;
static UINT16 by_KeyNumber;//==>按键代码

void Main_Events_Process(void)
{
  KB_Key_Process();
//  SAFE_KEY_OFF()    
  if(!Check_SafeKey())
  {
    // Press eSTOP
    Workout_Status = WORKOUT_RESET;
    
    //Led_FanControlReset();//==>关闭风扇
    return;      
  }
    
    
  
//  WORKOUT_RESET()
  if(Workout_Status == WORKOUT_RESET)
  {
    if(!Check_SafeKey())
    {
      by_WorkoutDFNewProgram=0;
      if((Digital_GetStatus() & BIT0)||(Digital_GetStatus() & BIT1))// 2014.07.16
      {
        Digital_Command(CmdSetInclineAction,0);//==>升降停止
      }
    }
    Digital_Set_Work(1);//==>在运动模式下
    Digital_Set_DigitalSpeedTarget(0);//==>速度归零
    
    if(bit.ErrorCode)
    {//==>当有错误码产生时就储存
      bit.ErrorCode = 0;
      Digital_ErroeCodeUpdate();
//      EEPROM_SaveError();//==>再将错误码存入
    }
    
    
    if(Digtial_ElevationVariationCheck(0) == 1)
    {// 当升降实际值与目标值误差正负,就清除之前所设定的目标值参数然后在重新取得
      Digital_Clear_ElevationOld();//==>先清除旧的AD暂存值
      Console_Information(Info_WorkoutIncline,Data_Set,0);
    }
    
    Digital_Set_AutoCheck(0);//==>离开自动更正模式
    Console_Initial();
    Console_WorkoutStatus(Data_Set,0);
    Workout_Status = WORKOUT_IDLE;
  }
//  WORKOUT_IDLE()
  else if(Workout_Status == WORKOUT_IDLE)
  {
    if(Timer_Counter(T_STEP,5,50))
    {
      if(Digtial_ElevationVariationCheck(0) == 1)
      {// 当升降实际值与目标值误差正负,就清除之前所设定的目标值参数然后在重新取得
        Digital_Clear_ElevationOld();//==>先清除旧的AD暂存值

        Console_Information(Info_WorkoutIncline,Data_Set,0);
        Timer_Counter_Clear(5);
      }
    } 
    by_Address = M_START;
  }
  //  WORKOUT_RUNNING()
  else if(Workout_Status == WORKOUT_RUNNING)
  {
    if(by_Address == M_START || by_Address == M_ReWork )
    {
      by_Status = M_NONE;  
      by_NoHRTime = 0;//==>做为Gerkin program心跳侦测使用累计
      by_PauseTimeShowChang = 0;
      by_HRSum = 0;
      by_Conversion = 0;
      //    UCHAR by_FitnessENDStatus = 0;
      _EN957_HRZero = 0;
      _EN957_Delay = 0;
      _ChangDFTime = 0;
      //
      by_HRControlTime = 0;
      by_HRControlNumber = 0;
      by_HRBuffer=0;
      by_PauseTime=0;
      
      
      
      bit.THR_Over=0;
      bit.FitTest=0;
      bit.WorkEnd=0;
      bit.KeyColse=0;
      bit.FirstTime = 0;
      bit.FirstHR = 0;
      bit.ChangProgram=0;
      bit.ShowCoolDownStart=0;
      bit.FitTest_END = 0;
      bit.RF900Show = 0;
      bit.RF900ShowStart = 0;
      bit.Choose = 0;
      
      Timer_Close(Time_AllClose);//==>关闭所有Time计数器
      
      if(by_Address == M_START)
      {   
        bit.WarmUp=0;
        bit.CoolDown=0;  
        //        Workout_CsafeWorkoutInformation(0);//==> CSAFE所需信息放入
        
        Console_UserTime(Data_Set,Console_Information(Info_TIME,Data_Get,0));//Console_Time(Data_Get,0)
        switch(Console_ProgramMode(Data_Get,0))
        {
        case TM_TargetHR:
          bit.WarmUp = 1;
          by_HoldBuffer = Console_Information(Info_TIME,Data_Get,0);//Console_Time(Data_Get,0);
          Console_Information(Info_TIME,Data_Set,120);
          Console_UserTime(Data_Set,120);
          break;
        case TM_Navy:
          bit.WarmUp=1;
          Console_Information(Info_TIME,Data_Set,180);
          Console_UserTime(Data_Set,180);
          break;
        case TM_Gerkin:
        case TM_WFI:
          bit.KeyColse=1;//==>关闭数字键功能
          bit.WarmUp=1;
          by_HoldBuffer=15*60;
          Console_Information(Info_TIME,Data_Set,180);
          Console_UserTime(Data_Set,180);
          if(Console_SpeedUnit(Data_Get,0) == C_KM)
            Console_StartSpeed(Data_Set,20);//==>2KPH 
          else  
            Console_StartSpeed(Data_Set,13);//==>1.3MPH 
          break;
        case TM_5K:
        case TM_10K:  
        case TM_Marine:
        case TM_PEB:
        case TM_Army:
        case TM_Airforce:
          Console_Information(Info_TIME,Data_Set,0);
          Console_UserTime(Data_Set,0);
          Console_MaxTime(Data_Set,99*60);
          break;
          
        }
        
        Console_NewTime(Data_Set);
        
        
        
        Console_Information(Info_WorkoutSpeed,Data_Set,Console_StartSpeed(Data_Get,0)*10);
        Digital_Set_DigitalSpeedTarget(1);//==>开始追击目标速度
        
        
        Digital_Command(CmdGetTreadmillInUsed,0);//==>人员侦测
        Console_GetInclineInformation();
        if(Digtial_ElevationVariationCheck(1) == 1)
        {//==>当升降实际值与目标值误差时,就清除之前所设定的目标值参数然后在重新取得
          Digital_Clear_ElevationOld();//==>先清除旧的AD暂存值  
          Digital_Set_InclineCheck(1);
        } 
        by_Address=M_ReWork;
      }
      else
      {
        if(!bit.Stop)
        {//==>当机台为运动中时
          Digital_Set_DigitalSpeedTarget(1);//==>开始追击目标速度
          Digital_Set_InclineCheck(1);//==>升降设定
          if(bit.WarmUp)
          {
            by_HoldBuffer = Console_Information(Info_TIME,Data_Get,0);//Console_Time(Data_Get,0);
            Console_Information(Info_TIME,Data_Set,120);
            Console_UserTime(Data_Set,120);
          }
        }
      }
      
      //
      Console_Time_Cul();
      Console_TimeStart(1);//==>当开始运动时就启动时间计数
      Console_WorkoutStatus(Data_Set,1);
      if(Console_ProgramMode(Data_Get,0) == TM_Navy)
      {
        Console_Set_StartAccumulated(0);//==>总时间停止累计
        Console_Set_FitTestTimeAcc(0);
      }
      else
      {
        Console_Set_StartAccumulated(1);//==>总时间开始累计
        Console_Set_FitTestTimeAcc(1);
      }    
      by_WorkoutDFNewProgram=0;
      by_Address = M_NEXT;
    }
    if(by_Status == M_NONE)
    {
        //UsbFunction_USB(0x500);
      
#ifdef _DebugClose 
        if(ErrorCodeDisplay_Mode(1) == M_RESET)
        {//==>运动模式 
            by_Status = M_RESET; 
        }
#endif
        if(!bit.ChangProgram && !bit.Stop && !bit.Pause && !bit.WorkEnd)
        {//==>运动中     
            if(Console_Calories_Counter_Proce()||\
               Console_Distance_Counter_Proce() )
            {// Calories计算累计进位时
             // Distance计算累计进位时 
                bit.ShowInformation = 1;
            } 

            if(_ChangDFTime == 0 && Console_Time_Counter_Proce())
            {//==>每秒钟成立ㄧ次
                bit.ShowInformation = 1;// 信息显示
                //===========> ESD对策用
                // <=====================
                // CSAFE 数据处理
//                Workout_CsafeWorkoutInformation(0);//==> CSAFE所需信息放入
                //
                if(!bit.CoolDown)
                {//==>非Cool down模式下
                    switch(Console_ProgramMode(Data_Get,0))
                    {
                        case TM_Manual:
                                 if(Console_Time_Seg_Proce() == 1)
                                 {
                                 }
                                 break;
                        case TM_5K:    
                        case TM_10K:  
                                 if(Console_5K10K_Seg_Proce() == 0)
                                 {                                     
                                 }
                                 break;
                        case TM_Rolling:
                        case TM_FatBurn:
                                 if((Console_ProgramMode(Data_Get,0) == TM_Rolling ||  
                                     Console_ProgramMode(Data_Get,0) == TM_FatBurn) &&
                                     Console_Time_Seg_Proce() == 0)
                                 { 
                                     break;
                                 }
             
                                 Console_GetInclineInformation();
                                 // 非Incline窗口切换
                                 //
                                 break;
                        case TM_TargetHR:
                                 if(Console_Time_Seg_Proce() == 1)
                                 {
  
                                 }
                                 if(!bit.WarmUp)
                                 {
                                     if(Console_GetHRControlMode() == 0)
                                     {// Incline
                                         by_HRControlTime = 5;// 5秒
                                         by_HRControlNumber = 1;
                                     }
                                     else
                                     {// Speed
                                         by_HRControlTime = 20;// 20秒
                                         if(Console_SpeedUnit(Data_Get,0) == C_KM)
                                             by_HRControlNumber = 5;// 0.5 km/h
                                         else   
                                             by_HRControlNumber = 3;// 0.3 mph
                                     }
                                     if(by_HoldBuffer >= by_HRControlTime - 1)
                                     {//==>每x秒做ㄧ次
                                         bit.THR_Safe=0;
                                         by_HoldBuffer=0;
                                         by_HRBuffer /= by_HRSum;
                                         if(by_HRBuffer > 40 || HeartRate >= 240)
                                         {
                                             if(by_HRBuffer > 220-Console_Information(Info_AGE,Data_Get,0))//Console_AGE(Data_Get,0)
                                             {//==>心跳值 > 目标值的最大范围值
                                                 bit.THR_Over=1;
                                             }
                                             else if(by_HRBuffer > TargetHR+4 ||
                                                     by_HRBuffer < TargetHR-4 )
                                             {//==>心跳值 > 目标值+4
                                                 if(by_HRBuffer > TargetHR+4)
                                                 {
                                                     Console_HRControlChange(D_DEC,by_HRControlNumber);
                                                 }
                                                 else
                                                 {
                                                     Console_HRControlChange(D_ADD,by_HRControlNumber);
                                                 }
                                                 if(Console_GetHRControlMode() == 0)
                                                 {// Incline
                                            
                                                 }
                                                 else
                                                 {// Speed
          
                                                 }
                                                 bit.ShowInformation = 1;
                                             }
                                             else bit.THR_Safe=1;
                                         }
                                         by_HRSum=0;
                                         by_HRBuffer=0;                        
                                     }
                                     else 
                                     {//==>每秒累加ㄧ次
                                         by_HoldBuffer++;
                                         if(HeartRate >= 40)
                                         {//==>当为此范围时就进行累加的动作
                                             by_HRBuffer += HeartRate;
                                             by_HRSum++;
                                         }
                                     }
                                 }
                                 //==> EN957 
                                 if(HeartRate < 40)
                                 {// 当没有心跳时
                                     if(_EN957_Delay >= 10)
                                     {//==>每 10 秒调整速度一次
                                         if(Console_SpeedUnit(Data_Get,0) == C_KM)
                                             _EN957_Delay = 80;
                                         else
                                             _EN957_Delay = 50;
                                         if(Console_Information(Info_WorkoutSpeed,Data_Get,0) > _EN957_Delay)//Console_Speed(Data_Get,0)
                                         {// 非起速时   
                                             bit.RF900Show = 0;
                                             if(Console_SpeedUnit(Data_Get,0) == C_KM)
                                                 Console_Information(Info_WorkoutSpeed,Data_Dec,10);
                                             else
                                                 Console_Information(Info_WorkoutSpeed,Data_Dec,6);
                                             bit.ShowInformation = 1;
                                         }
                                         _EN957_Delay = 1;
                                     }
                                     else if(_EN957_Delay != 0)
                                     {
                                         _EN957_Delay++;  
                                     }
                                     else
                                     {
                                         if(_EN957_HRZero >= 30)
                                             _EN957_Delay = 1; 
                                         else 
                                             _EN957_HRZero++;
                                     }
                                 }
                                 else
                                 {
                                     _EN957_HRZero = 0;
                                     _EN957_Delay = 0;
                                 }
                                 break;
                        case TM_WFI:
                        case TM_Gerkin:
                                 
                                 if(Console_Time_Seg_Proce() == 1)
                                 {
                                 } 
                                 if(!bit.WarmUp)
                                 {
                                     
                                     if(HeartRate > TargetHR)
                                     {
                                         by_HRBuffer++;                                     
                                     }
                                     else
                                     {
                                         by_HRBuffer=0;
                                     }
                                     if(by_HRBuffer >= 15 || Console_Parameter(Info_AccumulatedTime) >= 1080)
                                     {//==>心跳大于目标值持续15秒或运动18分钟
                                         by_NoHRTime = 0;
                                         bit.FitTest_END = 1;
//                                         by_FitnessENDStatus = 1;//==>Fitness test 正常结束
                                     } 
                                     else 
                                     {
                                         by_Conversion = Console_GerkinConversion();
                                         if(by_Conversion != 0)
                                         {
                                             if(bit.RF900Show == 0)
                                             {
                                                 if(by_Conversion == 1 && by_NoHRTime == 0 && !bit.ShowCoolDownStart) 
                                                 {// Speed
                                      
                                                     //
                                                     bit.ShowInformation = 1;
                                                 }    
                                                 else if(by_Conversion == 2) 
                                                 {// Inlcine
                       
                                                     //
                                                     bit.ShowInformation = 1;
                                                 }    
                                             }
                                         }
                                     }
                                     if(HeartRate < 40)
                                     {//==>当没有HR讯号时
                                         by_NoHRTime++;
                                         if(by_NoHRTime >= 15)
                                         {//==>当持续无心跳15秒就结束模式
                                             by_NoHRTime = 0;
                                             bit.FitTest_END = 1;
//                                             by_FitnessENDStatus = 2;//==>Fitness test 非正常结束
                                         }    
                                     } 
                                     if(Console_ProgramMode(Data_Get,0) == TM_WFI && bit.FitTest_END)
                                     {
                                         bit.RF900Show = 0;
//                                         Led_Set_Reset();
//                                         while(!Led_Get_String_End())
//                                         {
//                                             Show_Message(Message_GoalAchieved,0,0,Show_Auto);
//                                         }
                                         Console_VO2();
                                     }
                                 }
                                 else
                                 {//依据EN957的起速
                                     if(_EN957_Delay < 3)
                                     {
                                         _EN957_Delay++;
                                     }
                                     else
                                     {
                                         if(Console_SpeedUnit(Data_Get,0) == C_KM)
                                             _EN957_HRZero = 48;
                                         else
                                             _EN957_HRZero = 30;
                                         if(Console_Information(Info_WorkoutSpeed,Data_Get,0)/ 10 < _EN957_HRZero)//Console_Speed(Data_Get,0)
                                         {// 非目标速度时
                                              bit.RF900Show = 0;
                                              Console_Information(Info_WorkoutSpeed,Data_Add,1);
                                              // 关Cool down字符串显示
                                              if(bit.ShowCoolDownStart == 1)
                                              {
                                                  bit.ShowCoolDownStart = 0;
                                              }
           
                                         }
                                     }
                                 }
                                 break;
                        case TM_PEB:
                        case TM_Army:  
                        case TM_Airforce: 
                        case TM_Navy: 
                        case TM_Marine:  
                                 if(Console_Time_Seg_Proce() == 1)
                                 {
                                 }
                                 if(Console_DistanceEND() == 1)
                                 {//==>Fitness Test的距离判断
                                     bit.RF900Show = 0;
                                     bit.FitTest_END = 1;
//                                     by_FitnessENDStatus = 1;//==>Fitness test 正常结束
                                     Console_ShowDistanceTarget(Data_Set,1);//==>使用一般Distance显示模式
                                 } 
                                 break;
                    }
                }
                else
                {//==>为Cool down模式下时
                    by_HoldBuffer++;
                    if(by_HoldBuffer % 20 == 0)
                    {//==>每20's秀一次提示字符串
                        by_HoldBuffer = 0;
                        bit.RF900Show = 0;
                        bit.ShowCoolDownStart = 1;
                        bit.ShowInformation = 0;// 关信息显示
                    }
                }
                if(by_Status != M_RESET && (bit.FitTest_END || bit.THR_Over || 
                   Console_TimeOver(Data_Get,0) || 
                  ((Console_DistanceGoal_Over()) && !bit.CoolDown))) 
                {//==>HR超出最大目标值或时间计数结束或5K或C-SAFE目标达成时        
                    bit.RF900Show = 0;
                    bit.FitTest_END = 0;
 

                    if(bit.WarmUp)
                    {//==>Warm up 结束
                        bit.WarmUp=0;      
                            if(Console_ProgramMode(Data_Get,0) == TM_Navy)
                            {
                                NavyFitnessTestStart();                            
                            }
                            else
                            {
                                Console_Information(Info_TIME,Data_Set,by_HoldBuffer);
                                Console_UserTime(Data_Set,by_HoldBuffer);
                                Console_Time_Cul();
                                Console_TimeOver(Data_Set,0);
                                by_HoldBuffer=0;
                                bit.ShowInformation = 1;
                            }
                            Console_NewTime(Data_Set);// 取目前的Time值
                        if(Console_ProgramMode(Data_Get,0) == TM_Gerkin || 
                           Console_ProgramMode(Data_Get,0) == TM_WFI) 
                        {
                            if(Console_ProgramMode(Data_Get,0) == TM_WFI)
                            {
                                TargetHR = ((UCHAR)((float)(208-(0.7*Console_Information(Info_AGE,Data_Get,0)))*0.85));//Console_AGE(Data_Get,0)
                            }
                            else
                            {                          
                                TargetHR = ((UCHAR)((float)(220-Console_Information(Info_AGE,Data_Get,0))*0.85));//Console_AGE(Data_Get,0)
                            }    
                            Console_MaxTime(Data_Set,by_HoldBuffer);
                            if(Console_SpeedUnit(Data_Get,0) == C_KM)
                                Console_StartSpeed(Data_Set,72);//==>7.2KPH
                            else  
                                Console_StartSpeed(Data_Set,45);//==>4.5MPH
                        }
                        
                    }
                    else if(!bit.THR_Over && !bit.CoolDown && !bit.FitTest &&
                            (Console_ProgramMode(Data_Get,0) < TM_Gerkin ||
                             Console_ProgramMode(Data_Get,0) > TM_Marine))
                    {//==>结束进入 Cool down
   
                        bit.ShowInformation = 1;
                    }
                    else
                    {//==>运动时间结束时
                        //Led_FanControlReset();//==>关闭风扇
                        Console_WorkoutStatus(Data_Set,0);
                        Digital_Set_DigitalSpeedTarget(0);//==>速度归零
                        Digital_Command(CmdSetInclineAction,0);//==>升降停止  
                        bit.WorkEnd=1;
                        Timer_Counter_Clear(2);
//                        CSAFE_Work_Exit();
//                        if(Fitness_Status(by_FitnessENDStatus) == 1)
                        {
                            by_Status=M_RESET; 
                        }
                    }    
                }
                
          
                //
            }// <<== 单位秒子程序截止
            
            if(bit.ShowCoolDownStart && bit.RF900Show == 0)
            {//==>秀COOL DOWN提示字符串用
                if(Console_ProgramMode(Data_Get,0) == TM_WFI)
                {//==>
//                    WFI_End_CoolDownShow();
                }    
                else
                {
//                    Show_Message(Message_CoolDown,0,0,Show_DownToUp);
//                    if(Led_Get_String_End())
                    {//==>跑完ㄧ次字符串
                        bit.ShowCoolDownStart=0;
                        bit.ShowInformation = 1;
                    }
                    _ChangDFTime = 0;
                    bit.FirstTime = 0;
                }
            }
            
            if(!bit.CoolDown && \
              (Console_ProgramMode(Data_Get,0) == TM_Gerkin || \
               Console_ProgramMode(Data_Get,0) == TM_WFI)) 
            {//==>在Gerkin or WFI program下
                if(by_NoHRTime > 0)
                {// 当没有心跳时
                    if(HeartRate > 40)
                    {//==>当有心跳值时就还原显示
                        by_NoHRTime=0;
                        bit.ShowInformation = 1;
                    }
                    else if(HeartRate >= 40)//Heart_Get_HRSignal()
                    {//==>当有HR讯号时
                    }    
                    else  
                    {//==>如果发现无心跳时就秀提示字符串
                        bit.RF900Show = 0;
//                        Show_Message(Message_GraspHRSensors,0,0,Show_Auto);
                    }   
                    
                }
            }
        }
        else
        {//==>运动暂停或切换Program时
            _ChangDFTime = 0;
            bit.FirstTime = 0;
            if(bit.ChangProgram)
            {//==>当有调整Program时
                if(by_WorkoutDFNewProgram != 0)
                { 

//                    if(Led_Get_String_End())
                    {//==>字符串跑完后秀LED灯
                        bit.ChangProgram=0;
                    }
                }
                else
                {

                    if(by_KeyNumber == K_ENTER)
                    {
                        bit.ChangProgram=0;
                    }
                }
                if(!bit.ChangProgram)
                {
                    switch(Console_ProgramMode(Data_Get,0))
                    {
                        case TM_5K:         
                        case TM_10K:   
                        case TM_Rolling:
                        case TM_FatBurn:
                                 if(by_WorkoutDFNewProgram == 0)
                                 {// 运动中切入此Porgram
                                     bit.KeyColse=0;
                                     by_Status = Workout_LevelChange();
                                     switch(by_Status)
                                     {// 进入LEVEL选项
                                         case M_RESET:
                                         case M_SP1:// Press PAUSE key
                                         case M_SP2:// Press STOP key  
                                                  break;
                                         default:
                                                  by_Status = M_NONE;
                                                  break;
                                     }
                                 }
                                 break;    
                    }
                    by_WorkoutDFNewProgram = 0;
                    Console_GetInclineInformation();// 取对应的Incline
                    bit.ShowInformation = 1;
                }
            }
            else
            {
                if(bit.Pause)
                {//==>Pause time for Engineer mode default
                    if(by_PauseTimeShowChang <= 5)
                    {
//                        Led_Window(Window_4,2,(by_PauseTime/60*100)+(by_PauseTime%60));
                    }    
                    else
                    {
 
                    }    
                    if(Timer_Counter(T_STEP,2,10))
                    {
                        Timer_Counter_Clear(2);
                        if(by_PauseTime == 0)
                        {
//                            CSAFE_Work_Exit(); 
                            by_Status=M_RESET;
                        } 
                        else by_PauseTime--;
                        if(by_PauseTimeShowChang == 5)
                        {   
                        }
                        by_PauseTimeShowChang++;
                    }
                }
                else if(bit.WorkEnd && bit.THR_Over)
                {//==>当运动结束且THR目标值过大时
//                    Show_Message(Message_ExceedingMaxHR,0,0,Show_Auto);
                    if(Timer_Counter(T_STEP,2,300))
                    {
//                        Show_Message(Message_Blank,0,0,Show_Blank);
                        by_Status=M_RESET;
                    }
                }
                else if(bit.WorkEnd || bit.Stop)
                {//==>Stop time and Workout END time = 30`s for Summary screen

//                    Show_WorkoutSummary();            
                    by_Status = M_RESET;
                }
            }
        }
        if(bit.WorkEnd ==0 && bit.Stop == 0 )
        {
        }   
        by_KeyNumber = Key_Value;
        Key_Value = K_NONE;
        //======================================================================  
        //==>> 人员侦测功能
#ifdef _TreadSense 
        if((by_KeyNumber != K_NONE)||(HeartRate > 40))
        {
            by_NonInUserTime = 0;
            Digital_CeckNonInUser = 0;
        }
        if(!bit.Stop && !bit.Pause && !bit.WorkEnd && Digital_CeckNonInUser == 1 )//
        {
            switch(BeltStop_StatusCheck())
            {
                case 1:by_Status = M_RESET;
                       break;
                case 2:by_KeyNumber = K_STOP;
                       break;
                case 3:by_KeyNumber = K_PAUSE;
                       break;
            }
        }
#endif        
        //======================================================================
        if(by_Status == M_SP1)
        {
            by_Status = M_NONE;
            by_KeyNumber = K_PAUSE;           
        }
        else if(by_Status == M_SP2)
        {
            by_Status = M_NONE;
            by_KeyNumber = K_STOP;  
        }
        switch(by_KeyNumber)
        {
            case K_STOP | K_MASK:
//                     CSAFE_Work_Exit();
                     by_Status = M_RESET;
                     break;
            //case K_PAUSE:         
            case K_CoolDown:
                     if(!bit.ChangProgram && !bit.CoolDown && !bit.Stop && !bit.Pause && !bit.WorkEnd)
                     {
                         if(Console_ProgramMode(Data_Get,0) >= TM_Gerkin) 
                         {// Fitness Test Program
                             by_NoHRTime=0;
                             bit.FitTest=1;
                             Console_ShowDistanceTarget(Data_Set,1);//==>使用一般Distance显示模式
                             if(Console_ProgramMode(Data_Get,0) == TM_WFI)
                             {
                                 Console_VO2();
                             }
                         }
                         if(Console_ProgramMode(Data_Get,0) == TM_5K ||\
                            Console_ProgramMode(Data_Get,0) == TM_10K)
                         {
                             Console_ShowDistanceTarget(Data_Set,1);//==>使用一般Distance显示模式
                         }
                         _ChangDFTime = 0;
                         bit.FirstTime = 0;
//                         by_FitnessENDStatus = 2;//==>Fitness test 非正常结束
                         Start_CoolDown(); 
                
                         bit.ShowInformation = 1;
                     }
                     bit.RF900Show = 0;// 100.09.28
                     break;  
                     
            case K_STOP:

                     if(!bit.Stop && !bit.WorkEnd /*&& 
                        CSAFE_GetInformation(_Get_Mode) != Csafe_Finished*/)
                     {                        
                         bit.Pause=0;
                         if(bit.ChangProgram)
                         {
                             bit.ChangProgram=0;
                             by_WorkoutDFNewProgram=0;
                         }
                         bit.ShowCoolDownStart=0;
                         _ChangDFTime = 0;
                         bit.Stop=1;
                         Console_TimeStart(0);
                         Console_WorkoutStatus(Data_Set,0);
                         Digital_Set_DigitalSpeedTarget(0);//==>速度归零
                         if(Digtial_ElevationVariationCheck(0) == 1)
                         {//==>当升降实际值与目标值误差正负,就清除之前所设定的目标值参数然后在重新取得
                             Digital_Clear_ElevationOld(); //==>先清除旧的AD暂存值
                         }
                         Console_Information(Info_WorkoutIncline,Data_Set,0);
                         Timer_Counter_Clear(2);
//                         CSAFE_Work_Exit();
                     }
                     else
                     {
                         by_Status = M_RESET;
                     }
                     bit.RF900Show = 0;
                     break;
            //case K_CoolDown:         
            case K_PAUSE:
                     bit.RF900Show = 0;
                     if(!bit.Stop && !bit.Pause && !bit.WorkEnd)
                     {
                         bit.ShowCoolDownStart=0;
                         bit.ChangProgram=0;
                         by_WorkoutDFNewProgram=0;
                         _ChangDFTime = 0;
                         Console_TimeStart(0);
                         Console_WorkoutStatus(Data_Set,0);
                         Digital_Set_DigitalSpeedTarget(0);//==>速度归零
                         Timer_Counter_Clear(2);
//                         if(CSAFE_GetInformation(_Get_Mode) == Csafe_In_Use)
//                         {
//                             CSAFE_Answer(Csafe_Paused);//==>暂时停止运动 
//                         }
                         by_PauseTime=0;
                         by_PauseTimeShowChang=0;
                         if(Console_ProgramMode(Data_Get,0) < TM_Gerkin)
                         {
                             bit.Pause=1;
                             Digital_Command(CmdSetInclineAction,0);//==>升降停止
//                             Console_StartSpeed(Data_Set,EEPROM_ReadInformation(EE_StartSpeed));
//                             by_PauseTime=EEPROM_ReadInformation(EE_PauseTime);
                             bit.RF900ShowStart = 0;
                             
                         }
                         else
                         {
                             bit.Stop = 1;
//                             CSAFE_Work_Exit();
                             Console_Information(Info_WorkoutIncline,Data_Set,0);
                         }
                         bit.ShowInformation = 0;
                         break;
                     } 
                     else
                     {
                         if(Console_ProgramMode(Data_Get,0) == TM_Gerkin)
                         {
//                             CSAFE_Work_Exit();
                             by_Status = M_RESET;
                         }
                         else
                         {
            case K_GO:    bit.RF900Show = 0;
                             if(!bit.Pause && Console_ProgramMode(Data_Get,0) == TM_Navy && bit.WarmUp)
                             {//==>当为Navy模式且在Warm up状态下时允许跳过Warm up直接进行测试
                                 NavyFitnessTestStart();
                                 Console_NewTime(Data_Set);// 取目前的Time值
                             }           
                             if(bit.Pause && !bit.WorkEnd)
                             {
                                 if(Digital_Get_RPMStatus())
                                 {
//                                     by_Status = Main_MotorStatusCheckMessage(1);
//                                     if(by_Status == M_NONE)
//                                     {
//                                         if(by_PauseTimeShowChang <= 5)
//                                             Show_Message(Message_Paused,0,0,Show_Auto);
//                                         else
//                                             Show_Message(Message_PressStartToResume,0,0,Show_Auto);
//                                     }
                                 }
                                 else
                                 {
                                     by_Status = M_START;
                                 }
                                 if(by_Status == M_START)
                                 {
                                    by_Status=M_NONE;              
                                    bit.Stop=0;
                                    bit.Pause=0;
                                    Console_TimeStart(1);
                                    Console_WorkoutStatus(Data_Set,1);
                                    if((Console_ProgramMode(Data_Get,0) != TM_Gerkin)&&
                                       (Console_ProgramMode(Data_Get,0) != TM_WFI))
                                    {
                                        Console_Information(Info_WorkoutSpeed,Data_Set,Console_StartSpeed(Data_Get,0)*10);
                                    }    
                                    Digital_Set_DigitalSpeedTarget(1);//==>开始追击目标速度
                                    if(Digtial_ElevationVariationCheck(1) == 1)
                                    {//==>当升降实际值与目标值误差正负时,就清除之前所设定的目标值参数然后在重新取得
                                        Digital_Clear_ElevationOld();//==>先清除旧的AD暂存值
                                        Digital_Set_InclineCheck(1);//==>升降设定
                                    }
//                                    if(CSAFE_GetInformation(_Get_Mode) == Csafe_Paused) 
//                                    {
//                                        CSAFE_Answer(Csafe_In_Use);//==>继续运动时  
//                                    }
                                 }
                             }
                         }
                     }
                     break;
            case K_TimeU|K_MASK: 
            case K_TimeU:
            case K_TimeD|K_MASK: 
            case K_TimeD:  
                     bit.Choose = 1; 
                     if(!bit.WarmUp && !bit.ChangProgram && !bit.CoolDown && !bit.Stop && !bit.Pause && !bit.WorkEnd && \
                       (Console_ProgramMode(Data_Get,0) == TM_Manual || 
                        Console_ProgramMode(Data_Get,0) == TM_TargetHR || 
                        Console_ProgramMode(Data_Get,0) == TM_Rolling || 
                        Console_ProgramMode(Data_Get,0) == TM_FatBurn))
                     {
                             // 关Cool down字符串显示
                             if(bit.ShowCoolDownStart == 1)
                             {
                                 bit.ShowCoolDownStart = 0;
                             }
                             //
                             _ChangDFTime = 1;
                             Console_Set_StartAccumulated(0);//==>总时间停止累计
                             if(bit.FirstTime == 0) 
                             {
                                 bit.FirstTime = 1;
                                 Console_NewTime(Data_Set);// 取目前的Time值
                             }    
                             else
                             {
                                 Timer_Counter_Clear(4);
                             }   
                             if(by_KeyNumber == K_TimeU || by_KeyNumber == (K_TimeU|K_MASK))
                                 Console_NewTime(Data_Add);
                             else
                                 Console_NewTime(Data_Dec);

                             bit.ShowInformation = 1;
                     }
                     bit.RF900Show = 0;// 100.09.28
                     break;
         
            case K_SpeedU|K_MASK: 
            case K_SpeedU:    
            case K_SpeedD|K_MASK:
            case K_SpeedD:  
                     bit.Choose = 1; 
                     if(!bit.ChangProgram && !bit.Stop && !bit.KeyColse && !bit.Pause && !bit.WorkEnd)// && !bit.CoolDown
                     {

                         // 关Cool down字符串显示
                         if(bit.ShowCoolDownStart == 1)
                         {
                             bit.ShowCoolDownStart = 0;
                         }

                         //
                         if(by_KeyNumber == K_SpeedU || by_KeyNumber == (K_SpeedU|K_MASK))
                           Console_Information(Info_WorkoutSpeed,Data_Add,(by_KeyNumber & K_MASK)? 99:1);//by_SpeedBuffer);
                         else
                             Console_Information(Info_WorkoutSpeed,Data_Dec,1);//by_SpeedBuffer);

                         bit.ShowInformation = 1;
                         if(_ChangDFTime == 1)
                         {
                             _ChangDFTime = 2;
                         }
                     }
                     bit.RF900Show = 0;// 100.09.28
                     break;
            case K_InclineU:
            case K_InclineU|K_MASK:
            case K_InclineD:
            case K_InclineD|K_MASK:  
                     bit.Choose = 1; 
                     if(Console_ProgramMode(Data_Get,0) == TM_Navy && !bit.WarmUp) break;//==>980810 
                     if(!bit.ChangProgram && !bit.Stop && !bit.KeyColse && !bit.Pause && !bit.WorkEnd)
                     {
                         // 关Cool down字符串显示
                         if(bit.ShowCoolDownStart == 1)
                         {
                             bit.ShowCoolDownStart = 0;
                         }
                         //
                         if(by_KeyNumber == K_InclineU || by_KeyNumber == (K_InclineU|K_MASK))
                             Console_Information(Info_WorkoutIncline,Data_Add,0);
                         else
                             Console_Information(Info_WorkoutIncline,Data_Dec,0);
   
                         //
                         bit.ShowInformation = 1; 
                         if(_ChangDFTime == 1)
                         {
                             _ChangDFTime = 2;
                         }
                     }
                     bit.RF900Show = 0;// 100.09.28
                     
                     break;
            case K_ENTER:
                     if(!bit.ChangProgram && !bit.Stop && !bit.KeyColse && !bit.Pause && !bit.WorkEnd)
                     {

                     } 
                     else if(bit.ChangProgram)
                     {
                         if(by_WorkoutDFNewProgram > 4)
                         {               
//                             CSAFE_Work_Exit();
                             by_Status = M_RESET;
                         }
                     }
                     if(_ChangDFTime == 1)
                     {
                         _ChangDFTime = 2;
                     }
                     bit.RF900Show = 0;
                     break;
            case K_MANUAL:
            case K_FatBurn:
            case K_Rolling:
                     if(!bit.CoolDown && !bit.Stop && !bit.Pause && !bit.WorkEnd)
                     {
                         bit.ChangProgram=1;
                         if(Console_ProgramMode(Data_Get,0) != TM_Manual && \
                            Console_ProgramMode(Data_Get,0) != TM_Rolling && \
                            Console_ProgramMode(Data_Get,0) != TM_FatBurn  )      
                         {
                             if(by_KeyNumber == K_MANUAL && by_WorkoutDFNewProgram != TM_Manual)
                             {
                                 by_WorkoutDFNewProgram = TM_Manual;
                             }
                             else if(by_KeyNumber == K_FatBurn && by_WorkoutDFNewProgram != TM_FatBurn)
                             {
                                 by_WorkoutDFNewProgram = TM_FatBurn;
                             }
                             else if(by_KeyNumber == K_Rolling && by_WorkoutDFNewProgram != TM_Rolling)
                             {
                                 by_WorkoutDFNewProgram = TM_Rolling;
                             }
                             else
                             {
//                                 CSAFE_Work_Exit();
                                 Timer_Counter_Clear(8);//solve when press program key twice 
                                 by_Status=M_RESET;  //reset current program can't reset issue
                             }
                         }    
                         else 
                         {
                             Timer_Counter_Clear(1);
                             by_WorkoutDFNewProgram=0;
                             if(by_KeyNumber == K_MANUAL)
                             {

                                 Console_ProgramMode(Data_Set,TM_Manual);
                             }
                             else if(by_KeyNumber == K_FatBurn)
                             {
                                 Console_ProgramMode(Data_Set,TM_FatBurn);
                             }
                             else if(by_KeyNumber == K_Rolling)
                             {
                                 Console_ProgramMode(Data_Set,TM_Rolling);
                             }
                         }    

                         if(_ChangDFTime == 1)
                         {
                             _ChangDFTime = 2;
                         }
                     } 
                     bit.RF900Show = 0;
                     break;

            case K_Training:
                     if(!bit.CoolDown  && !bit.Stop && !bit.Pause && !bit.WorkEnd) 
                     {
                         bit.ChangProgram=1;
                         if(Console_ProgramMode(Data_Get,0) != TM_5K && \
                            Console_ProgramMode(Data_Get,0) != TM_10K )
                         {
                             if(by_WorkoutDFNewProgram != TM_5K &&\
                                by_WorkoutDFNewProgram != TM_10K)
                             {
                                 if(Console_ProgramMode(Data_Get,0) == TM_5K)
                                     by_WorkoutDFNewProgram=TM_10K;
                                 else 
                                     by_WorkoutDFNewProgram=TM_5K;
                             }
                             else
                             {
//                                 CSAFE_Work_Exit();
                                 Timer_Counter_Clear(8);
                                 by_Status=M_RESET;
                             }
                         }    
                         else 
                         {
                             //bit.Choose = 1; 
                             Timer_Counter_Clear(1);
                             by_WorkoutDFNewProgram=0;
                             if(Console_ProgramMode(Data_Get,0) == TM_5K)
                             {
                                 Console_ProgramMode(Data_Set,TM_10K);
                             }
                             else
                             {
                                 Console_ProgramMode(Data_Set,TM_5K);
                             }
                         }
                         if(_ChangDFTime == 1)
                         {
                             _ChangDFTime = 2;
                         }
                     } 
                     bit.RF900Show = 0;
                     break;

            case K_TargetHR:
                     if(!bit.Stop && !bit.CoolDown  && !bit.Pause && !bit.WorkEnd)
                     {
//                         bit.ChangProgram=1;
                         if(Console_ProgramMode(Data_Get,0) != TM_TargetHR)
                         {
                             if(by_WorkoutDFNewProgram != TM_TargetHR)
                             {
                                 by_WorkoutDFNewProgram=TM_TargetHR;
                             }
                             else
                             {
//                                 CSAFE_Work_Exit();
                                 Timer_Counter_Clear(8);
                                 by_Status=M_RESET;
                             }
                             bit.ChangProgram=1;
                         }   
                         if(_ChangDFTime == 1)
                         {
                             _ChangDFTime = 2;
                         }
                     }  
                     bit.RF900Show = 0;
                     break;
            case K_FitnessTest:
                     if(!bit.Stop && !bit.CoolDown  && !bit.Pause && !bit.WorkEnd)
                     {
//                         bit.ChangProgram=1;
                         if(Console_ProgramMode(Data_Get,0) < TM_Gerkin)
                         {                             
                             if(by_WorkoutDFNewProgram != TM_Gerkin)
                             {
                                 by_WorkoutDFNewProgram=TM_Gerkin;
                             }
                             else
                             {
//                                 CSAFE_Work_Exit();
                                 Timer_Counter_Clear(8);
                                 by_Status=M_RESET;
                             }
                             bit.ChangProgram=1;
                         }
                         if(_ChangDFTime == 1)
                         {
                             _ChangDFTime = 2;
                         }
                     }  
                     bit.RF900Show = 0;
                     break;  
                     

        }
        // 5K RUN and 10K RUN program change use
        if(bit.Choose == 1)
        {
            bit.Choose = 0; 
            if(bit.ChangProgram == 1)
            {
                if(Console_ProgramMode(Data_Get,0) == TM_5K ||\
                   Console_ProgramMode(Data_Get,0) == TM_10K )
                {
                    Timer_Counter_Clear(1);
                    by_WorkoutDFNewProgram=0;
                    if(Console_ProgramMode(Data_Get,0) == TM_5K)
                     {
                        Console_ProgramMode(Data_Set,TM_10K);
                     }
                    else
                     {
                        Console_ProgramMode(Data_Set,TM_5K);
                    }
                }
            }
        }

        //
        if(_ChangDFTime == 2 ||(_ChangDFTime == 1 && Timer_Counter(T_STEP,4,50)))
        {// 当调整运动时间后5秒或中途设为确认
            _ChangDFTime = 0;
            if(Console_ProgramMode(Data_Get,0) == TM_TargetHR)
            {// 因有两分钟的Warm up
                Console_Information(Info_TIME,Data_Set,Console_NewTime(Data_Get) - (Console_Parameter(Info_AccumulatedTime)-120));
            }
            else
                Console_Information(Info_TIME,Data_Set,Console_NewTime(Data_Get) - Console_Parameter(Info_AccumulatedTime));
            if(Console_Information(Info_TIME,Data_Get,0) != 0)
            {
                Console_UserTime(Data_Set,Console_NewTime(Data_Get));
                Console_ChangeTimeCul();// 重新计算单排时间
                Console_Set_StartAccumulated(1);//==>总时间累计start
            } 
            else
            {
                Console_TimeOver(Data_Set,1);//==>设定时间结束
            }
            bit.ShowInformation = 1;
        }

        //
        
        if(Check_SafeKey() == 0) 
        {
//            CSAFE_Work_Exit();
            by_Status = M_RESET;
        }
    }
    else if(by_Status == M_RESET)
    {
      Workout_Status = WORKOUT_RESET;
      by_Address = M_START;
    }
    // 
    //Led_FanControlReset();//==>关闭风扇
//    return by_Status;
}
}


void Main_Data_Initial(void)
{
  
  LCB_Initial();
  
  CSAFE_Initial();
  CSAFE_Power_On();
  
  TV_Initial();
  CAB_Initial();

  RFID_GATRM310_Initial();
  
  Com_Initial();
  
  Timer_Initial();
  
  Fan_Initial_Data();
  
  EEPROM_Read_Initial();
  
  KB_Initial();
}

void Main_ERP_Process(void)
{
  if(bit.ErpMode == TRUE && Timer_Counter(T_STEP,_Time_ERP, 5))   //收到命令后等待500ms,留够时间让回复命令发送到UI
  {
    HAL_Set_ERP_Power(OFF);
    HAL_ERP_Low_Power();
    HAL_System_Reset();
  }
}

int main(void)
{
  HAL_System_Initial();
  Main_Data_Initial();
  static UCHAR fan_level = 0;
  UCHAR Pre_level = fan_level;
  while(1)
  {
//    CSAFE_EngTest();
//    for(UINT i = 0;i < 800000;i++);  
//    Com_Send(0x0201, 0, 0);    
    Main_UCB_CMD_Process();
    Main_Events_Process();
    Main_ERP_Process();
    if(Pre_level != fan_level)
    {
      Fan_Set_Level(fan_level);
      Pre_level = fan_level;
    }
    CAB_SendCommand(_CAB_VOL_DOWN);
//    RFID_GATRM310_ReadDCF();
//    TV_SetCommand(TVCom_LAST,0);
//    if(CSAFE_TestStatus() == 1)
//    {
//      Digital_Command(CmdInitial,0);
//    }
  }

//  return 0;
}