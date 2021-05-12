#ifndef __CATCHCONTROL_H__
#define __CATCHCONTROL_H__

#include "main.h"
#include "math.h"
#include "robo_base.h"
//---------------------------------//


void catch_open(void);
void catch_line_pos(void);
void catch_line_speed(void);


//״̬��
//ARM��ɵ�4

typedef enum
{
  PREPARE_STATE,			//�ϵ���ʼ��״̬ 1s������
	
	FIRST,  //1
	SECOND, //2

	CALIBRATION_LINE,		//��˱궨  //3
	CALIBRATION_CLIP,    //���ӱ궨  //6
  OPEN_STATE,			//���Ӵ�      //7
  CLOSE_STATE,			//���ӹر�     //8
	
	
	ARCHERY_WRIST,
	ARCHERY_ELBOW, 
	
	ENDPOINT_STATE,			//һ��        //9
	POS_STATE,				//���뷢��     //10
	RESTART_LINE,			//��˻ر궨��  //11
	RESTART_CLIP,			//���ӻر궨��  //12
	
   
}WorkState_e;


typedef struct POS_TEST		//���Ի�е�ṹλ�õı�����ץȡ����Ҫ����궨����¼��ȷ�ĽǶȣ�
{
	 int POS_finger;
   int POS_line;
   int POS_elbow;
   int POS_wrist;
}POS_TEST;

typedef struct ROBOT_ASSIST
{
	 int flag;  //PID_Sendѡ���ٶȻ�λ�û���1λ�û���2�ٶȻ�
	 int direction;  //�궨line����---->ʡ
	 int state;  //ȷ�Ͻ�״̬
   int row;    //����ṹ��ʼ����
	 int shot;   //����������
   int catchOK;  //ȡ���	
	 int chassis_move;
	 int Continue ;
	 int kaiguan;
   int stop; //DEBUGʱ��ĳ��״̬
}ROBOT_ASSIST;


uint8_t GetWorkStatus(void);
void Work_Mode_Change(void);
#endif


