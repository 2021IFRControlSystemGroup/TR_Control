#ifndef   __CHASSIS_CONTROL_H__
#define   __CHASSIS_CONTROL_H__

#include "main.h"
#include "robo_base.h"

#define RC_CH_VALUE_MIN ((uint16_t)364 )
#define RC_CH_VALUE_OFFSET ((uint16_t)1024)
#define RC_CH_VALUE_MAX ((uint16_t)1684)

#define RC_SW_UP ((uint16_t)1)
#define RC_SW_MID ((uint16_t)3)
#define RC_SW_DOWN ((uint16_t)2)



//typedef struct 
//{
//	  float v1;
//	  float v2;
//	  float v3;
//}chassis_speed;
 
typedef struct remote_data
{
	  float speed;
	  float angle;
	  float angle_speed;
}remote_data;


//void chassis_speed_analysis(ROBO_BASE* Robo,remote_data data);
void chassis_control(void);

#endif

