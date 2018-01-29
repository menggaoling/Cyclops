#include "Treadmill.h"
#include "ProgramTable.h"



#define C_TIME_SEG_NUM        16   //16排
#define C_SPEED_MILE_MIN      50
//#define C_SPEED_MILE_MAX     1500 //1200
#define C_SPEED_KM_MIN        80
//#define C_SPEED_KM_MAX       2000
#define C_INCLINE_NUM         30   //incline 阶数 15%
#define C_HR_DF_MaxSpeed_KM   8   // 0.8
#define C_HR_DF_MaxSpeed_Mile 5   // 0.5 
#define C_HR_DF_MaxIncline    2   // 1%

extern char _NowWorkoutStatus;


unsigned short Console_Change(char by_D,char by_Handle,unsigned short by_Dat);

 struct{
       unsigned Mode         : 1;
       unsigned SEC          : 1;
       unsigned Distance     : 1;
       unsigned Speed        : 1;
       unsigned Calories     : 1;
       unsigned Time_Over    : 1;
       unsigned Time_Start   : 1;
       unsigned TIME_DIR     : 1;
       unsigned Start        : 1;
       unsigned Acc          : 1;//==>时间累计与否
       unsigned Conversion   : 1;//==>Fitness Test SPEED & INCLINE切换
       unsigned DistanceShow : 1;
       unsigned SpeedChang   : 1;
       unsigned SpeedChangDelay : 1;
       unsigned FitTestAcc : 1;//==>Fit test time 累计用 //==>980616
       unsigned CsafeTarget : 1;// CSAFE目标值设定旗标
       unsigned CsafeTargetDisable : 1;//取消CSAFE目标值设定旗标
       unsigned SoftwareUpdate : 1;// v2.2-5
       }Console;
//==============================================================================
// 0:Info_TIME
// 1:Info_WEIGHT
// 2:Info_HEIGHT
// 3:Info_StartIncline
// 4:Info_AGE  
// 5:Info_GENDER 
// 6:Info_LEVEL  
// 7:Info_PERCENT
// 8:Info_StartSpeed
// 9:Info_WorkoutSpeed
// 10:Info_WorkoutIncline
// 此11组排序与PROGRAM设定相关联,因此不可以乱调顺序
const struct
{
    unsigned char AdjustSize;   // 数值调整的倍数
    unsigned char Range[2];     // 数值范围H=Max;L=Min
}Console_Info[11] = 
{
  {60,4,99},
  {1 ,Param_UserWeight_Min_Kg,Param_UserWeight_Max_Kg},
  {1 ,_Height_Mile_L,_Height_Mile_H},
  {1 ,0,C_INCLINE_NUM},
  {1 ,Param_UserAge_Min,Param_UserAge_Max},
  {1 ,D_Male,2},
  {1 ,1,20},
  {5 ,_THRPercent_L,_THRPercent_H},
  {1 ,5,Param_MaxSpeed_MPH_Max},
  {1 ,5,Param_MaxSpeed_MPH_Max},
  {1 ,0,C_INCLINE_NUM}
};
//unsigned short _InforData[11]={0};

union {
  struct {
         unsigned char HEIGHT;
         unsigned char StartIncline;
         unsigned char AGE;
         unsigned char GENDER;
         unsigned char LEVEL; 
         unsigned char PERCENT;
         unsigned char FitTestSpeed;
         //unsigned char WorkoutIncline;
         //---------------------------
         unsigned char ProgramMode;
         unsigned char StartSpeed;
         unsigned char Language;
         unsigned char NavyTest;
         unsigned char Audio;
         unsigned char VOL;
         unsigned char DefaultVolume;
         unsigned char MaxVolume;
         unsigned char OutputVolume;
         unsigned char TimeSeg;
         unsigned char SegOld;
         unsigned char HRControlMode; // 控制模式
         unsigned char HRControlMax;  // 控制参数最大值
         unsigned char Incline;
         unsigned char MaxIncline;
         // 22 Byte
         //---------------------------
         unsigned short TIME ;
         unsigned short WEIGHT;
         unsigned short WorkoutSpeed;
         //---------------------------
         unsigned short MaxSpeed;
         unsigned short AvgSpeed;//==>平均速度
         unsigned short Calories;
         unsigned short CaloriesA;
         unsigned short CaloriesHour;
         unsigned short CaloriesData;
         unsigned short Distance;
         unsigned short DistanceA;
         unsigned short DistanceData;
         //unsigned short ChangeDistanceBuffer;         
         unsigned short MaxTime;
         unsigned short UserTime;
         unsigned short NewTime;//==>重新设定运动中的Time
         unsigned short AccumulatedTime;
         unsigned short TimeSegData;
         unsigned short TimeAccAvgSpeed;
         unsigned short CSAFEAccTime;
         unsigned short FitTestACCTime;
         unsigned short SleepTime;
         unsigned short SpeedChangDelayTime;
         unsigned short MinRPM;
         unsigned short MaxRPM;
         unsigned short LowElevation;
         unsigned short MaxElevation;
         unsigned short PowerOn;
         unsigned short LineChangDelay;
         unsigned short BMI;
         unsigned short VO2;  
         unsigned short DigitalError_Buffer[10];
         unsigned short WarmUpAccTime; // Only Warm up acc time
         unsigned short WorkoutAccTime;     // Only Workout acc time
         unsigned short CoolDwonAccTime;    // Only Cool down acc time
         // 86 Byte
         unsigned long TotalHJS;
         // 4 Byte
         } Data ;
  unsigned char Block[112] ;
} ConsoleInfor ;

//==============================================================================

void Console_Initial(void)
{
  Console.SoftwareUpdate=0;// v2.2-5
  Console.Time_Over = 0;
  Console.Time_Start = 0;
  Console.FitTestAcc = 0;
  Console.Start = 0;//判断是否按START KEY
  Console.Acc = 0;
  Console.Conversion = 0;
  Console.DistanceShow = 0;
  Console.SpeedChang = 0;
  Console.SpeedChangDelay = 0;
  Console.CsafeTarget = 0;
  Console.CsafeTargetDisable = 0;
  
  ConsoleInfor.Data.HRControlMode = 0; // Incline
  ConsoleInfor.Data.HRControlMax = C_HR_DF_MaxIncline;
  ConsoleInfor.Data.TimeSegData = 0;
  ConsoleInfor.Data.AccumulatedTime = 0;
  ConsoleInfor.Data.TimeAccAvgSpeed = 0;
  ConsoleInfor.Data.CSAFEAccTime = 0;
  ConsoleInfor.Data.FitTestACCTime = 0;
  ConsoleInfor.Data.Calories = 0;
  ConsoleInfor.Data.CaloriesA = 0;
  ConsoleInfor.Data.Distance = 0;
  ConsoleInfor.Data.DistanceA = 0;
  ConsoleInfor.Data.TimeSeg = 1;
  ConsoleInfor.Data.SegOld = 0;
  ConsoleInfor.Data.Incline = 0;
  //_InforData[Info_StartIncline] = 0;
  Console_Change(Info_StartIncline,Data_Set,0);
  ConsoleInfor.Data.ProgramMode = TM_Manual;//TM_QuickStart;
  //_InforData[Info_PERCENT] = _THRPercent_DF;//w_THRPercent
  Console_Change(Info_PERCENT,Data_Set,_THRPercent_DF);
  ConsoleInfor.Data.SpeedChangDelayTime = 0;
  ConsoleInfor.Data.AvgSpeed = 0;
  ConsoleInfor.Data.VO2 = 0;
  ConsoleInfor.Data.BMI = 0;
  //ConsoleInfor.Data.ChangeDistanceBuffer = 0;
  ConsoleInfor.Data.WarmUpAccTime = 0;
  ConsoleInfor.Data.WorkoutAccTime = 0;
  ConsoleInfor.Data.CoolDwonAccTime = 0;
}

// 转至对应数据区块位置
unsigned short Console_Change(char by_D,char by_Handle,unsigned short by_Dat)
{
// 0:Info_TIME
// 1:Info_WEIGHT
// 2:Info_HEIGHT
// 3:Info_StartIncline
// 4:Info_AGE  
// 5:Info_GENDER 
// 6:Info_LEVEL  
// 7:Info_PERCENT
// 8:Info_FitTestSpeed
// 9:Info_WorkoutSpeed
// 10:Info_WorkoutIncline
  // 对应数据区块位置 ConsoleInfor.Block[]
  unsigned char _InforTarget[11]={0,0,0,1,2,3,4,5,8,0,20};
  //
  if(by_D == Info_TIME)
  {
      if(by_Handle == Data_Set)
          ConsoleInfor.Data.TIME = by_Dat;
      else  
          return ConsoleInfor.Data.TIME;
  }
  else if(by_D == Info_WEIGHT)
  {
      if(by_Handle == Data_Set)
          ConsoleInfor.Data.WEIGHT = by_Dat;
      else  
          return ConsoleInfor.Data.WEIGHT;
  }
  else if(by_D == Info_WorkoutSpeed)
  {
      if(by_Handle == Data_Set)
          ConsoleInfor.Data.WorkoutSpeed = by_Dat;
      else  
          return ConsoleInfor.Data.WorkoutSpeed;
  }
  else
  {
      if(by_Handle == Data_Set)
          ConsoleInfor.Block[_InforTarget[by_D]] = by_Dat;
      else  
          return ConsoleInfor.Block[_InforTarget[by_D]];
  }
  return 0;
}

