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



//��ȡ״̬
uint8_t GetWorkStatus(void)
{
	return Work_Status;
}


/*
Ŀ������һ��״̬ȷ����Ŀ�겻�ı�
��������ٶ�
pid��״̬��βŽ���ת��
���Ӵ�Ϊ���ر�Ϊ��
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
		case FIRST:   //01     ok   ��ؽ�ֱ�ӵ�ȡ��λ��     
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
							if((robo_catch.Pos_systemElbow.Info.Speed==0)  &&  (robo_catch.Pos_systemWrist.Info.Speed)==0  &&  count>100  &&  catch_assist.stop==0)   //   �͵��pid�����йأ���pid�������һ��  &&robo_catch.Pos_system1.Info.Abs_Angle<-700000
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
							if((robo_catch.Pos_systemElbow.Info.Speed==0)  &&  (robo_catch.Pos_systemWrist.Info.Speed)==0  &&  count>100  &&  catch_assist.stop==0)   //   �͵��pid�����йأ���pid�������һ��robo_catch.Pos_system1.Info.Abs_Angle<-700000
							{
									if( catch_assist.chassis_move == 1 )   //   �͵��pid�����йأ���pid�������һ��  //�����ƶ���ң�����л������궨״̬
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
//				if(robo_catch.Pos_systemOpen.Info.Speed == 0 && count > 100)   //   �͵��pid�����йأ���pid�������һ��
//			  {

//            Work_Status = FORTH;	
////					   last_ARM1= robo_catch.Pos_systemOpen.Info.Abs_Angle;      //Ϊ����Ŀ��ֵ���䣬�ĳ���һ��״̬ȷ����С
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
//				if(robo_catch.Pos_systemOpen.Info.Speed == 0 && count > 100)   //   �͵��pid�����йأ���pid�������һ��
//			  {

//         //   Work_Status = CALIBRATION_LINE;	
////					   last_ARM1= robo_catch.Pos_systemOpen.Info.Abs_Angle;      //Ϊ����Ŀ��ֵ���䣬�ĳ���һ��״̬ȷ����С
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
				if(catch_assist.direction)   //��λ�û�ռ���²�pidinit��λ�û����� ���ǡ�   �²���߰�can1������
				{
					robo_catch.Pos_systemLine.Tar_Pos+= POS.POS_line;  //5000
				  robo_catch.Pos_systemFinger.Tar_Pos=0;
					catch_assist.flag=1;	 
					
					static int count = 0;
				  count++;
					if(robo_catch.Pos_systemLine.Info.Speed == 0&& count > 3) //500��-500 ֮���л�����ֹ
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
									 last_Open = robo_catch.Pos_systemFinger.Info.Abs_Angle;      //Ϊ����Ŀ��ֵ���䣬�ĳ���һ��״̬ȷ����С
									 count = 0;
								   catch_assist.chassis_move = 0;
							}		
			}			
	    break;
		}
  //����Զ�����
		
		case CLOSE_STATE:  //6    �����ù�����л�����
		{	
			if(catch_assist.catchOK && catch_assist.chassis_move == 1 )   //   ȡ�������ҵ���Զ����ܻص�ȫ����λ����ԭ��
			
					  if(catch_assist.state == 4) //ץȡ����λ4
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
								    ����ر�
									ʱû�������أ��Ʋ���
								    ����������ʾ���ӹر�ʱû������
								*/
										 Work_Status = ENDPOINT_STATE;	
										 last_Line = robo_catch.Pos_systemLine.Info.Abs_Angle;     //Ϊ����Ŀ��ֵ���䣬�ĳ���һ��״̬ȷ����С
										 count = 0;
										 catch_assist.catchOK=0;
				       }
					 }			
			break;
		}
		
		
		case ARCHERY_WRIST:   //07     ˳���ƶ���close��֮��-750000
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
		//�ƶ�������λ��
		case ARCHERY_ELBOW:   //08     ok   ��ؽ�ֱ�ӵ�ȡ��λ��     
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
				  static int count = 0;    //�жϽ���
				  count++;
				  if(robo_catch.Pos_systemFinger.Info.Speed==0  &&  \
								   robo_catch.Pos_systemLine.Info.Speed==0  &&  \
							     robo_catch.Pos_systemElbow.Info.Speed==0  &&  \
								   robo_catch.Pos_systemWrist.Info.Speed==0  &&  \
							     count>100  &&  catch_assist.stop==0) 
					{       
						if(catch_assist.row)   //��һ�˾�λ����һ������λ�ã�row=1���Է���
						{
							 catch_assist.catchOK=0;
							 catch_assist.row = 0;
							 catch_assist.shot = 1;            //��д�жϣ��Ӵ��ú�Ϊ1
							 count = 0;
							 Work_Status = POS_STATE;
							 last_Line = robo_catch.Pos_systemLine.Info.Abs_Angle;//Ϊ����Ŀ��ֵ���䣬�ĳ���һ��״̬ȷ����С
						}
				}		
			}
			break;
		}
		
		
		case POS_STATE:   //10      ����ƶ�����������λ�ã�row��ʾ���Է��䣬��һ����һ��ʱ���䣬shot��ʾ�Ѿ��������
		{
			catch_assist.state = 6;
			
			switch(catch_assist.shot)
			{
				
				case 1://��һ���Ѿ�����
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
									 if(catch_assist.row)  //���Էŵڶ���
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
									 if(catch_assist.row)  //���Էŵڶ���
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
									 if(catch_assist.row)  //���Էŵڶ���
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
			���һ����������λ�û��������
			*/
//����������±궨			
		case RESTART_LINE:  //11              OK           //���Ӵ�-���ر�+
		{
			robo_catch.Pos_systemLine.Tar_Pos=0;
				robo_catch.Pos_systemFinger.Tar_Pos=0;
			  robo_catch.Pos_systemElbow.Tar_Pos=-150000;   //-750000 -100000    
				robo_catch.Pos_systemWrist.Tar_Pos=-10000;
			
		    catch_assist.flag = 1;
        Work_Status = FIRST;
			break;
		 }
		case RESTART_CLIP:   //01     ok   ��ؽ�ֱ�ӵ�ȡ��λ��     
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
							if((robo_catch.Pos_systemElbow.Info.Speed==0)  &&  (robo_catch.Pos_systemWrist.Info.Speed)==0  &&  count>100  &&  catch_assist.stop==0)   //   �͵��pid�����йأ���pid�������һ��  &&robo_catch.Pos_system1.Info.Abs_Angle<-700000
							{
									Work_Status = SECOND;	
									count = 0;
							}		
			}			
	    break;
		}
		
	}
}



