#include "TV.h"

static struct
{
    unsigned int RX       : 1;
    unsigned int RX_Check : 1;
    unsigned int TX       : 1;
    unsigned int POWER    : 1;
    unsigned int POWER_ST : 1;
    unsigned int PO       : 1;
    unsigned int OffLine  : 1;// 100.08.03
	unsigned int TX_Switch	: 1;//
}TV;

u8 TV_ComBuffer[10][5];
u8 TV_RXBuffer[5];
u8 TV_Com;
u8 by_TV_TXByte=0;
u8 by_TV_TXStart=0;
u8 by_TV_TXEnd=0;
u8 by_TV_RX=0;
u8 _TryCommand = 0;
u16 by_TVCommandClear=0;

void TV_RxAssay(void);


//u8 _CommandTV = 0; 

void TV_Initial(void)
{
    u8 i,j;
    
    for(i = 0;i < 10;i++) 
    {
        for(j = 0;j < 5;j++)
        {
            TV_ComBuffer[i][j]=0;
        }
    }
    TV.OffLine = 0;
    TV.RX=0;
    TV.RX_Check=0;
    TV.TX=0;
    TV.POWER=0;
    TV.POWER_ST=0;
    TV.PO=0;
	TV.TX_Switch=0;
    by_TV_TXByte=0;
    by_TV_TXStart=0;
    by_TV_TXEnd=0;
    by_TVCommandClear=0;
    TV_SetCommand(TVCom_ReadStatus,0);    
}
/*******************************************************************************
* Function Name  : TV_SetCommand()
* Description    : Command 下達放入堆疊
* Input          : by_COM,by_Dat
* Output         : None
* Return         : None
*******************************************************************************/
void TV_SetCommand(u8 by_COM,u8 by_Dat)
{
    TV_Com=by_COM;
    TV_ComBuffer[by_TV_TXStart][0]=0x02;
    switch(by_COM)
    {
        case TVCom_Number:
        case TVCom_VOL:
        case TVCom_CH:
        case TVCom_POWER:
                 TV_ComBuffer[by_TV_TXStart][1]=by_COM;
                 TV_ComBuffer[by_TV_TXStart][2]=0x01;
                 TV_ComBuffer[by_TV_TXStart][3]=by_Dat;
                 TV_ComBuffer[by_TV_TXStart][4]=TV_ComBuffer[by_TV_TXStart][0]+TV_ComBuffer[by_TV_TXStart][1]+TV_ComBuffer[by_TV_TXStart][2]+TV_ComBuffer[by_TV_TXStart][3];
                 break;   
        case TVCom_ENTER:        
        case TVCom_RESET:
        case TVCom_LAST:
        case TVCom_ReadStatus:
        case TVCom_Mute:
        case TVCom_CC:  
        case TVCom_Aspect:
        case TVCom_Home:
                 TV_ComBuffer[by_TV_TXStart][1]=by_COM;
                 TV_ComBuffer[by_TV_TXStart][2]=0x00;
                 TV_ComBuffer[by_TV_TXStart][3]=TV_ComBuffer[by_TV_TXStart][0]+TV_ComBuffer[by_TV_TXStart][1]+TV_ComBuffer[by_TV_TXStart][2];
                 break;
    }
    by_TV_TXStart++;
    if(by_TV_TXStart > 9) by_TV_TXStart=0;
    if(by_TV_TXStart == by_TV_TXEnd)
    {
        if(by_TV_TXStart == 0)
            by_TV_TXStart=9;
        else 
            by_TV_TXStart--;
    }
    if(!TV.RX && !TV.RX_Check)
    {//==> 可以下Command時
        TV.TX=1;        
    }
}
/*******************************************************************************
* Function Name  : TV_AutoReply()
* Description    : 資料傳輸與接收處理 (1ms做ㄧ次)
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
     u16 by_TVDelay=0;
     u16 by_TVDelay1=0;
     u16 by_TVDelay2=0;
     u16 _PowerCheckDelay = 0;

void TV_AutoReply(void)
{
    
    if(TV.TX)
    {
        if(by_TVDelay > 350)
        {//==> 350ms後下command
            TV.TX=0;            
            by_TV_RX = 0;
            by_TVDelay=0;
            //USART_ITConfig(UART4,USART_IT_TXE,ENABLE);//==>USART4中斷打開
			TV.TX_Switch=1;
        }
        else
            by_TVDelay++;
        by_TVDelay1=0;
    }
    else
    {
        if(by_TVDelay1 > 1100)
        {//==> 1100ms後下command
            by_TVDelay1=0;
            TV_SetCommand(TVCom_ReadStatus,0);
        }
        else
            by_TVDelay1++;
        if(!TV.PO && TV.POWER_ST)
        {
            if(by_TVDelay2 > 3000)
            {//==> 3`s後下command
                TV.PO=1;
                by_TVDelay2=0;
                // start v2.1-8 預設台號
                //TV_SetCommand(TVCom_Number,EEPROM_ReadInformation(EE_CHANNEL)/10);
                //TV_SetCommand(TVCom_Number,EEPROM_ReadInformation(EE_CHANNEL)%10);
                // end
            }
            else
                by_TVDelay2++;
        }
        else 
            by_TVDelay2=0;
        by_TVDelay=0;
    }
    
    if(_PowerCheckDelay > 10000)  
    {
        TV_Initial();
        TV.OffLine = 1;
        _PowerCheckDelay = 0;
    }
    else _PowerCheckDelay++;
    if(!TV.RX && TV.RX_Check)
    {//==>接收資料完成準備進行接收資料的處理  
        TV.OffLine = 0;
        _PowerCheckDelay = 0;
        TV_RxAssay();
        by_TV_TXEnd++;
        if(by_TV_TXEnd > 9) by_TV_TXEnd=0;
        if(by_TV_TXEnd != by_TV_TXStart) 
        {//==>當還有Command未傳出時
            TV.TX=1;
        }
        TV.RX_Check=0;   
    }
    else if(TV.RX)
    {//==>如果TX資料傳送後500ms無回覆3次後就移除command
        by_TVCommandClear++;
        if(by_TVCommandClear > 500)
        {
            by_TVCommandClear=0;
            _TryCommand++;
            if(_TryCommand >= 3)
            {// 嘗試3次後就排除command
                _TryCommand = 0;
                
                if(by_TV_TXEnd == by_TV_TXStart) 
                {
                    TV.RX = 0;
                    TV.RX_Check = 0;
                }
                else
                {
                    by_TV_TXEnd++;
                    if(by_TV_TXEnd > 9) by_TV_TXEnd=0;
                    if(by_TV_TXEnd != by_TV_TXStart) 
                    {//==>當還有Command未傳出時就繼續傳送
                        TV.TX = 1;
                        TV.RX = 0;
                    }
                }
            }
            else
            {
                TV.TX = 1;
                TV.RX = 0;
            }
        }
    }
}
/*******************************************************************************
* Function Name  : TV_RxAssay()
* Description    : RX 資料分析
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TV_RxAssay(void)
{
    if(TV_RXBuffer[1] == 0xc6)
    {
        if(TV_Com == TVCom_ReadStatus)
        {
            if(TV_RXBuffer[3] & 0x80)
            {//==>取得狀態是否為開機
                TV.POWER=1;
                if(!TV.POWER_ST && TV_RXBuffer[3] != 0xe0)
                {
                    TV.POWER_ST=1;
                    TV.PO=0;
                }
                
            }
            else
            {//==>關機
                TV.POWER=0;
                TV.POWER_ST=0;
                TV.PO=0;
            }            
        }
    }
}
/*******************************************************************************
* Function Name  : TV_UartTxRx_Information()
* Description    : TV UART TX / RX 資料傳輸與接收 <放置在USART中斷中使用>
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
char TV_UartTxRx_Information(char by_D)
{
    if(by_D == 1)
    {// TX 資訊處理
        if(by_TV_TXByte >= TV_ComBuffer[by_TV_TXEnd][2]+4)
        {
            //_CommandTV = TV_ComBuffer[by_TV_TXEnd][1];
            TV_RXBuffer[0] = 0;
            TV_RXBuffer[1] = 0;
            TV_RXBuffer[2] = 0;
            TV_RXBuffer[3] = 0;
            TV_RXBuffer[4] = 0;
            
            TV.RX=1;
            by_TV_TXByte=0;  
            by_TV_RX=0;
            by_TVCommandClear=0;
			//USART_ITConfig(UART4,USART_IT_TXE,DISABLE);
            //USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);
			return 1;
        }
        else
        {
            /* Write one byte to the transmit data register */
            by_TV_TXByte++;
        }
    }
    else
    {// RX 資訊處理
        if(TV.RX)
        {           
            if(TV_RXBuffer[0] == 0x82)
            {
                by_TV_RX++;
                if(by_TV_RX >= 5)
                {
                    TV.RX_Check=1;
                    TV.RX=0;
                    by_TV_RX=0;
                    _TryCommand = 0;
					//USART_ITConfig(UART4,USART_IT_RXNE,DISABLE);
					return 1;
                }
            }
        }    
    }
	return 0;
}