/*******************************************************************************
* Function Name  : Console_Information()
* Description    : 仪表信息汇整处理
* Input          : Item Mode 
*                  Handle = Data_Dec / Data_Add / Data_Set / Data_Get 
*                  Number
* Output         : None
* Return         : Information
*******************************************************************************/
unsigned short _HoldKeySpeed; 
unsigned short Console_Information(unsigned char by_Item,unsigned char by_Handle,unsigned short by_Dat)
{
  unsigned short by_Bu ;
  unsigned short _Min ;
  unsigned short _Max ;
  unsigned short _AdjustSize;
  
  by_Bu = Console_Change(by_Item,Data_Get,0);
  _Min = Console_Info[by_Item].Range[0];
  _Max = Console_Info[by_Item].Range[1];
  _AdjustSize = Console_Info[by_Item].AdjustSize;
  if(Console.Mode == C_KM)
  {
     if(by_Item == Info_HEIGHT)
     {
         _Min = _Height_KM_L;
         _Max = _Height_KM_H;
     }
     else if(by_Item == Info_StartSpeed || 
             by_Item == Info_WorkoutSpeed )
     {
         _Min = 8;
     }
     
  }
  
  if(by_Item == Info_WEIGHT && Console.Mode == C_MILE)
  {
      _Min = Param_UserWeight_Min;
      _Max = Param_UserWeight_Max;
  }
  else if(by_Item == Info_WorkoutIncline || \
          by_Item == Info_StartIncline )
  {
      _Max = ConsoleInfor.Data.MaxIncline * 2/10 ;
  }
  else if(by_Item == Info_StartSpeed)
  {
      _Max = ConsoleInfor.Data.MaxSpeed;
  }
  else if(by_Item == Info_WorkoutSpeed)
  {
      _AdjustSize = by_Dat * 10;
      _Min *= 10;
      _Max = ConsoleInfor.Data.MaxSpeed * 10;
  }
  else if(by_Item == Info_LEVEL &&\
         (ConsoleInfor.Data.ProgramMode == TM_5K || \
          ConsoleInfor.Data.ProgramMode == TM_10K) )
  {
      _Max = 10;
  }
  if(by_Handle == Data_Set)
  {// Set Data 
      if(by_Item == Info_TIME)
      {
          Console_Change(by_Item,Data_Set,by_Dat);
          return 1;
      }
      else if(by_Item == Info_WorkoutSpeed)
      {
          Console_Change(by_Item,Data_Set,by_Dat);
          Digital_Set_TargetSpeedStart(1);
          Console.SpeedChangDelay=1;
          ConsoleInfor.Data.SpeedChangDelayTime=0;
          return 1;
      }
      else if(by_Item == Info_StartIncline)
      {
          if((by_Dat % 5 == 0)&&(by_Dat == 0 ||(by_Dat > 0 && by_Dat <= ConsoleInfor.Data.MaxIncline)))//150
          {
              Console_Change(by_Item,Data_Set,by_Dat * 2 / 10);
              return 1;
          }
      }
      else if(by_Item == Info_PERCENT)
      {
          if((by_Dat % 5 == 0)&&(by_Dat >= _Min && by_Dat <= _Max))
          {
              Console_Change(by_Item,Data_Set,by_Dat);
              return 1;
          }
      }
      else if(by_Item == Info_StartSpeed)
      {
          if(by_Dat >= _Min && by_Dat <= _Max)
          {
              Console_Change(by_Item,Data_Set,by_Dat);
              Console_Change(Info_WorkoutSpeed,Data_Set,by_Dat * 10);
              ConsoleInfor.Data.StartSpeed = by_Dat;
              return 1;
          }
      }
      else if(by_Item == Info_WorkoutIncline)
      {
          if(by_Dat > _Max) by_Dat=_Max;
          Console_Change(by_Item,Data_Set,by_Dat);
          Digital_Set_InclineCheck(1);
          return 1;
      }
      else
      {
          if(by_Dat >= _Min && by_Dat <= _Max)
          {
              Console_Change(by_Item,Data_Set,by_Dat);
              return 1;
          }
      }
  }
  else if(by_Handle == Data_Get)
  {// Get Data
      return Console_Change(by_Item,Data_Get,0);
  }
  else if(by_Handle == Data_Add)
  {// Data Add
      if(_Min == 0 && _Max == 0) return 0;
      if(by_Item == Info_TIME)
      {
          if(by_Bu < ConsoleInfor.Data.MaxTime) by_Bu += 60;
          else  by_Bu = 4*60;
      }
      else if(by_Item == Info_WorkoutSpeed || \
              by_Item == Info_WorkoutIncline )
      {
          if(by_Item == Info_WorkoutSpeed)
          {
              if(by_Dat == 99)
              {// Hold key
                  _AdjustSize = 1 * 10;
                  if(_HoldKeySpeed==0)
                      _HoldKeySpeed=ConsoleInfor.Data.WorkoutSpeed;
                  if(Console.Mode == C_KM)
                      _Max = _HoldKeySpeed+150;// +1.6 km
                  else   
                      _Max = _HoldKeySpeed+90;// +1 mile
                  if(_Max > ConsoleInfor.Data.MaxSpeed * 10)
                  {
                      _Max = ConsoleInfor.Data.MaxSpeed * 10;
                  }
              }
              else
                  _HoldKeySpeed=0;
          }
          by_Bu += _AdjustSize;
          if(by_Bu >= _Max) by_Bu = _Max;
      }
      else 
      {
          by_Bu += _AdjustSize;
          if(by_Bu > _Max) by_Bu = _Min;
      }
  }
  else if(by_Handle == Data_Dec)
  {// Data Dec
      if(_Min == 0 && _Max == 0) return 0;
      if(by_Item == Info_TIME)
      {
          if(by_Bu > 4*60) by_Bu -= 60;
          else  by_Bu = ConsoleInfor.Data.MaxTime;
      }
      else if(by_Item == Info_WorkoutSpeed || \
              by_Item == Info_WorkoutIncline )
      {
          if(by_Bu >= (_Min+_AdjustSize)) 
              by_Bu -= _AdjustSize;
          else 
              by_Bu = _Min;
      }
      else
      {
          if(by_Bu != 0 && by_Bu > _Min) 
              by_Bu -= _AdjustSize;
          else 
              by_Bu = _Max;
      }
  }
  Console_Change(by_Item,Data_Set,by_Bu);
  if(by_Item == Info_WorkoutSpeed)
  {
      Digital_Set_TargetSpeedStart(1);
      Console.SpeedChangDelay=1;
      ConsoleInfor.Data.SpeedChangDelayTime=0;
  }
  else if(by_Item == Info_WorkoutIncline)
  {
      Digital_Set_InclineCheck(1);
  }
      
  return 0;
 
}




/*******************************************************************************
* Function Name  : Console_Parameter()
* Description    : 仪表信息处理
* Input          : Item Mode 
* Output         : None
* Return         : Parameter
*******************************************************************************/
unsigned short Console_Parameter(char by_Item)
{
  unsigned short _Out = 0;
  
  switch(by_Item)
  {
      case Info_WarmUpAccTime:// Only Warm up acc time
               _Out = ConsoleInfor.Data.WarmUpAccTime;  
               break;
      case Info_WorkoutAccTime:// Only Workout acc time
               _Out = ConsoleInfor.Data.WorkoutAccTime;    
               break;
      case Info_CoolDwonAccTime:// Only Cool down acc time
               _Out = ConsoleInfor.Data.CoolDwonAccTime;  
               break;
      case Info_AccumulatedTime:// 总时间累计
               _Out = ConsoleInfor.Data.AccumulatedTime;  
               break;
      case Info_FitTestACCTime: 
               _Out = ConsoleInfor.Data.FitTestACCTime;
               break;
  }
  return _Out;
}


//=============================================================================================================================================
//====> 设定PROGRAM参数
//================================================================
u8 Console_ProgramMode(u8 by_D,u8 by_D1)
{
  if(by_D == Data_Set)
      ConsoleInfor.Data.ProgramMode = by_D1;
  else if(by_D == Data_Get)
      return ConsoleInfor.Data.ProgramMode;
  else if(by_D == Data_Add)
  {//because of fitness test & training workouts limit the range
      ConsoleInfor.Data.ProgramMode++;
      if(ConsoleInfor.Data.ProgramMode > 7) ConsoleInfor.Data.ProgramMode = 1;
  }
  else if(by_D == Data_Dec)
  {
      if(ConsoleInfor.Data.ProgramMode <= 1)
          ConsoleInfor.Data.ProgramMode = 7;
      else
          ConsoleInfor.Data.ProgramMode--;
      
  }
    
  return 0;
}
//=============================================================================================================================================
//====> 设定起速速度
//================================================================
u8 Console_StartSpeed(u8 by_D,u16 by_D1)
{
  u8 by_S;
  
  if(by_D == Data_Set)
  {
      if(Console.Mode == C_KM)
          by_S = 8;
      else 
          by_S = 5;
      if(by_D1 >= by_S && by_D1 <= ConsoleInfor.Data.MaxSpeed)  
      {  
          ConsoleInfor.Data.FitTestSpeed = by_D1;
          ConsoleInfor.Data.WorkoutSpeed = by_D1*10;
          ConsoleInfor.Data.StartSpeed = by_D1;
      }
      return 1;
  }
  else if(by_D == Data_Get)
      return ConsoleInfor.Data.StartSpeed;
  return 0;
}
//=============================================================================================================================================
//====> 设定高速速度
//================================================================
u8 Console_MaxSpeed(u8 by_D,u8 by_D1)
{
  if(by_D == Data_Set)
      ConsoleInfor.Data.MaxSpeed = by_D1;
  else if(by_D == Data_Get)
      return ConsoleInfor.Data.MaxSpeed;
  return 0;
}
//=============================================================================================================================================
//====> 设定 Max Incline
//================================================================
u8 Console_MaxIncline(u8 by_D,u8 by_D1)
{
  if(by_D == Data_Set)
      ConsoleInfor.Data.MaxIncline = by_D1;
  else if(by_D == Data_Get)
      return ConsoleInfor.Data.MaxIncline;
  return 0;
}

