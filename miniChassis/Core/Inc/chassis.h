#ifndef __CHASSIS_H__
#define __CHASSIS_H__

#include "main.h"


#define RC_CONTROL 0
#define PC_CONTROL 1

#define LF 0
#define RF 1
#define LB 2
#define RB 3





typedef struct
{	
	float     Speed_fdb;
	float     Speed_tar;
	float     Position_fdb;
	float     Position_tar;	
	float     Torque;
	float     Temperation; 
	float     angle_cnt;	
	float     moto_output;
	
}CHASSIS_DATA;	

typedef struct
{
	int16_t Power;
	int16_t BVol;
	int16_t Curr;
	uint8_t state;
}GET_INA226_DATA;

void Chassis_Speed_Protect(void);

void Chassis_Control(void);

void PID_Chassis_Init(void);

float Power_Limit(float power,float powerbuffer);

void RC_Chassis_control(void);

void PC_Chassis_control(void);

void Extended_Integral_PID(void);

void Overall_Motion_Ratio_Protect(void);

#endif
