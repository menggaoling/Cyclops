// v1 => 初版模組化沿用現有的CSAFE軟體建立.
// v2 => 加入大部分的command對應.
// v3 => 修改RF900的控制判斷.
// v4 => 修改非900控制判斷
// v5 => 軟體優化
// v6.20160519 => 模組化調整簡化
#include "C_safe.h"

static struct
{
    unsigned long Enable          : 1;
    unsigned long Reset           : 1;
    unsigned long Error1          : 1;//作為錯誤訊息回覆指標
    unsigned long Error2          : 1;
    unsigned long Report          : 1;//做為是否回覆判斷
    unsigned long Report_1        : 1;//做為判斷當主機設定參數給我時,如果訊框超出範圍而繼續傳第二或第三次訊框時
    unsigned long HR_Max          : 1;//做為判斷C-SAFE是否有去設定心跳最大值
    unsigned long End             : 1;//目標值結束判斷用
    unsigned long Stop            : 1;//判斷儀錶暫時停止運動時
    unsigned long RX_OK           : 1;
    unsigned long RX_Head         : 1;
    unsigned long RX_TEST         : 1;
    unsigned long FC              : 1;
    unsigned long ERROR           : 1;
    unsigned long EngTest         : 1;
    unsigned long EngCheck        : 1;
    unsigned long SetTX           : 1;
    unsigned long RF900_Return    : 1;
    unsigned long NetPulse        : 1;
    unsigned long HR_Avg          : 1;
    unsigned long CheckAnyKey     : 1;
    unsigned long TX_Switch       : 1;
    // sent and get flag
    unsigned long UserInfo        : 1;
    unsigned long Level           : 1;
    unsigned long Time            : 1;
    unsigned long Heart           : 1;
    unsigned long Distance        : 1;
    unsigned long Calories        : 1;
    unsigned long Online          : 1;
    unsigned long Watts           : 1;
    unsigned long Speed           : 1;
    unsigned long Grade           : 1; 
    unsigned long Gear            : 1;
    unsigned long Torque          : 1;
    unsigned long METs            : 1;
    unsigned long ChannelRange    : 1;
    unsigned long VolumeRange     : 1;
    unsigned long AudioMute       : 1;
    unsigned long AudioChannel    : 1;
    unsigned long AudioVolume     : 1;
    unsigned long Start           : 1;
    unsigned long CardioAudioMute : 1;
    //
} Csafe;

#define Max_RX_Buffer  5

unsigned char aby_RX_Buffer[Max_RX_Buffer][50];//RX input 32
unsigned char aby_RX_UP_List[12];//欲上載資料暫存
unsigned char aby_TX_Buffer[40];//TX output
unsigned char by_Buffer;
unsigned char by_RX_IN;//RX input Data Commend Toto
unsigned char com_i,j;
unsigned char _RX_Total = 0;
unsigned char _RX_Target = 0;
unsigned char by_TTTest = 0;
//======== UART =========
unsigned char by_RX_Point=0;
unsigned char by_TX_Point=0;
unsigned char by_TX_Length=0;
//==============================================================================
union {
  struct {
         unsigned char Gender ;
         unsigned char AGE ;
         unsigned char Level;
         unsigned char Time;
         unsigned char Program;
         unsigned char CSAFE_State;
         unsigned char Timeout;
         unsigned char SetGear;
         unsigned char AudioMute;
         unsigned char SetAudioChannel;
         unsigned char SetAudioVolume; 
         unsigned char CommunicationMode;
         unsigned char WeightUnits;
         unsigned char Incline;
         unsigned char Manufacturer;
         unsigned char CID;
         unsigned char Model;
         unsigned char Version;
         unsigned char Release;
         unsigned char Units;
         unsigned char DistanceUnits;
         unsigned char VerticalUnits; 
         unsigned char SpeedUnits;
         unsigned char Gear;
         unsigned char AudioChannel; 
         unsigned char AudioVolume;  
         unsigned char UserID;
         unsigned char ID_Number[5];
         unsigned char RF900_ControlBuffer[7];
         unsigned char HRDelay;
         unsigned char NowHR;
         unsigned char HR_Cur;
         unsigned char HR_Max;
         unsigned char HR_Target;
         unsigned char HR_Zone_Min;
         unsigned char HR_Zone_Max;
         unsigned char SetHRMax;
         unsigned char HR_Avg;
         // 38x1
         unsigned short ConstWatts;
         unsigned short Weight;
         unsigned short SetSpeed;
         unsigned short SetGrade;
         unsigned short SetTorque;
         unsigned short SetMETs;
         unsigned short SetChannelRange;
         unsigned short SetVolumeRange; 
         unsigned short AvgSpeed;
         unsigned short HR_TZone;
         unsigned short WorkoutTime;
         unsigned short WorkoutDistance;
         unsigned short WorkoutVertical;
         unsigned short Target_Distance;
         unsigned short Target_Calories;
         unsigned short WorkoutCalories;
         unsigned short Speed;
         unsigned short METs;
         unsigned short Watts;
         unsigned short MotorCurrent;
         unsigned short CsafeErrorCode;
         unsigned short Torque;   
         unsigned short TargetDistance;
         unsigned short TargetCalories;
         // 22x2
         unsigned long ClockTime;
         unsigned long ClockDate;
         unsigned long AccumulatedDistance;
         unsigned long AccumulatedTime;
         unsigned long Serial;
         unsigned long ServiceCode;
         // 6x4 
         } Data ;
  unsigned char Block[106] ;
} CSAFE_Info ;

//==============================================================================

