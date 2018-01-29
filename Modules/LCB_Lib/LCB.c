#include "LCB.h"



void LCB_Initial(void)
{
  LCB_ERP_WakeUp();
  LCB_Get_Type();
}
void LCB_ERP_WakeUp(void)
{
  UCHAR by_Enter = 0;
  UCHAR by_HoldBuffer = 0;
  
  Digital_CommandStart(1);// 停止自动下命令
  while(by_Enter == 0)
  {

      Hal_Set_LCB_Serial_Dir(TXD);
      HW_Set_IO_OutputLow(DIGITAL_TX_PORT,DIGITAL_TX_PIN);
      Timer_Counter_Clear(0);
      while(!Timer_Counter(T_STEP,0,5))
      {
      }
      HW_Set_IO_AF_PP(DIGITAL_TX_PORT,DIGITAL_TX_PIN);
      Digital_Initial();
      Digital_CommandStart(1);// 停止自动下命令
      Digital_Command(CmdEUPsMode,0x00);
      Timer_Counter_Clear(0);
      while(!Timer_Counter(T_STEP,0,5))
      {
          if(LCB_Get_EUPsState() != 0 || by_HoldBuffer >= 3)
          {// LCB响应唤醒或尝试3次失败
              by_Enter = 1;
              break;
          }
      }
      by_HoldBuffer++;
  }
  Digital_CommandBufferReset();//solve after power on error status has been clear but by_GetDkip not clear 
  Digital_CommandStart(0);// 允许自动下命令
  
  /*开机初始化 Daughter Board*/
  Digital_Command(CmdInitial,0);
  Digital_Command(CmdSetWorkStatus,0);
  Digital_Command(CmdSetWorkStatus,1);
  Digital_Command(CmdGetVersion,0);
  Digital_Command(CmdGetInclineLocation,0);
}

void LCB_Get_Type(void)
{
  Timer_Counter_Clear(0); 
  u8 by_QQQ=0;
  while(by_QQQ < 10)
  {//==>确认下控型号
    if(Timer_Counter(T_STEP,0,20))
    {
      Digital_Command(CmdGetVersion,0);  
      Timer_Counter_Clear(0); 
      by_QQQ++;
    }
    if(by_QQQ > 2)
    {//==>确认几次后还是无法确认就设为默认值
      Digital_SetMachineType(LCB_DELTA_C3);
      by_QQQ=100;  
    }
    else if(Digital_PowerON_MachineTypeCheck()==1)
    {
      by_QQQ=100;  
    }
  }
}