
#include "Communication.h"



#define BUFFER_SIZE	        280

#define SYNC_WORD               0x55AA

#define CMD_ACK                 0x0



typedef enum
{
    RPacketHead, RTransID, RCommd, RLen, RData, RCRC
} REC_STATE;

__no_init static DATA_PACKAGE    Uart_Rx_Package;      //接收数据包
__no_init static DATA_PACKAGE    Uart_Tx_Package;       //发送数据包

__no_init static UCHAR Uart_RxBUff[BUFFER_SIZE];                //接收暂存BUFF
__no_init static UCHAR Uart_TxBUff[BUFFER_SIZE];                //发送暂存BUFF

__no_init static USHORT by_RxPoint;                      //BUFF位置
__no_init static REC_STATE R_State;                     //接收步骤
__no_init static UCHAR by_TxTimeOut;

__no_init static USHORT Transaction_ID;
__no_init static USHORT Current_Packet_Size;

__no_init static struct
{
    unsigned RxOk       : 1; //接收OK
    unsigned TxOk       : 1;
    unsigned StartRx    : 1; //传输开始
    unsigned StartTx    : 1;
} Com;

void Com_Initial(void)
{
    by_RxPoint = 0;
    Com.RxOk = 0;
    Com.TxOk = 1;
    Com.StartRx = 0;
    R_State = RPacketHead;
    by_TxTimeOut = 0;
    Current_Packet_Size = 0;
    Transaction_ID = 0;

    memset(&Uart_Rx_Package, 0, sizeof(Uart_Rx_Package));
    memset(&Uart_Tx_Package, 0, sizeof(Uart_Tx_Package));
    memset(&Uart_RxBUff, 0, sizeof(Uart_RxBUff));
    memset(&Uart_TxBUff, 0, sizeof(Uart_TxBUff));
}

//CRC16-CCITT algorithm
USHORT Com_CRC16_Check(UCHAR *buffer, USHORT size)
{
    USHORT crc = 0xFFFF;

    if (buffer && size)
        while (size--)
        {
            crc = (crc >> 8) | (crc << 8);
            crc ^= *buffer++;
            crc ^= ((UCHAR) crc) >> 4;
            crc ^= crc << 12;
            crc ^= (crc & 0xFF) << 5;
        }

    return crc;

}

USHORT Com_Send( USHORT Command, USHORT Length, UCHAR *pData)
{
    //IF THE UART IS BUSY, WAIT FOR IT TO BECOME IDLE
    while(!Com.TxOk);
    
    if(Length < COM_MAX_DATA_LENGTH) //MAX ALLOWED DATA BYTE SIZE IS 128
    {
        USHORT current_index = 0;
        Current_Packet_Size = 0;

        memset(Uart_TxBUff, 0, sizeof(Uart_TxBUff));                    //Clear send BUFF
        
        //ADD PACKET HEAD, SEND SYNC BYTE 0xaa55
        Uart_TxBUff[current_index++] = ((SYNC_WORD&0xFF00)>>8);
        Uart_TxBUff[current_index++] = (SYNC_WORD&0x00FF);

        //ADD Transaction ID
        Uart_TxBUff[current_index++] = ((Transaction_ID&0xFF00)>>8);        
        Uart_TxBUff[current_index++] = (Transaction_ID&0x00FF);

        //ADD THE COMMAND
        Uart_TxBUff[current_index++] = (UCHAR)(Command >> 8);  //Lingo ID
        Uart_TxBUff[current_index++] = (UCHAR)CMD_ACK;         //ACK
        
        //ADD THE LENGTH,note that 2 bytes status and 2 bytes ACK data
        Uart_TxBUff[current_index++] = (UCHAR)(Length + 4 >> 8);
        Uart_TxBUff[current_index++] = (UCHAR)(Length + 4);
        
        
        //Add ACK data 
        Uart_TxBUff[current_index++] = (UCHAR)(Command & 0x00FF);
        Uart_TxBUff[current_index++] = 0;
        
        //ADD THE DATA to BUFFER
        memcpy(&Uart_TxBUff[current_index], pData, Length);
        current_index += Length;

        //CALCULATE THE CHECKSUM AND ADD IT TO THE PACKET
        USHORT checksum = Com_CRC16_Check(Uart_TxBUff, current_index);
        Uart_TxBUff[current_index++] = checksum;
        Uart_TxBUff[current_index++] = checksum >> 8;

        //SET A FLAG THAT ITS NOT OK FOR TRANSMISSION AS IT IS BUSY
        Com.TxOk = 0;

        //SET A FLAG TO MENTION TRANSMISSION STARTED
	by_TxTimeOut = 0; //RESET THE TIMEOUT COUNTER
        Com.StartTx = 1;

        //SET THE SIZE OF CURRENT PACKET FOR TRANSMISSION
        Current_Packet_Size = current_index;

        //ENABLE TRANSMISSION
        HAL_Com_TX_INT_Enable();
    }
    else
    {
        return ERROR;
    }

    return Current_Packet_Size;
}

