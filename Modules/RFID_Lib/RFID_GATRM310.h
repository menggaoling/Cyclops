/**
  
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RFID_GATRM310_H
#define __RFID_GATRM310_H


/* Includes ------------------------------------------------------------------*/

#include "HAL_Interface.h"

#define _RFID_LoopTime          100
#define _RFID_RxTimeout         (1000/_RFID_LoopTime)     // 1000 /100 = 10
#define _RFID_SearchTXPTime     (200/_RFID_LoopTime)
#define _MaxUID  1



// Command
#define APPL_STATE              0x00  //Gets the current application state of the SM-4200 or changes the state page 21
    //SUB_CMD 
    #define GetState                0x00
    #define ResetHW                 0x01
    #define IdleState               0x02
    #define StopState               0x03
    #define WatchState              0x04
    #define SAM_handle              0x05
    #define SAM_accept              0x06
    #define SAM_decline             0x07
    #define SAM_acceptOPT           0x08
#define SEARCH_TXP              0x01  //Searches for TXPs in the RF field and returns the UID of found TXPs page 23
#define SEARCH_TXP_2            0x17
    //RF_STD 
    #define LEGIC                   0x00  //LEGIC RF standard
    #define ISO15693                0x01  //ISO 15693
    #define ISO14443A               0x02  //ISO 14443 A
    #define ISO14433B               0x03  //ISO 14443 B
    #define INSIDE                  0x04  //INSIDE Secure
    #define SONY_FeliCa             0x05  //SONY FeliCa subset
#define SELECT_TXP              0x02  //Selects and authenticates one of the TXPs found with SEARCH_TXP (authentication only for LEGIC TXPs) page 28
#define SELECT_TXP_2            0x18
#define SEARCH_SEGMENT          0x03  //Selects a segment on the LEGIC TXP authenticated with SELECT_TXP page 33
#define SEARCH_SEGMENT_2        0x19
#define SWITCH_FS               0x22  //Switches the file system on a LEGIC Cross Standard TXP CTC4096-... page 37
#define READ                    0x05  //Reads a number of data bytes from the selected segment page 38
#define WRITE                   0x04  //Writes a number of data bytes to the selected segment page 40
#define MAKE_CRC                0x06  //Calculates the CRC value across the specified data bytes and writes it to the selected segment page 42
#define CHECK_CRC               0x07  //Checks the CRC value across the specified data bytes in the selected segment page 44
#define READ_DCF                0x1E  //Reads the Decremental Field (DCF) of LEGIC prime MIM page 46
#define WRITE_DCF               0x1F  //Writes the Decremental Field (DCF) of LEGIC prime MIM page 47
#define AUTHENTICATE_TXP        0x20  //Authenticates a third-party TXP with a specified key page 49
#define MIFARE                  0x23  //Reads and writes data on MIFARE TXPs page 53
#define ENVELOPE                0x08  //Reads and writes data in the transparent mode to a third-party TXP page 56
#define ENVELOPE_2              0x21  
#define SET_IDB                 0x09  //Sets the IDB configuration parameters in the SM-4200 page 61
#define GET_IDB                 0x0A  //Gets the IDB configuration parameters in the SM-4200 page 62
#define SET_PORT                0x0B  //Sets the status of the user definable I/O ports page 63
#define GET_PORT                0x0C  //Gets the status of the user definable I/O ports page 64
#define SET_BUZZER              0x0D  //Sets the buzzer properties (GPIO02/BUZZ) page 65
#define SET_USER_KEY            0x1A  //Writes a key for user defined encryption or authentication of third-party TXPs to the key storage page 66
#define GET_USER_KEY            0x1B  //Reads the fingerprint of a key from the key storage page 69
#define SELECT_USER_KEY         0x1C
#define AUTH_A                  0xF0  //Authenticates the host interface for secured host communication (1st step) page 73
#define AUTH_B                  0xF1  //Authenticates the host interface for secured host communication (2nd step) page 74
#define ENCRYPTED               0xF2  //Sends authenticated and encrypted commands in secure mode page 76


// IDB
#define VERSIONS                0x00
#define VERSIONS_2              0x28



typedef enum{
  NoStateReturn = 0 ,
  Idle,
  RFon,
  SAMwait,
  SAMhandling,
  LEGIC_FS,
  SegmentSelected,
  Transparent
} ApplSTATE ;


typedef enum{
  OK = 0 ,
  GeneralError,
  WrongCommandLegth,
  WrongParameterValue,
  WeongAppState,
  NoTXPresponse,
  AuthorizationMissing,
  OutOfRange = 0x80
} ReponseSTAT ;


typedef const struct {
  unsigned char Command ;
  unsigned char Length ;
  unsigned char LengthExtend ;
  unsigned char MinOfResponseLength ;
  unsigned char MaxOfResponseLength ;
} SM4200M_Command ;


typedef  union {
  struct {
    unsigned short Lo:8 ;
    unsigned short Hi:8 ;
  } Byte ;
  unsigned short All ;
} SM4200_CrC16 ;





typedef struct {
  unsigned char Length ;
  unsigned char Command ;
  unsigned char State ;
  unsigned char *Data ;
  SM4200_CrC16  CRC16 ; 
} SM4200_Response ;


typedef union {
  struct {
    unsigned char TAG ;
    unsigned char LEN ;
    unsigned char Product ;
    unsigned char Hardware ;
    unsigned char AppVersion ;
    unsigned char Bootloader[2] ;
    unsigned char FirmwareVersion[4] ;
  } Member ;
  unsigned char All[11] ;
} SM4200_Tag00 ;


typedef enum{
    LEGIC_STD = 0 ,       //LEGIC RF standard
    ISO15693_STD ,        //ISO 15693
    ISO14443A_STD ,       //ISO 14443 A
    ISO14433B_STD ,       //ISO 14443 B
    INSIDE_STD ,          //INSIDE Secure
    SONY_FeliCa_STD       //SONY FeliCa subset
} RF_Standard ;

typedef union {
  struct {
    RF_Standard RF_STD;
    unsigned char UIDSize ;
    unsigned char UID[10] ;
  } Member ;
  unsigned char All[12] ;
} TXP_UID ;




void RFID_GATRM310_Initial(void);
unsigned char RFID_GATRM310_Process(void);
unsigned char RFID_TXE_Status(unsigned char by_D);
void RFID_RxBuffer(unsigned char c);
unsigned char RFID_TxBuffer(void);
char RFID_GATRM310_TxRxInterrupt(char by_D);
void RFID_GATRM310_Set_HWFlag(unsigned char by_Flag);
unsigned char RFID_GATRM310_GetOnline(void);
unsigned char RFID_GATRM310_GetCard(void);



void RFID_GATRM310_ClearUID(void);

// start v2.001-3
// unsigned char RFID_GATRM310_GetUID(unsigned char _Mode,unsigned char _Dat,unsigned char _Dat1); 
TXP_UID RFID_GATRM310_GetUID(void);

// end
//


extern TXP_UID   RFID_UID[_MaxUID];
extern SM4200_Tag00    RFID_Version;
#endif /*__RFID_GATRM310_H */
/******************* (C) COPYRIGHT 2013 Johnsonfitness  *****END OF FILE****/
