

//#include "chassis_test.h"

//#include "main.h"
//#include "Motor.h"//接收遥控器数据，并进行解析

//chassis_t chassis;
///*上次找杆数据*/
//uint8_t last_sw1;
//uint8_t last_sw2;

///*底盘功能任务的函数*/
//void chassis_task(const void* argu);
//{
//		//初始化底盘pid参数
//		get_pid_param_init();
//		while(1)
//		{
//				//切换底盘状态
//				get_chassis_mode();
//				
//				switch(chassis.mode)
//				{
//						//底盘正常工作模式，右侧拨杆在上面
//						case CHASSIS_NORMAL;
//					{
//							chassis_control_information_get();
//					}break;
//					
//					//保持底盘锁死不动
//						default:
//						{
//								chassis.vy = 0;
//								chassis.vx = 0;
//							  chassis.vw = 0;
//						}break;
//				}
//				
//				if(chassis.mode == CHASSIS_RELAX)
//				{
//						send_chassis_moto_zero_current();
//				}
//				else
//				{
//						chassis_custom_control();
//				}
//				
//				//底盘任务周期控制
//				osdelay(10);
//		}
//}




//*/















