/**
  ******************************************************************************
  * File Name          : TIM.c
  * Description        : This file provides code for the configuration
  *                      of the TIM instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "tim.h"

/* USER CODE BEGIN 0 */
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include "chassis_control.h"
#include "usart2_route.h"
#include <math.h>
/* USER CODE END 0 */

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

/* TIM2 init function */
void MX_TIM2_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 83;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

}
/* TIM3 init function */
void MX_TIM3_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 84-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 999;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(tim_baseHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspInit 0 */

  /* USER CODE END TIM2_MspInit 0 */
    /* TIM2 clock enable */
    __HAL_RCC_TIM2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**TIM2 GPIO Configuration
    PA0-WKUP     ------> TIM2_CH1
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* TIM2 interrupt Init */
    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
  /* USER CODE BEGIN TIM2_MspInit 1 */

  /* USER CODE END TIM2_MspInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspInit 0 */

  /* USER CODE END TIM3_MspInit 0 */
    /* TIM3 clock enable */
    __HAL_RCC_TIM3_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**TIM3 GPIO Configuration
    PA6     ------> TIM3_CH1
    PA7     ------> TIM3_CH2
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* TIM3 interrupt Init */
    HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM3_IRQn);
  /* USER CODE BEGIN TIM3_MspInit 1 */

  /* USER CODE END TIM3_MspInit 1 */
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspDeInit 0 */

  /* USER CODE END TIM2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM2_CLK_DISABLE();

    /**TIM2 GPIO Configuration
    PA0-WKUP     ------> TIM2_CH1
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0);

    /* TIM2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM2_IRQn);
  /* USER CODE BEGIN TIM2_MspDeInit 1 */

  /* USER CODE END TIM2_MspDeInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspDeInit 0 */

  /* USER CODE END TIM3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM3_CLK_DISABLE();

    /**TIM3 GPIO Configuration
    PA6     ------> TIM3_CH1
    PA7     ------> TIM3_CH2
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_6|GPIO_PIN_7);

    /* TIM3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM3_IRQn);
  /* USER CODE BEGIN TIM3_MspDeInit 1 */

  /* USER CODE END TIM3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */


/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

//TIM_HandleTypeDef htim3;
//extern DISTANCE distance;

//double frand(void)
//{
//	return 2 * ((rand() / (double)RAND_MAX) - 0.5);
//}

//float kalman_filter(uint16_t z_real)
//{
//	static float x_last = 0;
//	static float p_last = 0.02;
//	float Q = 0.018;
//	float R = 0.542;
//	float kg;
//	float x_mid;
//	float x_now;
//	float p_mid;
//	float p_now;
//	float z_measure;
//	int i;
//	x_last = z_real + frand()*0.03;
//	x_mid = x_last;
//	for (i = 0; i < 20;i++)
//	{
//		x_mid = x_last;
//		p_mid = p_last + Q;
//		kg = p_mid / (p_mid + R);
//		z_measure = z_real + frand()*0.03;
//		x_now = x_mid + kg*(z_measure - x_mid);
//		p_now = (1 - kg)*p_mid;

//		p_last = p_now;
//		x_last = x_now;
//	}
//	return x_now;
//}

//uint8_t rising_flag = 0; //…œ…˝—ÿ≤∂ªÒ±Íº«Œª
//uint8_t rising_flag2 = 0;
//uint32_t cap_value1 = 0;
//uint32_t cap_value2 = 0;
//uint32_t cap_value3 = 0;
//uint32_t cap_value4 = 0;

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//  if( htim->Instance == TIM3)
//	{
//	   if(rising_flag)
//		 {
//		     cap_value1 +=1000000; //√ø¥Œ“Á≥ˆ◊ˆ“ª∏ˆº”ARRµƒº«¬º
//			 cap_value2 += 1000000;
////			 cap_value3 += 1000000;
//			 cap_value4 += 1000000;
//		 }
//	}
//	if( htim->Instance == TIM2)
//	{
//	   if(rising_flag2)
//		 {
//		     cap_value3 += 1000000;
//		 }
//	}
//}

////uint16_t distance=0;
//void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
//{
//	if( htim->Instance == TIM3)
//	{
//		if(!rising_flag)        //≤∂ªÒ…œ…˝—ÿ
//		{
//		  rising_flag=1;
//			__HAL_TIM_DISABLE(htim);
//			__HAL_TIM_SET_COUNTER(htim,0);
//			TIM_RESET_CAPTUREPOLARITY(htim,TIM_CHANNEL_1);//«Â≥˝…œ¥Œ…Ë÷√
//			TIM_RESET_CAPTUREPOLARITY(htim,TIM_CHANNEL_2);//«Â≥˝…œ¥Œ…Ë÷√
//			TIM_RESET_CAPTUREPOLARITY(htim,TIM_CHANNEL_3);//«Â≥˝…œ¥Œ…Ë÷√
//      TIM_RESET_CAPTUREPOLARITY(htim,TIM_CHANNEL_4);//«Â≥˝…œ¥Œ…Ë÷√

