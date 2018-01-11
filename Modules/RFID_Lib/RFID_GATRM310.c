/**
  ******************************************************************************
  * @file    RFID_GATRM310.c
  * @author  KunLung Lai
  * @version V1.0.0
  * @date    09/17/2014
  * @brief   CAB protocol
  ******************************************************************************
*/ 

//------------------------------------------------------------------------------
#include  "RFID_GATRM310.h"


//
//#define   RFID_GATRM310_PORT                     USART1
//#define   RFID_GATRM310_IRQ                      USART1_IRQn
//#define   RFID_GATRM310_TxRxInterrupt  		 USART1_IRQHandler



//------------------------------------------------------------------
// Functions
//------------------------------------------------------------------
unsigned char RFIDSendData[5] ;
//unsigned char RFIDReadData[255] ;
unsigned char bu_RFUID[10];
RF_Standard bu_RFSTD ;// v4.1-1

unsigned char _TagType=0;

SM4200_Response AnswerData ;
SM4200_Tag00    RFID_Version ;
ApplSTATE       RFID_STATE ;
TXP_UID         RFID_UID[_MaxUID] ;

unsigned short  RFID_Timeout ;
unsigned short  RFID_ProcessTime ;
unsigned char   RFID_ControlState ;
volatile union {
  struct {
    unsigned char RFID_HW_InitialOK:1 ;
    unsigned char RFID_Online:1 ;
    unsigned char CheckRxTimeout:1 ;
    unsigned char SearchTxpStart:1 ;
    unsigned char RxOK:1 ;    
    unsigned char Version:1 ;
	unsigned char TX_Switch:1;
  } Bit ;
  unsigned char All ;
} RFID_Flag ;


typedef volatile union {
  struct {
    unsigned char Find:1 ;
    unsigned char Count:7 ;
  } Bits ;
  unsigned char All ;
} TXP_Staut;

TXP_Staut RFID_UIDStatus ;



unsigned char SearchTXPIndex = 0 ;
unsigned char SearchTXPLoopTime = 0 ;
unsigned char SearchCounter = 0 ;

#define RFID_GATRM310_RxSize            256
unsigned char RFID_GATRM310_RxBuffer[RFID_GATRM310_RxSize] ;
unsigned char *RFID_GATRM310_RxStartPoint; 
unsigned char *RFID_GATRM310_RxEndPoint;

#define RFID_GATRM310_TxSize            256
unsigned char RFID_GATRM310_TxBuffer[RFID_GATRM310_TxSize] ;
unsigned char *RFID_GATRM310_TxStartPoint; 
unsigned char *RFID_GATRM310_TxEndPoint;

void RFID_GATRM310_RxFunc_Init(void) ;
unsigned char* RFID_GATRM310_RxFunc_NextPt(unsigned char *pt) ;
unsigned char RFID_GATRM310_RxFunc_IsEmpty(void) ;
unsigned char RFID_GATRM310_RxFunc_IsFull(void) ;
unsigned short RFID_GATRM310_RxFunc_Length(void) ;
unsigned char RFID_GATRM310_RxFunc_Getc(void) ;

void RFID_GATRM310_TxFunc_Init(void) ;
unsigned char* RFID_GATRM310_TxFunc_NextPt(unsigned char *pt) ;
unsigned char RFID_GATRM310_TxFunc_IsEmpty(void) ;
unsigned char RFID_GATRM310_TxFunc_IsFull(void) ;
unsigned short RFID_GATRM310_TxFunc_Length(void) ;
void RFID_GATRM310_TxFunc_Putc(unsigned char c) ;



SM4200M_Command* RFID_GATRM310_GetCommandDefine(unsigned char Command);
unsigned short RFID_GATRM310_CRC16(unsigned char *data, unsigned char TransMode );

unsigned char RFID_GATRM310_GetAPPL_State(unsigned char SubCmd);
unsigned char RFID_GATRM310_GetIDB(unsigned char Tag);
unsigned char RFID_GATRM310_ReadDCF(void);
unsigned char RFID_GATRM310_SEARCH_TXP(unsigned char Tag);
unsigned char RFID_GATRM310_SetCommand( unsigned char Command, unsigned char Length, unsigned char *DataPtr );