//=============================================================================================================================================
//====> 设定使用者预设运动时间
//================================================================
u16 Console_UserTime(u8 by_D,u16 by_D1)
{
  if(by_D == Data_Set)
      ConsoleInfor.Data.UserTime = by_D1;
  else if(by_D == Data_Get)
      return ConsoleInfor.Data.UserTime;
  return 0;    
}
//=============================================================================================================================================
//====> 设定时间正反计数
//================================================================
u8 Console_Get_TimeDIR(void)
{
  return Console.TIME_DIR; 
}
//=============================================================================================================================================
//====> 设定使用者最大运动时间
//================================================================
u16 Console_MaxTime(u8 by_D,u16 by_D1)
{
  if(by_D == Data_Set)
      ConsoleInfor.Data.MaxTime = by_D1;
  else if(by_D == Data_Get)
      return ConsoleInfor.Data.MaxTime;
  return 0;
}
/*******************************************************************************
* Function Name  : Console_Language()
* Description    : 语言
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
u8 Console_Language(u8 by_D,u8 by_D1)
{
  if(by_D == Data_Set)
      ConsoleInfor.Data.Language = by_D1;
  else
      return ConsoleInfor.Data.Language;
  return 0;
}
/*******************************************************************************
* Function Name  : Console_DisplaySpeedChange()
* Description    : 是否有调整速度判断,做为显示设置值或实际值用
* Input          : by_D = Data_Set / Data_Get
*                  by_D1 = Number
* Output         : None
* Return         : None
*******************************************************************************/
u8 Console_DisplaySpeedChange(u8 by_D,u8 by_D1)
{
  if(by_D == Data_Set)
      Console.SpeedChang = by_D1;
  else if(by_D == Data_Get)
      return Console.SpeedChang;
  return 0;
}
/*******************************************************************************
* Function Name  : Console_DigitalError()
* Description    : 错误码
* Input          : by_D = Data_Set / Data_Get
*                  by_D1 = Error Line
*                  by_D2 = Error Code
* Output         : None
* Return         : None
*******************************************************************************/
u16 Console_DigitalError(u8 by_D,u8 by_D1,u16 by_D2)
{
  if(by_D == Data_Set)
      ConsoleInfor.Data.DigitalError_Buffer[by_D1] = by_D2;
  else if(by_D == Data_Get)
      return ConsoleInfor.Data.DigitalError_Buffer[by_D1]; 
  return 0;
}
//=============================================================================================================================================
//====> 公英制设定
//================================================================
u8 Console_SpeedUnit(u8 by_D,u8 by_D1)
{
  if(by_D == Data_Set)
      Console.Mode = by_D1;
  else if(by_D == Data_Get)
      return Console.Mode;
  return 0;
}
//=======================================================================================================================================
//====> 休眠时间设定
//======================================================================
u16 Console_SleepTime(u8 by_D,u16 by_D1)
{
  if(by_D == Data_Set)
      ConsoleInfor.Data.SleepTime = by_D1;
  else if(by_D == Data_Get)
      return ConsoleInfor.Data.SleepTime;
  return 0;
}
//=======================================================================================================================================
//====> Power ON 设定
//================================================================================================================
u16 Console_PowerOn(u8 by_D,u16 by_D1)
{
  if(by_D == Data_Set)
      ConsoleInfor.Data.PowerOn = by_D1;
  else if(by_D == Data_Get)
      return ConsoleInfor.Data.PowerOn;
  return 0;
}
//===================================================================================================================================================
// 运动状态设定
// 0=Not Workout / 1=Workout
//===================================================================================================================================================
u8 Console_WorkoutStatus(u8 by_D,u8 by_D1)
{
  if(by_D == Data_Set)
      Console.Start = by_D1;
  else if(by_D == Data_Get)
      return Console.Start;
  return 0;
}

//===================================================================================================================================================
//设定升降高低点
// by_D : Send or Get模式
// by_D1 : 0=LOW / 1=HIGH
// by_D2 : ADC Number
//===================================================================================================================================================
u16 Console_HighLowADC(u8 by_D,u8 by_D1,u16 by_D2)
{
  if(by_D == Data_Set)
  {
      if(by_D1 == 0)// Low ADC
          ConsoleInfor.Data.LowElevation = by_D2;
      else
          ConsoleInfor.Data.MaxElevation = by_D2;
  }
  else if(by_D == Data_Get)
  {
      if(by_D1 == 0)// Low ADC
          return ConsoleInfor.Data.LowElevation;
      else
          return ConsoleInfor.Data.MaxElevation;
  }
  return 0;
}