//NOT SURE IF THIS IS A LOOPHOLE FOR SOME BUG SOMEDAY
//Since if memcpy is not atomic then, there may be inconsistency in read/write
//as ISR will be writing the received packet structure and the main looping
//thread copies the value from that structure.
UCHAR Com_GetCommand(DATA_PACKAGE *stData)
{
    UCHAR by_Out = 0;
    if(Com.RxOk)
    {
        memcpy(stData, &Uart_Rx_Package, sizeof(Uart_Rx_Package));
        memset(&Uart_Rx_Package, 0, sizeof(Uart_Rx_Package));
        Com.RxOk = 0;
        by_Out = TRUE;
    }
    return by_Out;
}

void Com_10msInt(void)
{
    if(Com.StartTx)     //防止发送超时锁死
    {
        if(by_TxTimeOut++ > 5)    //50ms
        {
            //FIRST DISABLE THE INTERRUPT OTHERWISE THE UART WILL CONTINUE SENDING WHATEVER IS THERE IN THE 
            HAL_Com_TX_INT_Disable();
            by_TxTimeOut = 0;
            Com.StartTx = 0;
            Com.TxOk = 1;
            memset(Uart_TxBUff, 0, sizeof(Uart_TxBUff));                    //清空发送数据BUFF
        }
    }
    else
    {
        by_TxTimeOut = 0;
    }
}

void Com_Tx_Int(void)
{
    static UCHAR nPoint = 0;

    if(nPoint < Current_Packet_Size)
    {
        HAL_Com_Uart_Send_Data(Uart_TxBUff[nPoint++]);
    }
    else
    {
        HAL_Com_TX_INT_Disable();
        nPoint = 0;
        Com.TxOk = 1;
	by_TxTimeOut = 0;
	Com.StartTx = 0;
        Transaction_ID++;
    }
}