void CSAFE_Initial(void)
{
  Csafe.RF900_Return = 0;
  Csafe.CheckAnyKey = 0;
  Csafe.SetTX = 0;
  Csafe.RX_OK=0;//===>input 判斷
  Csafe.RX_Head=0;
  Csafe.RX_TEST=0;
  Csafe.FC=0;
  Csafe.ERROR=0;
  Csafe.Online=0;
  Csafe.EngTest=0;
  Csafe.Error1=0;
  Csafe.Error2=0;
  Csafe.Start=0;
  Csafe.Report=0;
  Csafe.HR_Max=0;
  Csafe.Level=0;
  Csafe.Watts=0;
  Csafe.Distance=0;
  Csafe.Calories=0;
  Csafe.Heart=0;
  Csafe.Time=0;
  Csafe.End=0;
  Csafe.Stop=0;
  Csafe.NetPulse = 0; 
  Csafe.TX_Switch=0;
  _RX_Total = 0;
  CSAFE_Info.Data.SetHRMax=0;
  CSAFE_Info.Data.HR_TZone=0;
  CSAFE_Info.Data.HR_Zone_Min=0;
  CSAFE_Info.Data.HR_Zone_Max=0;
  CSAFE_Info.Data.HR_Avg=0;
  CSAFE_Info.Data.HR_Max=0;
  CSAFE_Info.Data.Time=0;
  CSAFE_Info.Data.Program = 0;
  CSAFE_Info.Data.Serial = 0;
  CSAFE_Info.Data.UserID=5;
  CSAFE_Info.Data.CSAFE_State = Csafe_Ready;
}
void CSAFE_Reset(void)//==>將C-SAFE系統參數重置
{
  Csafe.Error1=0;
  Csafe.Error2=0;
  Csafe.Start=0;
  Csafe.Report=0;
  Csafe.HR_Max=0;
  Csafe.Level=0;
  Csafe.Watts=0;
  Csafe.Distance=0;
  Csafe.Calories=0;
  Csafe.Heart=0;
  Csafe.Time=0;
  Csafe.End=0;
  Csafe.Stop=0;
  Csafe.UserInfo = 0;
  Csafe.Speed = 0;
  Csafe.Grade = 0; 
  Csafe.Gear = 0;
  Csafe.Torque = 0;
  Csafe.METs = 0;
  Csafe.ChannelRange = 0;
  Csafe.VolumeRange = 0;
  Csafe.AudioMute = 0;
  Csafe.AudioChannel = 0;
  Csafe.AudioVolume = 0;
  
  CSAFE_Info.Data.MotorCurrent = 0;
  CSAFE_Info.Data.CsafeErrorCode = 0;
  CSAFE_Info.Data.ServiceCode = 0;
  CSAFE_Info.Data.Gear = 0;
  CSAFE_Info.Data.Torque = 0;
  CSAFE_Info.Data.SetHRMax=0;
  CSAFE_Info.Data.HR_TZone=0;
  CSAFE_Info.Data.HR_Zone_Min=0;
  CSAFE_Info.Data.HR_Zone_Max=0;
  CSAFE_Info.Data.HR_Max=0;
  CSAFE_Info.Data.Time=0;
  CSAFE_Info.Data.Program = 0;
  CSAFE_Info.Data.Weight = 0;
  CSAFE_Info.Data.AGE = 0;
  CSAFE_Info.Data.AvgSpeed = 0;
  CSAFE_Info.Data.Timeout = 0;
  CSAFE_Info.Data.ClockTime = 0;
  CSAFE_Info.Data.ClockDate = 0;
  CSAFE_Info.Data.WorkoutTime = 0;
  CSAFE_Info.Data.WorkoutDistance = 0;
  CSAFE_Info.Data.WorkoutVertical = 0;
  CSAFE_Info.Data.AccumulatedTime = 0; 
  CSAFE_Info.Data.AccumulatedDistance = 0;
  CSAFE_Info.Data.WorkoutCalories = 0;
  CSAFE_Info.Data.Speed = 0;
  CSAFE_Info.Data.Weight = 0;
  CSAFE_Info.Data.Watts = 0;
  CSAFE_Info.Data.METs = 0;
  CSAFE_Info.Data.CommunicationMode = 0;
  //
  CSAFE_Info.Data.UserID=5;
  CSAFE_Info.Data.Gender = 1;
  //
}
//==============================================================================
//==> CSAFE 中斷 1`ms
//==============================================================================
void CSAFE_Counter_Int(void)
{
  static char by_Error=0;
  static unsigned int by_StatusChang=0;
  
  if(!Csafe.Start && (CSAFE_Info.Data.CSAFE_State == Csafe_In_Use || CSAFE_Info.Data.CSAFE_State == Csafe_Paused))
  {
      if(by_StatusChang >= 60000)//==>1分鐘後進行切換
      {
          by_StatusChang = 0;
          if(CSAFE_Info.Data.CSAFE_State == Csafe_In_Use)
              CSAFE_Answer(Csafe_Paused);
          else
              CSAFE_Answer(Csafe_In_Use);
      }
      else by_StatusChang++;
  }
  else by_StatusChang = 0;
  
  if(!Csafe.SetTX && (_RX_Target != _RX_Total))
  {
      if(by_TTTest < 3) by_TTTest++;
      else
      {
          by_TTTest = 0;  
          if(CSAFE_Info.Data.CommunicationMode != Audio_RF900)// v4 2012.09.12
          {// 非RF900控制器
              CSAFE_RX_Buffer_Decision();
          }
          else
          {// RF900控制器
              Csafe.RF900_Return = 1;
              if(aby_RX_Buffer[_RX_Target][1] == 0xE7)
              {
                  CSAFE_900M_E7DataAnalysis();
              }
          }
          _RX_Target++;
          if(_RX_Target >= Max_RX_Buffer) _RX_Target = 0;
          by_Error = 0;
      }
  }
  else
  {
      by_TTTest = 0;
      if(Csafe.RX_Head) by_Error++;//==>當沒有結束的F2時將做累計,也可做為Mast端多次傳輸時使用
      if(Csafe.ERROR || by_Error > 150)
      {//==>當有錯誤時
          by_Error = 0;
          Csafe.ERROR = 0;
          Csafe.RX_Head = 0;
          Csafe.Error2 = 1;
      }
  }
}
//==============================================================================
//==> 判斷RX_Buffer內的資料屬性  
//==============================================================================
void CSAFE_RX_Buffer_Decision(void)
{
  by_RX_IN=2;
  Csafe.Reset=0;
  Csafe.Report_1=0;
  Csafe.RX_TEST = 0;
  if(CSAFE_Mode_Judge()) 
      Csafe.Report=1;
  else 
      Csafe.Report=0;
  for(com_i = 0;com_i < (aby_RX_Buffer[_RX_Target][49] - 1);com_i++)
  {
      if(CSAFE_Mode_Check_Answer(aby_RX_Buffer[_RX_Target][com_i]) == 1)
      {// 在各MODE中可以接收其 COMMAND 時
          switch(aby_RX_Buffer[_RX_Target][com_i])
          {
              case cmdSetMETs:   
                        if(!Csafe.Report)
                        {
                            com_i+=2;
                            CSAFE_Info.Data.SetMETs = aby_RX_Buffer[_RX_Target][com_i+1];//Hi byte
                            CSAFE_Info.Data.SetMETs = CSAFE_Info.Data.SetMETs << 8;
                            CSAFE_Info.Data.SetMETs += aby_RX_Buffer[_RX_Target][com_i];//Low byte
                            com_i+=1;
                            Csafe.METs = 1;
                        }
                        break;
              case cmdSetChannelRange: 
                        if(!Csafe.Report)
                        {
                            com_i+=2;
                            CSAFE_Info.Data.SetChannelRange = aby_RX_Buffer[_RX_Target][com_i + 1];//Hi byte
                            CSAFE_Info.Data.SetChannelRange = CSAFE_Info.Data.SetChannelRange << 8;  
                            CSAFE_Info.Data.SetChannelRange += aby_RX_Buffer[_RX_Target][com_i];//Low byte
                            com_i+=1;
                            Csafe.ChannelRange = 1;
                        }
                        break;
              case cmdSetVolumeRange: 
                        if(!Csafe.Report)
                        {
                            com_i+=2;
                            CSAFE_Info.Data.SetVolumeRange = aby_RX_Buffer[_RX_Target][com_i + 1];//Hi byte
                            CSAFE_Info.Data.SetVolumeRange = CSAFE_Info.Data.SetVolumeRange << 8;  
                            CSAFE_Info.Data.SetVolumeRange += aby_RX_Buffer[_RX_Target][com_i];//Low byte
                            com_i+=1;
                            Csafe.VolumeRange = 1;
                        }
                        break;
              case cmdSetSpeed:
                        if(!Csafe.Report)
                        {
                            com_i+=2;
                            CSAFE_Info.Data.SetSpeed = aby_RX_Buffer[_RX_Target][com_i + 1];//Hi byte
                            CSAFE_Info.Data.SetSpeed = CSAFE_Info.Data.SetSpeed << 8;  
                            CSAFE_Info.Data.SetSpeed += aby_RX_Buffer[_RX_Target][com_i];//Low byte
                            com_i+=2;
                            Csafe.Speed = 1;
                        }
                        break;
              case cmdSetGrade:
                        if(!Csafe.Report)
                        {
                            com_i+=2;
                            CSAFE_Info.Data.SetGrade = aby_RX_Buffer[_RX_Target][com_i + 1];//Hi byte
                            CSAFE_Info.Data.SetGrade = CSAFE_Info.Data.SetGrade << 8;  
                            CSAFE_Info.Data.SetGrade += aby_RX_Buffer[_RX_Target][com_i];//Low byte
                            com_i+=2;
                            Csafe.Grade = 1;
                        }
                        break;
              case cmdSetTorque: 
                        if(!Csafe.Report)
                        {
                            com_i+=2;
                            CSAFE_Info.Data.SetTorque = aby_RX_Buffer[_RX_Target][com_i + 1];//Hi byte
                            CSAFE_Info.Data.SetTorque = CSAFE_Info.Data.SetTorque << 8;  
                            CSAFE_Info.Data.SetTorque += aby_RX_Buffer[_RX_Target][com_i];//Low byte
                            com_i+=2;
                            Csafe.Torque = 1;
                        }
                        break;          
              case cmdSetAudioMute: 
                        if(!Csafe.Report)
                        {
                            com_i+=2;
                            CSAFE_Info.Data.AudioMute = aby_RX_Buffer[_RX_Target][com_i];
                            Csafe.AudioMute = 1;
                        }
                        break;
              case cmdSetAudioChannel: 
                        if(!Csafe.Report)
                        {
                            com_i+=2;
                            CSAFE_Info.Data.SetAudioChannel = aby_RX_Buffer[_RX_Target][com_i];
                            Csafe.AudioChannel = 1;
                        }
                        break;
              case cmdSetAudioVolume:  
                        if(!Csafe.Report)
                        {
                            com_i+=2;
                            CSAFE_Info.Data.SetAudioVolume = aby_RX_Buffer[_RX_Target][com_i];
                            Csafe.AudioVolume = 1;
                        } 
                        break;
              case cmdSetGear:
                        if(!Csafe.Report)
                        {
                            com_i+=2;
                            CSAFE_Info.Data.SetGear = aby_RX_Buffer[_RX_Target][com_i];
                            Csafe.Gear = 1;
                        }
                        break;
              case cmdSetTime:
                        if(!Csafe.Report)
                        {
                            com_i+=2;
                            CSAFE_Info.Data.ClockTime += aby_RX_Buffer[_RX_Target][com_i+2];
                            CSAFE_Info.Data.ClockTime = CSAFE_Info.Data.ClockTime << 8;
                            CSAFE_Info.Data.ClockTime += aby_RX_Buffer[_RX_Target][com_i+1];
                            CSAFE_Info.Data.ClockTime = CSAFE_Info.Data.ClockTime << 8;
                            CSAFE_Info.Data.ClockTime += aby_RX_Buffer[_RX_Target][com_i];
                            com_i+=2;
                        }
                        break;
              case cmdSetDate:  
                        if(!Csafe.Report)
                        {
                            com_i+=2;
                            CSAFE_Info.Data.ClockDate = aby_RX_Buffer[_RX_Target][com_i+2];
                            CSAFE_Info.Data.ClockDate = CSAFE_Info.Data.ClockDate << 8;
                            CSAFE_Info.Data.ClockDate += aby_RX_Buffer[_RX_Target][com_i+1];
                            CSAFE_Info.Data.ClockDate = CSAFE_Info.Data.ClockDate << 8;
                            CSAFE_Info.Data.ClockDate += aby_RX_Buffer[_RX_Target][com_i];
                            com_i+=2;
                        }
                        break;
              case cmdSetTimeout:
                        if(!Csafe.Report)
                        {
                            com_i+=2;
                            CSAFE_Info.Data.Timeout = aby_RX_Buffer[_RX_Target][com_i];
                        }
                        break;
              case cmdUpList:
                        if(!Csafe.Report)
                        {
                            com_i+=1;
                            by_Buffer = aby_RX_Buffer[_RX_Target][com_i];
                            aby_RX_UP_List[9] = aby_RX_Buffer[_RX_Target][com_i]; //取得顯示資訊項目數
                            for(j = 0;j < by_Buffer;j++) 
                            {
                                aby_RX_UP_List[j]=aby_RX_Buffer[_RX_Target][com_i+j+1];
                            }
                            com_i+=by_Buffer;
                        }
                        break;
              case cmdIDDigits:
                        if(!Csafe.Report)
                        {
                            com_i+=2;
                            CSAFE_Info.Data.UserID = aby_RX_Buffer[_RX_Target][com_i];//取得USER ID的輸入位數
                        }
                        break;
              case cmdSetTWork:
                        if(!Csafe.Report)
                        {
                            com_i+=2;
                            if(aby_RX_Buffer[_RX_Target][com_i] <= 1) by_Buffer = aby_RX_Buffer[_RX_Target][com_i]*60;
                            else by_Buffer=60;
                            com_i+=1;
                            by_Buffer+=aby_RX_Buffer[_RX_Target][com_i];
                            com_i+=1;
                            CSAFE_Info.Data.Time = by_Buffer;
                            Csafe.Report_1=1;
                            Csafe.Time=1;
                        }
                        break;
              case cmdSetHorizontal:
              case cmdSetVertical:
                        if(!Csafe.Report)
                        {
                            com_i+=2;
                            CSAFE_Info.Data.TargetDistance = aby_RX_Buffer[_RX_Target][com_i]+(aby_RX_Buffer[_RX_Target][com_i+1]*256);
                            com_i+=2;
                            Csafe.Report_1=1;
                            Csafe.Distance=1;
                        }
                        break;
              case cmdSetCalories:
                        if(!Csafe.Report)
                        {
                            com_i+=2;
                            CSAFE_Info.Data.TargetCalories=aby_RX_Buffer[_RX_Target][com_i]+(aby_RX_Buffer[_RX_Target][com_i+1]*256);
                            com_i+=1;
                            Csafe.Report_1=1;
                            Csafe.Calories=1;
                        }
                        break;
              case cmdSetProgram:
                        if(!Csafe.Report)
                        {
                            com_i+=2;
                            CSAFE_Info.Data.Program=aby_RX_Buffer[_RX_Target][com_i];
                            com_i+=1;
                            CSAFE_Info.Data.Level = aby_RX_Buffer[_RX_Target][com_i];
                            Csafe.Report_1=1;
                            Csafe.Level=1;
                        }
                        break;
              case cmdSetUserInfo:
                        if(!Csafe.Report)
                        {
                            com_i+=2;
                            CSAFE_Info.Data.Weight = aby_RX_Buffer[_RX_Target][com_i]+(aby_RX_Buffer[_RX_Target][com_i+1]*256);
                            com_i+=3;
                            CSAFE_Info.Data.AGE = aby_RX_Buffer[_RX_Target][com_i];
                            com_i+=1;
                            CSAFE_Info.Data.Gender = aby_RX_Buffer[_RX_Target][com_i];
                            Csafe.Report_1=1;
                            Csafe.UserInfo = 1;
                        }
                        break;
              case cmdSetLevel:
                        if(!Csafe.Report)
                        {
                            com_i+=2;
                            CSAFE_Info.Data.Level = aby_RX_Buffer[_RX_Target][com_i];
                            Csafe.Report_1=1;
                            Csafe.Level=1;
                        }
                        break;
              case cmdSetTargetHR:
                        if(!Csafe.Report)
                        {
                            com_i+=2;
                            CSAFE_Info.Data.HR_Target = aby_RX_Buffer[_RX_Target][com_i];
                            Csafe.Heart=1;
                            Csafe.Report_1=1;
                        }
                        break;
              case cmdSetHRZone:
                        if(!Csafe.Report)
                        {
                            com_i+=2;
                            CSAFE_Info.Data.HR_Zone_Min = aby_RX_Buffer[_RX_Target][com_i];
                            CSAFE_Info.Data.HR_Zone_Max = aby_RX_Buffer[_RX_Target][com_i+1];
                            com_i+=1;
                            Csafe.Report_1=1;
                        }
                        break;
              case cmdSetHRMAX:
                        if(!Csafe.Report)
                        {
                            com_i+=2;
                            CSAFE_Info.Data.SetHRMax = aby_RX_Buffer[_RX_Target][com_i];
                            Csafe.Report_1=1;
                            Csafe.HR_Max=1;
                        }
                        break;
              case cmdSetPower: 
                        if(!Csafe.Report)
                        {
                            com_i+=2;
                            CSAFE_Info.Data.ConstWatts = aby_RX_Buffer[_RX_Target][com_i]+(aby_RX_Buffer[_RX_Target][com_i+1]*256);
                            com_i+=2;
                            Csafe.Report_1=1;
                            Csafe.Watts=1;
                        }
                        break;
              case cmdReset:
                        CSAFE_Info.Data.CSAFE_State=Csafe_Ready;
                        Csafe.FC = 0;
                        break;
              case cmdGoIdle:
                        if(!Csafe.Report) 
                        {
                            // NetPluse 
                            CSAFE_Info.Data.Program=0;  
                            Csafe.NetPulse = 0;
                            CSAFE_Reset();
                            //
                            CSAFE_Info.Data.CSAFE_State = Csafe_IDle;
                        }
                        break;
              case cmdGoHavelID:
                        if(!Csafe.Report) 
                        {
                            // NetPluse 
                            Csafe.NetPulse = 1;
                            //
                            CSAFE_Info.Data.CSAFE_State=Csafe_HaveID;
                        }
                        break;
              case cmdGoInUse:
                        if(!Csafe.Report)
                        {
                            CSAFE_Info.Data.CSAFE_State=Csafe_In_Use;
                            Csafe.Start=0;
                            Csafe.Report_1=0;
                            Csafe.Reset=0;
                        }
                        break;
              case cmdGoFinished:
                        if(!Csafe.Report) CSAFE_Info.Data.CSAFE_State=Csafe_Finished;
                        break;
              case cmdGoReady:
                        if(!Csafe.Report)
                        {
                            CSAFE_Reset();
                            CSAFE_Info.Data.CSAFE_State=Csafe_Ready;
                        }
                        break;
              case cmdBadID:
              case cmdGetStatus:
                        break;
              case cmdGetSpeed:
              case cmdGetProgram:
              case cmdGetCalories:
              case cmdGetHorizontal:
              case cmdGetVertical:
              case cmdGetTWork:
              case cmdGetPace:          
              case cmdGetGrade://(Incline)
              case cmdGetGear:
              case cmdGetTorque:    
                        if(!Csafe.Report)
                        {
                            CSAFE_RX_Answer_Buffer(aby_RX_Buffer[_RX_Target][com_i]);
                        }
                        break;
              case cmdGetUpList:
                        if(!Csafe.Report)
                        {
                            by_Buffer=aby_RX_UP_List[9];
                            for(j=0;j<by_Buffer;j++) CSAFE_RX_Answer_Buffer(aby_RX_UP_List[j]);
                        }
                        break;			  
              case cmdGetUserInFo:
              case cmdGetHRCur:
              case cmdGetHRTZone:
              case cmdGetHRAvg:
              case cmdGetHRMax:
              case cmdGetOdometer:  
              case cmdGetVersion:
              case cmdGetID:
              case cmdGetUnits:
              case cmdGetList:
              case cmdGetUtilization:
              case cmdGetPower:
              case cmdGetMETs:  
              case cmdGetSerial:  
              case cmdGetMotorCurrent:
              case cmdGetErrorCode:
              case cmdGetServiceCode:
              case cmdGetAudioChannel:
              case cmdGetAudioVolume:
              case cmdGetAudioMute:
                        CSAFE_RX_Answer_Buffer(aby_RX_Buffer[_RX_Target][com_i]);
                        break;
          }
      }
      else
      {
          Csafe.Reset=1;
          Csafe.Error1=1;
      }
  }
  if((aby_RX_Buffer[_RX_Target][0] == 0x00 && (Csafe.Error1 || Csafe.Error2)))
  {
      if(Csafe.FC) Csafe.FC = 0;
      else Csafe.FC = 1;
      if(Csafe.Error1) {Csafe.Error1 = 0;CSAFE_Error_Answer(1);}
      if(Csafe.Error2) {Csafe.Error2 = 0;CSAFE_Error_Answer(2);}
      Csafe.Reset=1;
  }
  //cmdEmptyFrame  當為空字串時
  if(aby_RX_Buffer[_RX_Target][0] == 0x00 && !Csafe.Reset)
  {
      if(Csafe.FC) Csafe.FC = 0;
      else Csafe.FC = 1;
  }
  //
  if(!Csafe.Reset && !Csafe.Report_1)
  {
      Csafe.Error1=0;
      Csafe.Error2=0;
      if(Csafe.FC) by_Buffer = 0x80;
      else by_Buffer = 0x00;
      aby_TX_Buffer[0] = 0xf1;
      aby_TX_Buffer[1] = CSAFE_Info.Data.CSAFE_State+by_Buffer;
      CSAFE_Number_Check();
      aby_TX_Buffer[by_RX_IN] = CSAFE_TX_XOR(by_RX_IN);
      aby_TX_Buffer[by_RX_IN+1] = 0xf2;
      CSAFE_Write(by_RX_IN+2);
  }
}
//*****************************
//*     內部資料F0~F3轉換     *
//*****************************
void CSAFE_Number_Check(void)
{
  unsigned char _NewData;
    
  for(com_i=2;com_i < by_RX_IN;com_i++)
  {
      if((aby_TX_Buffer[com_i] >= 0xf0)&& \
         (aby_TX_Buffer[com_i] <= 0xf3) )
      {
          _NewData = (aby_TX_Buffer[com_i] & 0x0f);
          aby_TX_Buffer[com_i]=0xf3;
          com_i+=1;
          for(j=by_RX_IN;j>com_i;j--)
          {
              aby_TX_Buffer[j]=aby_TX_Buffer[j-1];
          }
          aby_TX_Buffer[com_i]=_NewData;
          by_RX_IN+=1;
      }
  }
}
//*****************************
//*  訊框內部資料做XOR的動作  *
//*****************************
char CSAFE_TX_XOR(char by_Dat)
{
  char i;
  char w_Dat;
  
  w_Dat=aby_TX_Buffer[1];
  for(i=2;i<by_Dat;i++)
  {
      if(aby_TX_Buffer[i]==0xf3)
      {//當為f0,f1,f2,f3數值時需做另一處理
          w_Dat = w_Dat^(aby_TX_Buffer[i+1]|0xf0);
          i++;
      }
      else w_Dat=w_Dat^aby_TX_Buffer[i];
  }
  if(w_Dat >= 0xf0 && w_Dat <= 0xf3)
  {//==>XOR後如果為F0,F1,F2,F3時將做轉換
      aby_TX_Buffer[by_RX_IN]=0xf3;
      by_RX_IN+=1;
      w_Dat = w_Dat & 0x0f;
  }
  return(w_Dat);
}
//-----------------------------//
//*      RX資料回應暫存        *//
//-----------------------------//
void CSAFE_RX_Answer_Buffer(char by_Dat)
{
   union
   {
       unsigned int WORD;
       char BYTE[2];
   }Bu;
   unsigned long int by_DataBuffer; 
   unsigned char i;
   
   aby_TX_Buffer[by_RX_IN] = by_Dat;
   switch(by_Dat)
   {
       case cmdGetAudioChannel:
                 aby_TX_Buffer[by_RX_IN+1] = 1;//Bytes Number
                 aby_TX_Buffer[by_RX_IN+2] = CSAFE_Info.Data.AudioChannel;
                 by_RX_IN+=3;
                 break;
       case cmdGetAudioVolume:
                 aby_TX_Buffer[by_RX_IN+1] = 1;//Bytes Number
                 aby_TX_Buffer[by_RX_IN+2] = CSAFE_Info.Data.AudioVolume;
                 by_RX_IN+=3;
                 break;
       case cmdGetAudioMute:
                 aby_TX_Buffer[by_RX_IN+1] = 1;//Bytes Number
                 aby_TX_Buffer[by_RX_IN+2] = Csafe.CardioAudioMute;
                 by_RX_IN+=3;
                 break;
       case cmdGetGear:
                 aby_TX_Buffer[by_RX_IN+1] = 1;//Bytes Number
                 aby_TX_Buffer[by_RX_IN+2] = CSAFE_Info.Data.Gear;
                 by_RX_IN+=3;
                 break;
       case cmdGetTorque: 
                 aby_TX_Buffer[by_RX_IN+1] = 3;//Bytes Number
                 Bu.WORD = CSAFE_Info.Data.Torque;
                 aby_TX_Buffer[by_RX_IN+2] = Bu.BYTE[0];
                 aby_TX_Buffer[by_RX_IN+3] = Bu.BYTE[1];
                 aby_TX_Buffer[by_RX_IN+4] = 6; // inch
                 by_RX_IN+=5;
                 break;
       case cmdGetSerial: 
                 aby_TX_Buffer[by_RX_IN+1] = 9;//Bytes Number
                 by_DataBuffer = 100000000;
                 for(i=2;i<=9;i++)
                 {
                     aby_TX_Buffer[by_RX_IN+i] = (CSAFE_Info.Data.Serial / by_DataBuffer) + 0x30;
                     if(i == 9)
                     {
                         aby_TX_Buffer[by_RX_IN+10] = (CSAFE_Info.Data.Serial % 10) + 0x30;
                     }
                     else
                     {
                         CSAFE_Info.Data.Serial %= by_DataBuffer;
                         by_DataBuffer /= 10;
                     }
                 }
                 by_RX_IN+=11;  
                 break;
       case cmdGetMotorCurrent:
                 aby_TX_Buffer[by_RX_IN+1] = 3;//Bytes Number
                 Bu.WORD = CSAFE_Info.Data.MotorCurrent;
                 aby_TX_Buffer[by_RX_IN+2] = Bu.BYTE[0];
                 aby_TX_Buffer[by_RX_IN+3] = Bu.BYTE[1];
                 aby_TX_Buffer[by_RX_IN+4] = 97; // amperes
                 by_RX_IN+=5;
                 break;
       case cmdGetErrorCode:
                 aby_TX_Buffer[by_RX_IN+1] = 3;//Bytes Number
                 Bu.WORD = CSAFE_Info.Data.CsafeErrorCode;
                 aby_TX_Buffer[by_RX_IN+2] = Bu.BYTE[0];
                 aby_TX_Buffer[by_RX_IN+3] = Bu.BYTE[1];
                 aby_TX_Buffer[by_RX_IN+4] = 0; 
                 by_RX_IN+=5;
                 CSAFE_Info.Data.CsafeErrorCode = 0;
                 break;
       case cmdGetServiceCode:
                 aby_TX_Buffer[by_RX_IN+1] = 3;//Bytes Number
                 Bu.WORD = CSAFE_Info.Data.ServiceCode % 0x10000;
                 aby_TX_Buffer[by_RX_IN+2] = Bu.BYTE[0];
                 aby_TX_Buffer[by_RX_IN+3] = Bu.BYTE[1];
                 aby_TX_Buffer[by_RX_IN+4] = CSAFE_Info.Data.ServiceCode / 0x10000; 
                 by_RX_IN+=5;
                 break;
       case cmdGetPace:
                 by_DataBuffer = 36000 / CSAFE_Info.Data.AvgSpeed;
                 if(by_DataBuffer > 7200) by_DataBuffer = 7200;
                 aby_TX_Buffer[by_RX_IN+1]=0x03;//Bytes Number
                 aby_TX_Buffer[by_RX_IN+2]=by_DataBuffer / 3600;//小時
                 aby_TX_Buffer[by_RX_IN+3]=(by_DataBuffer % 3600) / 60;//分
                 aby_TX_Buffer[by_RX_IN+4]=(by_DataBuffer % 3600) % 60;//秒
                 by_RX_IN+=5;
                 break;
       case cmdGetMETs:
                 aby_TX_Buffer[by_RX_IN+1]=0x02;//Bytes Number
                 Bu.WORD = CSAFE_Info.Data.METs;
                 aby_TX_Buffer[by_RX_IN+2]=Bu.BYTE[0];
                 aby_TX_Buffer[by_RX_IN+3]=Bu.BYTE[1];
                 by_RX_IN+=4;
		break;				 
        case cmdGetPower:
                 aby_TX_Buffer[by_RX_IN+1]=0x03;//Bytes Number
                 Bu.WORD = CSAFE_Info.Data.Watts;
                 aby_TX_Buffer[by_RX_IN+2]=Bu.BYTE[0];
                 aby_TX_Buffer[by_RX_IN+3]=Bu.BYTE[1];
                 aby_TX_Buffer[by_RX_IN+4]=0x58; // Watts
                 by_RX_IN+=5;
		 break;
       case cmdGetVersion:
                 aby_TX_Buffer[by_RX_IN+1]=0x05;//Bytes Number
                 aby_TX_Buffer[by_RX_IN+2]=CSAFE_Info.Data.Manufacturer;//Manufacturer(製造商)(MATRIX)
                 aby_TX_Buffer[by_RX_IN+3]=CSAFE_Info.Data.CID;//CID
                 aby_TX_Buffer[by_RX_IN+4]=CSAFE_Info.Data.Model;//Model(型號)
                 aby_TX_Buffer[by_RX_IN+5]=CSAFE_Info.Data.Version;//Version(版本)
                 aby_TX_Buffer[by_RX_IN+6]=CSAFE_Info.Data.Release;//Release(授權書)
                 by_RX_IN+=7;
                 break;
       case cmdGetID:
                 aby_TX_Buffer[by_RX_IN+1]=0x05;//Bytes Number
                 for(i=2;i<=6;i++)
                 {
                     aby_TX_Buffer[by_RX_IN+i]=(CSAFE_Info.Data.ID_Number[i-2]+0x30);  
                 }
                 by_RX_IN+=7;
                 break;
       case cmdGetUnits:
                 aby_TX_Buffer[by_RX_IN+1]=0x01;//Bytes Number
                 aby_TX_Buffer[by_RX_IN+2] = CSAFE_Info.Data.Units;
                 by_RX_IN+=3;
                 break;
       case cmdGetList:
                 aby_TX_Buffer[by_RX_IN+1]=aby_RX_UP_List[9];//Bytes Number
                 for(j=0;j<aby_RX_UP_List[9];j++)
                 {
                     aby_TX_Buffer[by_RX_IN+2+j]=aby_RX_UP_List[j];
                 }
                 by_RX_IN=j+4;
                 break;
       case cmdGetUtilization:
                 aby_TX_Buffer[by_RX_IN+1]=0x03;//Bytes Number
                 Bu.WORD = CSAFE_Info.Data.AccumulatedTime % 0x10000;
                 aby_TX_Buffer[by_RX_IN+2]=Bu.BYTE[0];
                 aby_TX_Buffer[by_RX_IN+3]=Bu.BYTE[1];
                 aby_TX_Buffer[by_RX_IN+4]=CSAFE_Info.Data.AccumulatedTime / 0x10000;
                 by_RX_IN+=5;
                 break;
       case cmdGetTWork:
                 aby_TX_Buffer[by_RX_IN+1]=0x03;//Bytes Number
                 aby_TX_Buffer[by_RX_IN+2]=CSAFE_Info.Data.WorkoutTime / 3600;//小時
                 aby_TX_Buffer[by_RX_IN+3]=(CSAFE_Info.Data.WorkoutTime % 3600) / 60;//分
                 aby_TX_Buffer[by_RX_IN+4]=(CSAFE_Info.Data.WorkoutTime % 3600) % 60;//秒
                 by_RX_IN+=5;
                 break;
       case cmdGetHorizontal:
                 aby_TX_Buffer[by_RX_IN+1]=0x03;//Bytes Number
                 Bu.WORD = CSAFE_Info.Data.WorkoutDistance;
                 aby_TX_Buffer[by_RX_IN+2]=Bu.BYTE[0];
                 aby_TX_Buffer[by_RX_IN+3]=Bu.BYTE[1];
                 aby_TX_Buffer[by_RX_IN+4] = CSAFE_Info.Data.DistanceUnits;
                 by_RX_IN+=5;
                 break;
       case cmdGetVertical: 
                 aby_TX_Buffer[by_RX_IN+1]=0x03;//Bytes Number
                 Bu.WORD = CSAFE_Info.Data.WorkoutVertical;
                 aby_TX_Buffer[by_RX_IN+2]=Bu.BYTE[0];
                 aby_TX_Buffer[by_RX_IN+3]=Bu.BYTE[1];
                 aby_TX_Buffer[by_RX_IN+4] = CSAFE_Info.Data.VerticalUnits;
                 by_RX_IN+=5;
                 break;          
       case cmdGetOdometer:
                 aby_TX_Buffer[by_RX_IN+1]=0x05;//Bytes Number
                 aby_TX_Buffer[by_RX_IN+2]=(CSAFE_Info.Data.AccumulatedDistance % 65536) % 256;
                 aby_TX_Buffer[by_RX_IN+3]=(CSAFE_Info.Data.AccumulatedDistance % 65536) / 256;
                 aby_TX_Buffer[by_RX_IN+4]=(CSAFE_Info.Data.AccumulatedDistance / 65536) % 256;
                 aby_TX_Buffer[by_RX_IN+5]=(CSAFE_Info.Data.AccumulatedDistance / 65536) / 256;
                 aby_TX_Buffer[by_RX_IN+6]=CSAFE_Info.Data.DistanceUnits;
                 by_RX_IN+=7;
                 break;          
       case cmdGetCalories:
                 aby_TX_Buffer[by_RX_IN+1]=0x02;//Bytes Number
                 Bu.WORD = CSAFE_Info.Data.WorkoutCalories;
                 aby_TX_Buffer[by_RX_IN+2]=Bu.BYTE[0];
                 aby_TX_Buffer[by_RX_IN+3]=Bu.BYTE[1];
                 by_RX_IN+=4;
                 break;
       case cmdGetProgram:
                 aby_TX_Buffer[by_RX_IN+1]=0x02;//Bytes Number
                 aby_TX_Buffer[by_RX_IN+2]=CSAFE_Info.Data.Program;
                 aby_TX_Buffer[by_RX_IN+3]=CSAFE_Info.Data.Level;
                 by_RX_IN+=4;
                 break;
       case cmdGetSpeed:
                 aby_TX_Buffer[by_RX_IN+1]=0x03;//Bytes Number
                 Bu.WORD = CSAFE_Info.Data.Speed;
                 aby_TX_Buffer[by_RX_IN+2]=Bu.BYTE[0];
                 aby_TX_Buffer[by_RX_IN+3]=Bu.BYTE[1];
                 aby_TX_Buffer[by_RX_IN+4] = CSAFE_Info.Data.SpeedUnits;  
                 by_RX_IN+=5;
                 break;
       case cmdGetGrade://(Incline)
                 aby_TX_Buffer[by_RX_IN+1]=0x03;//Bytes Number
                 Bu.WORD = CSAFE_Info.Data.Incline;
                 aby_TX_Buffer[by_RX_IN+2]=Bu.BYTE[0];
                 aby_TX_Buffer[by_RX_IN+3]=Bu.BYTE[1];
                 aby_TX_Buffer[by_RX_IN+4]=0x4a;//%grade
                 by_RX_IN+=5;
                 break;
       case cmdGetUserInFo:
                 aby_TX_Buffer[by_RX_IN+1]=0x05;//Bytes Number
                 Bu.WORD = CSAFE_Info.Data.Weight;
                 aby_TX_Buffer[by_RX_IN+2]=Bu.BYTE[0];
                 aby_TX_Buffer[by_RX_IN+3]=Bu.BYTE[1];
                 aby_TX_Buffer[by_RX_IN+4] = CSAFE_Info.Data.WeightUnits;
                 aby_TX_Buffer[by_RX_IN+5] = CSAFE_Info.Data.AGE;
                 aby_TX_Buffer[by_RX_IN+6] = CSAFE_Info.Data.Gender;
                 by_RX_IN+=7;
                 break;
       case cmdGetHRCur:
                 aby_TX_Buffer[by_RX_IN+1] = 0x01;//Bytes Number
                 aby_TX_Buffer[by_RX_IN+2] = CSAFE_Info.Data.HR_Cur;
                 by_RX_IN+=3;
                 break;
       case cmdGetHRTZone:
                 aby_TX_Buffer[by_RX_IN+1]=0x03;//Bytes Number
                 aby_TX_Buffer[by_RX_IN+2]=CSAFE_Info.Data.HR_TZone / 3600;//小時
                 aby_TX_Buffer[by_RX_IN+3]=(CSAFE_Info.Data.HR_TZone % 3600) / 60;//分
                 aby_TX_Buffer[by_RX_IN+4]=(CSAFE_Info.Data.HR_TZone % 3600) % 60;//秒
                 by_RX_IN+=5;
                 break;
       case cmdGetHRAvg:
                 aby_TX_Buffer[by_RX_IN+1]=0x01;//Bytes Number
                 aby_TX_Buffer[by_RX_IN+2]=CSAFE_Info.Data.HR_Avg;
                 by_RX_IN+=3;
                 break;
       case cmdGetHRMax:
                 aby_TX_Buffer[by_RX_IN+1]=0x01;//Bytes Number
                 aby_TX_Buffer[by_RX_IN+2]=CSAFE_Info.Data.HR_Max;
                 by_RX_IN+=3;
                 break;

   }
}
//==============================================================================
//==> 電源啟動時
//==============================================================================
void CSAFE_Power_On(void)
{
  CSAFE_Reset();
  CSAFE_Info.Data.UserID=5;
  CSAFE_Info.Data.CSAFE_State=Csafe_Ready;
  Csafe.FC = 0;
  aby_TX_Buffer[0]=0xf1;
  aby_TX_Buffer[1]=Csafe_Ready;
  aby_TX_Buffer[2]=CSAFE_TX_XOR(1);
  aby_TX_Buffer[3]=0xf2;
  CSAFE_Write(4);
}
//==============================================================================
//==> 輸入使用者ID number
//==> Input : ID
//==============================================================================
void CSAFE_Set_ID_Number(unsigned long int by_Dat)
{
  CSAFE_Info.Data.HR_Avg=0;
  //CSAFE_Reset();
  CSAFE_Info.Data.Level=1;
  CSAFE_Info.Data.TargetDistance=0;
  CSAFE_Info.Data.TargetCalories=0;
  Csafe.Online = 1;//==>設定Cable線連線回傳狀態
  CSAFE_Answer(Csafe_HaveID);
  CSAFE_Info.Data.ID_Number[0]=by_Dat / 10000;
  by_Dat %= 10000;
  CSAFE_Info.Data.ID_Number[1]=by_Dat / 1000;
  by_Dat %= 1000;
  CSAFE_Info.Data.ID_Number[2]=by_Dat / 100;
  by_Dat %= 100;
  CSAFE_Info.Data.ID_Number[3]=by_Dat / 10;
  CSAFE_Info.Data.ID_Number[4]=by_Dat % 10;
}
void CSAFE_ClearIDNumber(void)
{
  CSAFE_Info.Data.ID_Number[0] = 0;
  CSAFE_Info.Data.ID_Number[1] = 0;
  CSAFE_Info.Data.ID_Number[2] = 0;
  CSAFE_Info.Data.ID_Number[3] = 0;
  CSAFE_Info.Data.ID_Number[4] = 0;
}
//==============================================================================
//==> 判斷按鍵後會進入哪一模式  
//==> OFFLINE & MANUAL MODE判斷用 
//==============================================================================
void CSAFE_Set_AnyKey(void)
{
  if(Csafe.CheckAnyKey == 0)
  {
      if(CSAFE_Info.Data.CSAFE_State == Csafe_Ready)
      {//go to Offline
          CSAFE_Answer(Csafe_Offline);
          Csafe.HR_Max=0;
          CSAFE_Info.Data.TargetDistance=0;
          CSAFE_Info.Data.TargetCalories=0;
      }
      else
      {
          if(CSAFE_Info.Data.CSAFE_State == Csafe_IDle)
          {//go to Manual
              CSAFE_Answer(Csafe_Manual);
              Csafe.HR_Max=0;
              CSAFE_Info.Data.TargetDistance=0;
              CSAFE_Info.Data.TargetCalories=0;
          }
      }
  }
}
//==============================================================================
//==>   在CSAFE的回覆副程式 
//==============================================================================
void CSAFE_Answer(char by_Dat)
{
  CSAFE_Info.Data.CSAFE_State = by_Dat;
  // NetPluse 
  if(CSAFE_Info.Data.CSAFE_State != Csafe_In_Use && \
     CSAFE_Info.Data.CSAFE_State != Csafe_Paused )
  {
      Csafe.NetPulse = 0 ; 
  }
  //
  if(Csafe.FC) 
      by_Buffer=0x80;
  else 
      by_Buffer=0x00;
  aby_TX_Buffer[0]=0xf1;
  aby_TX_Buffer[1]=by_Dat+by_Buffer;
  aby_TX_Buffer[2]=CSAFE_TX_XOR(1);
  aby_TX_Buffer[3]=0xf2;
  CSAFE_Write(4);
}
//==============================================================================
//==>        回覆錯誤訊息    
//==============================================================================
void CSAFE_Error_Answer(char by_Dat)
{
  //在不應該的情形下下Command to console
  if(by_Dat == 1) by_Buffer=0x10;
  //訊框錯誤
  if(by_Dat == 2) by_Buffer=0x20;
  //前訊框處理中
  if(by_Dat == 3) by_Buffer=0x30;
  //轉態
  if(Csafe.FC == 1) by_Buffer |= 0x80;
  
  aby_TX_Buffer[0]=0xf1;
  aby_TX_Buffer[1]=CSAFE_Info.Data.CSAFE_State+by_Buffer;
  aby_TX_Buffer[2]=CSAFE_TX_XOR(1);
  aby_TX_Buffer[3]=0xf2;
  CSAFE_Write(4);
}
//==============================================================================
//==> 判斷目前狀態是否可以對我下Command 
//==> 1:YES / 0:NO
//==============================================================================
// bit0:Ready
// bit1:IDld
// bit2:HaveID
// bit3:InUse
// bit4:Paused
// bit5:Finished
// bit6:Offline
// bit7:Manual
const unsigned char _SupportCheck[31][2]=
{// support mode , command
 {0x07,cmdSetTWork},
 {0x07,cmdSetHorizontal},
 {0x07,cmdSetCalories},
 {0x07,cmdSetProgram},
 {0x07,cmdSetUserInfo},
 {0x07,cmdSetLevel},
 {0x07,cmdSetTargetHR},
 {0x07,cmdSetHRZone},
 {0x07,cmdSetHRMAX},
 {0x07,cmdSetSpeed},
 {0x07,cmdSetGrade},
 {0x07,cmdSetGear},
 {0x07,cmdSetMETs},
 {0x07,cmdSetChannelRange},
 {0x07,cmdSetVolumeRange},
 {0x07,cmdSetAudioMute},
 {0x07,cmdSetAudioChannel},
 {0x07,cmdSetAudioVolume},
 {0x07,cmdSetTorque},
 {0x27,cmdGoIdle},
 {0x02,cmdGoHavelID},
 {0x1e,cmdGoInUse},
 {0x18,cmdGoFinished},
 {0x27,cmdGoReady},
 {0x04,cmdBadID},
 {0x03,cmdUpList},
 {0x01,cmdIDDigits},
 {0x03,cmdSetPower},
 {0x01,cmdSetTime},
 {0x01,cmdSetDate},
 {0x01,cmdSetTimeout}
};