/*******************************************************************************
* Function Name  : Console_CsafGoalDisable()
* Description    : CSAFE目标值旗标取消
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Console_CsafGoalDisable(void)
{
  Console.CsafeTargetDisable = 1;
  Console.CsafeTarget = 1;
}
/*******************************************************************************
* Function Name  : Console_CsafGoalEND()
* Description    : CSAFE目标值是否结束
* Input          : None
* Output         : None
* Return         : Console.CsafeTarget
*******************************************************************************/
u8 Console_CsafGoalEND(void)
{
  return Console.CsafeTarget;
}
/*******************************************************************************
* Function Name  : Console_NewTime()
* Description    : 调整运动中的新运动时间
* Input          : Add/Dec
* Return         : None
*******************************************************************************/
u16 Console_NewTime(u8 by_D)
{
  u16 by_Sum = 240;
  u16 _TotalTime = ConsoleInfor.Data.AccumulatedTime;
  
  if(ConsoleInfor.Data.ProgramMode == TM_TargetHR)
  {
      _TotalTime -= 120; // 因有两分钟的Warm up
  }
  if(_TotalTime >= by_Sum)
  {
      if(ConsoleInfor.Data.NewTime - 60 < _TotalTime) by_Sum = (u16)((_TotalTime/60)+1)*60;
  }
  if(by_D == Data_Set)
      ConsoleInfor.Data.NewTime = ConsoleInfor.Data.UserTime;
  else if(by_D == Data_Get)
      return ConsoleInfor.Data.NewTime;
  else if(by_D == Data_Add)
  {
      if(ConsoleInfor.Data.NewTime < by_Sum) 
          ConsoleInfor.Data.NewTime = by_Sum;//300 
      else if(ConsoleInfor.Data.NewTime < ConsoleInfor.Data.MaxTime) 
          ConsoleInfor.Data.NewTime += 60;
  }
  else if(by_D == Data_Dec)
  {
      if(ConsoleInfor.Data.NewTime <= by_Sum) 
          ConsoleInfor.Data.NewTime = by_Sum;
      else 
          ConsoleInfor.Data.NewTime -= 60;
  }
  return 0;
}
//=======================================================================================================================================
//Name  : Console_RPM_Parameter()
//Note  : RPM 参数设定应用
//Input : by_D1(模式选择)
//            0 = Get / 1 = Set / 2 = Add / 3 = Dec
//        by_D2(输入参数项目)
//            0 = MinRPM / 1 = MaxRPM
//        by_D3(输入参数)
//Output: 对应RPM参数
//======================================================================
u16 Console_RPM_Parameter(u8 by_D1,u8 by_D2,u16 by_D3)
{
  u16 by_Buffer = 0,_Min,_Max;
  
  switch(by_D1)
  {
      case 1:// Set
             if(by_D2 == 0)
             {
                 if(by_D3 >= 100 && by_D3 <=300)
                     ConsoleInfor.Data.MinRPM = by_D3;
                 else
                     ConsoleInfor.Data.MinRPM = D_MinRPM;
             }
             else
             {
                 if(by_D3 >= 3000 && by_D3 <= 6500)
                     ConsoleInfor.Data.MaxRPM = by_D3;
                 else
                     ConsoleInfor.Data.MaxRPM = D_MaxRPM;
             }
      case 0:// Get
             
             if(by_D2 == 0)
                 by_Buffer = ConsoleInfor.Data.MinRPM;
             else
                 by_Buffer = ConsoleInfor.Data.MaxRPM;
             
             break;
      case 2:// Add
      case 3:// Dec
             if(by_D2 == 0)
             {
                 by_Buffer = ConsoleInfor.Data.MinRPM;
                 _Min = 100;
                 _Max = 300;
             }
             else
             {
                 by_Buffer = ConsoleInfor.Data.MaxRPM;
                 _Min = 3000;
                 _Max = 6500;
             }
             if(by_D1 == 2)
             {// Add
                 by_Buffer += by_D3;
                 if(by_Buffer > _Max) by_Buffer = _Max;
             }
             else
             {// Dec
                 by_Buffer -= by_D3;
                 if(by_Buffer < _Min) by_Buffer = _Min;
             }
             if(by_D2 == 0)
                 ConsoleInfor.Data.MinRPM = by_Buffer;
             else
                 ConsoleInfor.Data.MaxRPM = by_Buffer;
  }
  return by_Buffer;
}
/*******************************************************************************
* Function Name  : Console_Marine()
* Description    : Marine Fitness Test结果换算
* Input          : None
* Output         : None
* Return         : by_Points <对应的成绩>
*******************************************************************************/
u8 Console_Marine(void)
{
  u8 by_Points;
  u16 by_TimeSum;
  
  by_TimeSum = ConsoleInfor.Data.FitTestACCTime;  
  //if(_InforData[Info_GENDER] == D_Male)
  if(ConsoleInfor.Data.GENDER == D_Male)  
  {
      if(by_TimeSum <= 1080)
      {//==> 18:00
          by_Points=100;
      }
      else if(by_TimeSum > 2100)
      {//==> 35:00
          by_Points=1;
      }
      else if(by_TimeSum <= 2040)
      {//==> 34:00
          by_TimeSum -= 1080;
          if(by_TimeSum % 10 == 0)
              by_Points=100-(by_TimeSum / 10);
          else
              by_Points=99-(by_TimeSum /10);
      }
      else if(by_TimeSum <= 2070)
      {//==> 34:30
          by_Points=3;
      }
      else if(by_TimeSum <= 2100)
      {//==> 35:00
          by_Points=2;
      }      
  }
  else
  {
      if(by_TimeSum <= 1260)
      {//==> 21:00
          by_Points=100;
      }
      else if(by_TimeSum > 2250)
      {//==> 37:30
          by_Points=1;
      }
      else
      {
          by_TimeSum -= 1260;
          if(by_TimeSum % 10 == 0)
              by_Points=100-(by_TimeSum / 10);
          else
              by_Points=99-(by_TimeSum /10);
      }
  } 
  return by_Points;  
}
/*******************************************************************************
* Function Name  : Console_GerkinConversion()
* Description    : Gerkin Fitness Test SPEED & INCLINE conversion
* Input          : None
* Output         : None
* Return         : SPEED or INCLINE
*******************************************************************************/
u8 Console_GerkinConversion(void)
{
  if((ConsoleInfor.Data.FitTestACCTime - 180) % 60 == 1)
  {
      if(!Console.Conversion)
      {
          Console.Conversion=1;
          if(ConsoleInfor.Data.FitTestACCTime == 181)
          {
              if(Console.Mode==C_KM)
                  Console_Information(Info_WorkoutSpeed,Data_Set,720);//==>7.2kph
              else  
                  Console_Information(Info_WorkoutSpeed,Data_Set,450);//==>4.5mph
          }
          else
          {
              if(Console.Mode==C_KM)
                  Console_Information(Info_WorkoutSpeed,Data_Add,8);//==> +0.8
              else
                  Console_Information(Info_WorkoutSpeed,Data_Add,5);//==> +0.5
          }
          return 1;
      }
      else
      {
          Console.Conversion=0;
          Console_Information(Info_WorkoutIncline,Data_Set,Console_Information(Info_WorkoutIncline,Data_Get,0)+4); //==> +2.0
          return 2;
      }  
  }
  else return 0;
} 
/*******************************************************************************
* Function Name  : Console_BMI()
* Description    : BMI结果换算
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Console_BMI(void)
{
  //ConsoleInfor.Data.BMI = Formula_BMI(_TM_,Console.Mode,_InforData[Info_WEIGHT],_InforData[Info_HEIGHT]);//w_Weight by_Height
  ConsoleInfor.Data.BMI = Formula_BMI(_TM_,Console.Mode,ConsoleInfor.Data.WEIGHT,ConsoleInfor.Data.HEIGHT);
}
u16 Console_Get_BMI(void)
{
  return ConsoleInfor.Data.BMI;
}
/*******************************************************************************
* Function Name  : Console_VO2()
* Description    : VO2结果换算
* Input          : None
* Output         : None
* Return         : None
* V02=56.981+(1.242*TT)-(0.805*BMI)
*******************************************************************************/
void Console_VO2(void)
{
  ConsoleInfor.Data.VO2 = Formula_VO2(_TM_,ConsoleInfor.Data.FitTestACCTime,ConsoleInfor.Data.BMI);
}
u16 Console_Get_VO2(void)
{
  return ConsoleInfor.Data.VO2;
}

