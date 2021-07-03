#ifndef __CAN_COMMMUNICATION_H__
#define __CAN_COMMMUNICATION_H__

#include "main.h"

//这不是一个状态机，这个是做完之后返回的状态
typedef enum
{
   LABVIEW_MOVE1,
	 CHASSIS_BD,
	 CHASSIS_FORWARD,
	 CHASSIS_CATCH,
	 CHASSIS_BACKWARD,
}catch_stageOK;

void can_communication(void);



typedef struct
{
   uint8_t pc_tx[10];
	 uint8_t arm_tx[3];

}can_TX_chassis;


//没做0，做了1
typedef struct
{
    uint8_t chasssisForwardOK;
	  uint8_t catchOver;
	  uint8_t CatchOK;
    uint8_t LabviewOK;
}TR_COMMUNICATION;





void chassis_send(void);
void chassis_sendtoPC(CAN_HandleTypeDef *hcan,uint8_t* Tx_Data);
void chassis_sendtoarm(CAN_HandleTypeDef *hcan,uint8_t* Tx_Data);
void chassis_receivePC(uint8_t* RX_Data,uint32_t chassis_Num);



#endif

