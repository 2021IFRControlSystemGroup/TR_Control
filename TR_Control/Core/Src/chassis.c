//#include "main.h"
//#include "chassis.h"
//#include "remote_analysis.h"

////#define abs(x) ((x)>0? (x):(-(x)))     

//#define CHASSIS_SPEED_MAX 4000.0f       
////#define PC_MAX_SPEED      4000.0f       

//#define Vw_max 8000
//#define Vx_max 2000
//#define Vy_max 2000
//#define Vw_top 6000


////extern YUN_DATA Yun_data[2];
////CHASSIS_DATA Chassis_Motor_Data[4]={0};
////extern PID_TypeDef pid_position[2];
////extern PID_TypeDef pid_speed[2];
////PID_TypeDef pid[4]={0};
////PID_TypeDef PID_Chassis_Fellow={0};
////s16 Vx;
////s16 Vy;
////s16 Vw;
////u16 yaw_tar = 0;
////u8 Chassis_Control_PCorRC=RC_CONTROL;
////extern WORKSTATE WorkState;
////u8 niuyao_flag=0;

////float Top_error=0;
////float Vx_cos=0;
////float Vx_sin=0;
////float Vy_sin=0;
////float Vy_cos=0;
////float Vx_top=0;
////float Vy_top=0;
////extern u32 time_tick_1ms;
///*
//void PID_Chassis_Init(void)
//{
//	//                              kp    ki           kd      Output_max     deviation_max      Int_Err_max    Input_max
//	PID_Init(&pid[0],             5.0f,    0.05f,    0.06f,           9000,             6000,      300000,       9000);
//	PID_Init(&pid[1],             5.0f,    0.05f,    0.06f,           9000,             6000,      300000,       9000);
//	PID_Init(&pid[2],             5.0f,    0.05f,    0.06f,           9000,             6000,      300000,       9000);
//	PID_Init(&pid[3],             5.0f,    0.05f,    0.06f,           9000,             6000,      300000,       9000);
//	PID_Init(&PID_Chassis_Fellow, 4.0f,        0,    0.20f,           9000,             6000,      300000,       9000);
//}
//*/
////void Chassis_RC_Mode(void)
////{
////	
////	s16 x_beyond_rc = 0;
////	s16 y_beyond_rc = 0;
////	s16 xy_beyond_rc = 0;
////	
////	Vx=(RC_Ctl.rc.ch1-1024)*CHASSIS_SPEED_MAX/660.0f;
////	Vy=(RC_Ctl.rc.ch0-1024)*CHASSIS_SPEED_MAX/660.0f;
////	if(WorkState==TOP_STATE)
////	{
////		if(Vx>Vx_max)  Vx=Vx_max;
////		if(Vx<-Vx_max) Vx=-Vx_max;		
////		if(Vy>Vy_max)  Vy=Vy_max;		
////		if(Vy<-Vy_max) Vy =-Vy_max;	
////		
////		x_beyond_rc = abs(Vx) - 1000;
////		y_beyond_rc = abs(Vy) - 1000;
////		
////		xy_beyond_rc = x_beyond_rc + y_beyond_rc;	
////		Vw = -4500 + xy_beyond_rc;		
////		 Vw = Vw_top;
////		
////	}
////	
////	Top_error=Top_error_angle(&Yun_data[YAW].Position_fdb);

////	Vx_cos=cos(Top_error/57.29578f);
////	Vx_sin=sin(Top_error/57.29578f);
////	Vy_cos=cos(Top_error/57.29578f);
////	Vy_sin=sin(Top_error/57.29578f);
////	
////	Vx_top=0.95f*(Vx*Vx_cos-Vy*Vy_sin);
////	Vy_top=Vy*Vy_cos+Vx*Vx_sin;
////	
////	Chassis_Motor_Data[0].Speed_tar=Vx_top+Vy_top+Vw;
////	Chassis_Motor_Data[1].Speed_tar=-Vx_top+Vy_top+Vw;
////	Chassis_Motor_Data[2].Speed_tar=-Vx_top-Vy_top+Vw;
////	Chassis_Motor_Data[3].Speed_tar=Vx_top-Vy_top+Vw;
////	
////}

