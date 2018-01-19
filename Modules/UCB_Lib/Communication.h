#ifndef __COMMUNICATION_H
#define __COMMUNICATION_H

#include "HAL_BSP.h"

#define COM_MAX_DATA_LENGTH     260


//----------------------------- Command List ---------------------------------//
#define CMD_SET_MACHINE_INFO				0x0201
#define CMD_SET_USER_INFO				0x0210
#define CMD_WORKOUT_START				0x0302
#define CMD_SET_SPEED    				0x0305
#define CMD_SET_INCLINE    				0x0306
#define CMD_SET_RESISTANCE    				0x0307
#define CMD_SET_KEY_VALUE    				0x0311
#define CMD_GET_WORKOUT_DATA  				0x0212
#define CMD_GET_ERROR_CODE  				0x0312
#define CMD_SET_WORKOUT_STATE  				0x0303
#define CMD_WORKOUT_STOP				0x0304
#define CMD_GET_RFID_STATE			        0x0216
#define CMD_GET_CSAFE_STATUS			        0x0217
#define CMD_GET_CSAFE_PROGRAM_SETTING		        0x0218
#define CMD_SET_FAN_LEVEL        		        0x0313
#define CMD_SET_HEARTRATE        		        0x0314
#define CMD_CLEAR_RFID            		        0x0315
#define CMD_GET_VERSION            		        0x0219
#define CMD_ENTER_UPDATE           		        0x0316
#define CMD_UPDATE_PACKAGE_DATA        		        0x0317
#define CMD_ENTER_SLEEP           		        0x0318
#define CMD_SET_FAN_RPM           		        0x0319
#define CMD_GET_FAN_RPM           		        0x0220



#define PROTECTED_SPEED_MAX   5000

//---------------- Command Type -----------------------//
#define WRITE_REGISTER			        0x01
#define READ_REGISTER			        0x02


//---------------- Device Type -----------------------//
#define DEVICE_TYPE_SMM					0x01
#define DEVICE_TYPE_MMM					0x02

//---------------- Control Type -----------------------//
#define CONTROL_TYPE_BIKE				0x00
#define CONTROL_TYPE_STEPPER			        0x01
#define CONTROL_TYPE_RAMP				0x02
#define CONTROL_TYPE_SWING				0x03
#define CONTROL_TYPE_TREADMILL			        0x04

//---------------- Incline or ECB Action -----------------------//
#define ACTION_STOP						0x00
#define ACTION_UP						0x01
#define ACTION_DOWN						0x02
#define ACTION_ZERO                                             0x03        

//---------------- Fan Action -----------------------//
#define FAN_STOP						0x00
#define FAN_LOW							0x01
#define FAN_MEDIUM						0x02
#define FAN_HIGH						0x03

//---------------- Beeper Action -----------------------//
#define BEEPER_OFF						0x00
#define BEEPER_ON						0x01

// ==============================================
// Status
// ==============================================
#define STATUS_BIT_ERROR				0x0001
#define STATUS_BIT_KEY					0x0002
#define STATUS_BIT_WDT_OVERFLOW			        0x0004
#define STATUS_BIT_SAFEKEY				0x0008
//#define MASK_CALIBRTDONE				0x0010
#define STATUS_BIT_ERP                                  0x0020
#define STATUS_BIT_PASSPORT_CONNECTED                   0x1000

// ==============================================
// Result
// ==============================================
#define RESULT_SUCCESS					0x0000
#define RESULT_INVALID_REGISTER			0x0001
#define RESULT_INVALID_DATA				0x0002
#define RESULT_INVALID_COMMAND			0x0003

// ==============================================
// Option function define
// ==============================================
#define MASK_ENBEEP			            0x0000
#define MASK_DISBEEP		            0x0001
//#define MASK_CALIBRATSET              0x0004


#define INCLINE_NO_MOVING		0x40
#define INCLINE_MOTOR_HITEMP	0x41
#define INCLINE_MOTOR_FALLOFF	0xa0
#define SPEED_SENSOR_FALLOFF	0xa0
#define SPEED_MOTO_OUTCNTL		0xaa
#define DIGITAL_COMM_NOACK		0xa0

//----------------Master device----------------------//

enum
{
    NO_SLAVE_ERROR,				//0
    INCLINE_ERROR,				//1
    LCB_CONTROLLER_ERROR,		//2
    UCB_CONTROLLER_ERROR,		//3
    DIGITAL_COMM_ERROR,			//4
    SPEED_ERROR,				//5
};

