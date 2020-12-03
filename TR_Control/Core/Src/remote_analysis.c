/**********************************头文件*****************************************/
#include "remote_analysis.h"
#include "stm32f4xx_hal.h"
#include "main.h"

/************************************变量*****************************************/
RC_Ctl_t RC_Ctl=RC_DATA_DEFAULT;

//uint8_t  Remote_frame_rx_len = 0;      
//KeyBoardTypeDef KeyBoardData[KEY_NUMS]={0};
	
/*********************************功能实现*****************************************/

/*****************************************
2.计算遥控器通道值

函数名称：RemoteData_analysis
函数功能：对大疆遥控器数据进行移位解析
函数参数：存放遥控器数据的数组
函数返回值：无
描述： 无
*****************************************/
//extern s8 Chassis_Control_Heading;
void RemoteData_analysis(uint8_t *sbus_rx_buffer)
{
	if(sbus_rx_buffer == 0)
	{
			return;
	}

	RC_Ctl.rc.ch2 = (sbus_rx_buffer[0]| (sbus_rx_buffer[1] << 8)) & 0x07ff; //!< Channel 0
	RC_Ctl.rc.ch3 = ((sbus_rx_buffer[1] >> 3) | (sbus_rx_buffer[2] << 5)) & 0x07ff; //!< Channel 1
	RC_Ctl.rc.ch0 = ((sbus_rx_buffer[2] >> 6) | (sbus_rx_buffer[3] << 2) | (sbus_rx_buffer[4] << 10)) & 0x07ff;//!< Channel 2	
	RC_Ctl.rc.ch1 = ((sbus_rx_buffer[4] >> 1) | (sbus_rx_buffer[5] << 7)) & 0x07ff; //!< Channel 3
	
	RC_Ctl.rc.switch_left = ((sbus_rx_buffer[5] >> 4)& 0x000C) >> 2; //!< Switch left
	RC_Ctl.rc.switch_right = ((sbus_rx_buffer[5] >> 4)& 0x0003); //!< Switch right
	RC_Ctl.mouse.x = sbus_rx_buffer[6] | (sbus_rx_buffer[7] << 8); //!< Mouse X axis
	RC_Ctl.mouse.y = sbus_rx_buffer[8] | (sbus_rx_buffer[9] << 8); //!< Mouse Y axis
	RC_Ctl.mouse.z = sbus_rx_buffer[10] | (sbus_rx_buffer[11] << 8); //!< Mouse Z axis
	RC_Ctl.mouse.press_l = sbus_rx_buffer[12]; //!< Mouse Left Is Press ?
	RC_Ctl.mouse.press_r = sbus_rx_buffer[13]; //!< Mouse Right Is Press ?
	RC_Ctl.key.v_l = sbus_rx_buffer[14]; //!< KeyBoard value
	RC_Ctl.key.v_h = sbus_rx_buffer[15];
	

//	Key_Analysis();
}

/*****************************************
3.将遥控器通道值通过usart传入板子  

函数名称：RemoteData_analysis
函数功能：对大疆遥控器数据进行移位解析
函数参数：存放遥控器数据的数组
函数返回值：无
描述： 无
*****************************************/

extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma;

uint8_t uart2_rx_buff = 0;uint8_t uart1_rx_buff = 0;
volatile uint8_t rx_len = 0;             //?????????
volatile uint8_t recv_end_flag = 0;    //??????????
uint8_t rx_buffer[100]={0};   //??????
uint8_t tx_buf[20] = { 0, 1, 2, 4, 5, 6, 7, 8, 9, 10, 0, 1, 2, 4, 5, 6, 7, 8, 9, 10 };

uint8_t   USART1_DMA_RC_BUF[2][USART1_DMA_RC_BUF_LEN] = {0};      //设置二维缓存数组
uint8_t  Remote_frame_rx_len = 0;  

ERROR_CHECK Error_Check;