/*******************************************************************************
* Function Name  : TV_GetPowe_OnOff()
* Description    : 取得TV是否開機判斷
* Input          : None
* Output         : None
* Return         : TV.POWER ; 0=OFF,1=ON
*******************************************************************************/
u8 TV_GetPowe_OnOff(void)
{
    return TV.POWER;
}

/*******************************************************************************
* Function Name  : TV_Get_Offline()
* Description    : 取得TV是否斷線
* Input          : None
* Output         : None
* Return         : TV.Offline ; 1=OFF,0=ON
*******************************************************************************/
u8 TV_Get_Offline(void)
{
    return TV.OffLine;
}


/*******************************************************************************
* Function Name  : TV_Get_PO()
* Description    : 取得TV狀態是否可以接收指令
* Input          : None
* Output         : None
* Return         : TV.PO ; 1=YES,0=NO
*******************************************************************************/
u8 TV_Get_PO(void)
{
    return TV.PO;
}

//==============================================================================
//TX中斷開關啟動旗標 0:Clear / 1:Get switch target status
//==============================================================================
unsigned char TV_TXE_Status(unsigned char by_D)
{
  if(by_D == 0)
  {
      TV.TX_Switch=0; 
  }
  return TV.TX_Switch;
}

//==============================================================================
// TX傳送資料傳出
//==============================================================================
char TV_TxBuffer(void)
{
	return TV_ComBuffer[by_TV_TXEnd][by_TV_TXByte];
}

//==============================================================================
// RX資料取得
//==============================================================================
void TV_RxBuffer(char by_D)
{
	TV_RXBuffer[by_TV_RX] = by_D;
}


