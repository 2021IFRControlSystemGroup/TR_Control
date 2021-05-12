#ifndef __CATCHCONTROL_H__
#define __CATCHCONTROL_H__

#include "main.h"
#include "math.h"
#include "robo_base.h"
//---------------------------------//


void catch_open(void);
void catch_line_pos(void);
void catch_line_speed(void);


//状态机
//ARM完成到4

typedef enum
{
  PREPARE_STATE,			//上电后初始化状态 1s钟左右
	
	FIRST,  //1
	SECOND, //2

	CALIBRATION_LINE,		//横杆标定  //3
	CALIBRATION_CLIP,    //夹子标定  //6
  OPEN_STATE,			//夹子打开      //7
  CLOSE_STATE,			//夹子关闭     //8
	
	
	ARCHERY_WRIST,
	ARCHERY_ELBOW, 
	
	ENDPOINT_STATE,			//一端        //9
	POS_STATE,				//对齐发射     //10
	RESTART_LINE,			//横杆回标定处  //11
	RESTART_CLIP,			//夹子回标定处  //12
	
   
}WorkState_e;


typedef struct POS_TEST		//调试机械结构位置的变量（抓取不需要额外标定，记录正确的角度）
{
	 int POS_finger;
   int POS_line;
   int POS_elbow;
   int POS_wrist;
}POS_TEST;

typedef struct ROBOT_ASSIST
{
	 int flag;  //PID_Send选择速度环位置环，1位置环，2速度环
	 int direction;  //标定line方向---->省
	 int state;  //确认进状态
   int row;    //发射结构开始发射
	 int shot;   //五次射箭个数
   int catchOK;  //取完箭	
	 int chassis_move;
	 int Continue ;
	 int kaiguan;
   int stop; //DEBUG时在某个状态
}ROBOT_ASSIST;


uint8_t GetWorkStatus(void);
void Work_Mode_Change(void);
#endif


