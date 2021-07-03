#include "usart2_route.h"
//#include "stdio.h"	

ROUTE_DATA Route_Data =  {0,0,0,0,0,0};    //位运算分析函数 

//int16_t angle;
//int16_t angle_error;
//float k;
//extern int time_flag;
//void RouteData_analysis(uint8_t *pData)    //位运算数据拼接
//{	

//	if(pData == 0)
//	{
//		return;
//	}	
//		  Route_Data.HEAD =  pData[0];
//	   if(Route_Data.HEAD == 165)
//		 {
//				Route_Data.DATA_LEN = pData[1];
//				
//			 }angle_error = ((pData[0]<<8)|pData[1]) & 0xffff	;     //和陈明昊确认是不是这些位
//				Route_Data.tar_y = ((pData[2]<<8)|pData[3]) & 0xffff	;
//				Route_Data.tar_speed = ((pData[4]<<8)|pData[5]) & 0xffff;	
//				angle = ((pData[6]<<8)|pData[7]) & 0xffff;	
//				Route_Data.tar_angel = angle*0.01;
//			 
//			 
//			 if(Route_Data.tar_speed>8000)
//			 {
//					Route_Data.tar_speed=8000;
//			 }
//			 if(Route_Data.tar_speed<-8000)
//			 {
//					Route_Data.tar_speed=-8000;
//			 }
//			 
//			
//}




