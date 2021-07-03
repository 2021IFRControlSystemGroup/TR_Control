#ifndef __REMOTE_MODECHANGE_H__
#define __REMOTE_MODECHANGE_H__



typedef  enum{
	STOPMODE,
  CATCHMODE,
	MOVELAUNCHMODE,
	DRGETMODE,
	REMOTEMODE,
}STATE;

typedef  enum{
 noL,
 ToL1,
 ToL2,
 ToL3,
 ToL4,
 ToL5,
}LAUNCH_STATE;


void chassis_send(void);
void remote_modechange(void);
void TRchassisWorking(void);



#endif


