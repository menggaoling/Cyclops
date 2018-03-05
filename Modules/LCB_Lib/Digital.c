#include "Digital.h"



#define eSTOP_DelayTime 400
#define InclineProtectionDelayTime 1000 

u16 MCBVersion;       //==>MCB版本
u16 InclineLocation=0;  //==>升降准位
u16 MotorRpm;         //==>马达速度
u16 ErrorCode_Hold;
u16 eSTOP_Time=0;
u16 by_Delay=0;//==>自动封包回传时间计数
u16 by_DigitalSpeedTarget=0;//==>目标速度追赶用
u16 by_TargetSpeedDelay=100;//==>速度加速率的延迟用
u16 by_ElevationOld=0;//==>旧升降目标值
u16 _CheckElevationCommandAD = 0;
u16 by_Freq;
u16 by_Current;
u16 by_OutputVolt;
u16 by_DC_BusVolt;
u16 by_IGBT_Temp;
u16 ErrorCode;          //==>错误码
u16 _ErrorCodeBuffer[5];//==>错误码暂存
u16 Console_SpeedRPM;
u16 _InclineProtectionDelay; 
u16 _InclineStopCheckTime = 0;
u16 by_TargetDelay=0;
u8 RX_Buffer[35];//==>信息取得暂存
u8 TX_Buffer[25][20];//==>信息传出暂存,20组command,5组插件用
u8 by_TX=0;//==>信息传出时的位置指针
u8 by_TX_Target=0;//==>Command持续放入暂存位置
u8 by_TX_Hold;//插件前的暂存
u8 by_TX_Insert = 20;// 插件command放入位置
u8 by_TX_Byte=0;//==>传输位数
u8 by_RX=0;//==>接收信息处理暂存区位置指针
u8 by_ErrorTime=0;//==>封包回传时间计数
u8 by_Status=0;//==>封包回传状态
u8 by_Command;//==>指令
u8 by_RX_ReturnTime=0;//==>数据接收是否结束时间计数
u8 by_TX_ConnectDelay=0;
u8 by_Error=0;//==>避免没接线材而持续作累计用 (网络失效的侦测部份)
u8 by_ErrorCommandSum;//==>不支持或错误的 command 重传次数计数
u8 MachineType = 0;       //==>机型(下板型号)
u8 by_GetDkip = 0;
u8 by_CheckInUser = 0;
u8 by_CheckInUserTimeNumber = 0;
u8 _ErrorCodeTarget = 0;//==>错误码储存位数
u8 _MotorStopTime = 0;
u8 by_GetADC_Delay=0;
u8 _CheckRpmStopTime;
u8 _RunningCommand;
u8 _InclineCheckAgainDelayTime;

u8 by_InUserModeMotoSafeTime = InUserMotoCheckTime;// 马达稳定时间
u8 by_NonInUserTime = 0;
u8 Digital_CeckNonInUser = 0;

void Digital_InclineSafeFunctionCheck(void);
//
struct
   {
   unsigned RX            : 1;// 接收中断判断旗标
   unsigned RX_Return     : 1;// 数据接收
   unsigned Chang         : 1;// 资料取得切换
   unsigned Error         : 1;// 封包错误判断与否
   unsigned Work          : 1;// 使用者是否开始进行运动判断
   unsigned Command       : 1;// 为了判断command是否可传输用
   unsigned RPMStop       : 1;// 马达是否停止判断
   unsigned Skip          : 1;// 判断是否清除错误码用
   unsigned TargetSpeed   : 1;// 是否追速度             
   unsigned Buffer_Over   : 1;// 输出暂存区已满与否
   unsigned TX            : 1;// 传出
   unsigned AutoCheck     : 1;// 机台自动更正是否启动
   unsigned Incline_Check : 1;// 升降COMMAND是否做动
   unsigned Incline_Shift : 1;// 升降是否移动
   unsigned RPM_Check     : 1;// RPM转速确认与否
   unsigned SpeedStart    : 1;// 当设定速度时
   unsigned DaughterBoardReset : 1;
   unsigned SAFE_Key      : 1;
   unsigned VersionStatus : 1;
   unsigned InclineStop   : 1; 
   unsigned Insert        : 1;// 指令插件
   unsigned InsertSend    : 1;// 插件指令开始传送
   unsigned PowerOn04A0Pass : 1;
   unsigned CommandStart : 1;
   unsigned Stop : 1;         // 跑带停止
   unsigned InclineStopCheck : 1;
   }Digital;



/*******************************************************************************
* Function Name  : Digital_ErrorCodeUpdate
* Description    : 储存前的错误码更新排序
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Digital_ErroeCodeUpdate(void)
{
  u8 i;
  if(_ErrorCodeTarget != 0)
  {
      
      for(i = 10;i > 0;i--)
      {
          if(i > _ErrorCodeTarget)
          {
              Console_DigitalError(Data_Set,i-1,Console_DigitalError(Data_Get,i-_ErrorCodeTarget-1,0));
              Console_DigitalError(Data_Set,i-_ErrorCodeTarget-1,0);
          }
          else
          {
              Console_DigitalError(Data_Set,i - 1,_ErrorCodeBuffer[_ErrorCodeTarget - i]);
              _ErrorCodeBuffer[_ErrorCodeTarget - i] = 0;
          }
      }
      _ErrorCodeTarget = 0;
  }
}
/*******************************************************************************
* Function Name  : Digital_ErrorCode_ChangToAddress()
* Description    : 将所取得的错误码进行位移的动作,到暂存位置
* Input          : None
* Output         : None
*******************************************************************************/
void Digital_ErrorCode_ChangToAddress(void)
{
  u8 i;
  
  for(i = 0;i < 5;i++)
  {
      if(_ErrorCodeBuffer[i] == ErrorCode)
      {//==>当有相同的就不存入
          i = 100;
      }
      else if(_ErrorCodeBuffer[i] == 0)
      {//==>当是没有的错误码就放入暂存区
          _ErrorCodeBuffer[i] = ErrorCode;
          if(Digtial_ErrorCode_Level(ErrorCode) == 1)
          {//==>Class C
              //BLX_Command(MAS_Error_Code,0);
          }
          i = 200;
          _ErrorCodeTarget+=1;
      }
  }
  if(i < 10)
  {//==>当暂存区没有空位时就需要自动搬移出一个位置
      for(i = 0;i < 4;i++)
      {
          _ErrorCodeBuffer[i] = _ErrorCodeBuffer[i + 1];
      }
      _ErrorCodeBuffer[4] = ErrorCode;
      if(Digtial_ErrorCode_Level(ErrorCode) == 1)
      {//==>Class C
          //BLX_Command(MAS_Error_Code,0);      
      }
      _ErrorCodeTarget+=1;
  }
  if(_ErrorCodeTarget > 5) _ErrorCodeTarget = 5;
}
/*******************************************************************************
* Function Name  : Digital_CheckClassCError()
* Description    : 确认暂存区中是否有错误码C的发生
* Input          : None
* Output         : 0=>NO 1=>YES
*******************************************************************************/
u8 Digital_CheckClassCError(void)
{
  u8 by_Out = 0;
  u8 i;
  
  for(i = 0;i < 5;i++)
  {
      if(Digtial_ErrorCode_Level(_ErrorCodeBuffer[i]) == 1)
      {
          i = 10;
          by_Out = 1;
      }
  }
  return by_Out;
}
/*******************************************************************************
* Function Name  : u16 Digtial_Set_ErrorCode()
* Description    : 设定错误码
* Input          : 错误码
* Output         : None
* Return         : None
*******************************************************************************/
void Digtial_Set_ErrorCode(u16 by_Dat)
{
    ErrorCode = by_Dat;
}
/*******************************************************************************
* Function Name  : u16 Digtial_Get_ErrorCode()
* Description    : 错误码取得
* Input          : None
* Output         : None
* Return         : ErrorCode
*******************************************************************************/
u16 Digtial_Get_ErrorCode(void)
{
    return(ErrorCode);
}
/*******************************************************************************
* Function Name  : Digital_Initial
* Description    : 数字通讯参数默认
* Input          : None
* Output         : UART1
* Return         : None
*******************************************************************************/
void Digital_Initial(void)
{   
    Digital.InclineStopCheck = 0;
    Digital.Stop = 0;
    _CheckRpmStopTime = 0;
    _RunningCommand = 0;
    _InclineCheckAgainDelayTime = 0;
    Digital.PowerOn04A0Pass = 0;// 第一次开机04A0排除
    //Digital.CommunicationStart = 0;
    //Digital.CommunicationSave = 0;
    //_Communication_TxTotal = 0;
    //_Communication_RxTotal = 0;
    _InclineProtectionDelay = 0; 
    by_Freq = 0;
    by_Current = 0;
    by_OutputVolt = 0;
    by_DC_BusVolt = 0;
    by_IGBT_Temp = 0;
    by_CheckInUser = 0x55;//==>Non Used
    Digital_CeckNonInUser = 0;
    Digital.VersionStatus=0;
    InclineLocation=0;
    Digital.SAFE_Key=0;
    eSTOP_Time=0;
    ErrorCode_Hold=0;//==>981104
    Digital.InsertSend = 0;
    Digital.Insert = 0;
    Digital.Incline_Shift=0;
    Digital.AutoCheck=0;
    Digital.RX=0;
    Digital.RX_Return=0;
    Digital.Chang=0;
    Digital.Error=0;
    Digital.Work=0;
    Digital.Buffer_Over=0;
    Digital.Incline_Check=0;
    Digital.TargetSpeed=0;
    Digital.RPMStop=0;//==>停止
    Digital.RPM_Check=0;
    by_RX=0;
    by_TX_Target=0;
    by_TX=0;
    by_TX_Byte=0;
    by_Error = 0;
    ErrorCode = 0;
    by_TargetSpeedDelay=100;
    by_TX_ConnectDelay=0;
    by_ElevationOld=0;
    by_ErrorCommandSum=0;
    Digital.DaughterBoardReset=0;
    
    _ErrorCodeBuffer[0] = 0;
    _ErrorCodeBuffer[1] = 0;
    _ErrorCodeBuffer[2] = 0;
    _ErrorCodeBuffer[3] = 0;
    _ErrorCodeBuffer[4] = 0;
    _ErrorCodeTarget = 0;
    
    Console_SpeedRPM = 0; 
    _MotorStopTime = 0;
}
/*******************************************************************************
* Function Name  : Digital_PowerON_MachineTypeCheck()
* Description    : 下控制支持判断
* Input          : None
* Output         : None
* Return         : 0 -> No 1 -> Yes
*******************************************************************************/
u8 Digital_PowerON_MachineTypeCheck(void)
{
   by_ElevationOld = Console_HighLowADC(Data_Get,0,0); 
   if(MachineType >= 0xC0)
       return 1;
   else
       return 0;
}
/*******************************************************************************
* Function Name  : Digital_GetMachineType()
* Description    : 下控制器的型号取得
* Input          : None
* Output         : None
* Return         : MachineType
*******************************************************************************/
u8 Digital_GetMachineType(void)
{
   return MachineType;
}
void Digital_SetMachineType(u8 by_D)
{
   MachineType = by_D;
}

