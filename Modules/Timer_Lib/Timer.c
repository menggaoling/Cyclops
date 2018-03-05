
#include "Timer.h"

USHORT B_Delay[Time_TotalBuffer];
UCHAR B_Time[Time_TotalBuffer];

// Delay 配置
// Time[0]  :
// Time[1]  :
// Time[2]  :
// Time[3]  :
// Time[4]  :
// Time[5]  : 
// Time[6]  :
// Time[7]  :
// Time[8]  :
// Time[9]  :
// Time[10] : WiFi Connect Check Time
// Time[11] : Phone Jack Sensor Check Time

// time 计数器清空重置
void Timer_Initial(void)
{
  UCHAR by_Loop;
  
  for(by_Loop=0;by_Loop < 10;by_Loop++)
  {
      B_Delay[by_Loop]=0;
      B_Time[by_Loop]=0;
  }
}
//关闭特定计数器
void Timer_Close(UCHAR by_Dat)
{
  UCHAR i;
  if(by_Dat == Time_AllClose)
  {
      for(i=0;i < 10;i++) B_Time[i]=0;
  }
  else B_Time[by_Dat]=0;
}
//每100ms做ㄧ次
void Timer_Flag_Int(void)
{
  UCHAR i;
  
  for(i = 0 ; i < Time_TotalBuffer ; i++)
  {
      if((B_Time[i] == 1)&&(B_Delay[i] < 9000)) B_Delay[i]++;
  }
}
//清空重制某一计数器
void Timer_Counter_Clear(UCHAR by_Index)
{
  B_Delay[by_Index]=0;
  B_Time[by_Index]=1;
}
//设定计数器 1.LOOP or STEP 2.第?计数器 3.计数TIME
UCHAR Timer_Counter(UCHAR by_Mode,UCHAR by_Who,USHORT w_Dat)
{
  UCHAR b_Out;
  b_Out=0;
  if(B_Time[by_Who]==1 && B_Delay[by_Who]>=w_Dat)
    {
    b_Out=1;
    if(by_Mode==T_LOOP) B_Delay[by_Who]=0;
      else B_Time[by_Who]=0;
    }
  return(b_Out);
}
void Timer_Delay(UCHAR delayTime)
{// Real Delay Time = delayTime * 0.1s 
    Timer_Counter_Clear(_Time_KeepDelay);
    while(!Timer_Counter(T_STEP,_Time_KeepDelay,delayTime));
};