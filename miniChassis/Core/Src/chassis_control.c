#include "chassis_control.h"
#include "usart2_route.h"
#include <math.h>


#define CHASSIS_SPEED_MAX 4000.0f    
  
extern CAN_HandleTypeDef hcan2;



PID pid_w={0};
PID pid_y={0};
PID pid_x={0};
DISTANCE distance = {0};
flag f;
extern RC_Ctl_t RC_CtrlData;


extern int16_t angle_error;
extern DISTANCE distance;
extern ROBO_BASE Robo_Base; 
extern ROUTE_DATA Route_Data;


/**************************************算法修改*****************************/
//chassis速度矢量,用于赋值的函数
void Vxyw_vector_upper(void)      //整车矢量转换
{
	  Robo_Base.Speed_X = Route_Data.tar_speed * cos( Route_Data.tar_angel);
	  Robo_Base.Speed_Y = -Route_Data.tar_speed * sin(Route_Data.tar_angel);
}




//根据传入的V,W分解三个轮子算法
void chassis_motor_speed(int Vx,int Vy,float w,ROBO_BASE* Robo)   //公用
{

    Robo->Speed_MotorLF.Tar_Speed = w + 2.0f/3.0f*Vx ;         
		Robo->Speed_MotorLB.Tar_Speed = w - 1.0f/3.0f*Vx  - sqrt(3.0f)*Vy/3.0f;
		Robo->Speed_MotorRF.Tar_Speed = w - 1.0f/3.0f*Vx  + sqrt(3.0f)*Vy/3.0f;  
}


void chassis_upperControl(void)   //上位机使用ok
{ 
	   Vxyw_vector_upper();
	   chassis_motor_speed(Robo_Base.Speed_X,Robo_Base.Speed_Y,Route_Data.angle_speed,&Robo_Base); //
	   PID_Send(&Robo_Base);

}


 void chassis_remoteControl(void)   //遥控器使用OK
{ 
	   Robo_Base.Speed_X=-(RC_CtrlData.rc.ch0-1024)*CHASSIS_SPEED_MAX/660.0f;
	   Robo_Base.Speed_Y=(RC_CtrlData.rc.ch1-1024)*CHASSIS_SPEED_MAX/660.0f;
	   Route_Data.angle_speed = (RC_CtrlData.rc.ch2-1024)/(10.0f*12*0.007f);
                     			 
	   chassis_motor_speed(Robo_Base.Speed_X,Robo_Base.Speed_Y,Route_Data.angle_speed,&Robo_Base); //
	   PID_Send(&Robo_Base);

}


void chassis_autoControl(int Vx,int Vy,float w,ROBO_BASE* Robo)  //无上位机，需赋值
{ 
	 
	   chassis_motor_speed(Vx, Vy, w,Robo);
	   PID_Send(&Robo_Base);
}


void chassis_stop(void) //停止用
{
	chassis_autoControl(0,0,0,&Robo_Base);
}


void duizhun_zizhaun(uint16_t w) //ok
{
 	 PID_General_Cal(&pid_w,distance.angle_error,w);
   Route_Data.angle_speed = pid_w.output;
}

void duizhun_y(uint16_t y)   //后面根据机械结构改为x  //ok
{
   float d;
	 d=distance.y;
	 PID_General_Cal(&pid_y,d,y);
	 Robo_Base.Speed_Y = -pid_y.output;
}



void duizhun_x(uint16_t x)   //后面根据机械结构改为x  //假如ok，放到函数里
{
   float d;
	 d = distance.distance2;
	 PID_General_Cal(&pid_x,d,x);
	 Robo_Base.Speed_X = pid_x.output;   //  x坐标写反
}

void flag_bsp(void)
{
  f.chassis_BD_ok = 0;
	f.chassis_advance_ok = 0;
	f.chassis_do = 0;
	f.chassis_catch_ok = 0;
}  



void chassis_map_realsence(uint16_t x,uint16_t y,uint16_t w)
{
  duizhun_zizhaun(w);
	duizhun_y(y);
	duizhun_x(x);
	chassis_autoControl(Robo_Base.Speed_X,Robo_Base.Speed_Y,Route_Data.angle_speed,&Robo_Base);
	if(Robo_Base.Speed_MotorLB.Info.Speed == 0 && Robo_Base.Speed_MotorLF.Info.Speed == 0 && Robo_Base.Speed_MotorRB.Info.Speed == 0)
	{
		f.chassis_do += 1;
	}
}







void send_to_upper(uint8_t* Tx_msg)
{
   /*传三个数据
     f.chassis_advance_ok
	   f.chassis_BD_ok
	   f.chassis_catch_ok
	    不会拆
	*/
	
//	Tx_msg[0] = ((int16_t)Speed_Motor->Speed_PID.output)>>8;
//	Tx_msg[1] = (int16_t)Speed_Motor->Speed_PID.output;
	Send_To_other(&hcan2,Robo_Base.Rx_CAN2,0X03,8);
}


void chassis_in_catchmode(void)
{
 if(Route_Data.tar_y == 0 )
      {
					chassis_upperControl();
			}
	   if(Route_Data.tar_y == 1 )
		 {
			 switch(f.chassis_do)
			 {
			   case 0: 
				 {
					 chassis_map_realsence(900,300,0); 
					 if(f.chassis_do == 1)
					 {
					   f.chassis_BD_ok = 1;   
					 }
					 break;
				 }
				 case 1: 
				 {
					 chassis_map_realsence(100,300,0); 
					 if(f.chassis_do == 2)
					 {
					   f.chassis_advance_ok = 1;  //同时爪子张开；假装直接取到箭
					 }
					 break;
				 }
				 case 2:
				 {
					 chassis_map_realsence(500,300,0); 
					 if(f.chassis_do == 2)
					 {
					   f.chassis_advance_ok = 1;  //同时爪子张开；假装直接取到箭
					 }
					 break;
				 }
			 }
			 
		  send_to_upper(Robo_Base.Tx_CAN1);
		 }
			 
			 
}