typedef struct tag_DATA_PACKAGE
{
    USHORT PacketHead;
    USHORT TransID;
    USHORT Command;
    USHORT Length;
    UCHAR  Data[COM_MAX_DATA_LENGTH]; //EARLIER WAS SET TO 100, BUT SHOULD BE 128
} DATA_PACKAGE;




typedef enum tagCONTROL_TYPE
{
    BIKE = 0,
    STEPPER,
    RAMP,
    SWING,
    TREADMILL,
    CLIMBMIL,
} CONTROL_TPYE;


typedef enum tagMCB_ECB_TYPE
{
    NO_MCB                          = 0x00,
    AC_LCB_JIS                      = 0x01,
    LCB1_JHT                        = 0x02,
    LCB3_JHT                        = 0x03,
    DC_LCB_JIS                      = 0x04,
    LCB_CD_COSTDN_JHT               = 0x05,
    TM_LCB_PANATTA                  = 0x06,
    LCB1_PWN_JHT                    = 0x07,
    LCB3_CURRENT_JHT                = 0x08,
    LCB1_PANATTA                    = 0x09,
    LCB2_A5X_REDESIGN               = 0x0A,
    EP_0B_JIS                       = 0x0B,     
    BIKE_0C_JIS                     = 0x0C,
    DC_LCB_20_275HP_JIS             = 0x0D,
    DC_LCB_30_325HP_JIS             = 0x0E,
    AC_LCB_20_275HP_JIS             = 0x0F,
    AC_LCB_30_325HP_JIS             = 0x10,
    AD_BOARD_JIS                    = 0x11,
    DIGITAL_ECB                     = 0x12,
    LCBA                            = 0x13,
    BIKE_LCB_1X                     = 0x14,
    INDUCTOR_BRAKE_INCLINE          = 0x15,
    GLOBAL_CLIMBMILL_EN             = 0x17,
    GLOBAL_CLIMBMILL_RETAIL         = 0x19,
    GLOBAL_15_20HP_110              = 0xB0,
    GLOBAL_15_20HP_220              = 0xB1,
    GLOBAL_20_25HP_110              = 0xB2,
    GLOBAL_20_25HP_220              = 0xB3,
    ECB_JIS                         = 0xB4,
    ECB_INDUCTOR_BRAKE              = 0xB5,
    GLOBAL_LCB_ECB_INCLINE          = 0xB6,
    GLOBAL_LCB_INDUCTION            = 0xB7,
    GLOBAL_LCB_INDUCTION_INCLINE    = 0xB8,
    GLOBAL_LCB_FULL                 = 0xB9,
    DCI_DAUGHTER_BOARD              = 0xC0,
    MCB_TOPTEK                      = 0xC1,
    MCB_DELTA                       = 0xC2,
    MCB_DELTA_COMMOCIAL             = 0xC3,
    GLOBAL_20_325HP_110             = 0xD0,
    GLOBAL_20_325HP_220             = 0xD1,
} MCB_ECB_TYPE;


typedef enum tagCOMM_RESULT
{
    CR_SUCCESS = 0,
    CR_ERROR,
    CR_ERROR_IOCTRL,
    CR_ERROR_CHECKSUM,
    CR_ERROR_DATA,
} COMM_RESULT;


typedef struct _T_STATUS
{
    unsigned bKey : 1;
    unsigned bSafeKey : 1;
    unsigned ErrorTimes : 3;
    unsigned bError : 1;
    unsigned bRec : 1;
} T_STATUS, *PT_STATUS;


typedef struct _T_COMSTATUS
{
    unsigned bRec : 1;
    unsigned bRecWrite : 1;
    unsigned bRecRead : 1;
    unsigned bRecError : 1;
    unsigned bSendRead : 1;
    unsigned bSendWrite : 1;
    volatile unsigned bSendReady : 1;
    unsigned bInting  : 1;
} T_COMSTATUS, *PT_COMSTATUS;



void Com_Initial(void);
USHORT Com_Send(USHORT Command, USHORT Length, UCHAR *pData);
UCHAR Com_GetCommand(DATA_PACKAGE *stData);
void Com_10msInt(void);
void Com_Tx_Int(void);
void Com_Rx_Int(void);
void Com_RxProcess(USHORT PacketSize);



#endif //__COMMUNICATION_H  