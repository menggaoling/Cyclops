#include "keyboard.h"
#include "Key_IO.h"

#define C_MAX_TIME          20000
#define C_TRUE_TIME         2//5 diaolong
#define C_HOLD_TIME_S       28//0.8s
#define C_HOLD_TIME_M       150//3s
#define C_HOLD_TIME_L       250//5s
#define C_HOLD_TIME_XL      500//10s
#define C_HOLD_TIME_XLL      1500//30s

#define C_REPLAY_TIME_S     24
#define C_REPLAY_TIME_M     12
#define C_REPLAY_TIME_FAST  3

#define C_DOUBLE_DOWN       0xAA

__no_init static USHORT w_KB_Time,w_KB_Time_Stop;
__no_init static UCHAR by_Double_Down;
__no_init static UCHAR by_Key_Backup;
UCHAR speedBT_more = 0;

CHAR KEY_Get_Buffer(CHAR by_Address);
void KB_Flag_20ms_Int(void);
void HW_Keyboard_ScanOut(UCHAR by_Index);
UCHAR HW_Keyboard_GetPort(void);
CHAR KB_Encoder(void);

#define  C_MEM_SIZE  6       

__no_init static CHAR aby_KEY_Buffer[C_MEM_SIZE];

struct {
	unsigned KB_Flag : 1;
	unsigned KB_MUSIC_OK : 1;
	unsigned KB_MYKEY_OK : 1;
	unsigned KB_First : 1;
}b;
void KB_Initial(void)
{
	w_KB_Time = 0;
	w_KB_Time_Stop=0;
	b.KB_MUSIC_OK = 0;
	b.KB_MYKEY_OK = 0;
	by_Double_Down = 0;
	by_Key_Backup=0;
}

void KB_Flag_20ms_Int(void)
{
	b.KB_Flag = 1;
}

#define C_KEY_NUM   6
static CHAR Key_Buffer[C_KEY_NUM];
//static CHAR test_value = 0;
void KB_Key_Process(void)
{
  CHAR by_Dat ,Exist_Flag = 0;  
  by_Dat = KB_Encoder();
  if(by_Dat != KB_NA && by_Dat != KB_NONE)
  {
    for(UCHAR i = 0;i < C_KEY_NUM; i++)
    {
      if(Key_Buffer[i] == by_Dat  )
      {
        Exist_Flag = 1;
        break;
      }
    }
    if(Exist_Flag == 0)
    {
      for(UCHAR i = 0;i < C_KEY_NUM; i++)
      {
        if(Key_Buffer[i] == 0 )
        {
          Key_Buffer[i] = by_Dat;
          break;
        }
      }
    }
  }
//  if(test_value == 5)
//    KB_Get_Code();
  
}