/*******************************************************************************
* Function Name  : Console_Gerkin()
* Description    : Gerkin Fitness Test结果换算
* Input          : None
* Output         : None
* Return         : by_PerformanceLevel 
*******************************************************************************/
u8 Console_Gerkin(void)
{
  u8 by_PerformanceLevel=0;
  u16 by_VO2=0;
  u8 x,y;
  if(ConsoleInfor.Data.FitTestACCTime > 180)
  {
      by_VO2=(ConsoleInfor.Data.FitTestACCTime - 180)/30; 
  }    
  by_VO2=aby_Gerkin_VO2[by_VO2];
  //y=_InforData[Info_AGE] / 10;
  y = ConsoleInfor.Data.AGE / 10;
  if(y < 2) y=0;
  else if(y > 5) y=3;
  else y-=2;
  for(x = 0;x < 5;x++)
  {
      if(ConsoleInfor.Data.GENDER == D_Male)//_InforData[Info_GENDER]
      {
          if(by_VO2 <= aby_GerkinMales[y][x])
          {
              by_PerformanceLevel=x;
              x=5;
          }  
          else if(by_VO2 > aby_GerkinMales[y][4])
          {
              by_PerformanceLevel=5;
              x=5;
          }
      }
      else
      {
          if(by_VO2 <= aby_GerkinFemales[y][x])
          {
              by_PerformanceLevel=x;
              x=5;
          }  
          else if(by_VO2 > aby_GerkinFemales[y][4])
          {
              by_PerformanceLevel=5;
              x=5;
          } 
      }  
  }
  return by_PerformanceLevel;  
}
/*******************************************************************************
* Function Name  : Console_AirForce()
* Description    : AirForce Fitness Test结果换算
* Input          : None
* Output         : None
* Return         : by_Points 
*******************************************************************************/
u16 Console_AirForce(void)
{
  u16 by_Points;
  u16 by_TimeSum;
  u8 x,y;
  //y=_InforData[Info_AGE] / 10;
  y = ConsoleInfor.Data.AGE / 10;
  if(y < 2) y=0;
  else if(y > 5) y=3;
  else y-=2;
  by_TimeSum=((ConsoleInfor.Data.FitTestACCTime / 60)*100)+(ConsoleInfor.Data.FitTestACCTime % 60);
  for(x = 0;x < 21;x++)
  {
      if(ConsoleInfor.Data.GENDER == D_Male)//_InforData[Info_GENDER]
      {
          if(by_TimeSum <= aby_AirForceMales[y][x])
          {
             by_Points=aby_AirForcePoints[x];
             x=21;
          }
          else if(by_TimeSum > aby_AirForceMales[y][20])
          {
             by_Points=0;
             x=21;
          }
      }
      else
      {
          if(by_TimeSum <= aby_AirForceFemales[y][x])
          {
             by_Points=aby_AirForcePoints[x];
             x=21;
          }
          else if(by_TimeSum > aby_AirForceFemales[y][20])
          {
             by_Points=0;
             x=21;
          }
      }
  }
  return by_Points;
}
/*******************************************************************************
* Function Name  : Console_Navy()
* Description    : Navy Fitness Test结果换算
* Input          : None
* Output         : None
* Return         : by_Points <对应的Level>
*******************************************************************************/
u8 Console_Navy(void)
{
  u8 by_Points;
  u8 x,y;
  u16 by_TimeSum;
  u16 by_TestTimeTable,by_TestTimeTable1;
  
  if(ConsoleInfor.Data.AGE >= 65)//_InforData[Info_AGE]
  {
      y=10;
  }
  else if(ConsoleInfor.Data.AGE > 19) //_InforData[Info_AGE]
  {
      y=(ConsoleInfor.Data.AGE-15)/5;//_InforData[Info_AGE]
  }
  by_TimeSum=((ConsoleInfor.Data.FitTestACCTime / 60)*100)+(ConsoleInfor.Data.FitTestACCTime % 60);
  for(x = 0;x < 12;x++)
  {
      if(ConsoleInfor.Data.GENDER == D_Male)//_InforData[Info_GENDER]
      {
          if(ConsoleInfor.Data.NavyTest == 0)
          {
              by_TestTimeTable=aby_NavyMales_LESS[y][x];
              by_TestTimeTable1=aby_NavyMales_LESS[y][11];
          }
          else
          {
              by_TestTimeTable=aby_NavyMales_GREATER[y][x];
              by_TestTimeTable1=aby_NavyMales_GREATER[y][11];
          }
      }
      else
      {
          if(ConsoleInfor.Data.NavyTest == 0)
          {
              by_TestTimeTable=aby_NavyFemales_LESS[y][x];
              by_TestTimeTable1=aby_NavyFemales_LESS[y][11];
          }
          else
          {
              by_TestTimeTable=aby_NavyFemales_GREATER[y][x];
              by_TestTimeTable1=aby_NavyFemales_GREATER[y][11];
          }
      }
      if(by_TimeSum <= by_TestTimeTable)
      {
          by_Points=12-x;
          x=12;
      } 
      else if(by_TimeSum > by_TestTimeTable1)
      {
          by_Points=1;
          x=12;
      }
  } 
  return by_Points;
}
/*******************************************************************************
* Function Name  : Console_DistanceEND()
* Description    : PEB Fitness Test结束判断
* Input          : None
* Output         : None
* Return         : 0 or 1 
*******************************************************************************/
u8 Console_DistanceEND(void)
{
  u16 by_D1,by_D;
  if(ConsoleInfor.Data.ProgramMode == TM_PEB || \
     ConsoleInfor.Data.ProgramMode == TM_Airforce || \
     ConsoleInfor.Data.ProgramMode == TM_Navy)
  {
      by_D1 = 150;
  }
  else if(ConsoleInfor.Data.ProgramMode == TM_Army) 
  {
      by_D1 = 200;
  }
  else if(ConsoleInfor.Data.ProgramMode == TM_Marine) 
  {
      by_D1 = 300;
  }
  by_D = ConsoleInfor.Data.Distance;
  if(Console.Mode == C_KM)
  {
      by_D1 = (u16)((float)by_D1 * 1.609);
      by_D = (u16)((float)by_D * 1.609);
  }
  
  if((by_D / 10) >= by_D1)//Console_FIT_Distance()
      return 1;
  else 
      return 0 ;
}
/*******************************************************************************
* Function Name  : Console_PEB()
* Description    : PEB Fitness Test结果换算
* Input          : None
* Output         : None
* Return         : by_Points <对应的成绩>
*******************************************************************************/
u8 Console_PEB(void)
{
  u8 by_Points;
  u8 x,y;
  u16 by_TimeSum;
  y=ConsoleInfor.Data.AGE / 5;//_InforData[Info_AGE]
  if((ConsoleInfor.Data.GENDER == D_Male && y >= 12)|| \
     (ConsoleInfor.Data.GENDER == D_Female && y >= 10))
  {
      if(ConsoleInfor.Data.GENDER == D_Male) 
          y=8;
      else 
          y=6;
  }    
  else if(y < 5) y=0;
  else y-=4;
  by_TimeSum=((ConsoleInfor.Data.FitTestACCTime / 60)*100)+(ConsoleInfor.Data.FitTestACCTime % 60);
  for(x = 0;x < 19;x++)
  {
      if(ConsoleInfor.Data.GENDER == D_Male)
      {
          if(by_TimeSum <= aby_PEBMales[y][x])
          {
              by_Points=95-(5*x);
              x=19;
          }
          else if(by_TimeSum > aby_PEBMales[y][18])
          {
              by_Points=5;
              x=19;
          }
      }
      else
      {
          if(by_TimeSum <= aby_PEBFemales[y][x])
          {
              by_Points=95-(5*x);
              x=19;
          }
          else if(by_TimeSum > aby_PEBFemales[y][18])
          {
              by_Points=5;
              x=19;
          }
      }
  }
  return by_Points;  
}
/*******************************************************************************
* Function Name  : Console_Army()
* Description    : Army Fitness Test结果换算
* Input          : None
* Output         : None
* Return         : by_Points <对应的成绩>
*******************************************************************************/
u8 Console_Army(void)
{
  u8 by_Points;
  if(ConsoleInfor.Data.GENDER == D_Male)
  {
      if(ConsoleInfor.Data.AGE <= 21)
      {
          by_Points=aby_ArmyMale1721[Console_ArmyScore(1300,2018)];
      }
      else if(ConsoleInfor.Data.AGE <= 26)
      {
          by_Points=aby_ArmyMale2226[Console_ArmyScore(1300,2200)];
      }
      else if(ConsoleInfor.Data.AGE <= 31)
      {
          by_Points=aby_ArmyMale2731[Console_ArmyScore(1318,2236)];
      }
      else if(ConsoleInfor.Data.AGE <= 36)
      {
          by_Points=aby_ArmyMale3236[Console_ArmyScore(1318,2418)];
      }
      else if(ConsoleInfor.Data.AGE <= 41)
      {
          by_Points=aby_ArmyMale3741[Console_ArmyScore(1336,2518)];
      }
      else if(ConsoleInfor.Data.AGE <= 46)
      {
          by_Points=aby_ArmyMale4246[Console_ArmyScore(1402,2536)];
      }
      else if(ConsoleInfor.Data.AGE <= 51)
      {
          by_Points=aby_ArmyMale4751[Console_ArmyScore(1424,2630)];
      }
      else if(ConsoleInfor.Data.AGE <= 56)
      {
          by_Points=aby_ArmyMale5256[Console_ArmyScore(1442,2630)];
      }
      else if(ConsoleInfor.Data.AGE <= 61)
      {
          by_Points=aby_ArmyMale5761[Console_ArmyScore(1518,2630)];
      }
      else//==> > 61
      {
          by_Points=aby_ArmyMale62_[Console_ArmyScore(1542,2630)];
      }
  }
  else
  {
      if(ConsoleInfor.Data.AGE <= 21)
      {
          by_Points=aby_ArmyFemale1721[Console_ArmyScore(1536,2354)];
      }
      else if(ConsoleInfor.Data.AGE <= 26)
      {
          by_Points=100-Console_ArmyScore(1536,2536);
      }
      else if(ConsoleInfor.Data.AGE <= 31)
      {
          by_Points=aby_ArmyFemale2731[Console_ArmyScore(1548,2630)];
      }
      else if(ConsoleInfor.Data.AGE <= 36)
      {
          by_Points=aby_ArmyFemale3236[Console_ArmyScore(1554,2630)];
      }
      else if(ConsoleInfor.Data.AGE <= 41)
      {
          by_Points=aby_ArmyFemale3741[Console_ArmyScore(1700,2630)];
      }
      else if(ConsoleInfor.Data.AGE <= 46)
      {
          by_Points=aby_ArmyFemale4246[Console_ArmyScore(1724,2630)];
      }
      else if(ConsoleInfor.Data.AGE <= 51)
      {
          by_Points=aby_ArmyFemale4751[Console_ArmyScore(1736,2630)];
      }
      else if(ConsoleInfor.Data.AGE <= 56)
      {
          by_Points=aby_ArmyFemale5256[Console_ArmyScore(1900,2630)];
      }
      else if(ConsoleInfor.Data.AGE <= 61)
      {
          by_Points=aby_ArmyFemale5761[Console_ArmyScore(1942,2630)];
      }
      else//==> > 61
      {
          by_Points=aby_ArmyFemale62_[Console_ArmyScore(2000,2630)];
      }
  }
  return by_Points; 
}
/*******************************************************************************
* Function Name  : Console_ArmyScore()
* Description    : Army Fitness Test成绩换算
* Input          : by_TimeStart , by_TimeEnd
* Output         : None
* Return         : by_Score
*******************************************************************************/
u8 Console_ArmyScore(u16 by_TimeStart,u16 by_TimeEnd)
{
  u8 by_Score;
  by_TimeStart=((by_TimeStart / 100)* 60)+(by_TimeStart % 100);
  by_TimeEnd=((by_TimeEnd / 100)* 60)+(by_TimeEnd % 100);
  if(ConsoleInfor.Data.FitTestACCTime <= by_TimeStart)
  {//==>最小范围值
      by_Score=0;
  }
  else if(ConsoleInfor.Data.FitTestACCTime >= by_TimeEnd)
  {//==>最大范围值
      by_Score=(by_TimeEnd - by_TimeStart) / 6;
  }
  else
  {//==>各区间值换算
      by_Score=(ConsoleInfor.Data.FitTestACCTime - by_TimeStart) / 6;
      if((ConsoleInfor.Data.FitTestACCTime - by_TimeStart) % 6 != 0)
      {//==>范围值外另加 1
          by_Score+=1;
      }     
  }
  return by_Score;
}
//=============================================================================================================================================
u32 Console_Get_AvgSpeed(void)
{
   u32 by_Buffer = ConsoleInfor.Data.Distance;
   
   if(Console.Mode==C_KM) 
   {
       by_Buffer = (u32)((float)(by_Buffer * 1.609));
   }
   ConsoleInfor.Data.AvgSpeed=(u32)(by_Buffer*10) / (u32)((ConsoleInfor.Data.TimeAccAvgSpeed*100)/360); 
   if(ConsoleInfor.Data.AvgSpeed < 1) 
   {
       ConsoleInfor.Data.AvgSpeed=1;
   }
   return ConsoleInfor.Data.AvgSpeed;
}
u16 Console_Get_AvgPace(void)
{
   u16 by_Dat;
   u8 by_Mod;

   by_Dat=36000 / Console_Get_AvgSpeed(); 
   by_Mod=by_Dat%60;
   by_Dat=((by_Dat/60)*100)+by_Mod;    
   if(by_Dat > 9959) by_Dat=9959;
   return by_Dat;
}