#define _NumberOfCommand              29
SM4200M_Command SM4200M_CommandList[_NumberOfCommand] = 
{
  {APPL_STATE,        0x04,0x00,0x05,0x05},        //0
  {SEARCH_TXP,        0x04,0x00,0x05,0x5D},        //1
  {SEARCH_TXP_2,      0x04,0x00,0x04,0xCC},        //2
  {SELECT_TXP,        0x07,0x00,0x05,0x05},        //3
  {SELECT_TXP_2,      0x07,0x00,0x07,0x07},        //4
  {SEARCH_SEGMENT,    0x06,0x12,0x0B,0x17},        //5   x 06~12
  {SEARCH_SEGMENT_2,  0x06,0x12,0x0D,0x19},        //6   x 06~12
  {SWITCH_FS,         0x04,0x00,0x04,0x04},        //7
  {READ,              0x06,0x00,0x06,0xCD},        //8
  {WRITE,             0x07,0xCE,0x04,0x04},        //9   x 07~CE
  {MAKE_CRC,          0x09,0x00,0x04,0x04},        //10
  {CHECK_CRC,         0x09,0x00,0x04,0x04},        //11
  {READ_DCF,          0x03,0x00,0x06,0x06},        //12
  {WRITE_DCF,         0x05,0x00,0x04,0x04},        //13
  {AUTHENTICATE_TXP,  0x05,0xCB,0x04,0x04},        //14  x 05~CB
  {MIFARE,            0x07,0x19,0x05,0x15},        //15  x 07~19
  {ENVELOPE,          0x05,0xCC,0x05,0xCD},        //16  x 05~CC
  {ENVELOPE_2,        0x06,0xCD,0x05,0xCD},        //17  x 06~CD
  {SET_IDB,           0x05,0x25,0x04,0x04},        //18  x 05~25
  {GET_IDB,           0x04,0x00,0x08,0x26},        //19  
  {SET_PORT,          0x05,0x00,0x04,0x04},        //20
  {GET_PORT,          0x03,0x00,0x07,0x07},        //21
  {SET_BUZZER,        0x07,0x00,0x04,0x04},        //22
  {SET_USER_KEY,      0x26,0x00,0x04,0x04},        //23
  {GET_USER_KEY,      0x04,0x00,0x0A,0x0A},        //24
  {SELECT_USER_KEY,   0x04,0x00,0x04,0x04},        //25
  {AUTH_A,            0x03,0x00,0x14,0x14},        //26
  {AUTH_B,            0x23,0x00,0x74,0x74},        //27
  {ENCRYPTED,         0x14,0xF4,0x05,0xF5}         //28  x 14~F4
} ;

unsigned char ApplStateSubCommand ;