char CSAFE_Mode_Check_Answer(char by_Dat)
{
  unsigned char i;
  unsigned char _CheckData=0;
  
  for(i=0;i<31;i++)
  {
      if(by_Dat == _SupportCheck[i][1])
      {
          if(CSAFE_Info.Data.CSAFE_State == Csafe_Ready)
              _CheckData = 0x01;
          else if(CSAFE_Info.Data.CSAFE_State == Csafe_IDle)
              _CheckData = 0x02;
          else if(CSAFE_Info.Data.CSAFE_State == Csafe_HaveID)
              _CheckData = 0x04;
          else if(CSAFE_Info.Data.CSAFE_State == Csafe_In_Use)
              _CheckData = 0x08;
          else if(CSAFE_Info.Data.CSAFE_State == Csafe_Paused)
              _CheckData = 0x10;
          else if(CSAFE_Info.Data.CSAFE_State == Csafe_Finished)
              _CheckData = 0x20;
          else if(CSAFE_Info.Data.CSAFE_State == Csafe_Offline)
              _CheckData = 0x40;
          else if(CSAFE_Info.Data.CSAFE_State == Csafe_Manual)
              _CheckData = 0x80;
          
          if((_SupportCheck[i][0] & _CheckData) == 0)
          {// Not support  
              // FC will not 轉態
              if(Csafe.FC) 
                  Csafe.FC = 0;
              else 
                  Csafe.FC = 1;
              //
              return 0;
          }
          //
          i = 99;
      }
  }
  return 1;
}
//==============================================================================
//==> 運動結束和系統重致時使用  
//==> 動資訊與狀態載入判斷
//==============================================================================
void CSAFE_Work_Exit(void)
{
  CSAFE_Info.Data.SetHRMax=0;
  Csafe.Error1=0;
  Csafe.Error2=0;
  Csafe.Online = 1;//==>設定Cable線連線回傳狀態
  switch(CSAFE_Info.Data.CSAFE_State)
  {
      case Csafe_Finished:
                 if(Csafe.NetPulse == 1) break;//==>配合NetPulse修正
      case Csafe_IDle:
      case Csafe_Offline:
                 CSAFE_Reset();
                 CSAFE_Answer(Csafe_Ready);
                 break;
      case Csafe_Manual:
                 CSAFE_Reset();
                 CSAFE_Answer(Csafe_IDle);
                 break;
      case Csafe_HaveID:
      case Csafe_Paused:
      case Csafe_In_Use:
                 CSAFE_Answer(Csafe_Finished);
                 break;
  }
}
//==============================================================================
//==> Offline or Manual Mode check
//==============================================================================
char CSAFE_Mode_Judge(void)
{
  if((CSAFE_Info.Data.CSAFE_State == Csafe_Offline)||\
     (CSAFE_Info.Data.CSAFE_State == Csafe_Manual)) 
      return 1;
  else 
      return 0;
}
//==============================================================================
// CSAFE all information platform
//==============================================================================
unsigned long int CSAFE_Information(unsigned char by_D,unsigned long int by_Data)
{
  switch(by_D)
  {
      case _Set_CommunicationMode: 
             CSAFE_Info.Data.CommunicationMode = by_Data;
             break;
      case _Set_AvgSpeed: 
             CSAFE_Info.Data.AvgSpeed = by_Data;
             break;
      case _Set_AccumulatedDistance: 
             CSAFE_Info.Data.AccumulatedDistance = by_Data;
             break;
      case _Set_HRCur:
             CSAFE_Info.Data.HR_Cur = by_Data;
             break;
      case _Set_Weight:
             CSAFE_Info.Data.Weight = by_Data;
             break;  
      case _Set_WeightUnits:
             CSAFE_Info.Data.WeightUnits = by_Data;
             break;
      case _Set_Gender:
             CSAFE_Info.Data.Gender = by_Data;
             break;
      case _Set_AGE:
             CSAFE_Info.Data.AGE = by_Data;
             break;
      case _Set_Incline:
             CSAFE_Info.Data.Incline = by_Data; 
             break;
      case _Set_HRMax:
             CSAFE_Info.Data.HR_Max = by_Data; 
             break;
      case _Set_HRAvg:
             CSAFE_Info.Data.HR_Avg = by_Data; 
             break;
      case _Set_HRTZone:
             CSAFE_Info.Data.HR_TZone = by_Data;
             break;
      case _Set_StartWorkout:// 進入Workout
             Csafe.Start = by_Data;
             break;
      case _Set_Online:// 連線狀態
             Csafe.Online = by_Data;
             break; 
      case _Set_WorkoutTime:
             CSAFE_Info.Data.WorkoutTime = by_Data;
             break;
      case _Set_WorkoutDistance:
             CSAFE_Info.Data.WorkoutDistance = by_Data;
             break;
      case _Set_WorkoutVertical:
             CSAFE_Info.Data.WorkoutVertical = by_Data;
             break;        
      case _Set_WorkoutCalories:
             CSAFE_Info.Data.WorkoutCalories = by_Data;
             break;
      case _Set_Speed:
             CSAFE_Info.Data.Speed = by_Data;
             break;
      case _Set_Level:
             CSAFE_Info.Data.Level = by_Data;
             break;
      case _Set_MET:
             CSAFE_Info.Data.METs = by_Data;
             break;
      case _Set_Watt:
             CSAFE_Info.Data.Watts = by_Data;
             break;
      case _Set_Program:
             CSAFE_Info.Data.Program = by_Data;
             break;
      case _Set_AccumulatedTime:
             CSAFE_Info.Data.AccumulatedTime = by_Data;
             break;
      case _Set_Manufacturer:
             CSAFE_Info.Data.Manufacturer = by_Data;
             break;
      case _Set_CID:
             CSAFE_Info.Data.CID = by_Data;
             break;
      case _Set_Model:
             CSAFE_Info.Data.Model = by_Data;
             break;
      case _Set_Version:
             CSAFE_Info.Data.Version = by_Data;
             break;
      case _Set_Release:
             CSAFE_Info.Data.Release = by_Data;
             break;
      case _Set_Units:
             CSAFE_Info.Data.Units = by_Data;
             break;
      case _Set_DistanceUnits:
             CSAFE_Info.Data.DistanceUnits = by_Data;
             break;
      case _Set_VerticalUnits:
             CSAFE_Info.Data.VerticalUnits = by_Data;
             break;              
      case _Set_SpeedUnits:
             CSAFE_Info.Data.SpeedUnits = by_Data;
             break;
      case _Set_Mode:
             CSAFE_Info.Data.CSAFE_State = by_Data;
             CSAFE_Answer(CSAFE_Info.Data.CSAFE_State);// 送出給主機
             break;
      case _Set_Serial:
             CSAFE_Info.Data.Serial = by_Data;
             break;
      case _Set_MotorCurrent:
             CSAFE_Info.Data.MotorCurrent = by_Data;
             break;
      case _Set_ErrorCode:
             CSAFE_Info.Data.CsafeErrorCode = by_Data;
             break;
      case _Set_ServiceCode:
             CSAFE_Info.Data.ServiceCode = by_Data;
             break;
      case _Set_Gear:       
             CSAFE_Info.Data.Gear = by_Data;
             break;
      case _Set_Torque:       
             CSAFE_Info.Data.Torque = by_Data;
             break;
      case _Set_AudioMute:       
             Csafe.CardioAudioMute = by_Data;
             break;
      case _Set_AudioChannel:       
             CSAFE_Info.Data.AudioChannel = by_Data;
             break;
      case _Set_AudioVolume:       
             CSAFE_Info.Data.AudioVolume = by_Data;
             break;
     //========================================================================== 
      case _Get_TargetUserInfo:// 設定旗標
               by_Data = Csafe.UserInfo;
               break;
      case _Get_Gender:// 設定值
               by_Data = CSAFE_Info.Data.Gender;
               break;
      case _Get_AGE:// 設定值
               by_Data = CSAFE_Info.Data.AGE;
               break;
      case _Get_Level:// 設定值
               by_Data = CSAFE_Info.Data.Level;
               break;
      case _Get_TargetLevel:// 設定旗標
               by_Data = Csafe.Level;
               break;
      case _Get_Time:// 設定值
               by_Data = CSAFE_Info.Data.Time;
               break;
      case _Get_TargetTime:// 設定旗標
               by_Data = Csafe.Time;
               break;         
      case _Get_THR_Max:// 最大範圍值
               by_Data = CSAFE_Info.Data.SetHRMax;
               break;
      case _Get_TargetHR:// 設定旗標
               by_Data = Csafe.Heart;
               break;
      case _Get_THR:// 設定值
               by_Data = CSAFE_Info.Data.HR_Target;
               break;  
      case _Get_HRZoneMin:
               by_Data = CSAFE_Info.Data.HR_Zone_Min; 
               break;
      case _Get_HRZoneMax:
               by_Data = CSAFE_Info.Data.HR_Zone_Max; 
               break;         
      case _Get_DFdistnce:// 設定值
               by_Data = CSAFE_Info.Data.TargetDistance;
               break;
      case _Get_TargetDistance:// 設定旗標
               by_Data = Csafe.Distance;
               break;
      case _Get_DFcalories:// 設定值
               by_Data = CSAFE_Info.Data.TargetCalories;
               break;
      case _Get_TargetCalories:// 設定旗標
               by_Data = Csafe.Calories;
               break;
      case _Get_DFprogram:// 設定值
               by_Data = CSAFE_Info.Data.Program;
               break;
      case _Get_Mode:// CSAFE模式狀態
               by_Data = CSAFE_Info.Data.CSAFE_State;
               break;
      case _Get_Online:// 連線狀態
               by_Data = Csafe.Online;
               break;    
      case _Get_Power:// 設定值
               by_Data = CSAFE_Info.Data.ConstWatts;
               break;
      case _Get_TargetPower:// 設定旗標
               by_Data = Csafe.Watts;
               break;
      case _Get_Weight:
               by_Data = CSAFE_Info.Data.Weight;         
               break;
      case _Get_SetTime:
               by_Data = CSAFE_Info.Data.ClockTime;         
               break;
      case _Get_SetDate:
               by_Data = CSAFE_Info.Data.ClockDate;         
               break;
      case _Get_SetTimeout:
               by_Data = CSAFE_Info.Data.Timeout;         
               break;
      case _Get_TargetSpeed:
               by_Data = Csafe.Speed;         
               break;
      case _Get_TargetGrade:
               by_Data = Csafe.Grade;         
               break;
      case _Get_TargetGear:
               by_Data = Csafe.Gear;         
               break;
      case _Get_TargetTorque:
               by_Data = Csafe.Torque;         
               break;
      case _Get_SetSpeed:
               by_Data = CSAFE_Info.Data.SetSpeed; 
               Csafe.Speed = 0;
               break;
      case _Get_SetGrade:
               by_Data = CSAFE_Info.Data.SetGrade; 
               Csafe.Grade = 0; 
               break;
      case _Get_SetGear:
               by_Data = CSAFE_Info.Data.SetGear;  
               Csafe.Gear = 0;
               break;
      case _Get_SetTorque:
               by_Data = CSAFE_Info.Data.SetTorque;  
               Csafe.Torque = 0;
               break;
      case _Get_TargetMETs:
               by_Data = Csafe.METs;  
               break;
      case _Get_TargetChannelRange:
               by_Data = Csafe.ChannelRange;  
               break;
      case _Get_TargetVolumeRange:
               by_Data = Csafe.VolumeRange;  
               break;
      case _Get_TargetAudioMute:
               by_Data = Csafe.AudioMute;  
               break;
      case _Get_TargetAudioChannel:
               by_Data = Csafe.AudioChannel;  
               break;
      case _Get_TargetAudioVolume:
               by_Data = Csafe.AudioVolume;  
               break;
      case _Get_SetMETs:
               by_Data = CSAFE_Info.Data.SetMETs;  
               Csafe.METs = 0;
               break;
      case _Get_SetChannelRange:
               by_Data = CSAFE_Info.Data.SetChannelRange;  
               Csafe.ChannelRange = 0;
               break;
      case _Get_SetVolumeRange:
               by_Data = CSAFE_Info.Data.SetVolumeRange;  
               Csafe.VolumeRange = 0;
               break;
      case _Get_SetAudioMute:
               by_Data = CSAFE_Info.Data.AudioMute;  
               Csafe.AudioMute = 0;
               break;
      case _Get_SetAudioChannel:
               by_Data = CSAFE_Info.Data.SetAudioChannel;  
               Csafe.AudioChannel = 0;
               break;
      case _Get_SetAudioVolume:
               by_Data = CSAFE_Info.Data.SetAudioVolume;  
               Csafe.AudioVolume = 0;
               break;
      case _Get_HRAvg:
               by_Data = CSAFE_Info.Data.HR_Avg;  
               break;  
      case _Get_CID:
               by_Data = CSAFE_Info.Data.CID;
               break;
      case _Get_Manufacturer:
               by_Data = CSAFE_Info.Data.Manufacturer;
               break;
      case _Get_Model:
               by_Data = CSAFE_Info.Data.Model;
               break;  
      default:
               by_Data=0;
               break;
  }
  return by_Data;
}
//==============================================================================
//==>寫入資料
//==> Input : DATA 數
//==============================================================================
void CSAFE_Write(char by_Length)
{
  Csafe.SetTX = 1;
  by_TX_Point = 0;
  by_TX_Length = by_Length;
  Csafe.TX_Switch=1; 
}
//==============================================================================
//TX中斷開關啟動旗標 0:Clear / 1:Get switch target status
//==============================================================================
unsigned char CSAFE_TXE_Status(unsigned char by_D)
{
  if(by_D == 0)
  {
      Csafe.TX_Switch=0; 
  }
  return Csafe.TX_Switch;
}
//==============================================================================
//==> Check sum 計算
//==============================================================================
char CSAFE_XOR_Check(void)
{
  u8 i,j;
  u8 by_Dat=0;
  u8 by_Dat1,by_Dat2;
  u8 by_Buffer;
  u8 by_j;
  
  if(aby_RX_Buffer[_RX_Target][by_RX_Point-2] == 0xf3)//==> 取得此訊框中的XOR值是否為F0~F3的特殊字元
  {
      by_Dat1 = aby_RX_Buffer[_RX_Target][by_RX_Point - 1] | 0xf0;
      by_Buffer=by_RX_Point-1;
      by_j=by_RX_Point-2;
  }
  else
  {
      by_Dat1 = aby_RX_Buffer[_RX_Target][by_RX_Point-1];
      by_Buffer=by_RX_Point;
      by_j=by_RX_Point-1;
  }
  for(i=0;i<by_j;i++)
  {
      if(aby_RX_Buffer[_RX_Target][i] == 0xf3)//---> 作為F0~F3參數轉換用  
      {
          i++;
          by_Dat2 = aby_RX_Buffer[_RX_Target][i] | 0xf0;
          aby_RX_Buffer[_RX_Target][i - 1] = by_Dat2;
          for(j = i;j < by_Buffer;j++)
          {//==>所有參數往右移一位,重F3開始
              aby_RX_Buffer[_RX_Target][j] = aby_RX_Buffer[_RX_Target][j+1];
          }
          by_Buffer--;
          by_j--;
          by_Dat=by_Dat^by_Dat2;
          i--;
      }
      else by_Dat=by_Dat ^ aby_RX_Buffer[_RX_Target][i];
  }
  by_RX_Point = by_Buffer;
  if(by_Dat==by_Dat1) return(1);
  else return(0);
}
//==============================================================================
//==> 資料傳輸中斷
//==============================================================================
char _RxCheckData;
char CSAFE_TxRx_Information(char by_D)
{
  if(by_D == 1)
  {// TX 資訊處理
      ++by_TX_Point;
      if(by_TX_Point >= by_TX_Length)
      {
          Csafe.SetTX = 0;
          by_TX_Point = 0;
          return 1;
      }
  }
  else
  {// RX 資訊處理
      if(Csafe.EngTest)
      {//==>工程模式中的C-SAFE功能測試用
          if(_RxCheckData == 0xf1)//==>訊框開始開始
          {
              by_RX_Point=0;//==>訊框內部資料位址指標累計
              Csafe.EngCheck=0;
          }
          else if(_RxCheckData == 0xf2)//==>訊框結束訊號
          {
              Csafe.EngTest=0;
              if(by_RX_Point == 2 && aby_RX_Buffer[_RX_Target][0] == 0x08  && aby_RX_Buffer[_RX_Target][1] == 0x00)
              {
                  Csafe.EngCheck=1;                    
              }
          }
          else
          {
              aby_RX_Buffer[_RX_Total][by_RX_Point] = _RxCheckData;
              ++by_RX_Point;
          }
      }
      else
      {//==>正常傳輸使用
          if(!Csafe.ERROR)
          {
              if(!Csafe.RX_Head)
              {
                  if(_RxCheckData == 0xf1)//==>訊框開始開始
                  {
                      Csafe.RX_Head=1;//==>訊框啟始指標
                      by_RX_Point=0;//==>訊框內部資料位址指標累計
                  }
                  else Csafe.ERROR=1;//==>回傳錯誤訊息
              }
              else
              {
                  if(_RxCheckData == 0xf2)//==>訊框結束訊號
                  {
                      if((by_RX_Point == 1 && aby_RX_Buffer[_RX_Total][0] == 0x00) || CSAFE_XOR_Check())
                      {//==> check sum ok
                          Csafe.RX_OK=1;//==>訊框結束指標
                          Csafe.RX_Head=0;
                          if(Csafe.FC) Csafe.FC=0;
                          else Csafe.FC=1;
                          aby_RX_Buffer[_RX_Total][49] = by_RX_Point;//==>接收位數存入
                          
                          _RX_Total++;
                          if(_RX_Total >= Max_RX_Buffer)
                          {  
                              _RX_Total = 0;
                          }
                            
                      }
                      else 
                          Csafe.ERROR = 1;
                      Csafe.Online = 0;//==>當主機有回傳正確封包時
                  }
                  else
                  {
                      aby_RX_Buffer[_RX_Total][by_RX_Point] = _RxCheckData;
                      ++by_RX_Point;
                  }
              }
          }
      }
  }
  return 0;
}
//==============================================================================
// TX傳送資料傳出
//==============================================================================
char CSAFE_TxBuffer(void)
{
  return aby_TX_Buffer[by_TX_Point];
}
//==============================================================================
// RX資料取得
//==============================================================================
void CSAFE_RxBuffer(char by_D)
{
  _RxCheckData = by_D;     
}
/*******************************************************************************
* Function Name  : CSAFE_NetPluse()
* Description    : NetPluse狀態設定 1 => Yes / 0 => No
* Input          : by_D
* Output         : None
* Return         : None
*******************************************************************************/
char CSAFE_GetNetPulse(void)
{
  return Csafe.NetPulse;
}
//==============================================================================
// HR資訊處理 ever 1 seconds
//==============================================================================
void CSAFE_HRInformation(void)
{
  if(CSAFE_Info.Data.HR_Cur > 40)
  {
      if((CSAFE_Info.Data.HR_Cur >= CSAFE_Info.Data.HR_Zone_Min)&&\
         (CSAFE_Info.Data.HR_Cur <= CSAFE_Info.Data.HR_Zone_Max)) 
      {
          CSAFE_Info.Data.HR_TZone++;
      }
      if(CSAFE_Info.Data.HR_Cur > CSAFE_Info.Data.HR_Max) CSAFE_Info.Data.HR_Max = CSAFE_Info.Data.HR_Cur;
      if(CSAFE_Info.Data.HR_Avg != 0)
      {
          if((CSAFE_Info.Data.HR_Cur > (CSAFE_Info.Data.NowHR + 5))||(CSAFE_Info.Data.HR_Cur < (CSAFE_Info.Data.NowHR - 5)))
          {
              Csafe.HR_Avg = 1;
              CSAFE_Info.Data.HRDelay = 0;
              CSAFE_Info.Data.NowHR = CSAFE_Info.Data.HR_Cur;
          }
          else
          {
              CSAFE_Info.Data.HRDelay++;
              if(CSAFE_Info.Data.HRDelay >= 10)
              {
                  CSAFE_Info.Data.HRDelay = 0;
                  if(Csafe.HR_Avg)
                  {
                      Csafe.HR_Avg = 0;
                      CSAFE_Info.Data.HR_Avg = (CSAFE_Info.Data.HR_Cur + CSAFE_Info.Data.HR_Avg) / 2;
                  }
              }
          }
      }
      else //==>第一次取值
      {
          CSAFE_Info.Data.HR_Avg = CSAFE_Info.Data.HR_Cur;
          CSAFE_Info.Data.NowHR = CSAFE_Info.Data.HR_Cur;
          CSAFE_Info.Data.HRDelay = 0;
          Csafe.HR_Avg = 0;
      }
  }
  else CSAFE_Info.Data.NowHR=0;
}
//==============================================================================
// 設定是否啟動任意鍵確認功能,避免進入MANUAL or OFFline模式
// Input 0:Yes 1:No
//==============================================================================
void CSAFE_SetCheckAnyKeyStatus(char by_Dat)
{
  if(by_Dat == 0)
      Csafe.CheckAnyKey = 0;
  else
      Csafe.CheckAnyKey = 1;
}
//==============================================================================
// 900M Cardio 控制指令副程式 (0x42 / 0x43 / 0x44) 
//==============================================================================
void CSAFE_900M_Cardio(char by_Command,char by_Num) 
{
  by_RX_IN = 4;
  aby_TX_Buffer[0] = 0xf1;
  aby_TX_Buffer[1] = by_Command;
  aby_TX_Buffer[2] = 0x01;//Bytes Number
  aby_TX_Buffer[3] = by_Num;
  aby_TX_Buffer[by_RX_IN] = CSAFE_TX_XOR(by_RX_IN);
  aby_TX_Buffer[by_RX_IN+1] = 0xf2;
  CSAFE_Write(by_RX_IN+2);
}
//==============================================================================
// 900M Cardio 控制指令0xE7副程式 
//==============================================================================
void CSAFE_900M_E7command(void)
{
  aby_TX_Buffer[0] = 0xf1;
  aby_TX_Buffer[1] = 0xE7;
  aby_TX_Buffer[2] = 0xE7;
  aby_TX_Buffer[3] = 0xf2;
  CSAFE_Write(4);
}
//==============================================================================
// 控制指令0xE7副程式取得資料後解析 
//==============================================================================
void CSAFE_900M_E7DataAnalysis(void)
{
  char i,j,Sum = 0; 
    
  CSAFE_Info.Data.RF900_ControlBuffer[0] = aby_RX_Buffer[_RX_Target][3];// FM Band
  CSAFE_Info.Data.RF900_ControlBuffer[1] = aby_RX_Buffer[_RX_Target][4];// Wireless Range Sensitity
  CSAFE_Info.Data.RF900_ControlBuffer[2] = aby_RX_Buffer[_RX_Target][5];// Transmitter Manufacturer
  Sum = 0;
  for(i=6;i<=12;i++)
  {
      for(j=0;j<8;j++)
      {
          if((aby_RX_Buffer[_RX_Target][i] >> j)& 0x01) Sum++; 
      }
  }
  for(i=13;i<=41;i+=2)
  {
      if((aby_RX_Buffer[_RX_Target][i]+aby_RX_Buffer[_RX_Target][i+1]) != 0) Sum++;
  }
  CSAFE_Info.Data.RF900_ControlBuffer[3] = Sum;// Max Channel
  CSAFE_Info.Data.RF900_ControlBuffer[4] = aby_RX_Buffer[_RX_Target][43];// Default Volume
  CSAFE_Info.Data.RF900_ControlBuffer[5] = aby_RX_Buffer[_RX_Target][44];// Software Revision
  CSAFE_Info.Data.RF900_ControlBuffer[6] = aby_RX_Buffer[_RX_Target][45];// Hardware Configuration
}
//==============================================================================
// 回傳RF900資訊的暫存資料
//==============================================================================
char CSAFE_900M_GetControlBuffer(char by_D)
{
  return CSAFE_Info.Data.RF900_ControlBuffer[by_D];
}
//==============================================================================
// 指令回覆狀態旗標 
// 0:Not return / 1:Yes
//==============================================================================
char CSAFE_900M_CommandStatus(void)
{
  return Csafe.RF900_Return;
}
//==============================================================================
// 指令回覆狀態旗標清除 
// 使用時機為下指令前進行清除,確保後續的回覆狀態更新.
//==============================================================================
void CSAFE_900M_CommandStatusClear(void)
{
  Csafe.RF900_Return = 0;
}
//==============================================================================
// 搭配工程模式的C-SAFE功能測試用
// 0:CSAFE normal communication mode 
// 1:CSAFE test communication mode
//==============================================================================
void CSAFE_CommunicationMode(char by_Dat)
{
  Csafe.EngTest = by_Dat;
  Csafe.EngCheck = 0;
  by_RX_Point = 0;//==>訊框內部資料位址指標累計
}
//==============================================================================
// 搭配工程模式的C-SAFE功能測試用 
//==============================================================================
void CSAFE_EngTest(void)
{
  Csafe.EngTest = 1;
  by_RX_Point = 0;//==>訊框內部資料位址指標累計
  Csafe.EngCheck = 0;
  aby_TX_Buffer[0] = 0xf1;
  aby_TX_Buffer[1] = 0x08;
  aby_TX_Buffer[2] = 0x00;
  aby_TX_Buffer[3] = 0xf2;
  CSAFE_Write(4);
}
//==============================================================================
// 搭配工程模式的C-SAFE功能測試狀態結果取得
// return 0:false 1:true
//==============================================================================
char CSAFE_TestStatus(void)
{
  return Csafe.EngCheck;
}

