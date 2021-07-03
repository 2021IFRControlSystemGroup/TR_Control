#include "can_commmunication.h"
#include "main.h"
#include "can.h"
#include "chassis_control.h"
#include "remote_modechange.h"
#include "usart2_route.h"


int16_t angle = 0;
int16_t angle_error = 0;
extern DISTANCE distance;
extern STATE state;
extern CAN_HandleTypeDef hcan2;
extern ROUTE_DATA Route_Data;
TR_COMMUNICATION TR_commmunication = {0,0,0,0};
extern LAUNCH_STATE launch_state;


/**************
                        stdid       dlc
设： chassis->pc:       0x03         8
     chassis->catch:    0x02         4
     chassis:           0x01         2
**************/

can_TX_chassis chassis_TX;

/*************
接收到上位机传来的xyw，labviewCatchOK，
传给上位机 xyw，TRCATCHOK
*************/


void chassis_send(void)
{
   chassis_sendtoPC(&hcan2,chassis_TX.pc_tx);
	 chassis_sendtoarm(&hcan2,chassis_TX.arm_tx);

}




void chassis_sendtoPC(CAN_HandleTypeDef *hcan,uint8_t* Tx_Data)     //上位机到位置，y=1，标定结束y=2,其他时候y=0;
{
  int can_box;
  CAN_TxHeaderTypeDef TxHeader;
  uint32_t TxMailbox; 

  TxHeader.RTR = 0;
  TxHeader.IDE = 0;            

  TxHeader.TransmitGlobalTime = DISABLE;
    TxHeader.StdId=0x03;
    TxHeader.DLC = 8;

		Tx_Data[0] = ((uint8_t)((state)) & 0xffff) ;
		Tx_Data[1] = (uint8_t)((((uint16_t)(distance.y)) & 0xff00) >> 8);
		Tx_Data[2] = (uint8_t)(((uint16_t)(distance.y)) & 0xff);
		Tx_Data[3] = (uint8_t)((((uint16_t)(distance.angle_error)) & 0xff00) >> 8);
		Tx_Data[4] = (uint8_t)(((uint16_t)(distance.angle_error)) & 0xff);
		Tx_Data[5] = (uint8_t)((((int16_t)(distance.distance2)) & 0xff00) >> 8);
		Tx_Data[6] = (uint8_t)(((int16_t)(distance.distance2)) & 0xff);
		Tx_Data[7] = (uint8_t)((TR_commmunication.CatchOK) & 0xffff) ;
//	  Tx_Data[8] = (uint8_t)((launch_state) & 0xffff) ;

		 

	can_box=HAL_CAN_GetTxMailboxesFreeLevel(hcan);
	if(can_box>0)
	{
		HAL_CAN_AddTxMessage(hcan, &TxHeader, Tx_Data, &TxMailbox);
	}
}
	
//底盘和抓取的进度改成一个枚举
void chassis_sendtoarm(CAN_HandleTypeDef *hcan,uint8_t* Tx_Data)
{
	int can_box;
  CAN_TxHeaderTypeDef TxHeader;
  uint32_t TxMailbox; 

  TxHeader.RTR = 0;
  TxHeader.IDE = 0;            

    TxHeader.TransmitGlobalTime = DISABLE;
    TxHeader.StdId=0x02;
    TxHeader.DLC = 8;

	

		Tx_Data[1] = (uint8_t)((TR_commmunication.chasssisForwardOK) & 0xffff);

		
		

	can_box=HAL_CAN_GetTxMailboxesFreeLevel(hcan);
	if(can_box>0)
	{
		HAL_CAN_AddTxMessage(hcan, &TxHeader, Tx_Data, &TxMailbox);
	}
}






extern PID pid;

void chassis_receivePC(uint8_t* RX_Data,uint32_t chassis_Num)
{
   if(chassis_Num == 0x03)
	 {
        angle_error = ((RX_Data[0]<<8)|RX_Data[1]) & 0xffff	;     //和陈明昊确认是不是这些位
				Route_Data.tar_y = ((RX_Data[2]<<8)|RX_Data[3]) & 0xffff	;
				Route_Data.tar_speed = ((RX_Data[4]<<8)|RX_Data[5]) & 0xffff;	
				angle = ((RX_Data[6]<<8)|RX_Data[7]) & 0xffff;	
				Route_Data.tar_angel = angle*0.01;
	      Route_Data.angle_speed=-((angle_error/100.0f)/(12*0.006f)); 

			 
		 
// 	 PID_General_Cal(&pid,angle_error,0);
//   Route_Data.angle_speed = pid.output;
		 
			 if(Route_Data.tar_speed>8000)
			 {
					Route_Data.tar_speed=8000;
			 }
			 if(Route_Data.tar_speed<-8000)
			 {
					Route_Data.tar_speed=-8000;
       }
		 }
}

int a1 =0;
void chassis_receiveARM(uint8_t* RX_Data,uint32_t chassis_Num)
{
   if(chassis_Num == 0x03)
       // angle_error 
	      a1 = ((RX_Data[0]<<8)|RX_Data[1]) & 0xffff	;     //和陈明昊确认是不是这些位
				Route_Data.tar_y = ((RX_Data[2]<<8)|RX_Data[3]) & 0xffff	;
				Route_Data.tar_speed = ((RX_Data[4]<<8)|RX_Data[5]) & 0xffff;	
				angle = ((RX_Data[6]<<8)|RX_Data[7]) & 0xffff;	
				Route_Data.tar_angel = angle*0.01;
			 
			 if(Route_Data.tar_speed>8000)
			 {
					Route_Data.tar_speed=8000;
			 }
			 if(Route_Data.tar_speed<-8000)
			 {
					Route_Data.tar_speed=-8000;
       }
}
	