//------------------------------------------------------------------------------
void RFID_GATRM310_Initial(void)
{
  //
  RFID_GATRM310_RxFunc_Init() ;
  RFID_GATRM310_TxFunc_Init() ;
  RFID_GATRM310_ClearUID() ;
  RFID_Timeout = 0 ;
  RFID_ProcessTime = 0;
  RFID_ControlState = 0;
  RFID_Flag.Bit.RFID_Online = 0 ;
  RFID_Flag.Bit.CheckRxTimeout = 0 ;
  RFID_Flag.Bit.SearchTxpStart = 0 ;
  RFID_Flag.Bit.RxOK = 1 ;
  RFID_Flag.Bit.Version = 0 ;
  SearchTXPLoopTime = 0;
  SearchCounter = 0 ;// for test
  RFID_UIDStatus.All = 0 ;

  //uart has been setup OK
  RFID_GATRM310_Set_HWFlag(1);
  //
  return;
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
char RFID_GATRM310_TxRxInterrupt(char by_D)
{
	if(by_D == 1)// TX
  	{
    	if(RFID_GATRM310_TxFunc_IsEmpty() != 0)
	  		return 1;	
  	}
 	return 0;
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void RFID_GATRM310_ClearUID(void)
{
  unsigned char i;
  
  // start v4.1-1
  for( i = 0 ; i < 12 ; i++ )
  {
      RFID_UID[0].All[i] = 0 ;
  }
  // end v4.1-1
  RFID_UIDStatus.Bits.Count = 0 ;
  RFID_UIDStatus.Bits.Find = 0;
  _TagType=0;
}


const unsigned char RFID_SearchSquenceTable[6] = {
  LEGIC,
  ISO15693,
  ISO14443A,
  ISO14433B,
  INSIDE,
  SONY_FeliCa
} ;
/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
unsigned char RFID_GATRM310_Process(void)
{
  SM4200_CrC16 rCRC,Check_CRC;
  SM4200M_Command *CmdPtr;
  unsigned char DataLength;
  unsigned char i,j;
  unsigned char TXP_Number;
  static unsigned char WaitPollingTime;
  unsigned char *ptr;
  //
  if(RFID_Flag.Bit.RFID_HW_InitialOK == 0)
      return 0;
  //
  RFID_ProcessTime += 1 ;
  if( RFID_ProcessTime >= _RFID_LoopTime )
      {
      RFID_ProcessTime = 0 ;
      
      switch( RFID_ControlState )
          {
          case  7 :
                  if(RFID_GATRM310_GetIDB(VERSIONS) )
                      RFID_ControlState = 0 ;
                  break ;
          case  1 :                  
                  if( RFID_STATE != Idle )
                      {
                      if(RFID_GATRM310_GetAPPL_State(ResetHW) )
                          {
                          SearchTXPIndex = 0 ;
                          RFID_ControlState = 5 ;
                          }
                      }
                  else
                      RFID_ControlState = 2 ;
                  break ;
          case  5 :
                  if( RFID_GATRM310_GetAPPL_State(IdleState) )
                      {
                      RFID_ControlState = 0 ;
                      }
                  break ;
          case  2 :
                  if( RFID_GATRM310_GetAPPL_State(GetState) )
                      {
                      RFID_ControlState = 0 ;
                      WaitPollingTime = 1 ; // 
                      }
                  break ;
          case  3 :
                  //
                  if( SearchTXPIndex >= 6 )
                      {
                      RFID_ControlState = 0 ;
                      RFID_Flag.Bit.SearchTxpStart = 0 ;
                      if( RFID_UIDStatus.Bits.Count != 0 )
                          RFID_UIDStatus.Bits.Find = 1 ;   
                      
                      }
                  else
                      {
                      if( RFID_GATRM310_SEARCH_TXP(RFID_SearchSquenceTable[SearchTXPIndex]) )
                          {
                          SearchTXPIndex += 1 ;
                          RFID_ControlState = 0 ;
                          }
                      }
                  //
                  break ;     
          case  4 :
                  if( WaitPollingTime == 0 )
                      RFID_ControlState = 3 ;   
                  else
                      WaitPollingTime -= 1 ;
                  break ;
          default :
                  if( RFID_GATRM310_RxFunc_IsEmpty() == 0 )// Not empty
                      {
                      AnswerData.Length = *RFID_GATRM310_RxStartPoint;
                      if(RFID_GATRM310_RxFunc_Length() > AnswerData.Length)
                          {
                          // Move data pointer to CRC
                          ptr = RFID_GATRM310_RxStartPoint;
                          for( i = 0; i < (AnswerData.Length-1); i++ )
                              ptr = RFID_GATRM310_RxFunc_NextPt(ptr);
                          // Check Crc
                          Check_CRC.All = RFID_GATRM310_CRC16(RFID_GATRM310_RxStartPoint,0);
                          rCRC.Byte.Hi = *ptr;
                          ptr = RFID_GATRM310_RxFunc_NextPt(ptr) ; // Go to Next Data point
                          rCRC.Byte.Lo = *ptr;
                          if( Check_CRC.All == rCRC.All )
                              {
                              RFID_Flag.Bit.CheckRxTimeout = 0 ;
                              RFID_Flag.Bit.RxOK = 1 ;
                              AnswerData.Length = RFID_GATRM310_RxFunc_Getc() ;
                              AnswerData.Command = RFID_GATRM310_RxFunc_Getc() ;
                              AnswerData.State = RFID_GATRM310_RxFunc_Getc() ;
                              CmdPtr = RFID_GATRM310_GetCommandDefine(AnswerData.Command) ;
                              if( CmdPtr != 0 )
                                  {
                                  if( AnswerData.Length >= CmdPtr->MinOfResponseLength && AnswerData.Length <= CmdPtr->MaxOfResponseLength )
                                      {
                                      // Save Pointer
                                      AnswerData.Data = RFID_GATRM310_RxStartPoint ;
                                      DataLength = AnswerData.Length - 4 ;
                                      AnswerData.Length = DataLength ;
                                      // Skip Data
                                      while(DataLength != 0)
                                          {
                                          RFID_GATRM310_RxFunc_Getc() ;
                                          DataLength -= 1 ;
                                          }
                                      // Skip CRC
                                      AnswerData.CRC16.Byte.Hi = RFID_GATRM310_RxFunc_Getc() ;
                                      AnswerData.CRC16.Byte.Lo = RFID_GATRM310_RxFunc_Getc() ;
                                      // Decoder Command
                                      if( AnswerData.State == 0 )
                                          {
                                          RFID_Flag.Bit.RFID_Online = 1 ;
                                          switch( AnswerData.Command )
                                              {
                                              case  GET_IDB           :
                                                                      if((*(AnswerData.Data) == VERSIONS ))
                                                                          {
                                                                          for( i =  0 ;  i < AnswerData.Length ; i++ )
                                                                              RFID_Version.All[i] = *(AnswerData.Data+i) ;

                                                                          RFID_Flag.Bit.Version = 1 ;
                                                                          }
                                                                      break ;
                                              case  APPL_STATE        :
                                                                      RFID_STATE = (ApplSTATE)*(AnswerData.Data) ;
                                                                      if( ApplStateSubCommand != 0 )
                                                                          {
                                                                           if( RFID_STATE == NoStateReturn )
                                                                               {
                                                                               RFID_ControlState = 2 ;
                                                                               }
                                                                          }
                                                                      else
                                                                          {
                                                                          if( RFID_STATE == Idle )
                                                                              {
                                                                              RFID_ControlState = 4 ;
                                                                              }
                                                                          else
                                                                              {
                                                                              if( RFID_STATE == RFon )
                                                                                  {
                                                                                  RFID_ControlState = 1 ;
                                                                                  SearchTXPIndex = 0 ;
                                                                                  }
                                                                              else
                                                                                  RFID_ControlState = 2 ;
                                                                              }
                                                                          }
                                                                      break ;
                                              case  SEARCH_TXP        :
                                                                      if( *(AnswerData.Data) != 0x00 )
                                                                          {
                                                                          TXP_Number = *(AnswerData.Data) ;
                                                                          for( j = RFID_UIDStatus.Bits.Count ;( j < (TXP_Number+RFID_UIDStatus.Bits.Count) && j < _MaxUID ) ; j++ )
                                                                              {
                                                                              AnswerData.Data = RFID_GATRM310_RxFunc_NextPt(AnswerData.Data) ; 
                                                                              RFID_UID[j].Member.UIDSize = *(AnswerData.Data) ;
                                                                              RFID_UID[j].Member.RF_STD = (RF_Standard)(SearchTXPIndex - 1);
                                                                              for( i = 0 ; i < RFID_UID[j].Member.UIDSize ; i++ )
                                                                                  {
                                                                                  AnswerData.Data = RFID_GATRM310_RxFunc_NextPt(AnswerData.Data) ; 
                                                                                  RFID_UID[j].Member.UID[i] = *(AnswerData.Data) ;
                                                                                  }
                                                                              }
                                                                          RFID_UIDStatus.Bits.Count += j ;
                                                                          _TagType=RFID_SearchSquenceTable[SearchTXPIndex-1];
                                                                          
                                                                          }
                                                                      //
                                                                      if( RFID_Flag.Bit.SearchTxpStart == 1 )
                                                                          RFID_ControlState = 3 ;
                                                                      //
                                                                      break ;
                                              }
                                          }
                                      }
                                  }
                              }
                          else // Skip to next                        
                              RFID_GATRM310_RxFunc_Getc() ;
                          }                      
                      }
                  break;                    
          }
      // Timeout
      if( RFID_Flag.Bit.CheckRxTimeout == 1 )
          {
          RFID_Timeout += 1 ;
          if( RFID_Timeout > _RFID_RxTimeout )
          	  {
              return 1;
			  /*
              USART_ITConfig(RFID_GATRM310_PORT, USART_IT_RXNE, DISABLE);
              USART_ITConfig(RFID_GATRM310_PORT, USART_IT_TXE, DISABLE);
              RFID_GATRM310_Initial() ;
              USART_ITConfig(RFID_GATRM310_PORT, USART_IT_RXNE, ENABLE);
              */
              }
          }
      else
          {
          RFID_Timeout = 0 ;
          //     
          if( RFID_Flag.Bit.RFID_Online == 0 )
              {
              RFID_ControlState = 7 ;
              } 
          else
              {
              if( RFID_Flag.Bit.SearchTxpStart == 0 && RFID_ControlState == 0 && RFID_UIDStatus.Bits.Find == 0 )
                  {                  
                  SearchTXPLoopTime += 1 ;
                  if( SearchTXPLoopTime >= _RFID_SearchTXPTime )
                      {
                      SearchTXPIndex = 0 ;
                      RFID_GATRM310_ClearUID() ;
                      RFID_Flag.Bit.SearchTxpStart = 1 ;
                      SearchTXPLoopTime = 0 ;
                      RFID_ControlState = 5 ;
                      SearchCounter += 1 ; // for test
                      }
                  }
              else
                  SearchTXPLoopTime = 0 ;
              }
          }
      //
      }
  //
  return 0;
}



/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
unsigned char RFID_GATRM310_GetAPPL_State(unsigned char SubCmd)
{
  RFIDSendData[0] = SubCmd ;  
  ApplStateSubCommand = SubCmd ;
  return RFID_GATRM310_SetCommand(APPL_STATE,1,&RFIDSendData[0]) ;
}

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
unsigned char RFID_GATRM310_GetIDB(unsigned char Tag)
{
  RFIDSendData[0] = Tag ;  
  return RFID_GATRM310_SetCommand(GET_IDB,1,&RFIDSendData[0]) ;
}

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
unsigned char RFID_GATRM310_ReadDCF(void)
{
  return RFID_GATRM310_SetCommand(READ_DCF,0,0) ;
}

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
unsigned char RFID_GATRM310_SEARCH_TXP(unsigned char Tag)
{
  RFIDSendData[0] = Tag ;  
  return RFID_GATRM310_SetCommand(SEARCH_TXP,1,&RFIDSendData[0]) ;
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
unsigned char RFID_GATRM310_SetCommand( unsigned char Command, unsigned char Length, unsigned char *DataPtr )
{
  unsigned char CmdLength ;
  unsigned char i ;
  //
  SM4200_CrC16 CrC16 ;
  SM4200M_Command *CmdPtr ;
  if( RFID_GATRM310_TxFunc_IsEmpty() )
      {
      //StartPtr = RFID_GATRM310_TxStartPoint ;
      CmdPtr = RFID_GATRM310_GetCommandDefine(Command) ;
      if( CmdPtr != 0 )
          {
          if( CmdPtr->LengthExtend != 0 )
              CmdLength = CmdPtr->Length + Length ;
          else
              CmdLength = CmdPtr->Length ;
          RFID_GATRM310_TxFunc_Putc( CmdLength ) ;
          RFID_GATRM310_TxFunc_Putc( Command ) ;
          // Data
          for( i = 0 ;  i < Length ; i++ )
              {
              if( DataPtr != 0 )
                  RFID_GATRM310_TxFunc_Putc( *(DataPtr+i) ) ;
              }
          // Calculator CRC
          CrC16.All = RFID_GATRM310_CRC16(RFID_GATRM310_TxStartPoint,1);
          RFID_GATRM310_TxFunc_Putc(CrC16.Byte.Hi) ;
          RFID_GATRM310_TxFunc_Putc(CrC16.Byte.Lo) ;
          // Start Transmit
          //USART_ITConfig(RFID_GATRM310_PORT,USART_IT_TXE,ENABLE);
          RFID_Flag.Bit.TX_Switch = 1;
          RFID_Flag.Bit.CheckRxTimeout = 1 ;
          //
          }
      return 1 ;
      }
  else
      return 0 ;
}

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
SM4200M_Command * RFID_GATRM310_GetCommandDefine(unsigned char Command)
{
  unsigned char i ;
  for( i = 0 ; i < _NumberOfCommand ; i++ )
      {
      if( SM4200M_CommandList[i].Command == Command )
          {
          return &SM4200M_CommandList[i] ;
          }
      }
  return 0 ;
}


/********************************************************************
*
* Filename: crc.c
*
* Description: CRC calculation
*
*
********************************************************************/
#define PRESET_VALUE    0xFFFF
#define POLYNOMIAL      0x8408
unsigned short RFID_GATRM310_CRC16(unsigned char *data, unsigned char TransMode )
{
  unsigned short crc = PRESET_VALUE;
  unsigned char number_of_bytes = *data - 2; // in a LEGIC command the
  // first byte of a array/
  // string holds the number
  // of following bytes
  for(int i = 0; i <= number_of_bytes; i++) // loop over all the bytes
      {// which are used to build
      // the crc
      crc = crc ^((*data)); // xor the crc with the next byte
      for(int j = 0; j < 8; ++j) // loop over each bit
          {
          if( crc & 0x0001) // if the last bit of the crc is 1
              crc = (crc >> 1)^POLYNOMIAL; // then shift the crc and xor the
          // crc with the polynomial
          else // else shift the crc
              crc = (crc >> 1);
          }
      // Shift to Next Data Point
      if( TransMode == 1 )
          data = RFID_GATRM310_TxFunc_NextPt(data) ;
      else
          data = RFID_GATRM310_RxFunc_NextPt(data) ;
      //
      }
  return crc;
}

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void RFID_GATRM310_RxFunc_Init(void)
{
  RFID_GATRM310_RxStartPoint = RFID_GATRM310_RxEndPoint = &RFID_GATRM310_RxBuffer[0] ;
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
unsigned char* RFID_GATRM310_RxFunc_NextPt(unsigned char *pt)
{
  return ((pt - &RFID_GATRM310_RxBuffer[0]) < (RFID_GATRM310_RxSize-1))?(pt+1):&RFID_GATRM310_RxBuffer[0] ;
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
unsigned char RFID_GATRM310_RxFunc_IsEmpty(void)
{
  return (RFID_GATRM310_RxStartPoint == RFID_GATRM310_RxEndPoint)?1:0;
} 


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
unsigned char RFID_GATRM310_RxFunc_IsFull(void)
{
  return (RFID_GATRM310_RxStartPoint == RFID_GATRM310_RxFunc_NextPt(RFID_GATRM310_RxEndPoint))?1:0 ;
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
unsigned short RFID_GATRM310_RxFunc_Length(void)
{ 
	if(RFID_GATRM310_RxStartPoint <= RFID_GATRM310_RxEndPoint ) 
	    return RFID_GATRM310_RxEndPoint - RFID_GATRM310_RxStartPoint ; 
	else 
	    return RFID_GATRM310_RxSize - (RFID_GATRM310_RxStartPoint - RFID_GATRM310_RxEndPoint); 
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void RFID_RxBuffer(unsigned char c)
{ 
	if(RFID_GATRM310_RxStartPoint == RFID_GATRM310_RxFunc_NextPt(RFID_GATRM310_RxEndPoint)) 
	    return ; 
	*RFID_GATRM310_RxEndPoint = c; 
	RFID_GATRM310_RxEndPoint = RFID_GATRM310_RxFunc_NextPt(RFID_GATRM310_RxEndPoint) ;
} 


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
unsigned char RFID_GATRM310_RxFunc_Getc(void)
{ 
	unsigned char result=0; 
  
	if(RFID_GATRM310_RxEndPoint != RFID_GATRM310_RxStartPoint)
      { 
      result = *RFID_GATRM310_RxStartPoint; 
      RFID_GATRM310_RxStartPoint = RFID_GATRM310_RxFunc_NextPt(RFID_GATRM310_RxStartPoint) ;
	  } 
  
	return result;
} 

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void RFID_GATRM310_TxFunc_Init(void)
{
  RFID_GATRM310_TxStartPoint = RFID_GATRM310_TxEndPoint = &RFID_GATRM310_TxBuffer[0] ;
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
unsigned char* RFID_GATRM310_TxFunc_NextPt(unsigned char *pt)
{
  return ((pt - &RFID_GATRM310_TxBuffer[0]) < (RFID_GATRM310_TxSize-1))?(pt+1):&RFID_GATRM310_TxBuffer[0] ;
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
unsigned char RFID_GATRM310_TxFunc_IsEmpty(void)
{
  return (RFID_GATRM310_TxStartPoint == RFID_GATRM310_TxEndPoint)?1:0;
} 


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
unsigned char RFID_GATRM310_TxFunc_IsFull(void)
{
  return (RFID_GATRM310_TxStartPoint == RFID_GATRM310_TxFunc_NextPt(RFID_GATRM310_TxEndPoint))?1:0 ;
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
unsigned short RFID_GATRM310_TxFunc_Length(void)
{ 
	if(RFID_GATRM310_TxStartPoint <= RFID_GATRM310_TxEndPoint ) 
	    return RFID_GATRM310_TxEndPoint - RFID_GATRM310_TxStartPoint ; 
	else 
	    return RFID_GATRM310_TxSize - (RFID_GATRM310_TxStartPoint - RFID_GATRM310_TxEndPoint); 
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void RFID_GATRM310_TxFunc_Putc(unsigned char c)
{ 
	if(RFID_GATRM310_TxStartPoint == RFID_GATRM310_TxFunc_NextPt(RFID_GATRM310_TxEndPoint)) 
	    return ; 
	*RFID_GATRM310_TxEndPoint = c; 
	RFID_GATRM310_TxEndPoint = RFID_GATRM310_TxFunc_NextPt(RFID_GATRM310_TxEndPoint) ;
} 


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
unsigned char RFID_TxBuffer(void)
{ 
  unsigned char result=0; 
  
  if(RFID_GATRM310_TxEndPoint != RFID_GATRM310_TxStartPoint)
  { 
      result = *RFID_GATRM310_TxStartPoint; 
      RFID_GATRM310_TxStartPoint = RFID_GATRM310_TxFunc_NextPt(RFID_GATRM310_TxStartPoint) ;
  } 
  return result;
} 
/*******************************************************************************
* Function Name  : RFID_GATRM310_GetCard
* Description    : 是否取得卡片資料
* Input          :
* Output         : 0 Notyet 1 Yes
* Return         : 
*******************************************************************************/
unsigned char RFID_GATRM310_GetCard(void)// v5.1-4
{ 
      return RFID_UIDStatus.Bits.Find;
}

TXP_UID RFID_GATRM310_GetUID(void)
{
	return RFID_UID[0];
}

/*******************************************************************************
* Function Name  : RFID_GATRM310_GetOnline
* Description    : RFID硬件通訊測試
* Input          : 
* Output         : 
* Return         : 0=NotYet 1=Online
*******************************************************************************/
unsigned char RFID_GATRM310_GetOnline(void)
{
  return RFID_Flag.Bit.RFID_Online;
}
/*******************************************************************************
* Function Name  : RFID_GATRM310_CheckTagType
* Description    : 判斷是否有支援對應 Tag type
* Input          : 
* Output         : 
* Return         : 0=NO 1=Yes
*******************************************************************************/
unsigned char RFID_GATRM310_CheckTagType(void)
{
  //if(_TagType == ISO14443A)
      return 1;
  //else 
  //    return 0;
  /*    
  switch(_TagType)    
  {
      case LEGIC:      //LEGIC RF standard
      case ISO15693:   //ISO 15693
      case ISO14443A:  //ISO 14443 A
      case ISO14433B:  //ISO 14443 B
      case INSIDE:     //INSIDE Secure
      case SONY_FeliCa://SONY FeliCa subset  
           return 1;     
  }  
  return 0;
  */    
}

void RFID_GATRM310_Set_HWFlag(unsigned char by_Flag)
{
	if(by_Flag == 0)
		RFID_Flag.Bit.RFID_HW_InitialOK = 0;
	else
		RFID_Flag.Bit.RFID_HW_InitialOK = 1;
}

unsigned char RFID_TXE_Status(unsigned char by_D)
{
	if(by_D == 0)
  	{
    	RFID_Flag.Bit.TX_Switch = 0;
  	}
  	return RFID_Flag.Bit.TX_Switch;
}

//------------------------------------------------------------------------------
