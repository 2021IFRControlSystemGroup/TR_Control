#include "led.h"
extern uint32_t SYS_Time;
void LED_Task(uint8_t LED_Mode)
{
  switch(LED_Mode)
  {
	  case 0:LED_ALL_Shine();break;
	  case 1:GREED_ALL_Shine();break;
	  case 2:RED_ALL_Shine();break;
	  case 3:GREEN_Quick_Shine();break;
  }
}

void LED_ALL_Shine(void)
{
  HAL_GPIO_WritePin(GPIOC, LED_RED_Pin|LED_GREEN_Pin, GPIO_PIN_SET);
}

void GREED_ALL_Shine(void)
{
  HAL_GPIO_WritePin(GPIOC, LED_GREEN_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOC, LED_RED_Pin, GPIO_PIN_RESET);
}

void RED_ALL_Shine(void)
{
  HAL_GPIO_WritePin(GPIOC, LED_RED_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOC, LED_GREEN_Pin, GPIO_PIN_RESET);
}

void GREEN_Quick_Shine(void)
{
  HAL_GPIO_WritePin(GPIOC, LED_RED_Pin, GPIO_PIN_RESET);
  if(SYS_Time%100<=10) HAL_GPIO_WritePin(GPIOC, LED_GREEN_Pin, GPIO_PIN_SET);
  else HAL_GPIO_WritePin(GPIOC, LED_GREEN_Pin, GPIO_PIN_RESET);
}
