#ifndef __ROBOBASE_H__
#define __ROBOBASE_H__

//---------头文件包含部分----------//
#include "main.h"
#include "math.h"
//---------------------------------//

//---------#define部分-------------//
#define PI (2*acos(0))							//PI圆周率的宏定义
#define ToDegree(a) (a/PI*180)					//弧度转化成角度的宏定义
#define ToRadian(a) (a/180*PI)					//角度转化成弧度的宏定义
#define TX_LENGTH 20							//上下位机通信字符总长度
#define ROTOR_ANGLE 8192						//转子机械角度
#define GEAR_RATIO 36							//电机减速比
#define ONE_CIRCLE (ROTOR_ANGLE*GEAR_RATIO)		//电机转动一圈的总机械角度
//---------------------------------//

//---------底盘结构体部分----------//
typedef struct Motor_Pos_Info		//进行位置环控制的电机信息
{
  int16_t Speed;					//电机速度				单位(rad/min 转/每分钟)
  uint16_t Angle;					//转子机械角度
  int32_t Abs_Angle;				//转子绝对机械角度
  float Relative_Angle;				//电机相对坐标角度		单位(° 度)
  uint8_t Temperature;				//电机温度				单位(℃ 摄氏度)
  int16_t Electric;					//电流					单位(mA 毫安)
  uint16_t Last_Angle;				//上一次的转子绝对角度
}Motor_Pos_Info;

typedef struct Motor_Speed_Info		//进行速度环控制的电机信息
{
  int16_t Speed;					//电机速度				单位(rad/min 转/每分钟)
  uint8_t Temperature;				//电机温度				单位(℃ 摄氏度)
  int16_t Electric;					//电流					单位(mA 毫安)
}Motor_Speed_Info;

typedef struct pid_init_val{		//电机PID参数结构体
	
	float Kp;
	float Ki;
	float Kd;
	
	float error;					//误差
	float error_last;				//上一次误差
	float error_max;				//最大误差
	float dead_line;				//死区
	
	float intergral;				//误差积分
	float intergral_max;			//误差积分最大值
	
	float derivative;				//误差微分
	
	float output;					//输出
	float output_max;				//输出最大值
	
}PID;

typedef struct Pos_System			//位置环系统
{
  Motor_Pos_Info Info;				//位置环电机信息
  PID Pos_PID;						//位置环PID参数
  PID Speed_PID;					//速度环PID参数
  float Tar_Pos;					//目标位置
  uint8_t Motor_Num;				//电机号码
}Pos_System;

typedef struct Speed_System			//速度环系统
{
  Motor_Speed_Info Info;			//速度环电机信息
  PID Speed_PID;					//速度环PID参数
  float Tar_Speed;					//目标速度
  uint8_t Motor_Num;				//电机号码
}Speed_System;



typedef struct TX_BUFFER			//上下位机通信结构体
{
	uint8_t Tx_buffer[TX_LENGTH];	//发送数据
	uint8_t length;					//发送数据长度
}TX_BUFFER;


typedef struct Robo_Catch			//抓捕装置结构体
{
//	Speed_System SPEED_systemFinger;		//速度环--夹子
//	Speed_System SPEED_systemLine;		//速度环--横杆
  Pos_System   Pos_systemLine;        //位置环--横杆
	Pos_System   Pos_systemFinger;         //位置环--夹子
	Pos_System   Pos_systemElbow;        
	Pos_System   Pos_systemWrist; 
	
	
	uint8_t Rx_CAN1[8];         //CAN1接收数据
	uint8_t Rx_CAN2[8];         //CAN2接收数据
	uint8_t Tx_CAN1[8];         //CAN1发送数据
	uint8_t Tx_CAN2[8];            //CAN2发送数据
	
	int open_flag;     //夹子结束标志—行程开关标志
  int line_speed_flag;   //横杆标定标志-光电门标志
	
}ROBO_CATCH;



//---------------------------------//

//-------------函数声明------------//
void Pos_Info_Analysis(Motor_Pos_Info* Motor,uint8_t* RX_Data);								//位置环电机数据分析的操作函数
void Speed_Info_Analysis(Motor_Speed_Info* Motor,uint8_t* RX_Data);							//速度环电机数据分析的操作函数
void SystemIO_Usart_ToString(int32_t System_Out,int32_t System_In);							//系统输入输入输出值转化成字符的函数
void PID_Init(PID *pid, float Kp, float Ki, float Kd, float error_max, float dead_line, float intergral_max, float output_max);		//PID参数初始化函数
void CATCH_Init(ROBO_CATCH* Robo);  																								//底盘PID参数初始化的接口函数

void PID_General_Cal(PID *pid, float fdbV, float tarV,uint8_t moto_num,uint8_t *Tx_msg);	//PID计算函数----为了向下兼容
void PID_Speed_Cal(Speed_System* Speed_Motor,uint8_t *Tx_msg);								//速度环系统PID计算函数
void PID_Pos_Cal(Pos_System* Pos_Motor,uint8_t *Tx_msg);									//位置环系统PID计算函数

void PID_Send(ROBO_CATCH* Robo);																//PID发送函数
void Send_To_Motor(CAN_HandleTypeDef *hcan,uint8_t* Tx_Data);								//CAN通信发送函数


void Catch_Speed_Analysis(ROBO_CATCH* Robo,uint8_t* RX_Data,uint32_t Motor_Num);
void Catch_Pos_Analysis(ROBO_CATCH* Robo,uint8_t* RX_Data,uint32_t Motor_Num);
//---------------------------------//
#endif


