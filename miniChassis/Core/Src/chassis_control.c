#include "chassis_control.h"
#include "usart2_route.h"
#include <math.h>

int32_t System_Out;    
int32_t System_In;     
extern ROBO_BASE Robo_Base; 
extern ROUTE_DATA Route_Data;
 extern DISTANCE distance;
 int chassis_w=0;
int chassis_X=0;
int chassis_Y=0;
//float a;
//float b;
int motor1;
int motor2;
int motor3;
extern int16_t angle_error;
//extern int distance1,distance2,distance3;

void chassis_speed_analysis(ROBO_BASE* Robo,ROUTE_DATA Route_Data)
{
    Route_Data.angle_speed=(angle_error/100.0f)/(12*0.006f);  //a*100/(12*0.000006);

	  Robo->Speed_X = Route_Data.tar_speed * cos( Route_Data.tar_angel);
	  Robo->Speed_Y = -Route_Data.tar_speed * sin(Route_Data.tar_angel);
    Robo->Speed_MotorLF.Tar_Speed = 1/3.0f*Route_Data.angle_speed*300.0f + 2.0f/3.0f*Robo->Speed_X ;         
		Robo->Speed_MotorLB.Tar_Speed = 1/3.0f*Route_Data.angle_speed*300.0f - 1.0f/3.0f*Robo->Speed_X  - sqrt(3.0f)*Robo->Speed_Y/3.0f;
		Robo->Speed_MotorRF.Tar_Speed = 1/3.0f*Route_Data.angle_speed*300.0f - 1.0f/3.0f*Robo->Speed_X  + sqrt(3.0f)*Robo->Speed_Y/3.0f;  

	
	  motor1=Robo->Speed_MotorLF.Info.Speed;
	  motor2=Robo->Speed_MotorLB.Info.Speed;
	  motor3=Robo->Speed_MotorRF.Info.Speed;

}  




void chassis_speed_rotate(ROBO_BASE* Robo,ROUTE_DATA Route_Data)
{
   if(distance.distance1>distance.distance2 && chassis_w==0) 
	 {
	  Robo->Speed_MotorLF.Tar_Speed = -200;         
		Robo->Speed_MotorLB.Tar_Speed = -200;
		Robo->Speed_MotorRF.Tar_Speed = -200;
	 }
     
	 else if(distance.distance1<distance.distance2 && chassis_w==0) 
   {
	  Robo->Speed_MotorLF.Tar_Speed = 200;         
		Robo->Speed_MotorLB.Tar_Speed = 200;
		Robo->Speed_MotorRF.Tar_Speed = 200;
	 }
	 else if(distance.distance1 == distance.distance2)
	 {
	  Robo->Speed_MotorLF.Tar_Speed = 0;         
		Robo->Speed_MotorLB.Tar_Speed = 0;
		Robo->Speed_MotorRF.Tar_Speed = 0;
		chassis_w=1;
	 }
} 
//底盘控制操作函数 
void chassis_control(void)
{ 
     

	   chassis_speed_analysis(&Robo_Base,Route_Data);
  
	 //pid计算输出电机结果并传给can
	   PID_Send(&Robo_Base);
			 
	 //传给上位机
	   SystemIO_Usart_ToString(System_Out,System_In);
	   
	   
}

