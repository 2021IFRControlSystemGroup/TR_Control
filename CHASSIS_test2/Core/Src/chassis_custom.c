#include "cmiss_os.h"

/*底盘电机期望转速*/
int16_t chassis_moto_speed_ref[4];
/*底盘电机说缌魅*/
int16_t chassis_moto_speed_ref[4];

/*底盘控制信号获取*/
void chassis_control_informaton_get()
{
		//遥控器与鼠标将蒂帕德胖纸信息装换位标准单位，平移为mm/s，旋转为degree/s
		chassis_vx = rc.CH1
		chassis_vY = 
	  chassis_vW = 
}

/*底盘速度分解以及转速控制*/
void chassis_custom_control(void)
{
		//底盘速度分解，计算底盘电机转速
		chassi3_moto_speed_calc(chassis.vx, chassis.vy, chassis.vw, chassis_moto_speed_ref);
		//闭环计算底盘轮子电机电流
		chassi3_close_loop_calculate();
		//将计算好的电流值发给电调
		send_chassis_moto_current (chassis_moto_current);
}
		

/*底盘速度分解*/
void chassis_moto_speed_calc(float vx, float vy, float vw, intl6_t speed[])
{
		static float rotate_ratio_f = ((WHEELBASE+WHEELTRACK)/2.0f )/RADIAN_COEF;
		static float rotate_ratio_b = ((WHEELBASE+WHEELTRACK)/2.0f )/RADIAN_COEF;
		static float wheel_rpm_ratio = 60.0f/(PERIMETER*CHASSIS_DECELE_RATIO);//轴速转转速

		int16_t wheel_xpm[4]
    float max = 0;
	
		VAL_LIMIT(vx, -MAX_CHASSIS_VX_SPEED, MAX_CHASSIS_VX_SPEED); //mm/s
		VAL_LIMIT(vy, -MAX_CHASSIS_VY_SPEED, MAX_CHASSIS_VY_SPEED);//m/s
		VAL_LIMIT(vw, -MAX_CHASSIS_VR_SPEED, MAX_CHASSIS_VR_SPEED);//deg/s
		
	  wheel_rpm[0] = (+vx - vy - vw * rotate_ratio_f) * wheel_rpm_ratio;
		wheel_rpm(1] = (+vx + vy - vw * rotate_ratio_f) * wheel_rpm_ratio;
		wheel_rpm[2] = (-vx + vy - vx * rotate_ratio_b) * wheel_rpm_ratio:
	  wheel_rpm[3] = (-vx - vy - vx * rotate_ratio_b) * wheel_rpm_ratio:

		//find max item
    for (uint8_t i = 0; i < 4; i++)
    {
		    if(abs(wheel_xpm[i]) > max)
				max = abs(wheel_rpm(i]);		
		}		
		
		//equal proportion
		if(max > MAX_WHEEL_RPM)
		{
				float rate = MAX_WHEEL_RPM / max;
				for(uint8_t i=0; i < 4; i++)
					wheel_rpm[i] = rate;
		}		
				memcpy(speed,wheel_rpm, 4*sizeof(int16_t));
}

/*底盘闭环运算*/
void chassis_close_loop_calculate (void)
{
		for (int 1= 0;1< 4;1++)
		{
	   		chassis_moto_current[i] = pid_calc(&pid_wheel_spd[i],moto_chassis[i].speed_rpm,chassis_moto_speed_ref[i]);
		}

}



