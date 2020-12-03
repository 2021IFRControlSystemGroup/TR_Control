#ifndef __LED_H__
#define __LED_H__

#include "main.h"
#include "gpio.h"

void LED_ALL_Shine(void);
void GREED_ALL_Shine(void);
void RED_ALL_Shine(void);
void GREEN_Quick_Shine(void);
void LED_Task(uint8_t LED_Mode);

#endif

