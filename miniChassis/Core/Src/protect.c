////#include "control.h"
//#include "protect.h"
//#include "chassis.h"

//#define abs(x) ((x)>0? (x):(-(x)))

//ERROR_CHECK Error_Check = {LOST_CYCLE,{0},{0},{0},{0}};
//extern WORKSTATE WorkState;
//extern u32 time_tick_1ms;
////extern GET_INA226_DATA INA226_Data;

//void LostCountAdd(u16* count)
//{
//	if(*count <= 0xFFFE) 
//		(*count)++;
//}
//	

//void LostCountFeed(u16* count)
//{
//	 *count = 0;
//}

//u8 LostCountCheck(u16 count, u8* state, const u16 cyclecount)
//{
//	if(count>2*cyclecount)
//		*state=1;
//	else
//		*state=0;
//	return *state;
//}

//void DeviceFpsFeed(u16 device_id)	//设备FPS记录
//{
//	Error_Check.fps_count[device_id]++;
//}

//void DeviceFpsRecord(void)	//1s cycle
//{
//	for(int i=0;i<LOST_NUM;i++)	//无需改变
//	{
//		Error_Check.fps[i]=Error_Check.fps_count[i];
//		Error_Check.fps_count[i]=0;
//	}
//}

//void Check_Mission(void)
//{
//	
//	if(time_tick_1ms%1000==0)
//	{
//		DeviceFpsRecord();
//	}
//	
//	for(int i=0;i<LOST_NUM;i++)
//	{
//		LostCountAdd(&Error_Check.count[i]);
//		LostCountCheck(Error_Check.count[i],&Error_Check.state[i],Error_Check.cyclecount[i]);
//	}
//	
//	for(int i=5;i<LOST_NUM;i++)	
//	{
//		if(Error_Check.state[i]==1)	
//		{
//			switch(i)
//			{
//				case 4:		GPIO_ResetBits(GPIOG,GPIO_Pin_1);	break;
//				case 5:		GPIO_ResetBits(GPIOG,GPIO_Pin_2);	break;
//				case 6:		GPIO_ResetBits(GPIOG,GPIO_Pin_3);	break;
//				case 7:		GPIO_ResetBits(GPIOG,GPIO_Pin_4);	break;	
//				case 8:		GPIO_ResetBits(GPIOG,GPIO_Pin_5);	break;				
//				case 9:		GPIO_ResetBits(GPIOG,GPIO_Pin_6);	break;
//				case 10:	GPIO_ResetBits(GPIOG,GPIO_Pin_7);	break;
//			}
////			WorkState = ERROR_STATE;
//		}
//		
//				
//  }
//	if(Error_Check.state[LOST_POWER]==1)
//	{
////		INA226_Data.state = 0;
//	}

//	if(Error_Check.state[LOST_DBUS]==1)
//	{
//		GPIO_ResetBits(GPIOG,GPIO_Pin_8);
//		WorkState=PROTECT_STATE;		
//	}	
//}
