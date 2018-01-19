#include "LCB.h"



void LCB_Initial(void)
{
  LCB_ERP_WakeUp();
    /*开机初始化 Daughter Board*/
  Digital_Command(CmdInitial,0);
  Digital_Command(CmdSetWorkStatus,0);
  Digital_Command(CmdSetWorkStatus,1);
  Digital_Command(CmdGetVersion,0);
  Digital_Command(CmdGetInclineLocation,0);
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
}