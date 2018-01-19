#include "Fan.h"

__no_init static USHORT by_Fan_Speed;
static unsigned int by_3s = 0;
static struct
{
    unsigned PROTECT_EN: 1;
    unsigned PROTECTED: 1;
} Fan;

#define C_SPEED_NUM 3

const USHORT w_Speed[4] = {0, FAN_SPEED_MIN, FAN_SPEED_MID, FAN_SPEED_MAX};


void Fan_Initial_Data(void)
{
    Fan_Set_Speed(0);
    Fan.PROTECT_EN = 0;
    Fan.PROTECTED = 0;
}

void Fan_Set_Level(unsigned by_Level)
{
//    static unsigned int by_Current = 0;
//
//    if ((by_Level > 0) && (by_Level < 4))
//    {
//        TIMER_CompareBufSet(FAN_TIMER, FAN_CCP, w_Speed[by_Level]);
//        FAN_IO_Sel_High();
//    }
//    else
//    {
//        TIMER_CompareBufSet(FAN_TIMER, FAN_CCP, 0);
//        FAN_IO_Sel_Low();
//    }
//    if (by_Current != TIMER_CompareBufGet(FAN_TIMER, FAN_CCP))
//    {
//        by_Current =  TIMER_CompareBufGet(FAN_TIMER, FAN_CCP);
//        if (by_Current == 0)
//        {
//            FAN_IO_Sel_Low();
//        }
//        else
//        {
//            FAN_IO_Sel_High();
//        }
//    }
}

void Fan_Inc_Speed(void)
{
    //if(Fan.PROTECTED) return;
    if (by_Fan_Speed < C_SPEED_NUM)
    {
        by_Fan_Speed++;
    }
    else
    {
        by_Fan_Speed = 0;
    }
    Fan_Set_Speed(by_Fan_Speed);
}

/*--------------------------------------------*/
void Fan_Dec_Speed(void)
{
    //if(Fan.PROTECTED) return;
    if (by_Fan_Speed > 0)
    {
        by_Fan_Speed--;
    }
    else
    {
        by_Fan_Speed = C_SPEED_NUM;
    }
    Fan_Set_Speed(by_Fan_Speed);
}

void Fan_Set_Speed(unsigned char by_Dat)
{
    by_Fan_Speed = by_Dat;
}


unsigned char Fan_Get_Speed(void)
{
    return (by_Fan_Speed);
}

void Fan_ProtectFun_Set(unsigned char by_Enable)
{
    if (by_Enable == 0)
    {
        Fan.PROTECT_EN = 0;
    }
    else
    {
        Fan.PROTECT_EN = 1;
    }
}

unsigned char Fan_Get_ProtectStat(void)
{
    if (Fan.PROTECTED)
        return 1;
    else
        return 0;
}


void Fan_1ms_Speed_Int(void)
{
    Fan.PROTECTED = 0; //wangnh
    if (Fan.PROTECTED)
    {
        Fan_Set_Level(0);
        if ((++by_3s) > 3030)
        {
            Fan_Set_Speed(by_Fan_Speed);
            Fan.PROTECTED = 0;
        }
    }
    else
    {
        Fan_Set_Level(by_Fan_Speed);
    }
}