////void Chassis_PC_Mode(void)                  //W,A,S,D控制车子前后左右移动
////{
////	if(RC_Ctl.rc.switch_left == RC_SWITCH_MIDDLE)  WorkState = STOP_STATE;
////	
////	if(KeyBoardData[KEY_W].value!=0)
////	{
////		if(Vx<PC_MAX_SPEED&&Vx>=0)
////		{
////			Vx=Vx+20;
////		}
////		else if(Vx<0)	
////		{
////			Vx=0;
////		}			
////	}
////	else if(KeyBoardData[KEY_S].value!=0)
////	{
////		if(Vx>-PC_MAX_SPEED&&Vx<=0)
////		{
////			Vx=Vx-20;
////		}
////		else if(Vx>0)	
////		{
////			Vx=0;
////		}		
////	}
////	else
////	{
////		Vx=0;
////	}


////	if(KeyBoardData[KEY_D].value!=0)
////	{
////		if(Vy<PC_MAX_SPEED&&Vy>= 0)
////		{
////			Vy=Vy + 10;
////		}
////		else if(Vy < 0)	
////		{
////			Vy=0;
////		}			
////	}
////	else if(KeyBoardData[KEY_A].value!=0)
////	{
////		if(Vy>-PC_MAX_SPEED&&Vy <= 0)
////		{
////			Vy=Vy - 10;
////		}
////		else if(Vy > 0)	
////		{
////			Vy = 0;
////		}		
////	}
////	else
////	{
////		Vy=0;
////	}
////	
////	if(WorkState==TOP_STATE)
////	{
////		if(Vx>Vx_max)  Vx=Vx_max;
////		if(Vx<-Vx_max) Vx=-Vx_max;		
////		if(Vy>Vy_max)  Vy=Vy_max;		
////		if(Vy<-Vy_max) Vy =-Vy_max;	
////		Vw=Vw_top;
////	}
////	
////	Top_error=Top_error_angle(&Yun_data[YAW].Position_fdb);

////	Vx_cos=cos(Top_error/57.29578f);
////	Vx_sin=sin(Top_error/57.29578f);
////	Vy_cos=cos(Top_error/57.29578f);
////	Vy_sin=sin(Top_error/57.29578f);
////	
////	Vx_top=Vx*Vx_cos-Vy*Vy_sin;
////	Vy_top=Vy*Vy_cos+Vx*Vx_sin;
////	
////	Chassis_Motor_Data[0].Speed_tar=Vx_top+Vy_top+Vw;
////	Chassis_Motor_Data[1].Speed_tar=-Vx_top+Vy_top+Vw;
////	Chassis_Motor_Data[2].Speed_tar=-Vx_top-Vy_top+Vw;
////	Chassis_Motor_Data[3].Speed_tar=Vx_top-Vy_top+Vw;
////}


////void Chassis_Control(void)
////{
////		if(WorkState!=CHECK_STATE&&WorkState!=STOP_STATE)	//模式切换
////	{
////		if(RC_Ctl.key.v_h!=0||RC_Ctl.key.v_l!=0)
////		{
////			Chassis_Control_PCorRC=PC_CONTROL;
////		}
////		else if(abs(RC_Ctl.rc.ch0-1024)>3||abs(RC_Ctl.rc.ch1-1024)>3||abs(RC_Ctl.rc.ch2-1024)>3||abs(RC_Ctl.rc.ch3-1024)>3)
////		{
////			Chassis_Control_PCorRC=RC_CONTROL;
////		}
////	}

