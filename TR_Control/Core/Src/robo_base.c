#include "robo_base.h"
#include "can.h"

extern ROBO_BASE* Robo_Base;
CAN_HandleTypeDef hcan2;

void BASE_Init(ROBO_BASE *Robo)       
{
  Speed_System* P_Speed=NULL;      
  P_Speed=&Robo->Speed_MotorLF; PID_Init(&P_Speed->Speed_PID,	7.0f,	0.1f,	0,	3000,	0,	3000,	5000); P_Speed->Motor_Num=0;
  P_Speed=&Robo->Speed_MotorLB; PID_Init(&P_Speed->Speed_PID,	7.0f,	0.1f,	0,	3000,	0,	3000,	5000); P_Speed->Motor_Num=1;
  P_Speed=&Robo->Speed_MotorRF; PID_Init(&P_Speed->Speed_PID,	7.0f,	0.1f,	0,	3000,	0,	3000,	5000); P_Speed->Motor_Num=2;
}


void Motor_Speed_Analysis(ROBO_BASE* Robo,uint8_t* RX_Data,uint32_t Motor_Num)
{
  Speed_System* S_Motor=NULL;
  switch(Motor_Num)
  {
    case 0x201:S_Motor=&Robo->Speed_MotorLF;break;
    case 0x202:S_Motor=&Robo->Speed_MotorLB;break;
    case 0x203:S_Motor=&Robo->Speed_MotorRF;break;
	  default:break;
  }if(S_Motor!=NULL) Speed_Info_Analysis(&S_Motor->Info,RX_Data);
}

void PID_Send(ROBO_BASE* Robo)
{
  Speed_System* P_Speed=NULL;
  P_Speed=&Robo->Speed_MotorLF; PID_Speed_Cal(P_Speed,Robo->Tx_CAN1);
  P_Speed=&Robo->Speed_MotorLB; PID_Speed_Cal(P_Speed,Robo->Tx_CAN1);
  P_Speed=&Robo->Speed_MotorRF; PID_Speed_Cal(P_Speed,Robo->Tx_CAN1);
  Send_To_Motor_(&hcan1,Robo->Tx_CAN1);
}

void SystemIO_Usart_ToString(int32_t System_Out,int32_t System_In)
{
  TX_BUFFER Usart_Tx;
  int32_t temp=1000000;
  uint8_t flag1=0;

  //转化系统输出值
  Usart_Tx.length=0;
  if(System_Out<0) Usart_Tx.Tx_buffer[Usart_Tx.length++]='-',System_Out=-System_Out;
  else if(System_Out==0) Usart_Tx.Tx_buffer[Usart_Tx.length++]='0',temp=0;

  while(temp!=0)
  {
    if(System_Out/temp!=0)
    {
	  flag1=1;
	  Usart_Tx.Tx_buffer[Usart_Tx.length++]='0'+System_Out/temp;
	  System_Out-=System_Out/temp*temp;
	  if(System_Out==0)
	  {
		temp/=10;
	    while(temp!=0)
		{
		  Usart_Tx.Tx_buffer[Usart_Tx.length++]='0';
		  temp/=10;
		}break;
	  }
    }else if(flag1) Usart_Tx.Tx_buffer[Usart_Tx.length++]='0';
	temp/=10;
  }Usart_Tx.Tx_buffer[Usart_Tx.length++]=' ';
  temp=10000;
  flag1=0;

  if(System_In<0) Usart_Tx.Tx_buffer[Usart_Tx.length++]='-',System_In=-System_In;
  else if(System_In==0) Usart_Tx.Tx_buffer[Usart_Tx.length++]='0',temp=0;

  while(temp!=0)
  {
    if(System_In/temp!=0)
    {
	  flag1=1;
	  Usart_Tx.Tx_buffer[Usart_Tx.length++]='0'+System_In/temp;
	  System_In-=System_In/temp*temp;
	  if(System_In==0)
	  {
		temp/=10;
	    while(temp!=0)
		{
		  Usart_Tx.Tx_buffer[Usart_Tx.length++]='0';
		  temp/=10;
		}break;
	  }
    }else if(flag1)  Usart_Tx.Tx_buffer[Usart_Tx.length++]='0';
	temp/=10;
  }
  Usart_Tx.Tx_buffer[Usart_Tx.length++]='\r';
  Usart_Tx.Tx_buffer[Usart_Tx.length]='\n';
}