//遥控器数据收发
//void USART2_dma_Handler(void)
//{ 
//	//
//  if(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_IDLE) != RESET)    //判断usart传输完成的中断
//  {
//		  
//		  (void)USART2->SR;   
//		  (void)USART2->DR;
//	
//		if(DMA_GetCurrentMemoryTarget(DMA2_Stream5) == 0)     //判断哪个缓存区   return temp
//		  {
//				 Remote_frame_rx_len = USART2_DMA_RC_BUF_LEN - __HAL_DMA_GET_COUNTER(&hdma_usart2_rx);			   //	
//				
//			   __HAL_DMA_DISABLE(&hdma_usart2_rx);			        //关闭dma，连续模式下不需要
//				
//			   DMA2_Stream5->NDTR = (uint16_t)USART2_DMA_RC_BUF_LEN;    	      //
//			  //DMA_DoubleBufferModeConfig(DMA2_Stream5, (uint32_t)&USART1_DMA_RC_BUF[1][0], DMA_Memory_1);   //配置双缓冲区
//				
//				//配置双缓冲区
//				HAL_UART_Receive_DMA(&huart2, (uint8_t *) &USART2_DMA_RC_BUF[1][USART2_DMA_RC_BUF_LEN], 18);     //缓存区1
////        SET_BIT(huart1.Instance->CR3, USART_CR3_DMAR);
////        HAL_DMAEx_MultiBufferStart_IT(&hdma_usart1_rx, (uint32_t) (&USART1->RDR), (uint32_t) &Uart_Buf[0].its_buf[0], (uint32_t) &Uart_Buf[1].its_buf[0], 20); //?????
//				/****************************/
//			   __HAL_DMA_ENABLE(&hdma_usart2_rx);         //使能dma
//				 
//				 if(Remote_frame_rx_len>=18)      //
//				 {
//						LostCountFeed(&Error_Check.count[LOST_DBUS]);		    //		看门狗	 ////
//				    RemoteData_analysis(USART2_DMA_RC_BUF[0]);    //
//				
//		     }
//		else 
//		  {
//				 Remote_frame_rx_len = USART2_DMA_RC_BUF_LEN - __HAL_DMA_GET_COUNTER(&hdma_usart2_rx);				
//			   __HAL_DMA_DISABLE(&hdma_usart2_rx);
//			   DMA2_Stream5->NDTR = (uint16_t)USART2_DMA_RC_BUF_LEN;   
//			   HAL_UART_Receive_DMA(&huart2, (uint8_t *) &USART2_DMA_RC_BUF[0][USART2_DMA_RC_BUF_LEN], 18);     //缓存区0
//			   __HAL_DMA_ENABLE(&hdma_usart2_rx);
//				
//				 if(Remote_frame_rx_len>=18)	  //
//				 {					 
//						LostCountFeed(&Error_Check.count[LOST_DBUS]);					// 
//						RemoteData_analysis(USART2_DMA_RC_BUF[1]);   
//				 }					 
//		  }
//		LostCountFeed(&Error_Check.count[LOST_DBUS]);
//	}
//}
//}	




void UsartReceive_IDLE(UART_HandleTypeDef *_huart)  
{  
	uint8_t this_frame_len = 0;
	
	  if(_huart->Instance == USART1)
	  { 
		if((__HAL_UART_GET_FLAG(_huart,UART_FLAG_IDLE) != RESET))  
		{   
			__HAL_UART_CLEAR_IDLEFLAG(_huart);       //
			HAL_UART_DMAStop(&huart1);               //
			
			Remote_frame_rx_len = USART1_DMA_RC_BUF_LEN - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
			if(uart2_rx_buff)               
			{
				uart2_rx_buff = 0;
				HAL_UART_Receive_DMA(&huart1, (uint8_t *) &USART1_DMA_RC_BUF[0][USART1_DMA_RC_BUF_LEN], 18);     //缓存区0
				/*deal   uart1_rx_buff[1]*/
				if(this_frame_len == 18)
					RemoteData_analysis(USART1_DMA_RC_BUF[1]);   

			}
			else
			{
				uart2_rx_buff = 1;
				HAL_UART_Receive_DMA(&huart1, (uint8_t *) &USART1_DMA_RC_BUF[1][USART1_DMA_RC_BUF_LEN], 18);     //缓存区0
				/*deal   uart1_rx_buff[0]*/
				if(this_frame_len == 18)
					RemoteData_analysis(USART1_DMA_RC_BUF[1]);   
			}
		}  
	  }
	  
}