/*******************************************************************************
* Function Name  : Digital_CheckSafeKeyStatus()
* Description    : 侦测按紧急开关后并是否取得0x02B2错误码
* Input          : None
* Output         : None
* Return         : Digital.SAFE_Key
*******************************************************************************/
u8 Digital_CheckSafeKeyStatus(void)//==>981015
{
  return Digital.SAFE_Key;
}
void Digital_ClearSafeKeyStatus(void)
{
  Digital.SAFE_Key=0;
}

/*******************************************************************************
* Function Name  : Digital_eSTOPKeyDelay()
* Description    : 紧急开关延迟时间
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u8 Digital_eSTOPKeyDelay(void)
{
  if(eSTOP_Time >= eSTOP_DelayTime)
      return 1;
  else 
      return 0;
}
void Digital_eSTOPKeyDelayClear(void)
{
   eSTOP_Time=0;
}

/*******************************************************************************
* Function Name  : Digital__DCI_DaughterBoard_Reset(
* Description    : DCI daughter board重置
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Digital_CommandBufferReset(void)
{
    Digital.DaughterBoardReset=1; 
}

/*******************************************************************************
* Function Name  : Digital_Set_AutoCheck(u8 by_Dat)
* Description    : 机台是否进入自动更正模式
* Input          : by_Dat
* Output         : 0->No 1->Yes
* Return         : None
*******************************************************************************/
void Digital_Set_AutoCheck(u8 by_Dat)
{
   Digital.AutoCheck=by_Dat;
}
/*******************************************************************************
* Function Name  : Digital_Set_SpeedTargetDelay(u16 by_Dat)
* Description    : 马达加速度速率延迟时间
* Input          : by_Dat
* Output         : by_TargetSpeedDelay
* Return         : None
*******************************************************************************/
void Digital_Set_SpeedTargetDelay(u16 by_Dat)
{
   by_TargetSpeedDelay=by_Dat;
}
/*******************************************************************************
* Function Name  : Digital_Set_DigitalSpeedTarget(u16 by_Dat)
* Description    : 马达速度追击目标值设定
* Input          : by_Dat
* Output         : by_DigitalSpeedTarget ; Digital.TargetSpeed
* Return         : None
*******************************************************************************/
void Digital_Set_DigitalSpeedTarget(u16 by_Dat)//==>设定追击起始值
{
  by_DigitalSpeedTarget = by_Dat * 10;
  if(by_Dat == 0)
  {// 准备停止跑带,所以需先行清空之前下的指令
      Digital.Buffer_Over = 1;// 先停止塞指令
      by_TX=0;
      by_TX_Target=0;
      by_TX_Hold = 0;
      by_TX_Insert = 20;// 插件起头
      Digital.Insert = 0;
      Digital.InsertSend = 0;// 插件不传送
      if(by_Status & BIT2) 
      {
          Digital_Command(CmdSetMotorCounter,0);// Speed Stop
      }
      if((by_Status & BIT0)||(by_Status & BIT1)) 
      {
          Digital_Command(CmdSetInclineAction,0);// Inlcine Stop
          Digital.InclineStopCheck = 1;
      }
      if(by_Status & BIT2)
      {
          Digital_Command(CmdSetMotorCounter,0);// Speed Stop
      }
      by_GetADC_Delay = 0;
      Digital.Buffer_Over = 0;// 重新允许塞指令
      Digital.Stop = 1;
      _CheckRpmStopTime = 0;
      Digital.Incline_Check = 0;
      _InclineCheckAgainDelayTime = 0;
  }
  Digital.TargetSpeed = 1;//==>开始追击
}
/*******************************************************************************
* Function Name  : Digital_Set_TargetSpeedStart(u8 by_Dat)
* Description    : 马达速度追击与否设定
* Input          : by_Dat
* Output         : Digital.TargetSpeed ; 1->开始追击 0->停止追击
* Return         : None
*******************************************************************************/
void Digital_Set_TargetSpeedStart(u8 by_Dat)
{
   Digital.TargetSpeed = by_Dat;
   if(by_Dat == 1)
   {
       Digital.RPM_Check=1;//==>当有追击时就开始做RPM的确认动作
   }
}
/*******************************************************************************
* Function Name  : Digital_Get_TargetSpeed()
* Description    : 取得速度追击旗标
* Input          : None
* Output         : None
* Return         : Digital.TargetSpeed
*******************************************************************************/
u8 Digital_Get_TargetSpeed(void)
{
   return(Digital.TargetSpeed);
}
/*******************************************************************************
* Function Name  : Digital_TargetSpeed_To_DigitalCommand()
* Description    : 马达速度追击主程序
* Input          : None
* Output         : CmdGetInclineLocation ; CmdSetMotorCounter ; Digital.TargetSpeed
* Return         : None
*******************************************************************************/
void Digital_TargetSpeed_To_DigitalCommand(void)
{
  if(by_TargetDelay > by_TargetSpeedDelay && !Digital.InsertSend)
  {//==>速度追击用延迟
      by_TargetDelay=0;
      if(Digital.TargetSpeed)//==>开始追击时
      {
         if(!Digital.Buffer_Over) //==>当暂存区还有位置时
         {
             if(by_GetADC_Delay == 2)//==>为配合升降的问题所以在速度追击中必须加入AD值的取得
             {
                 by_GetADC_Delay++;
                 Digital_Command(CmdGetInclineLocation,0);//==>取得ADC值
             }
             else if(by_GetADC_Delay == 4)//==>为配合RPM取值问题所以在速度追击中必须加入RPM值的取得
             {
                 by_GetADC_Delay=0;
                 Digital_Command(CmdGetMotorRpm,0);//==>取得RPM值
             }
             else
             {
                 by_GetADC_Delay++;
                 if(by_DigitalSpeedTarget == 0 || Digital.Stop == 1)// 2014.07.04
                 {// 当下要机台停止时就减速
                     Digital_Command(CmdSetMotorCounter,0);
                     Digital.TargetSpeed=0;//==>停止追击
                 }
                 else//==>当要机台追击下
                 {
                     if(by_DigitalSpeedTarget > Console_Information(Info_WorkoutSpeed,Data_Get,0))//==>减速  Console_Speed(Data_Get,0)
                     {
                         by_DigitalSpeedTarget-=10;
                         Digital_Command(CmdSetMotorCounter,by_DigitalSpeedTarget);
                         if(by_DigitalSpeedTarget <= Console_Information(Info_WorkoutSpeed,Data_Get,0))//Console_Speed(Data_Get,0)
                         {//==>当达到目标值时就停止追击
                             Digital.TargetSpeed=0;
                         }    
                     }
                     else if(by_DigitalSpeedTarget < Console_Information(Info_WorkoutSpeed,Data_Get,0))//==>加速  Console_Speed(Data_Get,0)
                     {
                         by_DigitalSpeedTarget+=10;
                         if(!Digital.Buffer_Over) Digital_Command(CmdSetMotorCounter,by_DigitalSpeedTarget);
                         if(by_DigitalSpeedTarget >= Console_Information(Info_WorkoutSpeed,Data_Get,0))//Console_Speed(Data_Get,0)
                         {//==>当达到目标值时就停止追击
                             Digital.TargetSpeed=0;
                         } 
                     }
                 }    
             }
         }
      }
  }
  else by_TargetDelay++;

}
/*******************************************************************************
* Function Name  : Digital_Set_InclineCheck(u8 by_Dat)
* Description    : 是否设定升降参数
* Input          : by_Dat ; 0->停止  1->启动
* Output         : Digital.Incline_Check
* Return         : None
*******************************************************************************/
void Digital_Set_InclineCheck(u8 by_Dat)
{
   Digital.Incline_Check = by_Dat;
}
/*******************************************************************************
* Function Name  : Digital_Clear_ElevationOld()
* Description    : 升降目标旧Incline值清除
* Input          : None
* Output         : by_ElevationOld
* Return         : None
*******************************************************************************/
void Digital_Clear_ElevationOld(void)
{
  by_ElevationOld=0;
}
/*******************************************************************************
* Function Name  : u16 Digital_Get_ElevationOld()
* Description    : 升降目标Incline参数值取得
* Input          : None
* Output         : None
* Return         : by_ElevationOld
*******************************************************************************/
u16 Digital_Get_ElevationOld(void)
{
  return by_ElevationOld; 
}

