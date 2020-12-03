/*****************************头文件******************************************/
//#include <math.h>
#include "chassis_control.h"
//#include "chassis.h"
#include "remote.h"
//#include "remote_analysis.h"

/******************************变量声明*************************************/
int32_t System_Out;
int32_t System_In;

//chassis_speed * chassis_targe;

extern ROBO_BASE Robo_Base; 
remote_data data;
extern RC_Ctl_t RC_CtrlData;

#define CHASSIS_SPEED_MAX 4000.0f    

/*****************************功能实现*********************************************/
void chassis_speed_analysis(ROBO_BASE* Robo,RC_Ctl_t RC_CtrlData)
{
	  Robo->Speed_X=(RC_CtrlData.rc.ch0-1024)*CHASSIS_SPEED_MAX/660.0f;
	  Robo->Speed_Y=(RC_CtrlData.rc.ch1-1024)*CHASSIS_SPEED_MAX/660.0f;
	
    Robo->Speed_MotorLF.Tar_Speed = 1/3.0f*data.angle_speed*300.0f + 2.0f/3.0f*Robo->Speed_X ;         
		Robo->Speed_MotorLB.Tar_Speed = 1/3.0f*data.angle_speed*300.0f - 1.0f/3.0f*Robo->Speed_X  - sqrt(3.0f)*Robo->Speed_Y/3.0f;
		Robo->Speed_MotorRF.Tar_Speed = 1/3.0f*data.angle_speed*300.0f - 1.0f/3.0f*Robo->Speed_X  + sqrt(3.0f)*Robo->Speed_Y/3.0f;  

}  

void chassis_control(void)
{ 

	 //获得电机目标速度(遥控器传入)
	   chassis_speed_analysis(&Robo_Base,RC_CtrlData);
	
	  
	 //pid计算输出电机结果并传给can
	   PID_Send(&Robo_Base);
			 
	 //传给上位机
	   SystemIO_Usart_ToString(System_Out,System_In);
	   
	   
}