////	if(WorkState==NORMAL_STATE||WorkState==TOP_STATE||WorkState==SMART_STATE||WorkState==STOP_STATE)
////	{
////		if(Chassis_Control_PCorRC==RC_CONTROL)
////			Chassis_RC_Mode();
////		else if(Chassis_Control_PCorRC==PC_CONTROL)
////			Chassis_PC_Mode();
////	}
////	if(WorkState!=TOP_STATE) 
////	{
////		yaw_tar =  YAW_INIT;
////		if(time_tick_1ms%500==0&&RC_Ctl.rc.switch_right==RC_SWITCH_UP)
////		{
////			if(niuyao_flag%2==0)
////			yaw_tar=YAW_INIT-680;
////			if(niuyao_flag%2!=0)
////			yaw_tar=YAW_INIT+680;
////			niuyao_flag++;
////		}
////			
////			if(yaw_tar-Yun_data[YAW].Position_fdb>4096)	
////			{
////			Vw=PID_Algorithm(&PID_Chassis_Fellow,yaw_tar,Yun_data[YAW].Position_fdb+8192);
////			}
////			else if(yaw_tar-Yun_data[YAW].Position_fdb<-4096)
////			{		
////				Vw=PID_Algorithm(&PID_Chassis_Fellow,yaw_tar,Yun_data[YAW].Position_fdb-8192);	
////			}
////			else
////			{
////				Vw=PID_Algorithm(&PID_Chassis_Fellow,yaw_tar,Yun_data[YAW].Position_fdb);
////			}
////			
////			if(yaw_tar<Yun_data[YAW].Position_fdb)
////			Vw= PID_Algorithm(&PID_Chassis_Fellow,yaw_tar,Yun_data[YAW].Position_fdb);
////			else
////			Vw=PID_Algorithm(&PID_Chassis_Fellow,yaw_tar,Yun_data[YAW].Position_fdb);
////			if(yaw_tar-Yun_data[YAW].Position_fdb>-50&&yaw_tar-Yun_data[YAW].Position_fdb<50)
////			Vw=0;
////	}
////	if(Vw>=Vw_max)Vw=Vw_max;
////	else if(Vw <=-Vw_max)	Vw =-Vw_max;	
////	Chassis_Speed_Protect();
////	Chassis_Motor_Data[0].moto_output=PID_Algorithm(&pid[0],Chassis_Motor_Data[0].Speed_tar,Chassis_Motor_Data[0].Speed_fdb);
////	Chassis_Motor_Data[1].moto_output=PID_Algorithm(&pid[1],Chassis_Motor_Data[1].Speed_tar,Chassis_Motor_Data[1].Speed_fdb);
////	Chassis_Motor_Data[2].moto_output=PID_Algorithm(&pid[2],Chassis_Motor_Data[2].Speed_tar,Chassis_Motor_Data[2].Speed_fdb);
////	Chassis_Motor_Data[3].moto_output=PID_Algorithm(&pid[3],Chassis_Motor_Data[3].Speed_tar,Chassis_Motor_Data[3].Speed_fdb);
////	
////}

////void Chassis_Speed_Protect(void)                       //整体轮速比例保护
////{
////	s32 chassis_speed_max=0;
////	float chassis_protect_ratio=1;
////	
////	chassis_speed_max=Chassis_Motor_Data[0].Speed_tar>Chassis_Motor_Data[1].Speed_tar?Chassis_Motor_Data[0].Speed_tar:Chassis_Motor_Data[1].Speed_tar;
////	chassis_speed_max=chassis_speed_max>Chassis_Motor_Data[2].Speed_tar?chassis_speed_max:Chassis_Motor_Data[2].Speed_tar;
////	chassis_speed_max=chassis_speed_max>Chassis_Motor_Data[3].Speed_tar?chassis_speed_max:Chassis_Motor_Data[3].Speed_tar;
////	
////	chassis_protect_ratio=CHASSIS_SPEED_MAX/chassis_speed_max;
////	if(chassis_protect_ratio>=1)	chassis_protect_ratio=1;
////		Chassis_Motor_Data[0].Speed_tar*=chassis_protect_ratio;
////		Chassis_Motor_Data[1].Speed_tar*=chassis_protect_ratio;
////		Chassis_Motor_Data[2].Speed_tar*=chassis_protect_ratio;
////		Chassis_Motor_Data[3].Speed_tar*=chassis_protect_ratio;
////}		

