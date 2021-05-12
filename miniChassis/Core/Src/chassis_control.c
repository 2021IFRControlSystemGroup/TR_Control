#include "chassis_control.h"
#include "usart2_route.h"
#include <math.h>

int32_t System_Out;    
int32_t System_In;     
extern ROBO_BASE Robo_Base; 
extern ROUTE_DATA Route_Data;
 
//float a;
//float b;
//int c;
void chassis_speed_analysis(ROBO_BASE* Robo,ROUTE_DATA Route_Data)
{
	
//	  a=cos(45); //�Ƕ�ת����
//	  b=sin(45);
//	  c=Route_Data.tar_angel;
	  Robo->Speed_X = -Route_Data.tar_speed * cos( Route_Data.tar_angel);
	  Robo->Speed_Y = -Route_Data.tar_speed * sin(Route_Data.tar_angel);
    Robo->Speed_MotorLF.Tar_Speed = 1/3.0f*Route_Data.angle_speed*300.0f + 2.0f/3.0f*Robo->Speed_X ;         
		Robo->Speed_MotorLB.Tar_Speed = 1/3.0f*Route_Data.angle_speed*300.0f - 1.0f/3.0f*Robo->Speed_X  - sqrt(3.0f)*Robo->Speed_Y/3.0f;
		Robo->Speed_MotorRF.Tar_Speed = 1/3.0f*Route_Data.angle_speed*300.0f - 1.0f/3.0f*Robo->Speed_X  + sqrt(3.0f)*Robo->Speed_Y/3.0f;  

}  

//���̿��Ʋ������� 
void chassis_control(void)
{ 

	 //��õ��Ŀ���ٶ�(ң��������)
	   chassis_speed_analysis(&Robo_Base,Route_Data);
  
	 //pid�������������������can
	   PID_Send(&Robo_Base);
			 
	 //������λ��
	   SystemIO_Usart_ToString(System_Out,System_In);
	   
	   
}



