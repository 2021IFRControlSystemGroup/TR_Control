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

//---------???????---------//
#include "uart_communicate.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
//--------------------------------//

//---------??????-----------//
UART_RX_BUFFER Uart2_Rx;
uint8_t Uart2_Tx[USART2_TX_LEN_MAX]={0};

UART_RX_BUFFER Uart1_Rx;
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
	Uart2_Rx.Buffer[0]=(uint8_t*)malloc(sizeof(uint8_t)*USART2_RX_LEN_MAX);
	Uart2_Rx.Buffer[1]=(uint8_t*)malloc(sizeof(uint8_t)*USART2_RX_LEN_MAX);
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
	Uart2_Rx.Buffer_Num = 0;
	Uart2_Rx.Length_Max=USART2_RX_LEN_MAX;
	HAL_UART_Receive_DMA(&huart2, Uart2_Rx.Buffer[0], USART2_RX_LEN_MAX);
	
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
	
	if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))  
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