u16 Console_Get_METs(void)
{
  return Formula_METs(_TM_,Console.Mode,ConsoleInfor.Data.WorkoutSpeed/10,ConsoleInfor.Data.Incline*10/2);//w_Target_Speed
}
//=======================================================================================================================================
//====> Distance计算
//======================================================================
u8 Console_Distance_Counter_Proce(void)
{
   u8 b_Out;
   u32 l_Dat;
   
   b_Out=0;
   if(Console.Distance)
   {
       Console.Distance=0;
       if(Console.Mode==C_KM)
       {
           l_Dat = (u32)((float)Console_Information(Info_WorkoutSpeed,Data_Get,0) * 0.62137);//Console_Speed(Data_Get,0)
       }
       else
       {
           l_Dat = Console_Information(Info_WorkoutSpeed,Data_Get,0);//Console_Speed(Data_Get,0);
       }
       l_Dat = l_Dat * 1000 / 360;
       ConsoleInfor.Data.DistanceData = l_Dat + 1;
       ConsoleInfor.Data.DistanceA += ConsoleInfor.Data.DistanceData;
       if(ConsoleInfor.Data.DistanceA >=1000)
       {
           ConsoleInfor.Data.Distance += (ConsoleInfor.Data.DistanceA/1000);
           ConsoleInfor.Data.DistanceA = (ConsoleInfor.Data.DistanceA%1000);
       }
       b_Out=1;
   }
   return(b_Out);
}
u16 Console_Get_Distance(void)
{
  return(ConsoleInfor.Data.Distance);
}
//================================================================================================================
void Console_SEC_Counter_Int(void) //每1m`s做一次的动作
{
  static u16 w_Time_ms=0;
  static u16 w_TimeTotal=0;
#ifndef _DebugClose 
  static u16 Debug_Time_Ctrl = 1000;
#endif    
  if(Console.Time_Start)//==>时间累计开启时
  {
#ifdef _DebugClose 
    if(w_TimeTotal >= 1000)//==>1秒做一次 1000
#else 
      if(w_TimeTotal >= Debug_Time_Ctrl)//==>1秒做一次 1000
#endif        
      {
         w_TimeTotal=0;
         Console.SEC=1;     //时间累计位准
      }
      else w_TimeTotal++;
  }
  else
  {
      w_TimeTotal=0;
      Console.SEC=0;
  }
  if(Console.Start)//当按START启动时
  {
#ifdef _DebugClose 
    if(w_Time_ms >= 1000) //==>1秒做一次 1000
#else 
      if(w_Time_ms >= Debug_Time_Ctrl)
#endif 
      {
          w_Time_ms=0; //清除累计
          Console.Distance=1;//距离累计位准
          Console.Calories=1;//卡路里累计位准
          if(Console.SpeedChangDelay)
          {//==>当有调整SPEED时
              if(ConsoleInfor.Data.SpeedChangDelayTime >= 4)//==>配合单位显示所以修正为4秒
              {//==>在4秒后速度窗口改秀实际值
                  Console.SpeedChangDelay=0;
                  Console.SpeedChang=1;
              }
              else 
              {
                  ConsoleInfor.Data.SpeedChangDelayTime++;              
                  Console.SpeedChang=0;
              }    
          }
      }
      else ++w_Time_ms;
  }
  else w_Time_ms=0;
}
//=======================================================================================================================================
//====> Time计数是否结束
//================================================================================================================
u8 Console_TimeOver(u8 by_D,u8 by_D1)
{
  u16 by_B = ConsoleInfor.Data.Distance;
  
  if(by_D == Data_Set)
      Console.Time_Over = by_D1;
  else if(by_D == Data_Get)
  {
      if(Console.CsafeTarget == 0)
      {
          if(Console.CsafeTargetDisable == 0)
          {// 判断旗标打开
              if(CSAFE_Information(_Get_TargetDistance,0) == 1)
              {// 设定 Distance Goal
                  if(Console.Mode == C_KM)
                  {
                      by_B = (u16)((float) by_B * 1.609);
                  }
                  if((by_B / 10) >= CSAFE_Information(_Get_DFdistnce,0))
                  {//==>目标值达到就结束运动
                      Console.CsafeTarget = 1;
                      Console.Time_Over = 1;
                  }
              }
              if(CSAFE_Information(_Get_TargetCalories,0) == 1)
              {// 设定 Calories Goal
                  if(ConsoleInfor.Data.Calories >= CSAFE_Information(_Get_DFcalories,0))
                  {//==>目标值达到就结束运动
                      Console.CsafeTarget = 1;
                      Console.Time_Over = 1;
                  }
              }
          }
      }
      return Console.Time_Over;
  }
  return 0;
}
//===================================================================================================================================================
// 时间是否开始计数
// 0=停止计数 1=启动计数
//===================================================================================================================================================
void Console_TimeStart(u8 by_D)
{
  Console.Time_Start = by_D;
}
//===================================================================================================================================================
void Console_ChangeTimeCul(void)
{// 运动中如果调整完时间需要重新取排数时间
  Console.TIME_DIR=0;
  ConsoleInfor.Data.TimeSegData = (ConsoleInfor.Data.UserTime * 10)/(C_TIME_SEG_NUM);
  ConsoleInfor.Data.LineChangDelay = ConsoleInfor.Data.TimeSegData / 10;
}
//===================================================================================================================================================
void Console_Time_Cul(void)//正数或倒数时间计算
{
  if(ConsoleInfor.Data.TIME > 0)//w_Time
  {
      Console.TIME_DIR=0;
      ConsoleInfor.Data.TimeSegData = (ConsoleInfor.Data.TIME*10)/(C_TIME_SEG_NUM);//w_Time
  }
  else
  {
      Console.TIME_DIR=1;
      ConsoleInfor.Data.TimeSegData=600;
  }
  ConsoleInfor.Data.LineChangDelay = ConsoleInfor.Data.TimeSegData/10;
}
//===================================================================================================================================================
u8 Console_Time_Counter_Proce(void)
{
   u8 b_Dat;
   
   b_Dat=0;
   if(Console.SEC)
   {
       b_Dat=1;
       Console.SEC=0;
       if(Console.TIME_DIR)
       {
           if(ConsoleInfor.Data.TIME >= ConsoleInfor.Data.MaxTime)//==>99:00
           {
               if(ConsoleInfor.Data.ProgramMode == TM_5K || \
                  ConsoleInfor.Data.ProgramMode == TM_10K || \
                  ConsoleInfor.Data.ProgramMode == TM_PEB || \
                  ConsoleInfor.Data.ProgramMode == TM_Army || \
                  ConsoleInfor.Data.ProgramMode == TM_Navy || \
                  ConsoleInfor.Data.ProgramMode == TM_Airforce || \
                  ConsoleInfor.Data.ProgramMode == TM_Marine )
                   ConsoleInfor.Data.TIME = 0;
               else
                   Console.Time_Over = 1;
           }
           else
           {
               //++w_LineTime;
               ++ConsoleInfor.Data.TIME;
               if(Console.Acc) ++ConsoleInfor.Data.AccumulatedTime;
               ++ConsoleInfor.Data.TimeAccAvgSpeed;
               ++ConsoleInfor.Data.CSAFEAccTime;
               if(Console.FitTestAcc) ++ConsoleInfor.Data.FitTestACCTime;//==>980616
           }
       }
       else
       {
           if(ConsoleInfor.Data.TIME == 0)
           {
               Console.Time_Over=1;
           }    
           else
           {
               //++w_LineTime;
               --ConsoleInfor.Data.TIME;
               if(Console.Acc) ++ConsoleInfor.Data.AccumulatedTime;
               ++ConsoleInfor.Data.TimeAccAvgSpeed;
               ++ConsoleInfor.Data.CSAFEAccTime;
               if(Console.FitTestAcc) ++ConsoleInfor.Data.FitTestACCTime;//==>980616
           }
       }
       if(Console.Time_Over == 0)
       {
//           switch(_NowWorkoutStatus)
//           {
//               case _WiFi_WorkoutState_WarmUp:
//                        ConsoleInfor.Data.WarmUpAccTime++;
//                        break;
//               case _WiFi_WorkoutState_Workout:
//                        ConsoleInfor.Data.WorkoutAccTime++;
//                        break;
//               case _WiFi_WorkoutState_CoolDown:
//                        ConsoleInfor.Data.CoolDwonAccTime++; 
//                        break;
//           }
       }
   }
   return(b_Dat);
}
//===========================================================
//====> 回传CSAFE专用时间累计
//===========================================================
u16 Console_Get_CsafeAccTime(void)
{
    return ConsoleInfor.Data.CSAFEAccTime;
}
u8 Console_Time_Seg_Proce(void)
{  
  u8 b_Out=0;
  u8 by_Dat;
  u16 w_TimeAccBuffer = 0;
  
  w_TimeAccBuffer = ConsoleInfor.Data.AccumulatedTime;
  
  if((ConsoleInfor.Data.ProgramMode == TM_Gerkin || \
      ConsoleInfor.Data.ProgramMode == TM_WFI )&& w_TimeAccBuffer > 180)
  {
      w_TimeAccBuffer -= 180;
  }
  else if(ConsoleInfor.Data.ProgramMode == TM_TargetHR && w_TimeAccBuffer > 120)
  {
      w_TimeAccBuffer -= 120;
  }
  if(Console.TIME_DIR) //正数
  {
      if(w_TimeAccBuffer > 1)
      {
          if(((w_TimeAccBuffer*10) % ConsoleInfor.Data.TimeSegData)==10)
          {
              if(ConsoleInfor.Data.TimeSeg < C_TIME_SEG_NUM) 
                  ++ConsoleInfor.Data.TimeSeg;
              else 
                  ConsoleInfor.Data.TimeSeg=1;
          }
      }
  }
  else //倒数
  {
      by_Dat=((w_TimeAccBuffer*10) / ConsoleInfor.Data.TimeSegData)+1;
      if(by_Dat > C_TIME_SEG_NUM)  
          ConsoleInfor.Data.TimeSeg=C_TIME_SEG_NUM;
      else 
          ConsoleInfor.Data.TimeSeg=by_Dat;
  }
  if(ConsoleInfor.Data.SegOld != ConsoleInfor.Data.TimeSeg)
  {
      b_Out=1;
      ConsoleInfor.Data.SegOld = ConsoleInfor.Data.TimeSeg;
      ConsoleInfor.Data.LineChangDelay = ConsoleInfor.Data.TimeSegData / 10;
  }
  return(b_Out);
}
//******在5K RUN MODE中换算排数用******
const unsigned char _10K[32]={ 3, 6, 9,12,16,19,22,25,28,32,35,38,41,44,47,50,
                              53,56,59,62,66,69,72,75,78,82,85,88,91,94,97,100}; 
