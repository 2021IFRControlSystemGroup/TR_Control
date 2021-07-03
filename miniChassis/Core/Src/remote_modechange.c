#include "remote_modechange.h"
#include "remote.h"
#include "can_commmunication.h"

extern RC_Ctl_t RC_CtrlData;
STATE state=0;
uint8_t last_state=0;
LAUNCH_STATE launch_state=0;

/********************************
ң����ʹ�ã�

�л�ģʽ��

s2=2:s1 = 1��ץȡģʽ
          3��ֹͣģʽ+��ʼ��
		      2�����Ӽ�
ch3 == 364 ����λ����
s1=2:s2 = 1:ң����

if( ch3 ==364 )  s1,s2�ֱ�������λ�úͲ��ƶ�λ�ã���������ʹ��ң��������С���˶���ʱ��
if��ch3 ��= 364�� s1,s2���л�ģʽʹ��

*********************************/

void remote_modechange(void)  // ����Ӷ�����ֻ�޸�״̬
{
	last_state = state;
	 if(RC_CtrlData.rc.ch3 == 364)
	{
	   state = MOVELAUNCHMODE;
		if(RC_CtrlData.rc.s2 == 3)  //MID
		{
			switch(RC_CtrlData.rc.s1 )
			{
				case 1:
				{
					launch_state = ToL1;
					break;
				}
				case 3:
				{
					launch_state = ToL2;
					break;
				}
				case 2:
				{
					launch_state = ToL3;
					break;
				}				
			}
		}
		if(RC_CtrlData.rc.s1 == 3) //MID
		{
			switch(RC_CtrlData.rc.s2 )
			{
				case 1:
				{
					launch_state = ToL4;
					break;
				}
				case 2:
				{
					launch_state = ToL5;
					break;
				}
				case 3: //MID
				{
					launch_state = noL;
					break;
				}				
			}
		}
	}
		if(RC_CtrlData.rc.ch3 != 364)
		{
			if(RC_CtrlData.rc.s2 == 2)
			{
					switch(RC_CtrlData.rc.s1)
					{
						case 1:
						{
							 state = CATCHMODE;
						}
						case 3:
						{
							 state = STOPMODE;
						}
						case 2:		
						{ 
							 state = DRGETMODE;
						}
					
					}
			}
			if(RC_CtrlData.rc.s1 == 3)
			{
				switch(RC_CtrlData.rc.s2)
				{
					case 1:
					{
						 state = REMOTEMODE;
						 break;
					}
			}
			}
		}
 
		if(last_state != state)
		{
		   chassis_send();
		}

	}	




void TRchassisWorking(void)
{
   switch(state)
	 {
		 case STOPMODE:
		 {
		 
			  break;
		 }
	   case CATCHMODE:
		 {
		   
			  break;
		 }
		 case MOVELAUNCHMODE:
		 {
			 switch(launch_state)
			 {
				 case noL:
				 {
				   
					  break;
				 }
			   case ToL1:
				 {
				    
					 break;
				 }
			    case ToL2:
				 {
				    
					 break;
				 }
				 case ToL3:
				 {
				    
					 break;
				 }
				 case ToL4:
				 {
				    
					 break;
				 }
				 case ToL5:
				 {
				    
					 break;
				 }
			 }
			  break;
		 }
	   case DRGETMODE:
		 {
		 
			  break;
		 }
		 case REMOTEMODE:
		 {
			 
			 break;
		 }
	 
	 }

}



