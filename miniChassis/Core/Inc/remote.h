#ifndef __REMOTE_H__
#define __REMOTE_H__

#define PCKEY_NUM 16
#include "main.h"

typedef  struct {     

	struct{         
				uint16_t ch0;         
				uint16_t ch1;         
				uint16_t ch2;         
				uint16_t ch3;         
				uint8_t  s1;         
				uint8_t  s2;     
				}rc; 
 
     struct{        
				int16_t x;         
				int16_t y;         
				int16_t z;         
				uint8_t press_l;         
				uint8_t press_r;    
				uint16_t time_press_l;
				uint16_t time_press_r;
				}mouse; 
 
     struct{         
				uint16_t vh;  
				uint16_t vl;  
				uint8_t pc_key[PCKEY_NUM];
				uint16_t time_pckey[PCKEY_NUM];
		   }key; 
	 
	uint16_t SW;				//down1684     up364
	 
	uint8_t update;
	 
}RC_Ctl_t;

#define RC_CtrlData_Initialize \
{\
	{1024,1024,1024,1024,2,2},\
	{0},\
	{0},\
	1024,\
	0,\
}

void Remote_Init(void);
void Usart_Receive_IDLE(UART_HandleTypeDef *_huart);
void RemoteDataProcess(uint8_t *pData);

#endif


