/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "can.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "robo_base.h"
#include "Catch_control.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define KEY_A   HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)  //光电门，标定结束
#define KEY_B   HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern ROBO_CATCH robo_catch;
extern POS_TEST POS;  
extern ROBOT_ASSIST catch_assist;
CAN_TxHeaderTypeDef TxMessage;
CAN_RxHeaderTypeDef RxMessage;
extern CAN_HandleTypeDef hcan1;
uint32_t Txmailbox;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	robo_catch.line_speed_flag=1;
	robo_catch.open_flag=1;
	catch_assist.direction = 0;
  catch_assist.catchOK = 0;
  catch_assist.chassis_move=0;
  catch_assist.Continue = 0;
  catch_assist.kaiguan=0; 
  catch_assist.stop=0;	
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
	
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_CAN1_Init();
  MX_TIM2_Init();
  MX_CAN2_Init();
  /* USER CODE BEGIN 2 */
	//CAN_FilterConfig();
	CAN_Filter_Init(&hcan1);
  HAL_CAN_Start(&hcan1);
  HAL_CAN_ActivateNotification(&hcan1,CAN_IT_RX_FIFO0_MSG_PENDING);
  
  CAN_Filter_Init(&hcan2);
  HAL_CAN_Start(&hcan2);
  HAL_CAN_ActivateNotification(&hcan2,CAN_IT_RX_FIFO0_MSG_PENDING);
	
	HAL_TIM_Base_Start_IT(&htim2);
	
 //抓捕装置
 					CATCH_Init(&robo_catch);   //移动回主函数

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
//   if(0 == KEY_A)
//   {
//   //  HAL_Delay(10);
//     robo_catch.line_speed_flag = 0;   //横杆标定标志-光电门标志
//     while(!KEY_A);
//     robo_catch.line_speed_flag = 1;
//	 } 
//		 if(0 == KEY_B)
//   {
//     HAL_Delay(10);
//     robo_catch.open_flag=0;            //夹子标定标志-开关标志
//     while(!KEY_B);
//     robo_catch.open_flag=1;   
//		} 
    /* USER CODE BEGIN 3 */

	 }
	
	
  /* USER CODE END 3 */

	 }



/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
//		if(hcan->Instance==CAN1)
//		{
//			CAN_RxHeaderTypeDef RxMeg;
//			HAL_CAN_GetRxMessage(&hcan1,CAN_RX_FIFO0,&RxMeg,robo_catch.Rx_CAN1);
//			Catch_Speed_Analysis(&robo_catch,robo_catch.Rx_CAN1,RxMeg.StdId);

//		}
	if(hcan->Instance==CAN2)
	{
			CAN_RxHeaderTypeDef RxMeg;
			HAL_CAN_GetRxMessage(&hcan2,CAN_RX_FIFO0,&RxMeg,robo_catch.Rx_CAN2);
			Catch_Pos_Analysis(&robo_catch,robo_catch.Rx_CAN2,RxMeg.StdId);
	}
 
}
int ele_flag=1;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
        if (htim->Instance == htim2.Instance)
        {
					if(robo_catch.Pos_systemElbow.Info.Electric<10000 )ele_flag=1;
					else ele_flag=0;
//					CATCH_Init(&robo_catch);   //移动回主函数
          Work_Mode_Change();					
	        PID_Send(&robo_catch);   
        }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
        while (1)
  {
		
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
