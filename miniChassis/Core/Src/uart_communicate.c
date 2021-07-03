//????:		uart_communicate.c
//?????:	uart_communicate.h
//????:
//		????DMA??????????????, ??????DMA?????.
//
//??:
//		2020/11/27
//
//??:	1.0V
//
//??: ???
//
//????:
//		????????, ??405????????????

//---------???????---------/
#include "uart_communicate.h"
#include "chassis_control.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "usart2_route.h"
//--------------------------------//

//---------??????-----------//
UART_RX_BUFFER Uart2_Rx;
uint8_t Uart2_Tx[USART2_TX_LEN_MAX]={0};
UART_RX_BUFFER Uart6_Rx;

UART_RX_BUFFER Uart1_Rx;
extern DISTANCE distance;

//--------------------------------//

//---------????????-------//
//--------------------------------//

//--------------------------------------------------------------------------------------------------//
//????:
//		printf????
//
//????:
//		???????, ?printf??????????.
//
//????:
//		???printf??.
//
//????:
//		????????, ???HAL_UART_Transmit(&huartx, (uint8_t *)&ch, 1, 0xFFFF);??huartx???????.
//
//--------------------------------------------------------------------------------------------------//
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* ?????????????? */
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
}


UART_RX_BUFFER Uart3_Rx;
#define USART3_RX_LEN_MAX 8
void Uart3_Eable(void)
{
	Uart3_Rx.Buffer[0] = (uint8_t*) malloc ( sizeof ( uint8_t)* USART3_RX_LEN_MAX ) ;
	Uart3_Rx.Buffer[1] = (uint8_t*) malloc ( sizeof ( uint8_t)* USART3_RX_LEN_MAX ) ;
	__HAL_UART_ENABLE_IT ( &huart3 , UART_IT_IDLE ) ;
	Uart3_Rx.Buffer_Num = 0 ;
	Uart3_Rx.Length_Max = USART3_RX_LEN_MAX ;
	HAL_UART_Receive_DMA ( &huart3, Uart3_Rx.Buffer[0], USART3_RX_LEN_MAX ) ;
}

void Uart2_Eable(void)
{
	Uart2_Rx.Buffer[0] = (uint8_t*) malloc ( sizeof ( uint8_t)* USART2_RX_LEN_MAX ) ;
	Uart2_Rx.Buffer[1] = (uint8_t*) malloc ( sizeof ( uint8_t)* USART2_RX_LEN_MAX ) ;
	__HAL_UART_ENABLE_IT ( &huart2 , UART_IT_IDLE ) ;
	Uart2_Rx.Buffer_Num = 0 ;
	Uart2_Rx.Length_Max = USART2_RX_LEN_MAX ;
	HAL_UART_Receive_DMA ( &huart2, Uart2_Rx.Buffer[0], USART3_RX_LEN_MAX ) ;
}

void Uart6_Eable(void)
{
	Uart3_Rx.Buffer[0] = (uint8_t*) malloc ( sizeof ( uint8_t)* USART3_RX_LEN_MAX ) ;
	Uart3_Rx.Buffer[1] = (uint8_t*) malloc ( sizeof ( uint8_t)* USART3_RX_LEN_MAX ) ;
	__HAL_UART_ENABLE_IT ( &huart6 , UART_IT_IDLE ) ;
	Uart3_Rx.Buffer_Num = 0 ;
	Uart3_Rx.Length_Max = USART3_RX_LEN_MAX ;
	HAL_UART_Receive_DMA ( &huart6, Uart6_Rx.Buffer[0], USART6_TX_LEN_MAX ) ;
}
//--------------------------------------------------------------------------------------------------//
//????:
//		printf
//
//????:
//		???????, ?printf??????????.
//
//????:
//		???printf??.
//
//????:
//		????????, ???HAL_UART_Transmit(&huartx, (uint8_t *)&ch, 1, 0xFFFF);??huartx???????.
//
//--------------------------------------------------------------------------------------------------//
void Usart_All_Init(void)
{
//	Uart2_Rx.Buffer[0]=(uint8_t*)malloc(sizeof(uint8_t)*USART2_RX_LEN_MAX);
//	Uart2_Rx.Buffer[1]=(uint8_t*)malloc(sizeof(uint8_t)*USART2_RX_LEN_MAX);
//	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
//	Uart2_Rx.Buffer_Num = 0;
//	Uart2_Rx.Length_Max=USART2_RX_LEN_MAX;
//	HAL_UART_Receive_DMA(&huart2, Uart2_Rx.Buffer[0], USART2_RX_LEN_MAX);
//	
	Uart1_Rx.Buffer[0]=(uint8_t*)malloc(sizeof(uint8_t)*USART1_RX_LEN_MAX);
	Uart1_Rx.Buffer[1]=(uint8_t*)malloc(sizeof(uint8_t)*USART1_RX_LEN_MAX);
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
	Uart1_Rx.Buffer_Num = 0;
	Uart1_Rx.Length_Max=USART1_RX_LEN_MAX;
	HAL_UART_Receive_DMA(&huart1, Uart1_Rx.Buffer[0], USART1_RX_LEN_MAX);
}

