#ifndef __USART2_ROUTE_H__
#define __USART2_ROUTE_H__
#include "stm32f4xx.h"


#define DMA_VISION_RX_BUF_LENGTH    100u //待修改
#define DMA_VISION_TX_BUF_LENGTH    100u //待修改

typedef  struct
{
	uint8_t HEAD;
	uint8_t DATA_LEN;
	int16_t tar_x;	//x坐标
	int16_t tar_y;	//y坐标
	int16_t tar_speed;	//目标速度
	float tar_angel;	//相对角度
	float angle_speed;
}ROUTE_DATA;

//typedef  struct
//{
//	int distance1;
//	int distance2;
//	int distance3;	
//}DISTANCE;



void USART2_Vision_Init(void);
void USART2_IRQHandler(void);
void RouteData_analysis(uint8_t *pData);
void State_Flag_Send(uint8_t state_flag);
void MYDMA_Config(DMA_Stream_TypeDef *DMA_Streamx,uint32_t chx,uint32_t par,uint32_t mar,uint16_t ndtr);
void MYDMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,uint16_t ndtr);

#define VISION_DATA_DEFAULT  {0,0,0,0,0,0,0,0,0,0,0,0};
#endif
