#include "cab.h"

#define _CabMaxCmdBufferSize  20
#define _CabProtocolMaxSize   10
#define _NullCommand          0xFF

const unsigned char CabCommandTable[_CAB_TotalCommand_] = {'0','1','2','3','4','5','6','7','8','9','B','C','D','E','F','G','H','I','J','K','L','M','Q','R','S','Z'} ;

__no_init unsigned char CabCommandBuffer[_CabMaxCmdBufferSize] ;
__no_init unsigned char CabSendBuffer[_CabProtocolMaxSize] ;
__no_init unsigned char CabInBufferIndex ;
__no_init unsigned char CabOutBufferIndex ;
__no_init unsigned char CabSendIndex ;
__no_init unsigned char CabSendLength ;
__no_init volatile unsigned char CabSendEnd ;

void CAB_Initial(void)
{
  for( CabOutBufferIndex = 0 ; CabOutBufferIndex < _CabMaxCmdBufferSize ; CabOutBufferIndex++ )
      CabCommandBuffer[CabOutBufferIndex] = _NullCommand ;
  
  for( CabOutBufferIndex = 0 ; CabOutBufferIndex < _CabProtocolMaxSize ; CabOutBufferIndex++ )
      CabSendBuffer[CabOutBufferIndex] = _CAB_END ;
  //
  CabInBufferIndex = 0 ;
  CabOutBufferIndex = 0 ;     
  CabSendIndex = 0 ;
  CabSendLength = 0 ;
  CabSendEnd = 1 ;  
  //
  return ;
}

void CAB_SendCommand(unsigned char _Cmd_)
{
  if(_Cmd_ < _CAB_TotalCommand_)
  {
      CAB_CommandToBuffer(_Cmd_);
  }
  return ;
}

void CAB_CommandToBuffer(unsigned char cmd)
{
  CabCommandBuffer[CabInBufferIndex] = cmd ;
  CabInBufferIndex += 1 ;
  if( CabInBufferIndex >= _CabMaxCmdBufferSize )
      CabInBufferIndex =  0 ;        
  return ;
}

      
// Command buffer process
unsigned char CAB_CommandProcess(void)
{
  if( CabCommandBuffer[CabOutBufferIndex] != _NullCommand )
  {
      if( CabSendEnd == 1 )
      {
          CabSendIndex = 0 ;
          CabSendEnd = 0 ;
          CabSendLength = 3 ;
          CabSendBuffer[0] = _CAB_START ;
          CabSendBuffer[1] = CabCommandTable[CabCommandBuffer[CabOutBufferIndex]] ;
          CabSendBuffer[2] = _CAB_END ;
          CabSendBuffer[3] = 0 ;
          return 1;// TX need enable
      }
  }    
  return 0;// Not yet
}

// Check how much data need send to TX         
unsigned char CAB_SendDataCheck(void)
{
  if( CabSendIndex < CabSendLength )
      return 1;// Have data
  else
  {
      CabSendIndex = 0 ;
      CabSendEnd = 1 ;
      CabCommandBuffer[CabOutBufferIndex] = _NullCommand ;
      CabOutBufferIndex += 1 ;
      if( CabOutBufferIndex >= _CabMaxCmdBufferSize ) CabOutBufferIndex =  0 ; 
      return 0;// Data end
  }
}

// Push send data 
unsigned char CAB_SendData(void)
{
  unsigned char _Output_;

  _Output_=CabSendBuffer[CabSendIndex];
  CabSendIndex += 1 ;
  return _Output_;
}