void chassis_rotate(void)
{ 

	 //获得电机目标速度(遥控器传入)
	   chassis_speed_rotate(&Robo_Base,Route_Data);
  
	 //pid计算输出电机结果并传给can
	   PID_Send(&Robo_Base);
			 
	 //传给上位机
	   SystemIO_Usart_ToString(System_Out,System_In);
	   
	   
}
//x
void chassis_speed_x(ROBO_BASE* Robo,ROUTE_DATA Route_Data)
{
	Robo->Speed_Y = 0;
	if((int)distance.distance1>1010) Robo->Speed_X = -500;
	if((int)distance.distance1<9990) Robo->Speed_X = 500;
	if((int)distance.distance1>9990 && (int)distance.distance1<1010 ) Robo->Speed_X = 0;
	

    Robo->Speed_MotorLF.Tar_Speed = 1/3.0f*Route_Data.angle_speed*300.0f + 2.0f/3.0f*Robo->Speed_X ;         
		Robo->Speed_MotorLB.Tar_Speed = 1/3.0f*Route_Data.angle_speed*300.0f - 1.0f/3.0f*Robo->Speed_X  - sqrt(3.0f)*Robo->Speed_Y/3.0f;
		Robo->Speed_MotorRF.Tar_Speed = 1/3.0f*Route_Data.angle_speed*300.0f - 1.0f/3.0f*Robo->Speed_X  + sqrt(3.0f)*Robo->Speed_Y/3.0f;  

	
} 
//底盘控制操作函数 
void chassis_x(void)
{ 

	 //获得电机目标速度(遥控器传入)
	   chassis_speed_x(&Robo_Base,Route_Data);
  
	 //pid计算输出电机结果并传给can
	   PID_Send(&Robo_Base);
			 
	 //传给上位机
	   SystemIO_Usart_ToString(System_Out,System_In);  
}


//y
//x
void chassis_speed_y(ROBO_BASE* Robo,ROUTE_DATA Route_Data)
{
  Robo->Speed_X = 0;
	if((int)distance.distance3>1010) Robo->Speed_Y = -500;
	if((int)distance.distance3<9990) Robo->Speed_Y = 500;
	if((int)distance.distance3<1010 && (int)distance.distance3>9990) Robo->Speed_Y = 0;
	

    Robo->Speed_MotorLF.Tar_Speed = 1/3.0f*Route_Data.angle_speed*300.0f + 2.0f/3.0f*Robo->Speed_X ;         
		Robo->Speed_MotorLB.Tar_Speed = 1/3.0f*Route_Data.angle_speed*300.0f - 1.0f/3.0f*Robo->Speed_X  - sqrt(3.0f)*Robo->Speed_Y/3.0f;
		Robo->Speed_MotorRF.Tar_Speed = 1/3.0f*Route_Data.angle_speed*300.0f - 1.0f/3.0f*Robo->Speed_X  + sqrt(3.0f)*Robo->Speed_Y/3.0f;  
} 
//底盘控制操作函数 
void chassis_y(void)
{ 

	 //获得电机目标速度(遥控器传入)
	   chassis_speed_y(&Robo_Base,Route_Data);
  
	 //pid计算输出电机结果并传给can
	   PID_Send(&Robo_Base);
			 
	 //传给上位机
	   SystemIO_Usart_ToString(System_Out,System_In);  
}


void chassis_speed_stop(ROBO_BASE* Robo,ROUTE_DATA Route_Data)
{
  Robo->Speed_X = 0;
	 Robo->Speed_Y = 0;

	

    Robo->Speed_MotorLF.Tar_Speed = 1/3.0f*Route_Data.angle_speed*300.0f + 2.0f/3.0f*Robo->Speed_X ;         
		Robo->Speed_MotorLB.Tar_Speed = 1/3.0f*Route_Data.angle_speed*300.0f - 1.0f/3.0f*Robo->Speed_X  - sqrt(3.0f)*Robo->Speed_Y/3.0f;
		Robo->Speed_MotorRF.Tar_Speed = 1/3.0f*Route_Data.angle_speed*300.0f - 1.0f/3.0f*Robo->Speed_X  + sqrt(3.0f)*Robo->Speed_Y/3.0f;  
} 
//底盘控制操作函数 
void chassis_stop(void)
{ 

	 //获得电机目标速度(遥控器传入)
	   chassis_speed_stop(&Robo_Base,Route_Data);
  
	 //pid计算输出电机结果并传给can
	   PID_Send(&Robo_Base);
			 
	 //传给上位机
	   SystemIO_Usart_ToString(System_Out,System_In);  
}