//			TIM_SET_CAPTUREPOLARITY(htim,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING);  //…Ë÷√≤∂ªÒœ¬Ωµ—ÿ
//			TIM_SET_CAPTUREPOLARITY(htim,TIM_CHANNEL_2,TIM_ICPOLARITY_FALLING);  //…Ë÷√≤∂ªÒœ¬Ωµ—ÿ
//			TIM_SET_CAPTUREPOLARITY(htim,TIM_CHANNEL_3,TIM_ICPOLARITY_FALLING);  //…Ë÷√≤∂ªÒœ¬Ωµ—ÿ
//			TIM_SET_CAPTUREPOLARITY(htim,TIM_CHANNEL_4,TIM_ICPOLARITY_FALLING);  //…Ë÷√≤∂ªÒœ¬Ωµ—ÿ
//			
//			__HAL_TIM_ENABLE(htim);
//		}
//    else                    //≤∂ªÒœ¬Ωµ—ÿ£¨±æ¥Œ≤∂ªÒΩ· ¯
//		{
//			uint32_t value;
//		  rising_flag=0;
//			cap_value1= HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);   //cap_value +  “ª√Î£∫cap_value/1000000£¨∫¡√Î£∫cap_value/%1000000/1000£¨Œ¢√Îcap_value?%1000
//			cap_value2= HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);   //cap_value +  “ª√Î£∫cap_value/1000000£¨∫¡√Î£∫cap_value/%1000000/1000£¨Œ¢√Îcap_value?%1000
////    cap_value3= HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);   //cap_value +  “ª√Î£∫cap_value/1000000£¨∫¡√Î£∫cap_value/%1000000/1000£¨Œ¢√Îcap_value?%1000
//      cap_value4= HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);   //cap_value +  “ª√Î£∫cap_value/1000000£¨∫¡√Î£∫cap_value/%1000000/1000£¨Œ¢√Îcap_value?%1000
//			
//			
//			distance.distance1 = cap_value1/10;
//			distance.distance2 = cap_value2/10;
////			distance.distance3 = cap_value3/10;
//			distance.distance4 = cap_value4/10;
//			
//			kalman_filter(distance.distance1);
//			kalman_filter(distance.distance2);
//			kalman_filter(distance.distance3);
//			kalman_filter(distance.distance4);
//			__HAL_TIM_DISABLE(htim);
////			__HAL_TIM_SET_COUNTER(htim,0);
//			TIM_RESET_CAPTUREPOLARITY(htim,TIM_CHANNEL_1);//«Â≥˝…œ¥Œ…Ë÷√
//			TIM_SET_CAPTUREPOLARITY(htim,TIM_CHANNEL_1,TIM_ICPOLARITY_RISING);  //…Ë÷√≤∂ªÒ…œ…˝—ÿ
//			TIM_RESET_CAPTUREPOLARITY(htim,TIM_CHANNEL_2);//«Â≥˝…œ¥Œ…Ë÷√
//			TIM_SET_CAPTUREPOLARITY(htim,TIM_CHANNEL_2,TIM_ICPOLARITY_RISING);  
//			TIM_RESET_CAPTUREPOLARITY(htim,TIM_CHANNEL_3);//«Â≥˝…œ¥Œ…Ë÷√
//			TIM_SET_CAPTUREPOLARITY(htim,TIM_CHANNEL_3,TIM_ICPOLARITY_RISING);  
//			TIM_RESET_CAPTUREPOLARITY(htim,TIM_CHANNEL_4);//«Â≥˝…œ¥Œ…Ë÷√
//			TIM_SET_CAPTUREPOLARITY(htim,TIM_CHANNEL_4,TIM_ICPOLARITY_RISING);  
//			
//			
//			__HAL_TIM_ENABLE(htim);
//	   
//		}
//		}
//	
//		if( htim->Instance == TIM2)
//	{
//		if(!rising_flag2)        //≤∂ªÒ…œ…˝—ÿ
//		{
//		  rising_flag2=1;
//			__HAL_TIM_DISABLE(htim);
//			__HAL_TIM_SET_COUNTER(htim,0);
//			TIM_RESET_CAPTUREPOLARITY(htim,TIM_CHANNEL_1);//«Â≥˝…œ¥Œ…Ë÷√
//			
//			TIM_SET_CAPTUREPOLARITY(htim,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING);  //…Ë÷√≤∂ªÒœ¬Ωµ—ÿ
//			
//			__HAL_TIM_ENABLE(htim);
//		}
//    else                    //≤∂ªÒœ¬Ωµ—ÿ£¨±æ¥Œ≤∂ªÒΩ· ¯
//		{
//			uint32_t value;
//		  rising_flag2=0;
//			 cap_value3= HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);   //cap_value +  “ª√Î£∫cap_value/1000000£¨∫¡√Î£∫cap_value/%1000000/1000£¨Œ¢√Îcap_value?%1000
//      
//			
//			distance.distance3 = cap_value3/10;
//			kalman_filter(distance.distance3);
//			__HAL_TIM_DISABLE(htim);
////			__HAL_TIM_SET_COUNTER(htim,0);
//			TIM_RESET_CAPTUREPOLARITY(htim,TIM_CHANNEL_1);//«Â≥˝…œ¥Œ…Ë÷√
//			TIM_SET_CAPTUREPOLARITY(htim,TIM_CHANNEL_1,TIM_ICPOLARITY_RISING);  //…Ë÷√≤∂ªÒœ¬Ωµ—ÿ
//			
//			
//			__HAL_TIM_ENABLE(htim);
//	   
//		}
//		}
//	}

	
	

