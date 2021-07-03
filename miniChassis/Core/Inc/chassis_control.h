#ifndef   __CHASSIS_CONTROL_H__
#define   __CHASSIS_CONTROL_H__

#include "main.h"
#include "robo_base.h"
#include "remote.h"


typedef struct remote_data
{
	  float speed;
	  float angle;
	  float angle_speed;
}remote_data;

typedef struct DISTANCE
{
	  int distance1;
	  float distance2;
	  int distance3;
	  int16_t y;
		int d13;
	
	  int16_t angle_error;
	 
}DISTANCE;



typedef enum
{
	PREPARE,
	ARMTOCATCH,
	CAHSSISCAL,
  CHASSISTOCATCH,
	CATCHING,
	CHASSISTOUPPER,
	ARMTOUPPER,
}WORKMODE_CATCH;

typedef struct
{
	int chassis_do;
  int chassis_BD_ok;
	int chassis_advance_ok;
	int chassis_catch_ok;
}flag;




void chassis_map_realsence(uint16_t x,uint16_t y,uint16_t w);
void duizhun_zizhaun(uint16_t w);
void duizhun_y(uint16_t y);   
void duizhun_x(uint16_t x);

void Vxyw_vector_upper(void);
void chassis_motor_speed(int Vx,int Vy,float w,ROBO_BASE* Robo);
void chassis_upperControl(void);
void chassis_remoteControl(void);
void chassis_autoControl(int Vx,int Vy,float w,ROBO_BASE* Robo);


void catchmodecontrol(void);
void chassis_in_catchmode(void);

void send_to_upper(uint8_t* Tx_msg);

void chassis_stop(void);
#endif

