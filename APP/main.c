#include "main.h"

void Main_Data_Initial(void)
{
  
  LCB_Initial();
  
  CSAFE_Initial();
  CSAFE_Power_On();
  
  TV_Initial();

  RFID_GATRM310_Initial();
  
  Com_Initial();
  
  Timer_Initial();
}

void Main_UCB_CMD_Process(void)
{
  DATA_PACKAGE PackageData;
  if(Com_GetCommand(&PackageData))
  {
    switch(PackageData.Command)
    {
    case CMD_SET_MACHINE_INFO:
      break;
    case CMD_SET_USER_INFO:
      break;
    case CMD_WORKOUT_START:
      break;
    case CMD_SET_SPEED:
      break;
    case CMD_SET_INCLINE:
      break;
    case CMD_SET_RESISTANCE:
      break;
    case CMD_SET_KEY_VALUE:
      break;
    case CMD_GET_WORKOUT_DATA:
      break;
    case CMD_GET_ERROR_CODE:
      break;
    case CMD_SET_WORKOUT_STATE:
      break;
    case CMD_WORKOUT_STOP:
      break;
    case CMD_GET_RFID_STATE:
      break;
    case CMD_GET_CSAFE_STATUS:
      break;
    case CMD_GET_CSAFE_PROGRAM_SETTING:
      break;
    case CMD_SET_FAN_LEVEL:
      break;
    case CMD_SET_HEARTRATE:
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
      break;
    default:break;
    }
  }
}
int main(void)
{
  Hal_System_Initial();
  Main_Data_Initial();
  CSAFE_EngTest();
  while(1)
  {
    for(unsigned int i = 0;i < 800000;i++);
    Com_Send(CMD_ENTER_SLEEP, 0, NULL);
    Main_UCB_CMD_Process();
//    RFID_GATRM310_ReadDCF();
//    TV_SetCommand(TVCom_LAST,0);
//    if(CSAFE_TestStatus() == 1)
//    {
//      Digital_Command(CmdInitial,0);
//    }
  }

//  return 0;
}