CHAR KB_Get_Code(void)
{
  CHAR by_Out;
  for(UCHAR i = 0;i < C_KEY_NUM; i++)
    {
      if(Key_Buffer[i] != 0)
      {
         by_Out = Key_Buffer[i];
         Key_Buffer[i] = 0;
//         test_value = 0;
         break;
      }
    }
  return by_Out;
}
//{
//	static CHAR by_Replay_Time = 0;
//	CHAR by_Dat = KB_NA;
//	CHAR by_Out = KB_NA;
//	CHAR by_replay = 0;
//	if(!b.KB_Flag)
//		return(by_Out);
//	b.KB_Flag = 0;
//	by_Dat = KB_Encoder();
//	//*******************************************************************
//	if(by_Dat == KB_NONE)//---0---
//	{
//		if(by_Double_Down != C_DOUBLE_DOWN)w_KB_Time = 0;
//		w_KB_Time_Stop = 0;
//		by_Replay_Time = 0;
//		if(by_Key_Backup == KB_StartOrStop )
//		{
//			if(b.KB_First) speedBT_more = 2;
//			by_Dat=by_Key_Backup;
//			by_Key_Backup=KB_NONE;        
//			b.KB_First = 0;
//		}
//		if(speedBT_more) 
//		{
//			//      by_Dat = by_Key_Backup;
//			speedBT_more--;
//		}
//		else
//		{
//			by_Key_Backup = KB_NONE;
//		}
//		return(by_Dat);
//	}
//	//*******************************************************************
//	if(w_KB_Time < C_MAX_TIME)
//	{
//		++w_KB_Time;
//	}
////	if((w_KB_Time_Stop < C_MAX_TIME) && (by_Dat==KB_StartOrStop) && !by_Mode)
////	{
////		++w_KB_Time_Stop;
////	}
//	//*******************************************************************
//	if(w_KB_Time_Stop == C_TRUE_TIME)//---0.1S---
//	{
//		by_Out = by_Dat;
//		return(by_Out);//---------------KB_STOP--------------
//	}
//	
//	if(w_KB_Time == C_TRUE_TIME)//---0.1S---
//	{
//                {
//                  if(by_Dat == KB_StartOrStop  && !b.KB_First)
//                  {      
//                          by_Key_Backup=by_Dat;
//                          by_Dat=KB_NONE;
//                          b.KB_First=1;
//                  }
//                }
//		by_Out = by_Dat;
//	}
//	//*******************************************************************
//	if(w_KB_Time > C_HOLD_TIME_S)//---0.8S---
//	{
//		if(by_Dat == KB_Up|| by_Dat == KB_Down )
//		{
//			by_replay=C_REPLAY_TIME_M;
//			
//			if(w_KB_Time>=C_HOLD_TIME_S*4)
//				by_replay=C_REPLAY_TIME_FAST;
//			
//			if(by_Replay_Time >= by_replay)
//			{
//				by_Out = by_Dat;
//				by_Replay_Time = 0;
//			}
//			else
//				++by_Replay_Time;
//		}
//	}
//	//*******************************************************************
//	if(w_KB_Time == C_HOLD_TIME_M)//---3S---
//	{
//		if(by_Dat == KB_StartOrStop )
//		{
//			by_Out = by_Dat + KB_HOLD_3S;
//			by_Key_Backup=KB_NONE;
//                        b.KB_First = 0;
//		}
//		if( by_Dat == KB_Up_Down)
//		{
//			by_Out = by_Dat + KB_HOLD_3S;
//		}
//		if( by_Dat == KB_KB_WattsOrProfile_UP)
//		{
//			by_Out = by_Dat + KB_HOLD_3S;
//		}
//	}
//	//*******************************************************************
//	if(w_KB_Time == C_HOLD_TIME_L)//---5S---
//	{
//          if(by_Dat == KB_SPEED_UP )
//		{
//			by_Out = KB_SPEED_UP + KB_HOLD_5S;
//		}
////		if(by_Dat == KB_StartOrStop && by_Mode == 1)
////		{
////			by_Out = by_Dat + KB_HOLD_5S;
////		}
//	}
//	return(by_Out);
//}

CHAR KB_Encoder(void)//
{
	CHAR by_Code = KB_NONE;
	CHAR by_Out=KB_NONE;
	CHAR by_Count=0;
	
	if(KEY_Get_Buffer(0) & BIT0) { by_Code = KB_Down;             ++by_Count; }
	if(KEY_Get_Buffer(0) & BIT1) { by_Code = KB_Up;               ++by_Count; }
	if(KEY_Get_Buffer(0) & BIT2) { by_Code = KB_StartOrStop;      ++by_Count; }
	
	
	if(KEY_Get_Buffer(1) & BIT0) { by_Code = KB_SPEED_UP;         ++by_Count; }
	if(KEY_Get_Buffer(1) & BIT1) { by_Code = KB_SPEED_DOWN;       ++by_Count; }
	if(KEY_Get_Buffer(1) & BIT2) { by_Code = KB_NONE;             ++by_Count; }
	
	
	if(KEY_Get_Buffer(2) & BIT0) { by_Code = KB_NONE;             ++by_Count; }
	if(KEY_Get_Buffer(2) & BIT1) { by_Code = KB_NONE;             ++by_Count; }
	if(KEY_Get_Buffer(2) & BIT2) { by_Code = KB_NONE;             ++by_Count; }
	
	
	if(KEY_Get_Buffer(3) & BIT0) { by_Code = KB_NONE;             ++by_Count; }
	if(KEY_Get_Buffer(3) & BIT1) { by_Code = KB_NONE;             ++by_Count; }
	if(KEY_Get_Buffer(3) & BIT2) { by_Code = KB_NONE;             ++by_Count; }
        
	if(KEY_Get_Buffer(4) & BIT0) { by_Code = KB_NONE;             ++by_Count; }
	if(KEY_Get_Buffer(4) & BIT1) { by_Code = KB_NONE;             ++by_Count; }
	if(KEY_Get_Buffer(4) & BIT2) { by_Code = KB_NONE;             ++by_Count; }        
        
	if(KEY_Get_Buffer(5) & BIT0) { by_Code = KB_NONE;             ++by_Count; }
	if(KEY_Get_Buffer(5) & BIT1) { by_Code = KB_NONE;             ++by_Count; }
	if(KEY_Get_Buffer(5) & BIT2) { by_Code = KB_NONE;             ++by_Count; }        
	
	
	if(by_Count==1)
	{
          by_Out = by_Code;
	}
	
	if((KEY_Get_Buffer(0) & BIT2) && (KEY_Get_Buffer(1) & BIT0) && (by_Count == 2))//stop  & incline up
	{  
          by_Out = KB_KB_WattsOrProfile_UP;
        } 
	if((KEY_Get_Buffer(0) & BIT1) && (KEY_Get_Buffer(0) & BIT2) && (by_Count == 2))//stop  & incline up
	{  
          by_Out = KB_Up_Down ;
        }  
	
	return(by_Out);
}