u8 Console_5K10K_Seg_Proce(void)
{
  unsigned short by_Dat;
  static u8 by_UB=0;
  
  // change to KM
  by_Dat=(u16)((float)ConsoleInfor.Data.Distance * 1.609);
  //
  if(by_Dat <= (_10K[0]*100))
      by_UB=0;
  else
  {
      if(by_Dat > (_10K[by_UB]*100))
      {
          if(by_UB < 31) by_UB++;
          if(by_UB+1 <= C_TIME_SEG_NUM)
              ConsoleInfor.Data.TimeSeg=by_UB+1;
          else
              ConsoleInfor.Data.TimeSeg=by_UB-15;
          return 1;
      }
  }
  return 0;
}

//*****判断是否结束此模式****e
u8 Console_DistanceGoal_Over(void)
{
  if((ConsoleInfor.Data.ProgramMode == TM_5K &&\
      Console_Get_GoalDistance() >= 500)||\
     (ConsoleInfor.Data.ProgramMode == TM_10K &&\
      Console_Get_GoalDistance() >= 1000)  )
      return 1;
  else 
      return 0;
}
u16 Console_Get_GoalDistance(void)
{
    u16 by_DisBuffer = ConsoleInfor.Data.Distance;
    
    by_DisBuffer=(u16)((float)by_DisBuffer * 1.609);
    return by_DisBuffer / 10;
}
// 保留5KRun Pogram切换时所产生的距离
//u16 Console_ChangeDistanceBuffer(char by_D)
//{
//  if(by_D == 0)
//  {
//      ConsoleInfor.Data.ChangeDistanceBuffer += ConsoleInfor.Data.Distance;
//  }
//  return ConsoleInfor.Data.ChangeDistanceBuffer;
//}

/*******************************************************************************
* Function Name  : Console_ClearDistance()
* Description    : 清除Distance累计
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Console_ClearDistance(void)
{
    ConsoleInfor.Data.Distance = 0;
    ConsoleInfor.Data.DistanceA = 0;
}
/*******************************************************************************
* Function Name  : Console_DistanceShow()
* Description    : Distance显示模式旗标
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u8 Console_ShowDistanceTarget(u8 by_D,u8 by_D1)
{
  if(by_D == Data_Set)
      Console.DistanceShow = by_D1;
  else if(by_D == Data_Get)
      return Console.DistanceShow;
  return 0;
}

/*******************************************************************************
* Function Name  : Console_GetInclineInformation()
* Description    : 各PROGRAM升降阶段值取得
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Console_GetInclineInformation(void)
{
  
  switch(ConsoleInfor.Data.ProgramMode)
  {
      case TM_Manual:
               Console_Information(Info_WorkoutIncline,Data_Set,ConsoleInfor.Data.StartIncline);
               break;  
      case TM_Rolling:                  
               Console_Information(Info_WorkoutIncline,Data_Set,aby_RollingHills[ConsoleInfor.Data.LEVEL-1][ConsoleInfor.Data.TimeSeg-1]/5);
               break;
      case TM_FatBurn:                  
               Console_Information(Info_WorkoutIncline,Data_Set,aby_FatBurn[ConsoleInfor.Data.LEVEL-1][ConsoleInfor.Data.TimeSeg-1]/5);
               break;
      case TM_5K:  
      case TM_10K:   
               Console_Information(Info_WorkoutIncline,Data_Set,aby_5KRun[ConsoleInfor.Data.LEVEL-1][ConsoleInfor.Data.TimeSeg-1]/5);
               break;  
      default: if(ConsoleInfor.Data.AccumulatedTime <= 1)         
                   Console_Information(Info_WorkoutIncline,Data_Set,0);
               break;
  }
    
}
/*******************************************************************************
* Function Name  : Console_GetInclineInformation()
* Description    : 各PROGRAM升降阶段值取得
* Input          : by_Dat=program , by_Dat1=排数 , by_Dat2=level
* Output         : None
* Return         : 参数值
*******************************************************************************/
u8 Console_GetInclinePfofile(u8 by_Dat,u8 by_Dat1,u8 by_Dat2)
{
    u8 by_R = 1;
    
    switch(by_Dat)
    {
        case TM_Rolling:                  
                 by_R = aby_RollingHills[by_Dat2][by_Dat1]/5;
                 break;
        case TM_FatBurn:                  
                 by_R = aby_FatBurn[by_Dat2][by_Dat1]/5;
                 break;
        case TM_5K:   
        case TM_10K:  
                 by_R = aby_5KRun[by_Dat2][by_Dat1]/5;
                 break;   
    }
    return by_R;
}
//=============================================================================================================================================
//====> 排数值
//================================================================
u8 Console_Get_Time_Seg(void)
{
  return ConsoleInfor.Data.TimeSeg;
}
//=============================================================================================================================================
u8 Console_Calories_Counter_Proce(void)
{
  u8 b_Out;
  u32 w_Cal_Weight,W_Cal_Incline,w_Cal_Speed;
  u32 l_Out = 0;
  
  b_Out = 0;
  if(Console.Calories)
  {
      Console.Calories = 0;
      
      if(Console.Mode == C_MILE)
      {
          w_Cal_Speed = ConsoleInfor.Data.WorkoutSpeed / 10;
          w_Cal_Weight = (u32)((float)(ConsoleInfor.Data.WEIGHT * 100 )/ 2.2);
      }
      else
      {
          w_Cal_Speed = (u32)((float)(ConsoleInfor.Data.WorkoutSpeed / 10)/ 1.609);
          w_Cal_Weight = ConsoleInfor.Data.WEIGHT * 100;
      }
      W_Cal_Incline = ConsoleInfor.Data.Incline * 10 / 2;
      if((Console.Mode == C_KM)&&(ConsoleInfor.Data.WorkoutSpeed == 650))
      {
          w_Cal_Speed = 41;
      }
      l_Out = Formula_Calories(_TM_,w_Cal_Speed,W_Cal_Incline);
      ConsoleInfor.Data.CaloriesHour = Formula_HourCalories(w_Cal_Weight,l_Out);
      ConsoleInfor.Data.CaloriesData = Formula_SecCalories(w_Cal_Weight,l_Out);
      
      ConsoleInfor.Data.CaloriesA += ConsoleInfor.Data.CaloriesData;
      if(ConsoleInfor.Data.CaloriesA >= 10000)
      {
          ConsoleInfor.Data.Calories += (ConsoleInfor.Data.CaloriesA / 10000);
          ConsoleInfor.Data.CaloriesA %= 10000;
          b_Out = 1;
      }
  }
  return b_Out;
}
//===================================================================================================================================================
u16 Console_Get_Calories_Hour(void)
{
  /*
   小时卡公式 :
   speed > 4.0 
   Calories/h = ((speed x 5.36)+(incline% x speed x 24.12)+3.5)x weight(kg) / 3.3333
   speed <= 4.0
   Calories/h = ((speed x 2.68)+(incline% x speed x 48.24)+3.5)x weight(kg) / 3.3333
  */
  return ConsoleInfor.Data.CaloriesHour;
}
//===================================================================================================================================================
u16 Console_Get_Calories(void)
{
  /*
   cal/second 秒卡公式 :
   speed > 4.0 
   Calories/h = ((speed x 5.36)+(incline% x speed x 24.12)+3.5)x weight(kg) / 12000
   speed <= 4.0
   Calories/h = ((speed x 2.68)+(incline% x speed x 48.24)+3.5)x weight(kg) / 12000 
   note : 
   分钟卡改为秒卡 => 12000 = 60 / 0.005
   分钟卡公式就 / 12000 改为 x 0.005 就可以了.
  */
  return ConsoleInfor.Data.Calories;
}
//===============================================================================================================================
//====> 总时间累计是否进行设定
//====> 0->No 1->Yes
//=========================================================================================
void Console_Set_StartAccumulated(u8 by_Dat)
{
  Console.Acc=by_Dat;
}
void Console_Set_FitTestTimeAcc(u8 by_Dat)
{
  Console.FitTestAcc=by_Dat;
}
//===============================================================================================================================
//====> 取得总时间累计
//=========================================================================================
//u16 Console_Get_Time_Accumulated(void)
//{
//  return ConsoleInfor.Data.AccumulatedTime;
//}
//===============================================================================================================================
//====> 取得总时间累计
//=========================================================================================
//u16 Console_Get_FitTestAccTime(void)
//{
//  return ConsoleInfor.Data.FitTestACCTime;
//}
/*******************************************************************************
* Function Name  : Console_NavyTestMode()
* Description    : Navy Fitness Test评分标准
* Input          : by_D (0:LESS 1:GREATER)
* Output         : None
* Return         : None
*******************************************************************************/
void Console_NavyTestMode(u8 by_D)
{
  ConsoleInfor.Data.NavyTest = by_D;
}
//===============================================================================================================================
//==> Bike_Set_Audio()
//==> 音源设定
//=========================================================================================
u8 Console_Audio(u8 by_D,u8 by_D1)
{
  if(by_D == Data_Set)
      ConsoleInfor.Data.Audio = by_D1;
  else if(by_D == Data_Get)
      return ConsoleInfor.Data.Audio;
  return 0;
}
//=============================================================================================================================================
//====> Volume 设定值参数
//================================================================
// 
u16 _VolumeNumber[33] = {   0, 425, 520, 615, 705, 805, 895, 995,
                         1090,1180,1275,1365,1455,1550,1640,1735,
                         1825,1915,2005,2100,2185,2275,2375,2455,
                         2545,2640,2725,2815,2905,2995,3080,3165,
                         3250};       