void Speed_Info_Analysis(Motor_Speed_Info* Motor,uint8_t* RX_Data)
{
  Motor->Speed=(uint16_t)RX_Data[2]<<8|RX_Data[3];
  Motor->Electric=(uint16_t)RX_Data[4]<<8|RX_Data[5];
  Motor->Temperature=RX_Data[6];
}



void PID_Init(PID *pid, float Kp, float Ki, float Kd, float error_max, float dead_line, float intergral_max, float output_max)           //参数赋值与初始化
{
	pid->Kp = Kp;                      
	pid->Ki = Ki;
	pid->Kd = Kd;
	pid->error_max = error_max;       
	pid->output_max = output_max;         
	pid->dead_line = dead_line;          
	
	pid->intergral_max = intergral_max;        
	
	pid->error = 0;                  
	pid->error_last = 0;              
	pid->intergral = 0;                
	pid->derivative = 0;             
	pid->output = 0;                  
}


void PID_Speed_Cal(Speed_System* Speed_Motor,uint8_t *Tx_msg)
{

	Speed_Motor->Speed_PID.error =  Speed_Motor->Tar_Speed - Speed_Motor->Info.Speed;
	if(Speed_Motor->Speed_PID.error > Speed_Motor->Speed_PID.error_max)
		Speed_Motor->Speed_PID.error = Speed_Motor->Speed_PID.error_max;
	if(Speed_Motor->Speed_PID.error < -Speed_Motor->Speed_PID.error_max)
		Speed_Motor->Speed_PID.error = -Speed_Motor->Speed_PID.error_max;
	if(Speed_Motor->Speed_PID.error > 0 && Speed_Motor->Speed_PID.error < Speed_Motor->Speed_PID.dead_line)
		Speed_Motor->Speed_PID.error = 0;
	if(Speed_Motor->Speed_PID.error < 0 && Speed_Motor->Speed_PID.error > Speed_Motor->Speed_PID.dead_line)
		Speed_Motor->Speed_PID.error = 0;
	
	Speed_Motor->Speed_PID.intergral = Speed_Motor->Speed_PID.intergral + Speed_Motor->Speed_PID.error;
	if(Speed_Motor->Speed_PID.intergral > Speed_Motor->Speed_PID.intergral_max)
		Speed_Motor->Speed_PID.intergral = Speed_Motor->Speed_PID.intergral_max;
	if(Speed_Motor->Speed_PID.intergral < -Speed_Motor->Speed_PID.intergral_max)
		Speed_Motor->Speed_PID.intergral = -Speed_Motor->Speed_PID.intergral_max;
	
	Speed_Motor->Speed_PID.derivative = Speed_Motor->Speed_PID.error - Speed_Motor->Speed_PID.error_last;
	Speed_Motor->Speed_PID.error_last = Speed_Motor->Speed_PID.error;
	
	Speed_Motor->Speed_PID.output = Speed_Motor->Speed_PID.Kp*Speed_Motor->Speed_PID.error + Speed_Motor->Speed_PID.Ki*Speed_Motor->Speed_PID.intergral + Speed_Motor->Speed_PID.Kd*Speed_Motor->Speed_PID.derivative;
	
	if(Speed_Motor->Speed_PID.output > Speed_Motor->Speed_PID.output_max)
		Speed_Motor->Speed_PID.output = Speed_Motor->Speed_PID.output_max;
	if(Speed_Motor->Speed_PID.output < -Speed_Motor->Speed_PID.output_max)
		Speed_Motor->Speed_PID.output = -Speed_Motor->Speed_PID.output_max;
	
	Tx_msg[Speed_Motor->Motor_Num*2] = ((int16_t)Speed_Motor->Speed_PID.output)>>8;
	Tx_msg[Speed_Motor->Motor_Num*2+1] = (int16_t)Speed_Motor->Speed_PID.output;
}


void Send_To_Motor_(CAN_HandleTypeDef *hcan,uint8_t* Tx_Data)
{
  CAN_TxHeaderTypeDef TxHeader;
  uint32_t TxMailbox; 

  TxHeader.RTR = 0;
  TxHeader.IDE = 0;            
  TxHeader.StdId=0x200;
  TxHeader.TransmitGlobalTime = DISABLE;
  TxHeader.DLC = 8;
        
  if (HAL_CAN_AddTxMessage(hcan, &TxHeader, Tx_Data, &TxMailbox) != HAL_OK)
  {
     Error_Handler();
  }
}


