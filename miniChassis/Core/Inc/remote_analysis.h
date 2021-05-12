#ifndef __REMOTE_ANALYSIS_H__
#define __REMOTE_ANALYSIS_H__

#include "main.h"

#define REMOTE_DATA_DEFAULT              {1024,1024,1024,1024,3,3};

#define RC_SWITCH_UP 1
#define RC_SWITCH_MIDDLE 3
#define RC_SWITCH_DOWN 2
#include "main.h"

#define  USART1_DMA_RC_BUF_LEN               20u  
     //




typedef struct
{
	struct
	{
		uint16_t ch0;
		uint16_t ch1;
		uint16_t ch2;
		uint16_t ch3;
		uint8_t switch_left;
		uint8_t switch_right;
	}rc;
	struct
	{
		int16_t x;
		int16_t y;
		int16_t z;
		uint8_t press_l;
		uint8_t press_r;
	}mouse;
	struct
	{
		uint8_t v_l;
		uint8_t v_h;
	}key;
}RC_Ctl_t;
/*
typedef struct
{
	u16 count;
	u8 value;
	u8 last;
	u8 statu;
}KeyBoardTypeDef;
*/
enum KEYBOARDID	//键位标识
{
	KEY_W,
	KEY_S,
	KEY_A,
	KEY_D,
	KEY_SHIFT,
	KEY_CTRL,
	KEY_Q,
	KEY_E,\
	\
	KEY_R,
	KEY_F,
	KEY_G,
	KEY_Z,
	KEY_X,
	KEY_C,
	KEY_V,
	KEY_B,
	KEY_NUMS,
};

enum LOST_TARGETS
{
	LOST_DBUS,
	LOST_JUDGE,	
	LOST_REFEREE,
	LOST_VISION,	
	LOST_POWER,
	LOST_CHASSIS_1,
	LOST_CHASSIS_2,
	LOST_CHASSIS_3,
	LOST_CHASSIS_4,
	LOST_PITCH,
	LOST_YAW,
	LOST_DRIVER,

	LOST_NUM,
};


typedef struct
{
	const uint16_t cyclecount[LOST_NUM];
	uint16_t count[LOST_NUM];
	uint8_t state[LOST_NUM];
	uint16_t fps[LOST_NUM];
	uint16_t fps_count[LOST_NUM];
}ERROR_CHECK;


#define RC_DATA_DEFAULT \
{\
	{1024,1024,1024,1024,3,3},\
	{0},\
	{0},\
}\

extern RC_Ctl_t RC_Ctl;
//void Key_Analysis(void);
//KeyBoardTypeDef KeyBoardData[KEY_NUMS];

void RemoteData_analysis(uint8_t *djiData);
//u8 ButtonStatu_Verdict(KeyBoardTypeDef * Key);

//void USART2_Remote_Init(void);


void LostCountFeed(uint16_t* count);
//uint32_t DMA_GetCurrentMemoryTarget(DMA_Stream_TypeDef* DMAy_Streamx);

void UsartReceive_IDLE(UART_HandleTypeDef *_huart);
void USART1_dma_IRQHandler(void);
////双缓存函数
//void (*dma_M0_rx_callback)(void);
//void (*dma_M1_rx_callback)(void);
//void dma_M0_callback(void);
//void dma_M1_callback(void);


#endif