//看门狗
void LostCountFeed(uint16_t * count)
{
    *count = 0;
}
//	
////分配缓存区
//uint32_t DMA_GetCurrentMemoryTarget(DMA_Stream_TypeDef* DMAy_Streamx)
//{
//  static uint32_t tmp = 0;    //
//  
////  /* Check the parameters */
////  assert_param(IS_DMA_ALL_PERIPH(DMAy_Streamx));

//  /* Get the current memory target */
//  if ((DMAy_Streamx->CR & DMA_SxCR_CT) != 0)             //
//  {
//    /* Current memory buffer used is Memory 1 */
//    tmp = 1;
//  }  
//  else
//  {
//    /* Current memory buffer used is Memory 0 */
//    tmp = 0;    
//  }
//  return tmp;
//}



//void LostCountFeed()
//{
//uint16_t * count
//}
//{
//	// *count = 0
//}



//uint32_t DMA_GetCurrentMemoryTarget(DMA_Stream_TypeDef* DMAy_Streamx)
//{
//  uint32_t tmp = 0;
//  
//  /* Check the parameters */
//  assert_param(IS_DMA_ALL_PERIPH(DMAy_Streamx));

//  /* Get the current memory target */
//  if ((DMAy_Streamx->CR & DMA_SxCR_CT) != 0)
//  {
//    /* Current memory buffer used is Memory 1 */
//    tmp = 1;
//  }  
//  else
//  {
//    /* Current memory buffer used is Memory 0 */
//    tmp = 0;    
//  }
//  return tmp;
//}

//;



























/******************************串口接收dma中断************************/
//UART_HandleTypeDef huart1;
//DMA_HandleTypeDef hdma_usart1_tx;
//DMA_HandleTypeDef hdma_usart1_rx;
//DMA_HandleTypeDef hdma;

//volatile uint8_t rx_len = 0;             //?????????
//volatile uint8_t recv_end_flag = 0;    //??????????
//uint8_t rx_buffer[100]={0};   //??????

//void USART1_IRQHandler(void)    //????
//{
//    uint32_t tmp_flag = 0;
//    uint32_t temp;
//    
//    HAL_UART_IRQHandler(&huart1);
//    if(USART1 == huart1.Instance){
//        tmp_flag =__HAL_UART_GET_FLAG(&huart1,UART_FLAG_IDLE); //??IDLE???
//            
//        if((tmp_flag != RESET))//idle?????
//        {  
//            recv_end_flag = 1;  // ????????1 
//            __HAL_UART_CLEAR_IDLEFLAG(&huart1);//?????                     
// //           temp = UartHandle.Instance->SR;  //???????SR,??SR?????????SR??????
////            temp = UartHandle.Instance->DR; //???????????

//             HAL_UART_DMAStop(&huart1); //     
//            temp  =  __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);// ??DMA?????????                     
//            rx_len =  BUFFER_SIZE - temp; //?????????????,???????????
//               
//            HAL_UART_Receive_DMA(&huart1,rx_buffer,BUFFER_SIZE);//????DMA??
//        }
//    }
// }

//void USART1_IRQHandler(void)
//{
//  /* USER CODE BEGIN USART1_IRQn 0 */

