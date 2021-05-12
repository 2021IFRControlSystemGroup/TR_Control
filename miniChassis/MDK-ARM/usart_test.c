///************uart接收遥控器数据*************/
//void USART1_IRQHandler(void)
//{
//HAL_UART_IRQHandler(&uart1Handler); 
//}


//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)//
//{
//    if(huart->Instance==USART1)//判断是哪个串口
//    {
//        //设置接收完成标志
//    if(aRxBuffer[0]!='\n')
//    {
//        if(recLength>=199)//限定长度
//        {
//            doWhenRecMsg();//执行函数
//        }
//        else
//        {
//            recData[recLength]=aRxBuffer[0];
//            recLength++;
//        }
//    }
//    else//
//    {
//        doWhenRecMsg();
//    }
//    HAL_UART_Receive_IT(&uart1Handler, (u8 *)aRxBuffer, 1);//????????
//}
//    
//}













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