//--------------------------------------------------------------------------------------------------//
//????:
//		??DMA??????
//
//????:
//		?????????DMA????, ????????????, 
//
//????:
//		UART_HandleTypeDef ????
//		uint8_t* ????
//		uint8_t ????
//
//????:
//		????????, ???????,?????????memcpy??????????.
//
//--------------------------------------------------------------------------------------------------//
void uart_sendData_DMA(UART_HandleTypeDef *huart, uint8_t *Data, uint8_t len)
{
	if(huart->Instance == USART2)
	{
		memcpy(Uart2_Tx, Data, len);
		HAL_UART_Transmit_DMA(&huart2, Uart2_Tx, len);
	}
}

//--------------------------------------------------------------------------------------------------//
//????:
//		??DMA??????
//
//????:
//		??DMA???, ????????????????????
//
//????:
//		UART_HandleTypeDef* ????
//		DMA_HandleTypeDef* DMA??
//		UART_RX_BUFFER* ???????
//		void(*)(uint8_t *pData) ????????
//
//--------------------------------------------------------------------------------------------------//
void Uart_DMA_Process(UART_HandleTypeDef *huart,DMA_HandleTypeDef* hdma_usart_rx,UART_RX_BUFFER* Uart_Rx,void(*DataProcessFunc)(uint8_t *pData))
{
	uint8_t this_frame_len = 0;
	
	if((__HAL_UART_GET_FLAG(huart,UART_FLAG_RXNE) != RESET))  
		{   
			__HAL_DMA_DISABLE(hdma_usart_rx);
			__HAL_UART_CLEAR_IDLEFLAG(huart);  
			
			this_frame_len = Uart_Rx->Length_Max - __HAL_DMA_GET_COUNTER(hdma_usart_rx);
			if(Uart_Rx->Buffer_Num)
			{
				Uart_Rx->Buffer_Num = 0;
				HAL_UART_Receive_DMA(huart, Uart_Rx->Buffer[0], Uart_Rx->Length_Max);
				if(this_frame_len == Uart_Rx->Length_Max)
					if(DataProcessFunc) DataProcessFunc(Uart_Rx->Buffer[1]);
			}
			else
			{
				Uart_Rx->Buffer_Num = 1;
				HAL_UART_Receive_DMA(huart, Uart_Rx->Buffer[1], Uart_Rx->Length_Max);
				if(this_frame_len == Uart_Rx->Length_Max)
//				if(this_frame_len == 0)
					if(DataProcessFunc) DataProcessFunc(Uart_Rx->Buffer[0]);
			}
		}
	
	}		
double frand(void)
{
	return 2 * ((rand() / (double)RAND_MAX) - 0.5);
}

float kalman_filter(uint16_t z_real)
{
	static float x_last = 0;
	static float p_last = 0.02;
	float Q = 0.018;
	float R = 0.542;
	float kg;
	float x_mid;
	float x_now;
	float p_mid;
	float p_now;
	float z_measure;
	int i;
	x_last = z_real + frand()*0.03;
	x_mid = x_last;
	for (i = 0; i < 20;i++)
	{
		x_mid = x_last;
		p_mid = p_last + Q;
		kg = p_mid / (p_mid + R);
		z_measure = z_real + frand()*0.03;
		x_now = x_mid + kg*(z_measure - x_mid);
  	p_now = (1 - kg)*p_mid;

		p_last = p_now;
		x_last = x_now;
	}
	return x_now;
}








extern ROUTE_DATA Route_Data;
int16_t angle_error13;

void Uart6_DMA_Process_front(uint8_t *pData)
{
	int angle;
  int HEAD;
	HEAD =  pData[0];
	if(HEAD == 0x5A)
	{
	      distance.distance1 = ((pData[2]<<8)|pData[3]) & 0xffff;
				distance.distance3 = ((pData[4]<<8)|pData[5]) & 0xffff;	
        distance.angle_error = ((pData[6]<<8)|pData[7]) & 0xffff;	
		
				angle = distance.angle_error * (3.14/180);
		    distance.y = distance.distance1 * cos(angle);
		kalman_filter(distance.y);
	}
}




void Uart_DMA_Process_53(uint8_t *pData,uint8_t number)
{
	int d;
	if(number==2)
	{
		d=(uint16_t)pData[4]<<8|pData[5];
		distance.distance2 = d * 1.1547f;
	}
	
}

