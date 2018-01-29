#include "Fan.h"

__no_init static USHORT by_Fan_Speed;
__no_init static USHORT by_Fan_Level;
__no_init static USHORT RPM_Value;
UCHAR Timer_Count;

#define C_SPEED_NUM 3
const USHORT w_Speed[4] = {0, FAN_SPEED_MIN, FAN_SPEED_MID, FAN_SPEED_MAX};


void Fan_Initial_Data(void)
{
  by_Fan_Speed = 0;
  by_Fan_Level = 0;
  RPM_Value = 0;
  Timer_Count = 0;
  Fan_Set_Level(by_Fan_Level);
}

void Fan_Set_Level(UCHAR by_Level)
{

    if ((by_Level > 0) && (by_Level < 4))
    {
        HAL_Set_Fan_Duty(w_Speed[by_Level]);
    }
    else
    {
        HAL_Set_Fan_Duty(0);
    }
}

void Fan_Inc_Level(void)
{
    if (by_Fan_Level < C_SPEED_NUM)
    {
        by_Fan_Level++;
    }
    else
    {
        by_Fan_Level = 0;
    }
    Fan_Set_Level(by_Fan_Level);
}

/*--------------------------------------------*/
void Fan_Dec_Level(void)
{
    if (by_Fan_Level > 0)
    {
        by_Fan_Level--;
    }
    else
    {
        by_Fan_Level = C_SPEED_NUM;
    }
    Fan_Set_Level(by_Fan_Level);
}

void Fan_Set_Speed(USHORT by_Dat)
{
  if(by_Dat < FAN_TIMER_TOP)
    HAL_Set_Fan_Duty(by_Dat);
}


USHORT Fan_Get_Speed(void)
{
    return (by_Fan_Speed);
}

void Fan_1ms_Int(void)
{
  USHORT Temp_Value;
  if(Timer_Count >= 100)//100mS interrupt
  {
    Timer_Count=0;
    Temp_Value = HAL_Fan_TIM_GetCounter()*FAN_PARAMETER;
    if(RPM_Value > Temp_Value)
    {
      if((RPM_Value - Temp_Value) > FAN_PARAMETER)
        RPM_Value = Temp_Value;
    }
    else if(RPM_Value < Temp_Value)
    {
       if((Temp_Value - RPM_Value) > FAN_PARAMETER)
        RPM_Value = Temp_Value;
    }    
    HAL_Fan_TIM_SetCounter(0);
  }
  else Timer_Count++;  
}