/*******************************************************************************
* Function Name  : Digtial_ElevationVariationCheck()
* Description    : 升降马达实际AD值与目标AD值是否误差过大
* Input          : 0=>Reset 归零
*                  1=>Workout 重启
* Output         : None
* Return         : Yes / No
*******************************************************************************/
u8 Digtial_ElevationVariationCheck(u8 by_D)
{
    if(by_D == 0) // Reset 归零
    {
        if(InclineLocation >= ((Console_HighLowADC(Data_Get,0,0))+256))
        {// > 最低点值
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else // Workout
    {
        if((InclineLocation > (Console_HighLowADC(Data_Get,0,0)) &&
            InclineLocation < (Console_HighLowADC(Data_Get,1,0))) &&
           (InclineLocation >= by_ElevationOld+(2*128) || 
            InclineLocation <= by_ElevationOld-(2*128)) )
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }    
}

/*******************************************************************************
* Function Name  : u8 Digital_Check_Buffer()
* Description    : 数字通讯输出缓存器是否已满
* Input          : None
* Output         : None
* Return         : Digital.Buffer_Over ; 1->满了 0->尚有空位
*******************************************************************************/
u8 Digital_Check_Buffer(void)
{
    return(Digital.Buffer_Over);
}
/*******************************************************************************
* Function Name  : Digital_Set_Work(u8 by_Dat)
* Description    : 数字通讯升降切换使用指针 (作为使用者是否开始进行运动判断)
* Input          : by_Dat
* Output         : Digital.Work ; 1->Yes 0->No
* Return         : None
*******************************************************************************/
void Digital_Set_Work(u8 by_Dat)
{
    Digital.Work=by_Dat;
}
/*******************************************************************************
* Function Name  : Digital_Command(u8 by_Cmd,u16 by_Dat)
* Description    : 数字通讯 command 设定
* Input          : by_Cmd - command
                   by_Dat - 设定参数值
* Output         : by_TX_Target ; Digital.Buffer_Over ; Digital.TX ; IO_Set_Digital_CTL(); TX_Buffer[][]
* Return         : None
*******************************************************************************/
u8 Digital_Command(u8 by_Cmd,u32 by_Dat)// Sinkyo
{
    u8 _Buffer = 0;
    u16 by_A = 0;
    u16 by_B = 0;
    
    Digital.Command=0;
    if(!Digital.Insert)
    {// 正常
        _Buffer = by_TX_Target;
    }
    else
    {// 插件
        _Buffer = by_TX_Insert;
    }  
    TX_Buffer[_Buffer][0]=0x00;//==> Start
    TX_Buffer[_Buffer][1]=0xff;//==> Address
    switch(by_Cmd)
    {
        case CmdInitial:           //  0x70 //设定下板初始化 
        case CmdGetStatus:         //  0x71 //状态取得
        case CmdGetErrorCode:      //  0x72 //错误代码取得
        case CmdSkipCurrentError:  //  0x76 //移除错误码
        case CmdGetRollerRpm:      //  0xf8 //取得滚轮速度
        case CmdGetMotorRpm:       //  0xf9 //取得马达速度
        case CmdGetInclineLocation://  0xfa //取得升降准位
        case CmdGetVersion:        //  0x73 //取得MCB版本
        //case CmdGetDCI_Version:    //  0x78 //取得DCI的下控软件版本  
        case CmdDCI_MANUAL_CALIBRATION:  // 0xab 
        case CmdDCI_GetInclineRange:  
        case CmdGetTreadmillInUsed:      // 0x95  
        case CmdGetMainMotorInformation: // 0x96  
                 TX_Buffer[_Buffer][2]=by_Cmd;
                 TX_Buffer[_Buffer][3]=0;   //==>data长度
                 TX_Buffer[_Buffer][4]=Digital_CRC8((u8*)TX_Buffer[_Buffer],4);
                 break;
        case CmdSetInclineAction:  //  0xf5 //设定升降控制
                 Digital.Incline_Shift=0;         
        case CmdSetWorkStatus:     //  0xf7 //设定运动状态    
        case CmdDCI_FORCE_INCLINE_OPERATION:   // 0x94  
          
                 TX_Buffer[_Buffer][2]=by_Cmd;
                 TX_Buffer[_Buffer][3]=1;   //==>data长度
                 TX_Buffer[_Buffer][4]=by_Dat;
                 TX_Buffer[_Buffer][5]=Digital_CRC8((u8*)TX_Buffer[_Buffer],5);
                 break;
        case CmdSetInclineLocation://  0xf6 //设定升降位置
                 if(Digital.Work)//==>如果不在工程模式的状况下
                 {
                     if(by_Dat==0)
                     {
                         by_Dat = Console_HighLowADC(Data_Get,0,0);
                     }
                     else
                     {
                         
                         by_A = Console_HighLowADC(Data_Get,1,0)-Console_HighLowADC(Data_Get,0,0);
                         // Incline为欲设定的升降高度, Range = 0 ~ 1500 (代表0~15%
                         by_Dat = (by_Dat * 100) / 2;                         
                         by_Dat = (u32)( by_Dat * by_A ) / 1500;
                         by_Dat += Console_HighLowADC(Data_Get,0,0);
                     }
                     if(by_ElevationOld != by_Dat)
                     {//==>当新旧AD参数不同时就进行指令下达动作
                         Digital.Incline_Shift = 1;
                         by_ElevationOld = by_Dat;
                     }
                     else
                     {
                         Digital.Command=1;//==>相同参数时就不做command的传输
                     }
                 }
                   
        case CmdSetMotorCounter:   //  0xf1 //设定马达转速
                 if(by_Cmd==CmdSetMotorCounter)
                 {
                     if(by_Dat != 0)
                     {
                         if(Digital.PowerOn04A0Pass == 0)
                         {// 解除 by Pass 动作
                             Digital.PowerOn04A0Pass = 1;
                         }
                         Console_SpeedRPM = by_Dat;// 仪表下的速度值
                         if(Console_SpeedUnit(Data_Get,0) == C_KM)
                         {
                             by_Dat = (u32)((float)by_Dat * 0.62137);
                         }
                         if(by_Dat < 50) by_Dat = 50;
                         /* =============================================>>
                         170 RPM : LowSpeed 0.5 mph
                         4070RPM : HighSpeed 12.0 mph
                         5112RPM : HighSpeed 15.0 mph
                         LowSpeed  : 50
                         HighSpeed : 1500 //1200
                         RPM = 170RPM + (((Speed - LowSpeed) * (5112RPM - 170RPM)) / (HighSpeed - LowSpeed))
                         <<============================================= */
                         //if(MachineType == LCB_DCI)
                         //    by_Dat = 170 + ((u32)((by_Dat - 50) * 4942) / 1450);
                         //else
                         //{
                             by_A = Console_RPM_Parameter(0,0,0);// Get MinRPM
                             by_B = Console_RPM_Parameter(0,1,0);// Get MaxRPM
                             by_Dat = by_A + ((u32)((by_Dat - 50) * by_B) / 1500);
                             //by_Dat = 178 + ((u32)((by_Dat - 50) * Console_Get_Parameter()) / 1450);
                         //}
                         if(_MotorStopTime == 0)
                         {// START
                             _MotorStopTime = 1;
                         }
                         
                     } 
                     else 
                     {// 回起速
                         _MotorStopTime = 0;
                         Console_SpeedRPM = Console_StartSpeed(Data_Get,0) * 10;
                     }
                     if(by_Dat > 5300)
                     {
                         Digital.Command=1;//==>不做command的传输
                     }
                 }
                 if(!Digital.Command)
                 {
                     TX_Buffer[_Buffer][2]=by_Cmd;
                     TX_Buffer[_Buffer][3]=2;   //==>data长度
                     TX_Buffer[_Buffer][4]=by_Dat/256;
                     TX_Buffer[_Buffer][5]=by_Dat%256;
                     TX_Buffer[_Buffer][6]=Digital_CRC8((u8*)TX_Buffer[_Buffer],6);
                 }
                 break;
        case CmdDCI_SetInclineRange://==>升降AD值范围设定 (10~240)   
                 /*
                 TX_Buffer[_Buffer][2]=by_Cmd;
                 TX_Buffer[_Buffer][3]=4;   //==>data长度
                 TX_Buffer[_Buffer][4]=0x05;// 10  
                 TX_Buffer[_Buffer][5]=0x00;
                 TX_Buffer[_Buffer][6]=0x78;// 240  
                 TX_Buffer[_Buffer][7]=0x00;
                 TX_Buffer[_Buffer][8]=Digital_CRC8((u8*)TX_Buffer[_Buffer],8);
                 */
                 break;   
        case CmdDCI_SET_RPM_SPEED_RANGE://==>RPM值范围设定 (15mile=165~5300)(12mile=165~4300)   
                 /*
                 TX_Buffer[_Buffer][2]=by_Cmd;
                 TX_Buffer[_Buffer][3]=8;   //==>data长度
                 TX_Buffer[_Buffer][4]=0x00; 
                 TX_Buffer[_Buffer][5]=0xa5;
                 TX_Buffer[_Buffer][6]=0x14; 
                 TX_Buffer[_Buffer][7]=0xb4;
                 TX_Buffer[_Buffer][8]=0;//0x01; 
                 TX_Buffer[_Buffer][9]=0;//0xd1;
                 TX_Buffer[_Buffer][10]=0;//0x31; 
                 TX_Buffer[_Buffer][11]=0;//0x6a;
                 TX_Buffer[_Buffer][12]=Digital_CRC8((u8*)TX_Buffer[_Buffer],12);
                 */
                 break; 
        case CmdEUPsMode: // Sinkyo
             Digital_Set_LCBEUPsState(0);     
             TX_Buffer[_Buffer][2]=by_Cmd;
             TX_Buffer[_Buffer][3]=1;   //==>data长度
             TX_Buffer[_Buffer][4]=by_Dat;
             TX_Buffer[_Buffer][5]=Digital_CRC8((u8*)TX_Buffer[_Buffer],5);
             break;
        case CmdLCBDeviceData :  // Sinkyo 
            // LCB_Set_EEPromGetState(0);
             TX_Buffer[_Buffer][2]=by_Cmd;
             TX_Buffer[_Buffer][3]=3;   //==>data长度
             TX_Buffer[_Buffer][4]=by_Dat >> 16; // second command
             TX_Buffer[_Buffer][5]=(u8)((u16)by_Dat>>8);
             TX_Buffer[_Buffer][6]=(u8)by_Dat;
             TX_Buffer[_Buffer][7]=Digital_CRC8((u8*)TX_Buffer[_Buffer],7);
             break;                        
    }
    if(!Digital.Command)//==>当参数不同时就送command,针对升降与马达的command
    {
        if(!Digital.Insert)
        {// 正常
            by_TX_Target++;
            if(by_TX_Target > 19) by_TX_Target=0;
            if(Digital.InsertSend)
            {
                if(by_TX_Target == by_TX_Hold)//==>当如果目前处理的指标与下次所要存入的位置一样时就必须不做处理
                {
                    if(by_TX == 0) by_TX_Target=19;
                    else by_TX_Target--;
                    Digital.Buffer_Over=1;//==>暂存区已满
                }
            }
            else
            {
                if(by_TX_Target == by_TX)//==>当如果目前处理的指标与下次所要存入的位置一样时就必须不做处理
                {
                    if(by_TX == 0) by_TX_Target=19;
                    else by_TX_Target--;
                    Digital.Buffer_Over=1;//==>暂存区已满
                }
            }
            //
        }
        else
        {// Command插件旗标
            by_TX_Insert++;
            if(by_TX_Insert > 24) by_TX_Insert = 20;
            if(by_TX_Insert == by_TX)
            {//插件塞满了就不继续塞
                if(by_TX == 20) 
                    by_TX_Insert = 24;
                else
                    by_TX_Insert--;
            }
            Digital.Insert = 0;
            Digital.InsertSend = 1;// 插件传送
        }
        if(!Digital.RX)//==>当完成接收时才继续送下一封包
        {
            Digital.TX=1;
        }
    }
    return Digital.Buffer_Over;
}
/*******************************************************************************
* Function Name  : u8 Digital_CRC8(u8 *ptr,u8 DATALENGTH)
* Description    : 数字通讯 CRC code
* Input          : *ptr ; DATALENGTH
* Output         : None
* Return         : CHECKDATA
*******************************************************************************/
const u8 CrcTab[16]={0x00,0x31,0x62,0x53,0xc4,0xf5,0xa6,0x97,0xb9,0x88,0xdb,0xea,0x7d,0x4c,0x1f,0x2e};
u8 Digital_CRC8(u8 *ptr,u8 DATALENGTH)
{
    u8 crcheckcnt,crchecktemp,crchalf,CHECKDATA=0;
    for(crcheckcnt =0; crcheckcnt < DATALENGTH; crcheckcnt ++)
    {
        crchecktemp = *(ptr+crcheckcnt);
        crchalf =(CHECKDATA/16);
        CHECKDATA<<=4;
        CHECKDATA^=CrcTab[crchalf ^( crchecktemp / 16)];
        crchalf =(CHECKDATA/16);
        CHECKDATA<<=4;
        CHECKDATA^=CrcTab[crchalf ^( crchecktemp & 0x0f)];
    }
    return CHECKDATA;
}
/*******************************************************************************
* Function Name  : u8 Digital_Get_RPMStatus()
* Description    : 回传马达的状态 (含升降马达)
* Input          : None
* Output         : None
* Return         : Digital.RPMStop ; 0->停止   1->运动
*******************************************************************************/
u8 Digital_Get_RPMStatus(void)
{
       return Digital.RPMStop;
    
}
/*******************************************************************************
* Function Name  : Digital_AutoReply()
* Description    : 数字通讯封包处理 ; 每 1ms 处理ㄧ次
* Input          : None
* Output         : CmdSetInclineLocation ; CmdGetInclineLocation ; CmdGetErrorCode
* Return         : None
*******************************************************************************/
void Digital_AutoReply(void)
{
  static u8 by_CheckInUserDelayTime = 0;
  static u16 by_SpeedInclineCheckDelay = 0;    
 
  //
  if(Digital.TX)
  {//==>准备进行TX封包的传输
      by_TX_ConnectDelay++;
      if(by_TX_ConnectDelay >= 50)
      {//==>command and command间隔需要50ms          
          by_TX_ConnectDelay=0;
          Digital.TX=0;
          // 送插件指令
          if(Digital.InsertSend)
          {
              if(by_TX < 20)
              {// 第一次插件就放入启始位置
                  by_TX_Hold = by_TX;
                  by_TX = 20;//by_TX_Insert;
              }
          }
          if(TX_Buffer[by_TX][4+TX_Buffer[by_TX][3]] == Digital_CRC8((u8*)TX_Buffer[by_TX],4+TX_Buffer[by_TX][3]))
          {//==>当封包中的 CRC 码比对正确时
           HAL_Set_LCB_Serial_Tx();
          }
          else
          {//==>移除 command
              Digital.RX=0;
              Digital.RX_Return=0;
              by_TX++;
              if(Digital.InsertSend)
              {// 插件
                  if(by_TX > 24) by_TX=20;
              }
              else
              {// 正常
                  if(by_TX > 19) by_TX=0;
              }
          }
          //
      }    
  }
  //
  if(Digital.RX)//==>准备处理接收封包
  { 
      if(Digital.CommandStart == 0) by_ErrorTime++;
      if(by_ErrorTime > 200)
      {//==>当持续200ms没封包时就重传               
          by_ErrorTime=0;
          if(by_Error > 20) //5000ms=250msx20 ;250=200ms+50ms的封包间隔 
          {//==>约5秒后储存错误码
              by_Error=0;
              ErrorCode=0x04B0;//0x04a0
              Digital_ErrorCode_ChangToAddress();
              by_GetDkip = 0;
          }
          else by_Error++;//==>避免没接线材而持续作累计用 (网络失效的侦测部份)
          Digital.RX=0;
          by_RX=0;
          Digital.TX=1;
      }
      else
      {
          if(Digital.RX_Return)//==>TX数据传输下
          {
              by_RX_ReturnTime++;
              if(by_RX_ReturnTime > 10)
              {//==>当连续 10ms没有byte读入时就判定为封包接收完成,同时也可以避免下控因半双工的方向切换问题而导致封包的传输错误
                  Digital_RX();//==>开始处理接收信息
                  if(Digital.Skip)//==>当有错误码发生时
                  {
                      Digital.Skip=0;
                      if(Digital_CheckError())
                      {//==>错误码是否成立
                          if(Digital.AutoCheck && ErrorCode != 0)// ==0x0140)//==>在自动更正中排除此错误码
                          {
                              ErrorCode=0;
                              Digital_Command(CmdInitial,1);//==>S001-01 for DCI
                              Digital_Command(CmdSetWorkStatus,1);//==>S001-01 for DCI
                          }
                          else
                          {
                              Digital_ErrorCode_ChangToAddress();
                          }
                      }
                      else ErrorCode=0;
                  }
                  if(!Digital.Error && ((Digital.InsertSend == 1 && (by_TX == by_TX_Insert)) ||
                                        (Digital.InsertSend == 0 && (by_TX == by_TX_Target))))
                  {//==>当如果Buffer中没有其他command与错误封包的话就不做续传的动作
                     
                  }
                  else
                  {//==>续传 command               
                      Digital.Error=0;
                      Digital.TX=1;
                  }
              }
          }
      }
  }
  // Other
  if(!Digital.CommandStart)
  {
      // Incline Target 014.07.04 
      Digital_InclineSafeFunctionCheck();
      // Speed Target
      Digital_TargetSpeed_To_DigitalCommand();
      //
      if(eSTOP_Time < eSTOP_DelayTime)
      {
          eSTOP_Time++;  
      }
      if(by_Delay > 400)//==>每400ms自动传输1次
      {
          by_Delay=0;
          Digital.Chang=1;
      }
      else by_Delay++;
      if(Digital.Chang && !Digital.Buffer_Over) 
      {//==>当暂存区还有位置且数字通讯每400ms自动取得信息1次
          if(_RunningCommand == 0)
          {
              Digital_Command(CmdGetInclineLocation,0);
              _RunningCommand = 1;
          }
          else if(_RunningCommand == 1)
          {
              Digital_Command(CmdGetMotorRpm,0);
              _RunningCommand = 0;
              Digital.Chang=0;
          }
      }
      if(by_SpeedInclineCheckDelay >= 1000)
      {//==>每1秒作一次
          by_SpeedInclineCheckDelay=0;
          if(!Digital.InsertSend)
          {//==>符合对应下控制器
              if(Console_WorkoutStatus(Data_Get,0) == 1 && by_CheckInUserTimeNumber != 0xff &&
                 Digital_GetMachineType() > LCB_TopTek )
              {//==>当机台是在运动且有打开侦测的情形下进行人员侦测动作
                  by_CheckInUserDelayTime++;
                  if(by_CheckInUserDelayTime >= 1)// 2
                  {//==> 每x秒下一次command
                      by_CheckInUserDelayTime = 0;
                      if(!Digital.Buffer_Over)
                      {
                          Digital_Command(CmdGetTreadmillInUsed,0);//==>人员侦测
                      }
                  }
                  if(by_CheckInUser == 0xff)//==>> 人员侦测功能
                  {//==> In Used (0xff)
                      by_NonInUserTime = 0;
                      Digital_CeckNonInUser = 0;
                  }
                  else if(by_CheckInUser == 0x55)
                  {//==> Non Used (0x55)
                      by_NonInUserTime++;
                      if(by_NonInUserTime >= by_CheckInUserTimeNumber - by_InUserModeMotoSafeTime)
                      {//==>持续 x 秒后就确认无人
                          Digital_CeckNonInUser = 1;
                      }
                  }
              }
              else
              {
                  by_InUserModeMotoSafeTime = InUserMotoCheckTime;// 马达稳定时间
                  by_NonInUserTime = 0;
                  by_CheckInUserDelayTime = 0;
                  Digital_CeckNonInUser = 0;
              }
              if(!Digital.Buffer_Over)
              {
                  Digital_Command(CmdGetMainMotorInformation,0);//==>下控信息取得
              }
          }
          // 马达相关动作确认确认 ==>>
          Digital_MotorFunctionCheck();
          // <<======
      }   
      else by_SpeedInclineCheckDelay++;
      //
  }
  //
}


/*******************************************************************************
* Function Name  : Digital_UartTx_Int()
* Description    : 数字通讯 UART TX  数据传输 <放置在USART中断中使用>
* Input          : None
* Output         : TX_Buffer[][] ; by_Command ; RX_Buffer[by_RX]
* Return         : None
*******************************************************************************/
void Digital_UartTx_Int(void)
{
  if(by_TX_Byte > (5+TX_Buffer[by_TX][3]))
  {
    Digital.RX=1;
    Digital.RX_Return=0;
    by_TX_Byte=0;
    by_RX_ReturnTime=0;
    by_TX_ConnectDelay=0;
    by_Command=TX_Buffer[by_TX][2];
    /* Disable the USART2 Transmit interrupt */
    HAL_Set_LCB_Serial_Rx();
  }
  else 
  {
    HAL_LCB_Serial_Send_Data(TX_Buffer[by_TX][by_TX_Byte]);
    by_TX_Byte++; 
  }
}
/*******************************************************************************
* Function Name  : Digital_UartTxRx_Information()
* Description    : 数字通讯 UART RX 数据接收 <放置在USART中断中使用>
* Input          : None
* Output         : TX_Buffer[][] ; by_Command ; RX_Buffer[by_RX]
* Return         : None
*******************************************************************************/
void Digital_UartRx_Int(void)
{
  /* Read one byte from the receive data register */
  RX_Buffer[by_RX] = HAL_LCB_Serial_Receive_Data();
  
  if(Digital.RX)
  {
    if(RX_Buffer[0] == 0x01)
    {//==>当起始byte = 0x01时才会采纳后续的封包数据
      by_RX++;
    }
    by_RX_ReturnTime=0;
    by_TX_ConnectDelay=0;
    Digital.RX_Return=1;
  }
}




/*******************************************************************************
* Function Name  : Digital_RX()
* Description    : 数字通讯 RX 数据处理
* Input          : None
* Output         : Digital.RPMStop ; MCBVersion ; RollerRpm ; MotorRpm ; InclineLocation ; ErrorCode ; Digital.Buffer_Over ; Digital.RX ; by_TX ; by_RX
                   Digital.Error ; by_Status
* Return         : None
*******************************************************************************/
void Digital_RX(void)
{
    union
    {
       unsigned int WORD;
       char BYTE[2];
    }byData;
    u8 by_ErrorLoop=0;
    
    by_Status = RX_Buffer[1];
#ifdef _DebugClose    
    if((by_Status & BIT5)||(RX_Buffer[0]!=0x01)||(RX_Buffer[2]!=by_Command)||(RX_Buffer[by_RX-1]!=Digital_CRC8((u8*)RX_Buffer,by_RX-1)))
#else // Debug时关闭BIT5的机制,避免有误动作!
    if((RX_Buffer[0]!=0x01)||(RX_Buffer[2]!=by_Command)||(RX_Buffer[by_RX-1]!=Digital_CRC8((u8*)RX_Buffer,by_RX-1)))  
#endif      
    {//==>有以上问题时就重传上一笔封包
        Digital.Error=1;
        if(by_Status & BIT5) by_ErrorTime = 0;//==>当有错误或不支持的command时
        if(by_Status & BIT5 || by_ErrorCommandSum >= 3)
        {//==>当重传3次还是失败后就移除这command
            Digital.Error=0;
            if(Digital.InsertSend)
            {// 插件
                if(by_TX != by_TX_Insert)
                {
                    by_TX++;
                    if(by_TX > 24) by_TX=20;
                }
            }
            else
            {// 正常
                if(by_TX != by_TX_Target)  
                {
                    by_TX++;
                    if(by_TX > 19) by_TX=0;
                }
            }
            Digital.Buffer_Over=0;//==>暂存区尚有位置可存放command
            by_ErrorCommandSum=0;
        }
        else by_ErrorCommandSum++;
        if(by_Command == CmdInitial || by_Command == CmdSetWorkStatus)
        {// 不连续发生时三道指令就重新传
            by_GetDkip = 0;
            Digital.TargetSpeed=0;//==>停止追击
            by_ErrorCommandSum=0;
            by_TX=0;
            by_TX_Target=0;
            Digital.Buffer_Over=0;
            by_TX_Hold = 0;
            by_TX_Insert = 20;// 插件起头
            Digital.Insert = 0;
            Digital.InsertSend = 0;// 插件不传送
            Digital_Command(CmdInitial,0);
            Digital_Command(CmdSetWorkStatus,0);
            Digital_Command(CmdSetWorkStatus,1);
        }
        if(by_Command == CmdGetTreadmillInUsed)
        {// 当人员侦测Command错误时需先清除累计
            by_NonInUserTime = 0;
        }
    }
    else
    {
        by_ErrorTime = 0;
        Digital.Error=0;
        by_Error=0;
        by_ErrorCommandSum=0;
        //********************************************//
        //**** 马达讯号侦测是否转动               ****//
        //********************************************//
        //B3:码盘(TM) RPM状态          0: 停止 1:动作
        //B2:主马达动作状态            0: 停止 1:动作
        //B1:升降马达上升状态          0: 停止 1:上升
        //B0:升降马达下降状态          0: 停止 1:下降
        if((by_Status & 0x0f)==0)//==>0x0f(BIT0,1,2,3)
        {
            Digital.RPMStop=0;//==>停止
            Digital.Stop = 0;
        }
        else
        {
            Digital.RPMStop=1;//==>运动
        }
        if(RX_Buffer[3]==2)//==>取的 Lenght byte数
        {
            byData.BYTE[0]=RX_Buffer[5];
            byData.BYTE[1]=RX_Buffer[4];
        }
        switch(by_Command)
        {
            case CmdSetInclineLocation://  0xf6
                     _CheckElevationCommandAD = by_ElevationOld;
                     break;
            case CmdEUPsMode:     
                     if(RX_Buffer[3] == 1)
                     {
                         if(RX_Buffer[4] != 0x01)
                            Digital_Set_LCBEUPsState(0xff);
                         else
                            Digital_Set_LCBEUPsState(RX_Buffer[4]);
                     }
                     else
                          Digital_Set_LCBEUPsState(0xff);
                     break;     
          
            case CmdGetTreadmillInUsed:      // 0x95 
                     by_CheckInUser = RX_Buffer[4];
                     break;
            case CmdGetMainMotorInformation: // 0x96
                     byData.BYTE[0]=RX_Buffer[5];
                     byData.BYTE[1]=RX_Buffer[4];
                     by_Freq = byData.WORD;
                     byData.BYTE[0]=RX_Buffer[7];
                     byData.BYTE[1]=RX_Buffer[6];
                     by_Current = byData.WORD;
                     byData.BYTE[0]=RX_Buffer[9];
                     byData.BYTE[1]=RX_Buffer[8];
                     by_OutputVolt = byData.WORD;
                     byData.BYTE[0]=RX_Buffer[11];
                     byData.BYTE[1]=RX_Buffer[10];
                     by_DC_BusVolt = byData.WORD;
                     byData.BYTE[0]=RX_Buffer[13];
                     byData.BYTE[1]=RX_Buffer[12];
                     by_IGBT_Temp = byData.WORD;
                     break;
            case CmdSkipCurrentError:  //==>0x76 移除错误码
                     by_GetDkip = 0;
                     break;
            case CmdGetRollerRpm:      //==>0xf8 取得滚轮速度
                     //RollerRpm=byData.WORD;
                     break;
            case CmdGetMotorRpm:       //==>0xf9 取得马达速度
                     MotorRpm=byData.WORD;
                     break;
            case CmdGetInclineLocation://==>0xfa 取得升降准位
                     InclineLocation=byData.WORD;
                     break;
            case CmdGetErrorCode:      //==>0x72 错误代码取得
                     if(RX_Buffer[3] > 2)
                     {//==>当错误码有好几个时
                         for(by_ErrorLoop = 0;by_ErrorLoop < (RX_Buffer[3]/2);by_ErrorLoop++)
                         {
                             byData.BYTE[0]=RX_Buffer[5+(by_ErrorLoop*2)];
                             byData.BYTE[1]=RX_Buffer[4+(by_ErrorLoop*2)];
                             ErrorCode = byData.WORD;
                             if(Digital_CheckError())
                             {//==>错误码成立
                                 Digital_ErrorCode_ChangToAddress();
                             }
                             else
                             {
                                 ErrorCode = 0;
                             }
                         } 
                     }              
                     else if(RX_Buffer[3] == 2) ErrorCode = byData.WORD;
                     Digital.Skip=1;
                     break;
            case CmdGetVersion:        //==>0x73 取得MCB版本
                     Digital.VersionStatus=1;
                     MCBVersion=byData.WORD;
                         MachineType=byData.BYTE[1];
                     break;         
                     /*
            case CmdGetDCI_Version:    //==>0x78 DCI专用版本特殊码取得
                     DCI_MCBVersion=(RX_Buffer[6]-0x30)*10;
                     DCI_MCBVersion=(DCI_MCBVersion+(RX_Buffer[8]-0x30))*10;
                     DCI_MCBVersion=(DCI_MCBVersion+(RX_Buffer[9]-0x30))*10;
                     DCI_MCBVersion=(DCI_MCBVersion+(RX_Buffer[19]-0x30))*10;
                     DCI_MCBVersion=(DCI_MCBVersion+(RX_Buffer[21]-0x30))*10;
                     DCI_MCBVersion+=(RX_Buffer[22]-0x30);
                     break;
                     
            case CmdDCI_GetInclineRange:
                     byData.BYTE[0]=RX_Buffer[5];
                     byData.BYTE[1]=RX_Buffer[4];
                     by_Incline_A=byData.WORD;
                     byData.BYTE[0]=RX_Buffer[7];
                     byData.BYTE[1]=RX_Buffer[6];
                     by_Incline_B=byData.WORD;
                     break;   
                     */
        }
        if(by_TX != by_TX_Target)// 确保是非重置的暂存区才做 
        {
            by_TX++;
            if(!Digital.InsertSend)
            {// 正常
                if(by_TX > 19) by_TX = 0;
            }
            else
            {// 插件            
                if(by_TX == by_TX_Insert)
                {// 插件的指令下完了
                    by_TX_Insert = 20;// 插件起头
                    by_TX = by_TX_Hold;// 还原上次正常传送停止的位置 
                    Digital.InsertSend = 0;
                }
            }
            Digital.Buffer_Over=0;//==>暂存区尚有位置可存放command
        }
    }
    if(by_Status & BIT6)
    {//==>当下控有回传错误码时,就下取得错误码command
        Digital.Error = 0;
        if(by_GetDkip == 0)
        {
            by_GetDkip = 1;
            Digital.Insert = 1;// Command插件旗标
            Digital_Command(CmdGetErrorCode,0);
            Digital.Insert = 1;// Command插件旗标
            Digital_Command(CmdSkipCurrentError,0);
        }
    }
    Digital.RX=0;//==>接收完成
    by_RX=0;
    if(Digital.DaughterBoardReset && !Digital.InsertSend)
    {//==>RESET下控制器
        _MotorStopTime = 0;
        Console_SpeedRPM = 0;
        by_GetDkip = 0;
        Digital.DaughterBoardReset = 0; 
        by_TX = 0;
        by_TX_Target = 0;
        Digital.Buffer_Over=0;
        by_TX_Insert = 20;// 插件起头
        by_TX_Hold = 0;
        Digital.Insert = 0;
        Digital.InsertSend = 0;
        Digital.TargetSpeed = 0;//==>停止追击
        by_DigitalSpeedTarget=0;
        Digital_Command(CmdInitial,0);
        Digital_Command(CmdSetWorkStatus,0);
        Digital_Command(CmdSetWorkStatus,1);
        if(Digital.AutoCheck == 1 || Digtial_ElevationVariationCheck(0) == 1)
        {//==>当升降实际值与目标值误差正负,就清除之前所设定的目标值参数然后在重新取得
            by_ElevationOld=0;//==>先清除旧的AD暂存值
            //Console_Incline(Data_Set,0);
            Digital.Work=1;
            Console_Information(Info_WorkoutIncline,Data_Set,0);
        }
        Digital.AutoCheck = 0;  
    }
}
/*
u8 Digital_CheckInclineRange(void)
{
    if(by_Incline_A == 0x0500 && by_Incline_B == 0x7800)
    {
        return 1;
    }
    else 
    {
        return 0;
    }
}
*/
/*******************************************************************************
* Function Name  : u8 Digtial_ErrorCode_Level(u16 by_Dat)
* Description    : 错误码的等级 Class A,Class B,Class C
* Input          : by_Dat ; 错误代码
* Output         : None
* Return         : Class
*******************************************************************************/
u8 Digtial_ErrorCode_Level(u16 by_Dat)
{
   union
   {
       u16 WORD;
       u8 BYTE[2];
   }Dat;
   Dat.WORD=by_Dat;
   if(Dat.BYTE[0] >= 0xa0) return(1);//==> Class C
   else if(Dat.BYTE[0] >= 0x40) return(2);//==> Class B
   else return(3);//==> Class A

}
/*******************************************************************************
* Function Name  : u16 Digtial_Get_MCBVersion()
* Description    : MCB版本取得
* Input          : None
* Output         : None
* Return         : MCBVersion
*******************************************************************************/
u16 Digtial_Get_MCBVersion(void)
{
    return(MCBVersion);
}
/*******************************************************************************
* Function Name  : u16 Digtial_Get_InclineLocation()
* Description    : 升降准位取得
* Input          : None
* Output         : None
* Return         : InclineLocation
*******************************************************************************/
u16 Digtial_Get_InclineLocation(void)
{
    return(InclineLocation);
}
/*******************************************************************************
* Function Name  : u16 Digtial_Get_RollerRpm()
* Description    : 滚轮RPM取得
* Input          : None
* Output         : None
* Return         : RollerRpm
*******************************************************************************/
u16 Digtial_Get_RollerRpm(void)
{
    return(MotorRpm);//RollerRpm
}
/*******************************************************************************
* Function Name  : u16 Digtial_Get_MotorRpm()
* Description    : 马达RPM取得
* Input          : None
* Output         : None
* Return         : MotorRpm
*******************************************************************************/
u16 Digtial_Get_MotorRpm(void)
{
    return Console_SpeedRPM;
}

/*******************************************************************************
* Function Name  : Digital_CheckError()
* Description    : 确认错误码是否符合规范
* Input          : None
* Output         : None
* Return         : Yes=1 / No=0
*******************************************************************************/
u8 Digital_CheckError(void)
{
   if(ErrorCode == 0x02B2)
   {
       Digital.SAFE_Key=1;
       Digital.TargetSpeed=0;//==>停止追击
   }
   else if(ErrorCode == 0x0441)
   {
   }
   else //if((ErrorCode >= 0x0040 && ErrorCode <= 0x0045)||(ErrorCode >= 0x00A0 && ErrorCode <= 0x00A1) || 
        //   (ErrorCode >= 0x0140 && ErrorCode <= 0x0144)||(ErrorCode >= 0x01A0 && ErrorCode <= 0x01AD) || 
        //   (ErrorCode >= 0x0240 && ErrorCode <= 0x0248)|| ErrorCode == 0x0201 || ErrorCode == 0x029F  || 
        //   (ErrorCode >= 0x02A0 && ErrorCode <= 0x02BD)||(ErrorCode >= 0x0440 && ErrorCode <= 0x0442) ||  
        //   (ErrorCode >= 0x04A0 && ErrorCode <= 0x04A1)|| ErrorCode == 0x0340 || ErrorCode == 0x04B0)  
   {
       if(ErrorCode == 0x04A0)
       {
           if(Digital.PowerOn04A0Pass == 0)
           {// 解除 by Pass 动作
               Digital.PowerOn04A0Pass = 1;
               return 0;
           }
       }
       return 1;
   }
   
   return 0;

}
//------------------------------------------------------------------------------
// S003-01
/*******************************************************************************
* Function Name  : Digital_GetStatus()
* Description    : 通讯状态代码取得
* Input          : None
* Return         : by_Status
*******************************************************************************/
u8 Digital_GetStatus(void) 
{
  return by_Status;
}
/*******************************************************************************
* Function Name  : Digital_InclineProtection()
* Description    : 升降控制保护机制
* Input          : None
* Return         : 0=>正常 ; 1=>保护启动
*******************************************************************************/
u8 Digital_InclineProtection(u8 by_Dat)
{
  u16 _ADC_Buffer,_ADC_A,_ADC_B;
  
  
  if((by_Status & 0x03) != 0)
  {// 升降有作动时
      // 计算出准备下的对应AD参数
      if(by_Dat == 0)
      {
          _ADC_Buffer = Console_HighLowADC(Data_Get,0,0);
      }
      else
      {
          _ADC_A = (Console_HighLowADC(Data_Get,1,0) - Console_HighLowADC(Data_Get,0,0))/ 30;
          _ADC_B = (Console_HighLowADC(Data_Get,1,0) - Console_HighLowADC(Data_Get,0,0))% 30;
          if(by_Dat <= _ADC_B)
          {
              _ADC_Buffer = (Console_HighLowADC(Data_Get,0,0) + (by_Dat*_ADC_A)+ by_Dat);
          }
          else
          {
              _ADC_Buffer = (Console_HighLowADC(Data_Get,0,0) + (by_Dat*_ADC_A)+ _ADC_B);
          }
      }
      //
      if((((by_Status & 0x01) == 0x01)&&(InclineLocation < _ADC_Buffer))||  
         (((by_Status & 0x02) == 0x02)&&(InclineLocation > _ADC_Buffer)))
      {// UP且有反向 or DOWN且有反向
          Digital_Command(CmdSetInclineAction,0);//==>升降停止
          _InclineProtectionDelay = 1;
          return 1; 
      }
  }
  else
  {
      if(Digital.InclineStop == 0)
      {// 升降停止 , 但尚未确认前又有command下来
          Digital_Command(CmdSetInclineAction,0);//==>升降停止
          _InclineProtectionDelay = 1;
          return 1;
      }
  }
  return 0;
}

/*******************************************************************************
* Function Name  : void Digital_Clear_ErrorTime()
* Description    : 断线计数(Error : 0x04B0)
* Input          : None
* Output         : None
* Return         : InclineLocation
*******************************************************************************/
void Digital_Clear_ErrorCount(void)
{
    by_Error = 0;
}

/*******************************************************************************
* Function Name  : Digital_GetMCB_Information()
* Description    : 取下控制器的信息
* Input          : 信息代码
* Return         : 
*******************************************************************************/
u16 Digital_GetMCB_Information(u8 by_D)
{
  switch(by_D)
  {
      case 1:// 频率
             return by_Freq;
      case 2:// 电流
             return by_Current;
      case 3:// 输出电压
             return by_OutputVolt;
      case 4:// DC Bus电压
             return by_DC_BusVolt;
      case 5:// IGBT 温度
             return by_IGBT_Temp;
  }
  return 0;
}
/*******************************************************************************
* Function Name  : Digital_CommandStart()
* Description    : 自行下命令与否
* Input          : by_D
* Output         : None
* Return         : None
*******************************************************************************/
void Digital_CommandStart(char by_D)
{
  Digital.CommandStart = by_D;
}
/*******************************************************************************
* Function Name  : Digital_MotorFunctionCheck()
* Description    : 马达相关动作确认确认
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Digital_MotorFunctionCheck(void)
{
  // Error Code judge
  if(_MotorStopTime != 0 && MotorRpm == 0)
  {
      _MotorStopTime++;
      if(_MotorStopTime > 40)
      {// 持续40秒没动作              
          ErrorCode = 0x00A1;// 速度无回传
          Digital_ErrorCode_ChangToAddress();
          by_GetDkip = 0;
      }
  }
  // 确保 Speed and Incline 停止动作补偿
  if(Digital.Stop == 1)
  {
      if(_CheckRpmStopTime >= 2)  
      {
          _CheckRpmStopTime = 0;
          if((by_Status & BIT2)&&(Digital.Buffer_Over == 0)) 
          {
              Digital_Command(CmdSetMotorCounter,0);// Speed Stop
          }
          if(((by_Status & BIT0)||(by_Status & BIT1))&&
             (Digital.InclineStopCheck == 1)&&
             (Digital.Buffer_Over == 0)) 
          {
              Digital_Command(CmdSetInclineAction,0);// Inlcine Stop
          }
      }
      else
          _CheckRpmStopTime++;
  }
  // 确保升降马达定位指令补偿
  if(_InclineCheckAgainDelayTime != 0)
  {
      if(_InclineCheckAgainDelayTime >= 3)
      {
          if(_CheckElevationCommandAD != by_ElevationOld)
          {// 收发不同步需要再补发一次
              if(Digital.Buffer_Over == 0)
              {
                  by_ElevationOld = 0;
                  Digital_Command(CmdSetInclineLocation,Console_Information(Info_WorkoutIncline,Data_Get,0));//Console_Incline(Data_Get,0)
                  _InclineCheckAgainDelayTime = 1;
              }
          }
          else
          {
              _InclineCheckAgainDelayTime = 0;
          }
      }
      else _InclineCheckAgainDelayTime++;
  }                             
}
/*******************************************************************************
* Function Name  : Digital_InclineSafeFunctionCheck()
* Description    : 升降切换延迟,保护机制
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Digital_InclineSafeFunctionCheck(void)
{
  if((by_Status & 0x03) == 0)
  {
      if(_InclineStopCheckTime > 300) // 持续300ms没动作
          Digital.InclineStop = 1; 
      else 
          _InclineStopCheckTime++;
  }
  else
  {
      _InclineStopCheckTime = 0;
      Digital.InclineStop = 0;
  }
  if(_InclineProtectionDelay > 0)
  {// 保护延迟启动
      _InclineProtectionDelay++;
      if(_InclineProtectionDelay > InclineProtectionDelayTime)
      {// 保护机制延迟约x秒
          _InclineProtectionDelay = 0;
      }
  }
  else
  { 
      if(Digital.Incline_Check && !Digital.Buffer_Over && !Digital.InsertSend)
      {//==>当下升降指令且暂存区还有位置时
          if(Digital_InclineProtection(Console_Information(Info_WorkoutIncline,Data_Get,0)) == 0)//Console_Incline(Data_Get,0)
          {// 当正常动作时
              if(!Digital.Buffer_Over)
              {
                  if(Digital_Command(CmdSetInclineLocation,Console_Information(Info_WorkoutIncline,Data_Get,0)) == 0)//Console_Incline(Data_Get,0)
                  {//==>升降设定
                      Digital.Incline_Check=0;
                      _InclineCheckAgainDelayTime = 1;
                      Digital.InclineStopCheck = 0;
                      _CheckElevationCommandAD = 0;
                  }
                  
              }
              if(!Digital.Buffer_Over)
              {
                  Digital_Command(CmdGetInclineLocation,0);
              }
          }
      }
  }
}