u16 Console_GetVolumeNumber(u8 by_D)
{
  if(by_D > 32) by_D = 32;
  return _VolumeNumber[by_D];
}
void Console_AddUserVolume(void)
{
  if(ConsoleInfor.Data.VOL < ConsoleInfor.Data.MaxVolume)
  {
      ConsoleInfor.Data.VOL++;
  }
}
void Console_DecUserVolume(void)
{
  if(ConsoleInfor.Data.VOL >= 1)
  {
      ConsoleInfor.Data.VOL--;
  }
}
u8 Console_GetUserVolume(void)
{
  return ConsoleInfor.Data.VOL;
}

void Console_SetDefaultVolume(u8 by_Dat)
{
  ConsoleInfor.Data.DefaultVolume = by_Dat;
}
u8 Console_GetDefaultVolume(void)
{
  return ConsoleInfor.Data.DefaultVolume;
}
void Console_SetMaxVolume(u8 by_Dat)
{
  ConsoleInfor.Data.MaxVolume = by_Dat;
}
u8 Console_GetMaxVolume(void)
{
  return ConsoleInfor.Data.MaxVolume;
}
void Console_SetOutputVolume(u8 by_Dat)
{
  ConsoleInfor.Data.OutputVolume = by_Dat;
  ConsoleInfor.Data.VOL = by_Dat;
}
u8 Console_GetOutputVolume(void)
{
  return ConsoleInfor.Data.OutputVolume;
}
//
//=============================================================================================================================================
//====> HR Control Speed / Incline 设定值参数
//================================================================
// by_D:0=Incline / 1=Speed
void Console_SetHRControlModel(u8 by_D)
{ 
  ConsoleInfor.Data.HRControlMode = by_D;
  if(by_D == 0)
      ConsoleInfor.Data.HRControlMax = C_HR_DF_MaxIncline;
  else
  {
      if(Console.Mode == C_KM)
          ConsoleInfor.Data.HRControlMax =  C_HR_DF_MaxSpeed_KM;
      else
          ConsoleInfor.Data.HRControlMax =  C_HR_DF_MaxSpeed_Mile;
  }
}
u8 Console_GetHRControlMode(void)
{
  return ConsoleInfor.Data.HRControlMode;
}
// by_D:ADD or DEC
void Console_HRControlMax(u8 by_D)
{
  if(ConsoleInfor.Data.HRControlMode == 0)
  {// Incline
      if(by_D == D_ADD)
      {
          ConsoleInfor.Data.HRControlMax++;
          if(ConsoleInfor.Data.HRControlMax > C_INCLINE_NUM) ConsoleInfor.Data.HRControlMax = C_INCLINE_NUM;
      }
      else
      {
          if(ConsoleInfor.Data.HRControlMax > 0) ConsoleInfor.Data.HRControlMax--;//C_HR_DF_MaxIncline
      }
  }
  else
  {// Speed
      if(by_D == D_ADD)
      {
          ConsoleInfor.Data.HRControlMax++;
          if(ConsoleInfor.Data.HRControlMax > ConsoleInfor.Data.MaxSpeed)
          {
              ConsoleInfor.Data.HRControlMax = ConsoleInfor.Data.MaxSpeed;//C_HR_DF_MaxSpeed_KM;
          }
      }
      else
      {
          if(Console.Mode == C_KM)
              by_D = C_HR_DF_MaxSpeed_KM;
          else
              by_D = C_HR_DF_MaxSpeed_Mile;
          if(ConsoleInfor.Data.HRControlMax > by_D)
              ConsoleInfor.Data.HRControlMax--;
          else
              ConsoleInfor.Data.HRControlMax = by_D;
      }
  }
}
// by_D1:ADD or DEC 
// by_D2:调整参数
void Console_HRControlChange(u8 by_D1,u16 by_D2)
{
  if(ConsoleInfor.Data.HRControlMode == 0)
  {// Incline
      if(by_D1 == D_ADD)
      {
          ConsoleInfor.Data.Incline += by_D2;
          if(ConsoleInfor.Data.Incline > ConsoleInfor.Data.HRControlMax)
          {
              ConsoleInfor.Data.Incline = ConsoleInfor.Data.HRControlMax;
          }
      }
      else
      {
          if(ConsoleInfor.Data.Incline > by_D2)
              ConsoleInfor.Data.Incline -= by_D2;
          else
              ConsoleInfor.Data.Incline = 0;
      }
      Digital_Set_InclineCheck(1);
  }
  else
  {// Speed
      
      if(by_D1 == D_ADD)
      {
          by_D2 *= 10;
          ConsoleInfor.Data.WorkoutSpeed += by_D2;//w_Target_Speed
          if(ConsoleInfor.Data.WorkoutSpeed > (ConsoleInfor.Data.HRControlMax * 10))//w_Target_Speed
          {
             ConsoleInfor.Data.WorkoutSpeed = ConsoleInfor.Data.HRControlMax * 10;//w_Target_Speed
          }
          Digital_Set_TargetSpeedStart(1);
          Console.SpeedChangDelay=1;
          ConsoleInfor.Data.SpeedChangDelayTime=0;     
      }
      else
      {
          Console_Information(Info_WorkoutSpeed,Data_Dec,by_D2);
      }
  }
}
u8 Console_GetHRControlMax(void)
{
  if(ConsoleInfor.Data.HRControlMode == 0)
  {// Incline
      return ConsoleInfor.Data.HRControlMax * 10 / 2;
  }
  else
  {// Speed
      return ConsoleInfor.Data.HRControlMax;
  }
}
//


/*******************************************************************************
* Function Name  : Bike_HJS
* Description    : Headphone Jack Sensor
* Input          : 动作 / 参数
* Output         : None
* Return         : None
*******************************************************************************/
long unsigned int Console_HJS(unsigned char by_Mo,long unsigned int by_D)
{
  switch(by_Mo)
  {
      case Data_Set:
              ConsoleInfor.Data.TotalHJS = by_D;
              break;
      case Data_Get:
              return ConsoleInfor.Data.TotalHJS;
      case Data_Total:
              if(IO_HJS() == 1)
              {
                  Timer_Counter_Clear(_Time_HJSCheck);
              }
              else
              {
                  if(Timer_Counter(T_STEP,_Time_HJSCheck,3))
                  {
                      ConsoleInfor.Data.TotalHJS++;
                      if(ConsoleInfor.Data.TotalHJS > 999999) ConsoleInfor.Data.TotalHJS = 999999;
                  }
              }
              break;
  }
  return 0; 
}
// start v2.2-5
//=============================================================================
//==> Bike_State.SoftwareUpdate=0 : console will can update
//==> Bike_State.SoftwareUpdate=1 : console will can not update
//=============================================================================
unsigned char Console_AutoSoftwareUpdateStatus(unsigned char by_Mode)
{
  if(by_Mode != 0)
  {
      Console.SoftwareUpdate=1;
  }
  return Console.SoftwareUpdate;
}
// end v2.2-5

unsigned short Console_GetTimeSegData(void)
{
  return ConsoleInfor.Data.TimeSegData / 10;
}