//  /* USER CODE END USART1_IRQn 0 */
////  HAL_UART_IRQHandler(&huart1);
//  /* USER CODE BEGIN USART1_IRQn 1 */
//if(__HAL_UART_GET_IT_SOURCE(&huart1,UART_IT_IDLE)!=RESET) //??????????
//	{
//		__HAL_UART_CLEAR_IDLEFLAG(&huart1);//???????
//		
//	/* Current memory buffer used is Memory 0 */
//      if((((DMA_Stream_TypeDef   *)hdma_usart1_rx.Instance)->CR & DMA_SxCR_CT) == 0U)
//	  {
//		  __HAL_DMA_DISABLE(&hdma_usart1_rx);
//	       /* Transfer complete Callback for memory1 */
//		  if(dma_M1_rx_callback!=NULL)dma_M1_rx_callback();
//		__HAL_DMA_ENABLE(&hdma_usart1_rx);
//	  }
//      /* Current memory buffer used is Memory 1 */
//      else
//      {
//		  __HAL_DMA_DISABLE(&hdma_usart1_rx);
//            if(dma_M0_rx_callback!=NULL)
//			dma_M0_rx_callback();
//			
//       /* Transfer complete Callback for memory0 */
//		  __HAL_DMA_ENABLE(&hdma_usart1_rx);
//      }
//	}
//  /* USER CODE END USART1_IRQn 1 */
//}





//void Key_Analysis(void)
//{
//	
//	

//	KeyBoardData[KEY_W].value=RC_Ctl.key.v_l&0x01;
//	KeyBoardData[KEY_S].value=(RC_Ctl.key.v_l&0x02)>>1;
//	KeyBoardData[KEY_A].value=(RC_Ctl.key.v_l&0x04)>>2;
//	KeyBoardData[KEY_D].value=(RC_Ctl.key.v_l&0x08)>>3;

//	KeyBoardData[KEY_SHIFT].value=(RC_Ctl.key.v_l&0x10)>>4;
//	KeyBoardData[KEY_CTRL].value=(RC_Ctl.key.v_l&0x20)>>5;
//	KeyBoardData[KEY_Q].value=(RC_Ctl.key.v_l&0x40)>>6;
//	KeyBoardData[KEY_E].value=(RC_Ctl.key.v_l&0x80)>>7;

//	KeyBoardData[KEY_R].value=RC_Ctl.key.v_h&0x01;
//	KeyBoardData[KEY_F].value=(RC_Ctl.key.v_h&0x02)>>1;
//	KeyBoardData[KEY_G].value=(RC_Ctl.key.v_h&0x04)>>2;
//	KeyBoardData[KEY_Z].value=(RC_Ctl.key.v_h&0x08)>>3;
//	KeyBoardData[KEY_X].value=(RC_Ctl.key.v_h&0x10)>>4;
//	KeyBoardData[KEY_C].value=(RC_Ctl.key.v_h&0x20)>>5;
//	KeyBoardData[KEY_V].value=(RC_Ctl.key.v_h&0x40)>>6;
//	KeyBoardData[KEY_B].value=(RC_Ctl.key.v_h&0x80)>>7;
//	
//	for(int keyid=0;keyid<KEY_NUMS;keyid++)	//放在定时器里
//	{
//		ButtonStatu_Verdict(&KeyBoardData[keyid]);
//	}
//	

//}


//键位处理函数
//结果值：1:短按 2:长按 0:未按

//u8 ButtonStatu_Verdict(KeyBoardTypeDef * Key)	
//{																		
//	if(Key->last==1)
//	{
//		Key->count++;
//	}
//	else
//	{
//		Key->count=0;
//	}
//	
//	if(Key->count>10)	//防抖动部分 10ms
//	{
//		if(Key->count<500)	//1s
//		{
//			if(Key->last==1&&Key->value==0)
//			{
//				Key->statu=1;
//			}
//		}
//		else
//		{			
//			if(Key->last==1&&Key->value==0)
//			{
//				Key->statu=2;
//			}
//		}
//	}
//	else
//	{
//		Key->statu=0;
//	}
//	Key->last=Key->value;
//	
//	return Key->statu;
//}