void Com_Rx_Int(void)
{
    static UCHAR CmdFirstByteReceived = 0;
    static USHORT dataLength = 0;
    UCHAR by_Data = HAL_Com_Uart_Receive_Data();

    switch(R_State)
    {
    case RPacketHead: //THE PACKET START HAS 0xaa55
        by_RxPoint = 0;
        if(CmdFirstByteReceived == 1) //THIS IS THE SECOND BYTE OF THE PACKET HEAD
        {
            if(by_Data == (SYNC_WORD&0x00FF)) //IF THE SECOND BYTE RECEIVED IS CORRECT 0xAA
            {
                Com.StartRx = 1;
                //RECEIVED PACKET HEAD SUCCESSFULLY
                Uart_RxBUff[by_RxPoint++] = ((SYNC_WORD&0xFF00)>>8);//0x55
                Uart_RxBUff[by_RxPoint++] = (SYNC_WORD&0x00FF);//0xAA
                R_State = RTransID;
            }
        }
        else if(by_Data == ((SYNC_WORD&0xFF00)>>8))//CAN BE THE PACKET HEAD 0x55
        {
            CmdFirstByteReceived = 1;
            break; //WAIT FOR THE SECOND BYTE OF PACKET HEAD TO CONFIRM PACKET HEAD
        }
        CmdFirstByteReceived = 0;
        dataLength = 0;
        break;
    case RTransID:   
      //Start byte
        Uart_RxBUff[by_RxPoint++] = by_Data;
        CmdFirstByteReceived = ~CmdFirstByteReceived;// == 0) ? 1 : 0;
        //WHEN BOTH Transaction ID BYTE RECEIVED, MOVE TO NEXT step
        if(CmdFirstByteReceived == 0)
            R_State = RCommd;
        break;
    case RCommd:            //接收命令
        Uart_RxBUff[by_RxPoint++] = by_Data;

        //TOGGLE THIS FLAG ON RECEIVING EACH COMMAND BYTE
        //WHEN THIS FLAG IS 1 THAT MEANS THE FIRST COMMAND BYTE RECEIVED
        //AND WHEN THIS FLAG IS 0 THAT MEANS BOTH COMMAND BYTE RECEIVED
        CmdFirstByteReceived = ~CmdFirstByteReceived; // == 0) ? 1 : 0;

        //WHEN BOTH COMMAND BYTE RECEIVED, MOVE TO NEXT step
        if(CmdFirstByteReceived == 0)
            R_State = RLen;
        break;
    case RLen:      //接收长度

        CmdFirstByteReceived = ~CmdFirstByteReceived; // == 0) ? 1 : 0;

        Uart_RxBUff[by_RxPoint++] = by_Data;

        if(CmdFirstByteReceived == 0)
        {
            dataLength = (Uart_RxBUff[by_RxPoint - 2]<<8) + by_Data;

            if(dataLength >= BUFFER_SIZE)        //超过最大长度回到同步头
            {
                Com.StartRx = 0;
                by_RxPoint = 0;
                R_State = RPacketHead;
            }
            else if(dataLength == 0)
            {
                R_State = RCRC;
            }
            else
            {
                R_State = RData;
            }
        }
        break;
    case RData:     //接收数据区
        Uart_RxBUff[by_RxPoint++] = by_Data;
        if(--dataLength == 0)
        {
            R_State = RCRC;
        }
        break;
    case RCRC:      //CRC位
        Uart_RxBUff[by_RxPoint++] = by_Data;
        CmdFirstByteReceived = ~CmdFirstByteReceived;// == 0) ? 1 : 0;

        if(CmdFirstByteReceived == 0)
        {
            R_State = RPacketHead;
            Com.StartRx = 0;
            Com_RxProcess(by_RxPoint);
            by_RxPoint = 0;
        }
        break;
    default:
        R_State = RPacketHead;
        break;
    }
}

//收到有效数据处理
void Com_RxProcess(USHORT PacketSize)
{
    USHORT crcValue = Uart_RxBUff[PacketSize - 2] +
                              (Uart_RxBUff[PacketSize - 1] << 8);
    USHORT CRCResult = Com_CRC16_Check(Uart_RxBUff, PacketSize - 2);
    //WHILE CALCULATING THE CHECKSUM, IT SHOULDNOT INCLUDE THE CRC FIELD
    if(crcValue == CRCResult) //CRC校验
    {
        Uart_Rx_Package.TransID = Uart_RxBUff[2] << 8 + Uart_RxBUff[3];
        Uart_Rx_Package.Command = Uart_RxBUff[5] + (Uart_RxBUff[4] << 8 );
        Uart_Rx_Package.Length  = Uart_RxBUff[7] + (Uart_RxBUff[6] << 8 );
        memcpy(Uart_Rx_Package.Data, &Uart_RxBUff[8], Uart_Rx_Package.Length);  //copy数据
        Com.RxOk = 1;
    }

    memset(Uart_RxBUff, 0, sizeof(Uart_RxBUff));                    //清空接收数据BUFF
}
