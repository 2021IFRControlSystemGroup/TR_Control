#include "robo_base.h"
#include "can.h"
#include "Catch_control.h"
#include "main.h"


POS_TEST POS;  
ROBOT_ASSIST catch_assist;
WorkState_e Work_Status = PREPARE_STATE;
extern ROBO_CATCH robo_catch;
int last_Open;
int last_Line;
int last_ARM1;
int last_ARM2;



//获取状态
uint8_t GetWorkStatus(void)
{
	return Work_Status;
}


/*
目标由上一次状态确定，目标不改变
本电机的速度
pid进状态多次才结束转动
夹子打开为负关闭为正
*/
void Work_Mode_Change(void)
{	
		switch(GetWorkStatus())
	{
		case PREPARE_STATE: //0
		{
			  robo_catch.Pos_systemLine.Tar_Pos=0;
				robo_catch.Pos_systemFinger.Tar_Pos=0;
			  robo_catch.Pos_systemElbow.Tar_Pos=0;
			  robo_catch.Pos_systemWrist.Tar_Pos=0;
			
		    catch_assist.flag = 1;
        Work_Status = FIRST;
			
			  break;
		}
//elbow
		case FIRST:   //01     ok   肘关节直接到取箭位置     
		{
      catch_assist.state = 11;		
      
			if(catch_assist.state == 11)
				 {
									 
							robo_catch.Pos_systemLine.Tar_Pos=0;
							robo_catch.Pos_systemFinger.Tar_Pos=0;
							robo_catch.Pos_systemElbow.Tar_Pos=-850000;//
							robo_catch.Pos_systemWrist.Tar_Pos=0;
								 
							catch_assist.flag = 1;
							static int count = 0;
							count++;
							if((robo_catch.Pos_systemElbow.Info.Speed==0)  &&  (robo_catch.Pos_systemWrist.Info.Speed)==0  &&  count>100  &&  catch_assist.stop==0)   //   和电机pid参数有关，把pid参数设大一点  &&robo_catch.Pos_system1.Info.Abs_Angle<-700000
							{
									Work_Status = SECOND;	
									count = 0;
							}		
			}			
	    break;
		}
		
		case SECOND:   //02     ok        
		{
      catch_assist.state = 12;		
      
			if(catch_assist.state == 12)
				 {
					    robo_catch.Pos_systemLine.Tar_Pos=0;
							robo_catch.Pos_systemFinger.Tar_Pos=0;
							robo_catch.Pos_systemElbow.Tar_Pos=-850000;   //-750000 -100000    POS.POS_elbow
							robo_catch.Pos_systemWrist.Tar_Pos=140000;   //140000
		 
						  catch_assist.flag = 1;
							static int count = 0;
							count++;
							if((robo_catch.Pos_systemElbow.Info.Speed==0)  &&  (robo_catch.Pos_systemWrist.Info.Speed)==0  &&  count>100  &&  catch_assist.stop==0)   //   和电机pid参数有关，把pid参数设大一点robo_catch.Pos_system1.Info.Abs_Angle<-700000
							{
									if( catch_assist.chassis_move == 1 )   //   和电机pid参数有关，把pid参数设大一点  //底盘移动：遥控器切换三个标定状态
									{

											Work_Status =   CALIBRATION_LINE;	
											count = 0;
									}		
							}	
						}							
	    break;
		}
//		  case THIRD:   //3     ok        
//		{
//      state = 13;		
//      
//			if(state == 13)
//				 {
//						robo_catch.Pos_system1.Tar_Pos = -100000 ;     
//            robo_catch.Pos_system2.Tar_Pos = 10000 ;   					 
//						flag = 1;
//				static int count = 0;
//				count++;
//				if(robo_catch.Pos_systemOpen.Info.Speed == 0 && count > 100)   //   和电机pid参数有关，把pid参数设大一点
//			  {

//            Work_Status = FORTH;	
////					   last_ARM1= robo_catch.Pos_systemOpen.Info.Abs_Angle;      //为了让目标值不变，改成上一个状态确定大小
////					   last_ARM2= robo_catch.Pos_systemOpen.Info.Abs_Angle; 
//					   count = 0;
//				}		
//			}			
//	    break;
//		}
//		case FORTH:   //04     ok        
//		{
//      state = 14;		
//      
//			if(state == 14)
//				 {
//						robo_catch.Pos_system1.Tar_Pos = 20000 ;     
//            robo_catch.Pos_system2.Tar_Pos = 20000 ;   					 
//						flag = 1;
//				static int count = 0;
//				count++;
//				if(robo_catch.Pos_systemOpen.Info.Speed == 0 && count > 100)   //   和电机pid参数有关，把pid参数设大一点
//			  {

//         //   Work_Status = CALIBRATION_LINE;	
////					   last_ARM1= robo_catch.Pos_systemOpen.Info.Abs_Angle;      //为了让目标值不变，改成上一个状态确定大小
////					   last_ARM2= robo_catch.Pos_systemOpen.Info.Abs_Angle; 
//					   count = 0;
//				}		
//			}			
//	    break;
//		}
//		
		
		
//CATCH		
		case CALIBRATION_LINE:  //3  OK
		{ 
			catch_assist.state=1;
			
			if(catch_assist.kaiguan==0) //robo_catch.line_speed_flag
			{ 
				if(catch_assist.direction)   //被位置环占，猜测pidinit被位置环覆盖 不是。   猜测分线板can1有问题
				{
					robo_catch.Pos_systemLine.Tar_Pos+= POS.POS_line;  //5000
				  robo_catch.Pos_systemFinger.Tar_Pos=0;
					catch_assist.flag=1;	 
					
					static int count = 0;
				  count++;
					if(robo_catch.Pos_systemLine.Info.Speed == 0&& count > 3) //500和-500 之间切换而静止
					{
					     catch_assist.direction=0;
						   robo_catch.Pos_systemLine.Info.Abs_Angle=0;
					}
//				Work_Status = CALIBRATION_LINE;
				}else
				{
				  robo_catch.Pos_systemLine.Tar_Pos-= POS.POS_line;  //5000
				  robo_catch.Pos_systemFinger.Tar_Pos=0;
				
					catch_assist.flag=1;
					static int count = 0;
				  count++;
					if(robo_catch.Pos_systemLine.Info.Speed  == 0 && count > 3 )
					{
					     catch_assist.direction=1; 
						robo_catch.Pos_systemLine.Info.Abs_Angle=0;
				
			
					}
				}
			    
				  
			}
			else
				{
				  Work_Status = CALIBRATION_CLIP;
					 robo_catch.Pos_systemLine.Tar_Pos= 0;  //5000
				  robo_catch.Pos_systemFinger.Tar_Pos=0;
					last_Open = robo_catch.Pos_systemFinger.Info.Abs_Angle;    
				  last_Line = robo_catch.Pos_systemLine.Info.Abs_Angle;
				
				}					
					
			break;
		}
		case CALIBRATION_CLIP:  //4  ok
		{
			catch_assist.state = 1;
			
			if(robo_catch.open_flag == 1)
			{
			   robo_catch.Pos_systemLine.Tar_Pos= last_Line;  //5000
				  robo_catch.Pos_systemFinger.Tar_Pos+=POS.POS_finger;
		catch_assist.flag = 1;
			}	
			if(robo_catch.open_flag == 0)
			{
				  catch_assist.state=2;
			    robo_catch.Pos_systemLine.Info.Abs_Angle=0;
			    robo_catch.Pos_systemFinger.Info.Abs_Angle=0;
				
			    catch_assist.flag = 0;
				  Work_Status = OPEN_STATE;
				
				  robo_catch.Pos_systemLine.Tar_Pos=0;
			    robo_catch.Pos_systemFinger.Tar_Pos=0;
				
				  last_Open = robo_catch.Pos_systemFinger.Info.Abs_Angle;    
				  last_Line = robo_catch.Pos_systemLine.Info.Abs_Angle;
				
			}
			break;
		}
		case OPEN_STATE:   //5     ok        
		{
      catch_assist.state = 3;		
			if(catch_assist.state == 3)
				 {
					    robo_catch.Pos_systemFinger.Tar_Pos=600000;
					    robo_catch.Pos_systemLine.Tar_Pos=0;
							robo_catch.Pos_systemElbow.Tar_Pos=-850000;   //-750000 -100000    
							robo_catch.Pos_systemWrist.Tar_Pos=140000;   //140000
		 
            
						  catch_assist.flag = 1;
							static int count = 0;
							count++;
							if(robo_catch.Pos_systemFinger.Info.Speed==0  &&  \
								 robo_catch.Pos_systemLine.Info.Speed==0  &&  \
							   robo_catch.Pos_systemElbow.Info.Speed==0  &&  \
								 robo_catch.Pos_systemWrist.Info.Speed==0  &&  \
							    count>100  &&  catch_assist.stop==0)   //   
							{
									 catch_assist.catchOK=0;
									 Work_Status = CLOSE_STATE;	
									 last_Open = robo_catch.Pos_systemFinger.Info.Abs_Angle;      //为了让目标值不变，改成上一个状态确定大小
									 count = 0;
								   catch_assist.chassis_move = 0;
							}		
			}			
	    break;
		}
  //底盘远离箭架
		
		case CLOSE_STATE:  //6    ：利用光电门切换到，
		{	
			if(catch_assist.catchOK && catch_assist.chassis_move == 1 )   //   取到箭并且底盘远离箭架回到全场定位坐标原点
			
					  if(catch_assist.state == 4) //抓取到置位4
			      {
								robo_catch.Pos_systemFinger.Tar_Pos=0;
								robo_catch.Pos_systemLine.Tar_Pos=0;
								robo_catch.Pos_systemElbow.Tar_Pos=-850000;   //-750000 -100000    
								robo_catch.Pos_systemWrist.Tar_Pos=-10000;   //140000
														
				        catch_assist.flag = 1;   
								static int count = 0;
								count++;
								if(robo_catch.Pos_systemFinger.Info.Speed==0  &&  \
								   robo_catch.Pos_systemLine.Info.Speed==0  &&  \
							     robo_catch.Pos_systemElbow.Info.Speed==0  &&  \
								   robo_catch.Pos_systemWrist.Info.Speed==0  &&  \
							     count>100  &&  catch_assist.stop==0) 
								{
								/*
								    如果关闭
									时没遇到开关，灯不亮
								    遇到开关提示夹子关闭时没误差，灯亮
								*/
										 Work_Status = ENDPOINT_STATE;	
										 last_Line = robo_catch.Pos_systemLine.Info.Abs_Angle;     //为了让目标值不变，改成上一个状态确定大小
										 count = 0;
										 catch_assist.catchOK=0;
				       }
					 }			
			break;
		}
		
		
		case ARCHERY_WRIST:   //07     顺序移动到close完之后，-750000
		{
      catch_assist.state = 11;		
      
			if(catch_assist.state == 11)
				 {
					 
							robo_catch.Pos_systemFinger.Tar_Pos=0r;  
							robo_catch.Pos_systemLine.Tar_Pos=0;
							robo_catch.Pos_systemElbow.Tar_Pos=-850000;   //-750000 -100000    
							robo_catch.Pos_systemWrist.Tar_Pos=-10000;   //140000
							catch_assist.flag = 1;
							static int count = 0;
				      count++;
							if(robo_catch.Pos_systemFinger.Info.Speed==0  &&  \
												 robo_catch.Pos_systemLine.Info.Speed==0  &&  \
												 robo_catch.Pos_systemElbow.Info.Speed==0  &&  \
												 robo_catch.Pos_systemWrist.Info.Speed==0  &&  \
												 count>100  &&  catch_assist.stop==0) 
							{
									 if(catch_assist.Continue) //
									 {
											 Work_Status = ENDPOINT_STATE;	
											 count = 0;
											 catch_assist.Continue=0;
									 }
				}		
			}			
	    break;
		}
		//移动到发射位置
		case ARCHERY_ELBOW:   //08     ok   肘关节直接到取箭位置     
		{
       catch_assist.state = 11;		
            

			if( catch_assist.state == 11)
				 {
						    robo_catch.Pos_systemFinger.Tar_Pos=0;
								robo_catch.Pos_systemLine.Tar_Pos=0;
								robo_catch.Pos_systemElbow.Tar_Pos=-150000;   //-750000 -100000    
								robo_catch.Pos_systemWrist.Tar_Pos=-10000;   //140000
            					 
						    catch_assist.flag = 1;
				        static int count = 0;
				        count++;
							 if(robo_catch.Pos_systemFinger.Info.Speed==0  &&  \
								   robo_catch.Pos_systemLine.Info.Speed==0  &&  \
							     robo_catch.Pos_systemElbow.Info.Speed==0  &&  \
								   robo_catch.Pos_systemWrist.Info.Speed==0  &&  \
							     count>100  &&  catch_assist.stop==0) 
							 {
										Work_Status =  ARCHERY_WRIST;	
										 count = 0;
								}		
			}			
	    break;
		}
		
		
		case ENDPOINT_STATE:    //9  OK     
		{
			static int count = 0;
				  count++;
			catch_assist.state = 5;
			if(catch_assist.state == 5)    
			{
			    robo_catch.Pos_systemLine.Tar_Pos = 1100000;//1000000
				  robo_catch.Pos_systemFinger.Tar_Pos =  0;   
			    catch_assist.flag = 1;
				  static int count = 0;    //判断结束
				  count++;
				  if(robo_catch.Pos_systemFinger.Info.Speed==0  &&  \
								   robo_catch.Pos_systemLine.Info.Speed==0  &&  \
							     robo_catch.Pos_systemElbow.Info.Speed==0  &&  \
								   robo_catch.Pos_systemWrist.Info.Speed==0  &&  \
							     count>100  &&  catch_assist.stop==0) 
					{       
						if(catch_assist.row)   //在一端就位，第一个发射位置，row=1可以发射
						{
							 catch_assist.catchOK=0;
							 catch_assist.row = 0;
							 catch_assist.shot = 1;            //编写判断：接触好后为1
							 count = 0;
							 Work_Status = POS_STATE;
							 last_Line = robo_catch.Pos_systemLine.Info.Abs_Angle;//为了让目标值不变，改成上一个状态确定大小
						}
				}		
			}
			break;
		}
		
		
		case POS_STATE:   //10      横杆移动，箭到发射位置，row表示可以发射，第一箭在一端时发射，shot表示已经发射完的
		{
			catch_assist.state = 6;
			
			switch(catch_assist.shot)
			{
				
				case 1://第一个已经射完
				{
							static int count = 0;
							count++;
							robo_catch.Pos_systemLine.Tar_Pos = 240000;//1000000
							robo_catch.Pos_systemFinger.Tar_Pos =  0; 
							robo_catch.Pos_systemElbow.Tar_Pos=-150000;   //-750000 -100000    
							robo_catch.Pos_systemWrist.Tar_Pos=-10000;   //140000
							catch_assist.flag = 1;									
						 
				      if(robo_catch.Pos_systemFinger.Info.Speed==0  &&  \
								   robo_catch.Pos_systemLine.Info.Speed==0  &&  \
							     robo_catch.Pos_systemElbow.Info.Speed==0  &&  \
								   robo_catch.Pos_systemWrist.Info.Speed==0  &&  \
							     count>100  &&  catch_assist.stop==0) 
							{
									 if(catch_assist.row)  //可以放第二个
										 catch_assist.shot =2;
									 if(catch_assist.shot == 2)
									 {
											 count = 0;
											 last_Line = robo_catch.Pos_systemLine.Info.Abs_Angle;
											 catch_assist.row = 0;
									 }
							}
							break;
					}
				
					case 2:
				{
					    static int count = 0;
							count++;
							robo_catch.Pos_systemLine.Tar_Pos =10000;//1000000
							robo_catch.Pos_systemFinger.Tar_Pos =  POS.POS_finger+last_Open; 
							robo_catch.Pos_systemElbow.Tar_Pos=-150000;   //-750000 -100000    
							robo_catch.Pos_systemWrist.Tar_Pos=-10000;   //140000
							catch_assist.flag = 1;									
						 
				      if(robo_catch.Pos_systemFinger.Info.Speed==0  &&  \
								   robo_catch.Pos_systemLine.Info.Speed==0  &&  \
							     robo_catch.Pos_systemElbow.Info.Speed==0  &&  \
								   robo_catch.Pos_systemWrist.Info.Speed==0  &&  \
							     count>100  &&  catch_assist.stop==0) 
							{
									 if(catch_assist.row)  //可以放第二个
										 catch_assist.shot =3;
									 if(catch_assist.shot == 3)
									 {
											 count = 0;
											 last_Line = robo_catch.Pos_systemLine.Info.Abs_Angle;
											 catch_assist.row = 0;
									 }
							}
							break;
					}
				case 3:
				{
					    static int count = 0;
							count++;
							robo_catch.Pos_systemLine.Tar_Pos = 800000;//1000000
							robo_catch.Pos_systemFinger.Tar_Pos =  0; 
							robo_catch.Pos_systemElbow.Tar_Pos=-150000;   //-750000 -100000    
							robo_catch.Pos_systemWrist.Tar_Pos=-10000;   //140000
							catch_assist.flag = 1;									
						 
				      if(robo_catch.Pos_systemFinger.Info.Speed==0  &&  \
								   robo_catch.Pos_systemLine.Info.Speed==0  &&  \
							     robo_catch.Pos_systemElbow.Info.Speed==0  &&  \
								   robo_catch.Pos_systemWrist.Info.Speed==0  &&  \
							     count>100  &&  catch_assist.stop==0) 
							{
									 if(catch_assist.row)  //可以放第二个
										 catch_assist.shot =4;
									 if(catch_assist.shot == 4)
									 {
											 count = 0;
											 last_Line = robo_catch.Pos_systemLine.Info.Abs_Angle;
											 catch_assist.row = 0;
									 }
							}
							break;
					}
				case 4:
				{
							static int count = 0;
							count++;
							robo_catch.Pos_systemLine.Tar_Pos = -1150000;//1000000
							robo_catch.Pos_systemFinger.Tar_Pos =  POS.POS_finger+last_Open; 
							robo_catch.Pos_systemElbow.Tar_Pos=-150000;   //-750000 -100000    
							robo_catch.Pos_systemWrist.Tar_Pos=-10000;   //140000
							catch_assist.flag = 1;							
						  
				     if(robo_catch.Pos_systemFinger.Info.Speed==0  &&  \
								robo_catch.Pos_systemLine.Info.Speed==0  &&  \
							  robo_catch.Pos_systemElbow.Info.Speed==0  &&  \
								robo_catch.Pos_systemWrist.Info.Speed==0  &&  \
							  count>100  &&  catch_assist.stop==0) 
						 {
								if(catch_assist.row)
								   catch_assist.shot =5;
								if(catch_assist.shot == 5	)
								{
										count = 0;
										last_Line = robo_catch.Pos_systemLine.Info.Abs_Angle;
										catch_assist.row=0;
										Work_Status = RESTART_LINE;
										catch_assist.catchOK=0;
						     }							
							}
							break;
					}
				}	
			break;
			}
     
			
	/*
			添加一个函数：让位置环电机结束
			*/
//消除误差重新标定			
		case RESTART_LINE:  //11              OK           //夹子打开-，关闭+
		{
			robo_catch.Pos_systemLine.Tar_Pos=0;
				robo_catch.Pos_systemFinger.Tar_Pos=0;
			  robo_catch.Pos_systemElbow.Tar_Pos=-150000;   //-750000 -100000    
				robo_catch.Pos_systemWrist.Tar_Pos=-10000;
			
		    catch_assist.flag = 1;
        Work_Status = FIRST;
			break;
		 }
		case RESTART_CLIP:   //01     ok   肘关节直接到取箭位置     
		{
      catch_assist.state = 11;		
      
			if(catch_assist.state == 11)
				 {
									 
							robo_catch.Pos_systemLine.Tar_Pos=0;
							robo_catch.Pos_systemFinger.Tar_Pos=0;
							robo_catch.Pos_systemElbow.Tar_Pos=-850000;//
							robo_catch.Pos_systemWrist.Tar_Pos=-10000;
								 
							catch_assist.flag = 1;
							static int count = 0;
							count++;
							if((robo_catch.Pos_systemElbow.Info.Speed==0)  &&  (robo_catch.Pos_systemWrist.Info.Speed)==0  &&  count>100  &&  catch_assist.stop==0)   //   和电机pid参数有关，把pid参数设大一点  &&robo_catch.Pos_system1.Info.Abs_Angle<-700000
							{
									Work_Status = SECOND;	
									count = 0;
							}		
			}			
	    break;
		}
		
	}
}



