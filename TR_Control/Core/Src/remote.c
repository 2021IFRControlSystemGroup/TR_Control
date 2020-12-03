#include "remote.h"
#define LENGTH 18
uint8_t Rx_buffer[2][LENGTH]={{0}};
RC_Ctl_t RC_CtrlData=RC_CtrlData_Initialize;
extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;
uint8_t uart1_rx_buff_num=0;


void RemoteDataProcess(uint8_t *pData) 
{     	
	if(pData == 0)     
	{         
		return;     
	}          
	RC_CtrlData.rc.ch0 = ((int16_t)pData[0] | ((int16_t)pData[1] << 8)) & 0x07FF;      
	RC_CtrlData.rc.ch1 = (((int16_t)pData[1] >> 3) | ((int16_t)pData[2] << 5)) & 0x07FF;    
	RC_CtrlData.rc.ch2 = (((int16_t)pData[2] >> 6) | ((int16_t)pData[3] << 2) | ((int16_t)pData[4] << 10)) & 0x07FF;     
	RC_CtrlData.rc.ch3 = (((int16_t)pData[4] >> 1) | ((int16_t)pData[5]<<7)) & 0x07FF;          
	RC_CtrlData.rc.s1 = ((pData[5] >> 4) & 0x000C) >> 2;     
	RC_CtrlData.rc.s2 = ((pData[5] >> 4) & 0x0003); 
 
    RC_CtrlData.mouse.x = ((int16_t)pData[6]) | ((int16_t)pData[7] << 8);     
	RC_CtrlData.mouse.y = ((int16_t)pData[8]) | ((int16_t)pData[9] << 8);     
	RC_CtrlData.mouse.z = ((int16_t)pData[10]) | ((int16_t)pData[11] << 8);     
 
    RC_CtrlData.mouse.press_l = pData[12];     
	RC_CtrlData.mouse.press_r = pData[13];       
	RC_CtrlData.key.vh = ((int16_t)pData[14]);
	RC_CtrlData.key.vl = ((int16_t)pData[15]);      
	RC_CtrlData.SW = (uint16_t)((pData[17]<<8)|pData[16]);
	
	RC_CtrlData.update = 1;
}

void Usart_Receive_IDLE(UART_HandleTypeDef *_huart)
{
	uint8_t this_frame_len=0;
	if(_huart->Instance == USART1)
	  { 
		if((__HAL_UART_GET_FLAG(_huart,UART_FLAG_IDLE) != RESET))  
		{   
			__HAL_DMA_DISABLE(&hdma_usart1_rx);
			__HAL_UART_CLEAR_IDLEFLAG(_huart);  
			
			this_frame_len = LENGTH - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
			if(uart1_rx_buff_num)
			{
				uart1_rx_buff_num = 0;
				HAL_UART_Receive_DMA(&huart1, Rx_buffer[0], LENGTH);
				/*deal   uart1_rx_buff[1]*/
				if(this_frame_len == 18)
					RemoteDataProcess(Rx_buffer[1]);
			}
			else
			{
				uart1_rx_buff_num = 1;
				HAL_UART_Receive_DMA(&huart1, Rx_buffer[1], LENGTH);
				if(this_frame_len == 18)
					RemoteDataProcess(Rx_buffer[0]);
			}
		}
	}
}

void Remote_Init(void)
{
	__HAL_UART_ENABLE_IT(&huart1,UART_IT_IDLE);
	HAL_UART_Receive_DMA(&huart1,Rx_buffer[0],LENGTH);
}


