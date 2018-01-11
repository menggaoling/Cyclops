#include "main.h"

void Main_Data_Initial(void)
{
  Digital_Initial();
  
  CSAFE_Initial();
  CSAFE_Power_On();
  
  TV_Initial();

  RFID_GATRM310_Initial();
}
int main(void)
{
  Hal_System_Initial();
  Main_Data_Initial();
  CSAFE_EngTest();
  while(1)
  {
    for(unsigned int i = 0;i < 800000;i++);
    TV_SetCommand(TVCom_LAST,0);
//    if(CSAFE_TestStatus() == 1)
//    {
//      Digital_Command(CmdInitial,0);
//    }
  }

//  return 0;
}