CHAR KEY_Get_Buffer(CHAR by_Address)
{
  return(aby_KEY_Buffer[by_Address]);
}

void KEY_Scan_Int(void)
{
    static CHAR by_Index = 0,by_Times2 = 0;
    aby_KEY_Buffer[by_Index++] = HW_Keyboard_GetPort();
    if(by_Index >= C_MEM_SIZE)
      by_Index = 0;
    HW_Keyboard_ScanOut(by_Index);
    by_Times2++;
    if(by_Times2 >= 20)
    {
      by_Times2 = 0;
      KB_Flag_20ms_Int();
    }
}

void HW_Keyboard_ScanOut(UCHAR by_Index)
{
	switch(by_Index)
	{
		case 0:
		HAL_Set_IO_Low(KEYBOARD_SCAN5_PORT, KEYBOARD_SCAN5_PIN);//设置为输入
		HAL_Set_IO_High(KEYBOARD_SCAN0_PORT, KEYBOARD_SCAN0_PIN);//置输出口为高
		break;
		
		case 1:		
		HAL_Set_IO_Low(KEYBOARD_SCAN0_PORT, KEYBOARD_SCAN0_PIN);
		HAL_Set_IO_High(KEYBOARD_SCAN1_PORT, KEYBOARD_SCAN1_PIN);      
		break;
		
		case 2:
		HAL_Set_IO_Low(KEYBOARD_SCAN1_PORT, KEYBOARD_SCAN1_PIN);
		HAL_Set_IO_High(KEYBOARD_SCAN2_PORT, KEYBOARD_SCAN2_PIN);
		break;
		
		case 3:
		HAL_Set_IO_Low(KEYBOARD_SCAN2_PORT, KEYBOARD_SCAN2_PIN);
		HAL_Set_IO_High(KEYBOARD_SCAN3_PORT, KEYBOARD_SCAN3_PIN);
		break;
                
                case 4:
		HAL_Set_IO_Low(KEYBOARD_SCAN3_PORT, KEYBOARD_SCAN3_PIN);
		HAL_Set_IO_High(KEYBOARD_SCAN4_PORT, KEYBOARD_SCAN4_PIN);
		break;
                
                case 5:
		HAL_Set_IO_Low(KEYBOARD_SCAN4_PORT, KEYBOARD_SCAN4_PIN);
		HAL_Set_IO_High(KEYBOARD_SCAN5_PORT, KEYBOARD_SCAN5_PIN);
		break;                
                
		default:break;
	}
}

UCHAR HW_Keyboard_GetPort(void)
{	
	UINT by_Out=0;
	by_Out |= (HAL_Get_IO(KEYBOARD_KeyIn0_PORT, KEYBOARD_KeyIn0_PIN)<<0);
	by_Out |= (HAL_Get_IO(KEYBOARD_KeyIn1_PORT, KEYBOARD_KeyIn1_PIN)<<1);
	by_Out |= (HAL_Get_IO(KEYBOARD_KeyIn2_PORT, KEYBOARD_KeyIn2_PIN)<<2);
	return (by_Out);
}