/* TIM3 init function */
//void MX_TIM3_Init(void)
//{
//  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
//  TIM_MasterConfigTypeDef sMasterConfig = {0};

//  htim3.Instance = TIM3;
//  htim3.Init.Prescaler = 83;
//  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
//  htim3.Init.Period = 999;
//  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
//  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
//  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
//  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
//  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
//  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
//  {
//    Error_Handler();
//  }

//}

//void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
//{

//  if(tim_baseHandle->Instance==TIM3)
//  {
//  /* USER CODE BEGIN TIM3_MspInit 0 */

//  /* USER CODE END TIM3_MspInit 0 */
//    /* TIM3 clock enable */
//    __HAL_RCC_TIM3_CLK_ENABLE();

//    /* TIM3 interrupt Init */
//    HAL_NVIC_SetPriority(TIM3_IRQn, 5, 0);
//    HAL_NVIC_EnableIRQ(TIM3_IRQn);
//  /* USER CODE BEGIN TIM3_MspInit 1 */

//  /* USER CODE END TIM3_MspInit 1 */
//  }
//}

//void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
//{

//  if(tim_baseHandle->Instance==TIM3)
//  {
//  /* USER CODE BEGIN TIM3_MspDeInit 0 */

//  /* USER CODE END TIM3_MspDeInit 0 */
//    /* Peripheral clock disable */
//    __HAL_RCC_TIM3_CLK_DISABLE();

//    /* TIM3 interrupt Deinit */
//    HAL_NVIC_DisableIRQ(TIM3_IRQn);
//  /* USER CODE BEGIN TIM3_MspDeInit 1 */

//  /* USER CODE END TIM3_MspDeInit 1 */
//  }
//}


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

uint8_t rising_flag = 0; //œ¬Ωµ—ÿ≤∂ªÒ±Íº«Œª
uint32_t cap_value = 0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if( htim->Instance == TIM3)
	{
	   if(rising_flag)
		 {
		     cap_value +=1000000; //√ø¥Œ“Á≥ˆ◊ˆ“ª∏ˆº”ARRµƒº«¬º
		 }
	}
}

uint16_t Distance=0;
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if( htim->Instance == TIM3)
	{
		if(!rising_flag)        //≤∂ªÒ…œ…˝—ÿ
		{
		  rising_flag=1;
			__HAL_TIM_DISABLE(htim);
			__HAL_TIM_SET_COUNTER(htim,0);
			TIM_RESET_CAPTUREPOLARITY(htim,TIM_CHANNEL_1);//«Â≥˝…œ¥Œ…Ë÷√
			TIM_SET_CAPTUREPOLARITY(htim,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING);  //…Ë÷√≤∂ªÒœ¬Ωµ—ÿ
			
			__HAL_TIM_ENABLE(htim);
		}
    else                    //≤∂ªÒœ¬Ωµ—ÿ£¨±æ¥Œ≤∂ªÒΩ· ¯
		{
			uint32_t value;
		  rising_flag=0;
			cap_value= HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);   //cap_value +  “ª√Î£∫cap_value/1000000£¨∫¡√Î£∫cap_value/%1000000/1000£¨Œ¢√Îcap_value¨%1000
			Distance = cap_value/10;
			kalman_filter(Distance);
			__HAL_TIM_DISABLE(htim);
			TIM_RESET_CAPTUREPOLARITY(htim,TIM_CHANNEL_1);//«Â≥˝…œ¥Œ…Ë÷√
			TIM_SET_CAPTUREPOLARITY(htim,TIM_CHANNEL_1,TIM_ICPOLARITY_RISING);  //…Ë÷√≤∂ªÒœ¬Ωµ—ÿ
			
			__HAL_TIM_ENABLE(htim);
	   
		}
		}
	}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
