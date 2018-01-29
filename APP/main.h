#ifndef __MAIN_H
#define __MAIN_H

#include "HAL_Interface.h"
#include "Digital.h"
#include "C_safe.h"
#include "TV.h"
#include "RFID_GATRM310.h"
#include "Communication.h"
#include "Treadmill.h"
#include "LCB.h"
#include "Fan.h"


#define WORKOUT_RESET   0xA0
#define WORKOUT_IDLE    0xB0
#define WORKOUT_RUNNING 0xC0


//Status mask
#define ERROR_CODE_MASK          0x0001
#define CSFAE_STATUS_MASK        0x0002
#define RFID_STATUS_MASK         0x0004
#define CSAFE_RECV_SETTINGS_MASK 0x0008
#define RFID_UID_MASK            0x0010
#define SAFETY_KEY_MASK          0x0020


#define K_NONE      0x00
#define K_NULL      0xff
#define K_MASK      0x0200//0x80
#define K_ERROR     0xfe

enum
{
// UP keypad (Main key)  
  K_GO = 1,
  K_STOP,   
  K_PAUSE,  
  K_InclineU,
  K_InclineD,   
  K_SpeedU,
  K_SpeedD,  
  K_BACK,
  K_ENTER,
  K_MANUAL,
  K_Training,
  K_TargetHR,
  K_FatBurn,
  K_Rolling,
  K_FitnessTest,
  K_0,
  K_1,
  K_2,
  K_3,
  K_4,
  K_5,
  K_6,
  K_7,
  K_8,
  K_9,
  K_T1,
  K_T2,
  K_T3,
  K_T4,
  K_TimeU,
  K_TimeD,
  K_TV_POWER,
  K_TV_VU,
  K_TV_VD,
  K_TV_CU,
  K_TV_CD,
  K_TV_LAST,
  K_Mute,
  K_CC,
  K_CoolDown,
    // End
  LowerOverlayEnd,
// Quick keypad
  K_QuickSpeedU = LowerOverlayEnd,
  K_QuickSpeedD,
  K_QuickInclineU,
  K_QuickInclineD,
  K_QuickGO,
  K_QuickSTOP,
  // End
  QuickKeyEnd,  
// Other 
  K_IDSD = QuickKeyEnd, //==>Incline Down & Speed Down
  K_ClearNumber     
};


#endif //__MAIN_H  
