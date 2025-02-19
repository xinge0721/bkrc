#include "stm32f4xx.h"
#include "CanP_Hostcom.h"
#include "delay.h"
#include "roadway_check.h"
#include "cba.h"
#include "Timer.h"
#include "mytest.h"
#include "tba.h"
#include <stdio.h>
#include "RC522.h"
#include "voicebroadcast.h"
uint16_t RFIDtime = 0;               // ����ֵ2
/*��������*/
int ReadSpecial_Count = 0;
int ReadWhite_Count = 0;
uint8_t Pass_Flag = 0;    //���׿���־λ 1����ѭ�����м�����׿� 2����ʮ��·�ڶ����׿�
uint8_t ok = 0;
uint8_t Pass_two = 0;
uint8_t Pass_One = 1;
uint8_t Track_Back_Flag = 0;
uint8_t Go_First = 0;
unsigned char	Special_Star = 0; //����������α�־λ 1������������� 0��û�н���������� 
uint8_t Rt_Tmp = 0;
/*********/

uint8_t wheel_L_Flag =0, wheel_R_Flag = 0;
uint8_t wheel_Nav_Flag = 0;
uint8_t Go_Flag = 0, Back_Flag = 0;
uint8_t Track_Forward_Flag = 0;
uint8_t Track_go=0;
uint8_t turnflag=1;
uint8_t Line_Flag = 0;					 //��״̬��־λ
uint8_t Line_Stop =0;
uint8_t Stop_Flag = 0;//������־������״̬��־λ
int LSpeed = 0,RSpeed = 0;
int LCar_Spend = 0;
int RCar_Spend = 0;
uint16_t temp_MP = 0;
uint16_t temp_Nav = 0;
uint8_t stop_delay=0;
uint32_t Wheel_flag = 0;
uint8_t Ok = 0;
uint8_t stop_delayfalg=0;
uint8_t RFID_Flag = 0;          	           // RFID����־λ
//_________________________________________________________
int16_t Roadway_cmp;
extern int16_t CanHost_Mp;

/*
	����ͬ��
**/
void Roadway_mp_syn(void)
{
	Roadway_cmp = CanHost_Mp;
}

/*
	���̻�ȡ����ȡ���˶���
**/
uint16_t Roadway_mp_Get(void)
{
	uint32_t ct;
	if(CanHost_Mp > Roadway_cmp)
		ct = CanHost_Mp - Roadway_cmp;
	else
		ct = Roadway_cmp - CanHost_Mp;
	if(ct > 0x8000)
		ct = 0xffff - ct;
	return ct;	
}

//_______________________________________________________________



//_________________________________________________________
uint16_t Roadway_Navig;
extern uint16_t CanHost_Navig;

/*
	�Ƕ�ͬ��
**/
void Roadway_nav_syn(void)
{
	Roadway_Navig = CanHost_Navig;
}

/*
	��ȡ�ǶȲ�ֵ
**/
uint16_t Roadway_nav_Get(void)
{
	uint16_t ct;
	if(CanHost_Navig > Roadway_Navig)
		ct = CanHost_Navig - Roadway_Navig;
	else
		ct = Roadway_Navig - CanHost_Navig;
	while(ct >= 36000)
		ct -= 36000;
	return ct;
}

//_______________________________________________________________
//�����־λ����
void Roadway_Flag_clean(void)
{
	Track_Forward_Flag = 0;
	Track_Back_Flag = 0;
	wheel_L_Flag = 0;
	wheel_R_Flag = 0;
	Back_Flag = 0;
	Stop_Flag = 0;
	Go_Flag = 0;
	temp_MP = 0;
	turnflag=0;
	Track_go=0;
}

/*
	ǰ���ͺ��˾����⣬��ʹ��mytest�ļ��������ǰ(���)�߶�Զ����Forward(Back_1)֮����м���
  ����д�˶�ʱ�����м��
*/
void Go_and_Back_Check(void)
{	
	if(Go_Flag == 1)		//ǰ����־λ
	{
		//temp_MP��ƽ�����(�����ֶ�����)���͸�����������ֵ��ƽ����Է�������ֵ(�ֶ����õ�����ֵ)���С�ڵ��������Լ����˵�����ֵ(�������˶�Զ)
		//����ƽ����Է���ָ��(�ֶ����õ�ָ��)�߶�Զ�����С�ڵ��ڳ��Լ����˶�Զ�����ж������ǰ������
		//����������ǰ��40cm����Ϊ�ⲿ���ؿ���ǰ��������40cm��Ȼ����ǰ����45cm��Ҳ�ж���ǰ����ɣ�Ȼ����������ˣ������������ٶ�Ϊ0
		if(temp_MP <= Roadway_mp_Get()) 
		{
			Go_Flag = 0;         
			Stop_Flag = 3;       //Stop_Flag������״̬��־λ������3��ʱ���Ǵ���ƽ����Է��͸�����ǰ������cm�����������
			RFID_Flag=2;
			Send_UpMotor(0,0);  //���İ巢�͸���������壬�ٶ�Ϊ0
		}
	} 
	else if(Back_Flag == 1)  //���˱�־λ
	{
		//temp_MP��ƽ�����(�����ֶ�����)���͸�����������ֵ��ƽ����Է�������ֵ(�ֶ����õ�����ֵ)���С�ڵ��������Լ����˵�����ֵ(�������˶�Զ)
		//����ƽ����Է���ָ��(�ֶ����õ�ָ��)�߶�Զ�����С�ڵ��ڳ��Լ����˶�Զ�����ж�����˺�������
		//��������������40cm����Ϊ�ⲿ���ؿ��ܺ��˳�����40cm��Ȼ����������45cm��Ҳ�ж���������ɣ�Ȼ����������ˣ������������ٶ�Ϊ0
		if(temp_MP <= Roadway_mp_Get()) //temp_MP��ƽ����Է��͸�����������ֵ����������ֵ���С�ڵ����������ڵ�����ֵ
		{
			Back_Flag = 0;
			Stop_Flag=3;           //Stop_Flag������״̬��־λ������3��ʱ���Ǵ���ƽ����Է��͸��������˶���cm�����������
			Send_UpMotor(0,0);		//���İ巢�͸���������壬�ٶ�Ϊ0
		}
	}
}
/**
	����ת��
*/

void turn_left(void)
{	
	if(turnflag == 1)		//ǰ����־λ
	{
		if(temp_MP <= Roadway_mp_Get()) 
		{
			turnflag = 0;         
			Stop_Flag = 20;       //Stop_Flag������״̬��־λ������3��ʱ���Ǵ���ƽ����Է��͸�����ǰ������cm�����������
			Send_UpMotor(0,0);  //���İ巢�͸���������壬�ٶ�Ϊ0
			temp_MP =0;
		}
	} 
}

uint8_t Roadway_GoBack_Check(void)//���ǰ����Щ��־λ����0�򷵻�1�����򷵻�0
{
	return ((Track_Back_Flag == 0)&&(Go_Flag == 0)&&(Back_Flag == 0)&&(Track_Forward_Flag == 0)&&(wheel_L_Flag == 0)&&(wheel_R_Flag == 0))? 1:0;
}

/**
	����ѭ��ǰ����⡪ѭ��ǰ������m
*/

void trackcheckdistacne(void)
{	
	if(Track_go == 1)		//ǰ����־λ
	{
		if(temp_MP > Roadway_mp_Get()) 
		{
			Track_go = 0;         
			Stop_Flag = 30;       //Stop_Flag������״̬��־λ������3��ʱ���Ǵ���ƽ����Է��͸�����ǰ������cm�����������
			Send_UpMotor(0,0);  //���İ巢�͸���������壬�ٶ�Ϊ0
			temp_MP =0;
		}
	} 
}


/**
	ѭ����ת����,��ʹ��mytest�ļ����������ת��Turn_Left��Turn_Right�������м���
  ����д�˶�ʱ��2ms����һ�μ��
*/
uint32_t Mp_Value = 0;
void wheel_Track_check(void)
{ 	
	uint16_t Track_Value = 0;
	
	if(wheel_L_Flag == 1) //���б�־λ����ת
	{
		Track_Value = Get_Host_UpTrack(TRACK_H8);//��ȡѭ����ĸ�8λ
		//ѭ���壬ѭ�������˷���1(�޺���)��ѭ�������˷���0(�к���)
		if(!(Track_Value & 0X10)) //�Ѿ���ת����,�ҵ�ѭ���ߣ�ֹͣ,//1110 1111 & 0001 0000=0000 0000      Ȼ��!=1�Ϳ��Խ���    
		{	
			if(Wheel_flag > 50) //Wheel_flag��ʼֵ��0��һ��ʼ����ȥ���if�������Wheel_flag++��һֱ��50���ܽ�ȥ���Ϳ���֪����ѭ���߻�������һ��
			{
				wheel_L_Flag = 0;
				Wheel_flag=0;						//����ѭ�����Ժ���0
				Stop_Flag=2;            //����״̬��־λ��ת��ɹ�
				Send_UpMotor(0,0);
			}
		}
		else if(Track_Value == 0Xff) //ѭ����ȫ������û�е�ѭ������
		{			
			Wheel_flag++;
		}
	} 
	else if(wheel_R_Flag == 1)//���б�־λ����ת
	{
		Track_Value = Get_Host_UpTrack(TRACK_H8);
		 if(!(Track_Value & 0X08)) //�ҵ�ѭ���ߣ�ֹͣ��//1111 0111 & 0000 1000=0000 0000      Ȼ��!=1�Ϳ��Խ���
			{	
				if(Wheel_flag > 50)
				{					
					wheel_R_Flag=0;
					Wheel_flag=0;
					Stop_Flag=2;         //����״̬��־λ��ת��ɹ�
					Send_UpMotor(0,0);
				}
			}
			else if( Track_Value == 0Xff) 
			{				
				Wheel_flag++;
			}
	}
}

/***************************************************************
** ���ܣ�     ������ƺ���
** ������	  	L_Spend����������ٶ�
**            R_Spend����������ٶ�
** ����ֵ��   ��	  
****************************************************************/
void Control(int L_Spend,int R_Spend)
{
	if(L_Spend>=0)
	{	
		if(L_Spend>100)L_Spend=100;if(L_Spend<5)L_Spend=5;		//�����ٶȲ���
	}
	else 
	{
		if(L_Spend<-100)L_Spend= -100;if(L_Spend>-5)L_Spend= -5;     //�����ٶȲ���
	}	
	if(R_Spend>=0)
	{	
		if(R_Spend>100)R_Spend=100;if(R_Spend<5)R_Spend=5;		//�����ٶȲ���
	}
	else
	{	
		if(R_Spend<-100)R_Spend= -100;if(R_Spend>-5)R_Spend= -5;		//�����ٶȲ���		
	}
	Send_UpMotor(L_Spend ,R_Spend);	  //�����ٶ�
}


/***************************************************************
** ���ܣ�     ǰ��ѭ������
** ������	  �����Ǵ�������ֵ
** ����ֵ��   ��
�Ҳ�����������������ƫ���ʱ��1111 0111 ���������������ת���Ҳ෴ת
Ȼ��ƫ��1110 1111��������1110 0111 ���ٶ�
****************************************************************/
void Track_Correct(uint8_t gd)
{	
	//ѭ����ȫ�� ��ʮ��·�� 0000 0000   
	//�����־��1111 1111
	//���߰׿��ӳ���ȫ��1111 1111
	if(gd == 0x00 || gd==0xFF)	
	{	
		stop_delay++;
		if(stop_delay==10)//5ms����ĳ������ʶ���ʮ��·��
		{
			stop_delay=0;
			stop_delayfalg=1;
		}
		if(stop_delayfalg==1)//ͣ��
		{
			Track_Forward_Flag = 0;
			Stop_Flag = 1;  //ѭ�����
			if(gd==0xFF)
			{
				RFID_Flag=1;
			}
			Send_UpMotor(0,0);//ֹͣ
		}
	}	
	else	
	{
	   Stop_Flag=0;															// 1110 0111 
		 if(gd == 0XE7)//1���м�3/4��������⵽���ߣ�ȫ�����У������ҵ��ƫ��
		 { 
			 LSpeed=70;
			 RSpeed=95;
		 }	
		 if(Line_Flag != 2)//ƫ�� 1111 0111  
		 {
			 if(gd == 0XF7 )
			 {
					LSpeed=95;		
				  RSpeed=-30;		//��ת
				  Line_Flag=0;
			 }
			  else if(gd == 0XF3 || gd == 0XFB || gd==0xF1) 		 //2���м�4��3��������⵽���ߣ�΢�ҹ�	1111 0011   1111 1011  1111 0001
			  { 
				  LSpeed=95;		
				  RSpeed=-70;		//��ת
				  Line_Flag=0;
			  }	
				
			  else if(gd == 0XF9 || gd == 0XFD || gd==0xF8)//3���м�3��2��������⵽���ߣ���΢�ҹ�		 1111 1001  1111 1101   1111 1000	
			  { 
				   LSpeed=90;   
				   RSpeed=-80;		//��ת
				   Line_Flag=0;
			  }
			  else if(gd == 0XFC )							 //4���м�2��1��������⵽���ߣ�ǿ�ҹ�   1111 1100
			  {  
				  LSpeed = 90;    
				  RSpeed = -90;    //��ת
				  Line_Flag=0;
			  }																
			  else if(gd==0XFE)								 //5�����ұ�1��������⵽���ߣ���ǿ�ҹ�			 1111 1110
			  { 
				   LSpeed = 95;      
				   RSpeed = -100;			//��ת
				   Line_Flag=1;
			  }
				
		 }
		
		 if(Line_Flag != 1)//ƫ��
		 { 		
				if(gd == 0XEF)			//1110 1111!!!!!!!!!
				{
					RSpeed = 100;    
				  LSpeed = -20;    //��ת
				  Line_Flag = 0;
				}
			  else if(gd == 0XCF || gd == 0XDF || gd==0x8F)						//6���м�6��5��������⵽���ߣ�΢���	1100 1111  1101 1111     1000 1111
			  { 
				   RSpeed = 95;    
				   LSpeed = -70;    //��ת
				   Line_Flag = 0;
			  }													 					 
			  else if(gd == 0X9F || gd == 0XBF || gd==0x1F) //7���м�7��6��������⵽���ߣ���΢���		  /1001 1111  	1011 1111   0001 1111
			  { 
				   RSpeed = 95;       
				   LSpeed = -80;      //��ת 
				   Line_Flag = 0;
			  }
			  else if(gd == 0X3F) //8���м�8��7��������⵽���ߣ�ǿ���		  0011 1111  
			  {  
				   RSpeed = 95;   			
				   LSpeed = -90;					//��ת
				   Line_Flag = 0;
			  }
			  else if(gd == 0X7F)									//9������8��������⵽���ߣ���ǿ���		 0111 1111 	
			  { 
				   RSpeed = 95; 				
				   LSpeed = -100;				//��ת
				   Line_Flag=2;
			  }
				
		  }
	 }	
	if(Track_Forward_Flag != 0)
	{						
		Control(LSpeed,RSpeed);
	}
}


/***************************************************************
** ���ܣ�     ����ѭ������
** ������	  �����Ǵ�������ֵ
** ����ֵ��   ��
****************************************************************/
void Track_AssistCorrect(uint8_t gd)
{
	if(gd == 0x00 || gd == 0xFF)	//ѭ����ȫ�����ȫ�� ֹͣ
	{	
		stop_delay++;
		if(stop_delay==60)//30ms����ĳ������ʶ���ʮ��·��
		{
			stop_delay=0;
			stop_delayfalg=1;
		}
		if(stop_delayfalg==1)
		{
			//Track_Back_Flag = 0;
			//Stop_Flag = 1;  //ѭ�����
		//Send_UpMotor(0,0);//ֹͣ
		}
				
	}	
	else	
	{
	  Stop_Flag=0;														
		if(gd == 0XE7)//1���м�3/4��������⵽���ߣ�ȫ������ 1110 0111
		{ 
			LSpeed = LCar_Spend;
			RSpeed = RCar_Spend;
		}		
		if(Line_Flag != 2)//ƫ�� -85,-70,//����ǰ��+70,���ֺ���-70
				
		{		
			if(gd == 0XF7)				//1111 0111
			{
				LSpeed= LCar_Spend;
				RSpeed=	RCar_Spend;
				Line_Flag=0;
			}
			else if(gd == 0XF3 || gd == 0XFB || gd == 0XF1) 		 //2���м�4��3��������⵽���ߣ�΢�ҹ�	 // 1111 0011   1111 1011  1111 0001
			{ 
				LSpeed= LCar_Spend+155;
				RSpeed=	RCar_Spend-5;
				Line_Flag=0;
			}																 // 1111 1001  1111 1101				 1111 1000		
			else if(gd == 0XF9 || gd == 0XFD || gd == 0XF8)//3���м�3��2��������⵽���ߣ���΢�ҹ�		
			{ 
				 LSpeed= LCar_Spend+160;
				 RSpeed= RCar_Spend-10;				 
				 Line_Flag=0;
			}                   						 // 1111 1100
			else if(gd == 0XFC)							 //4���м�2��1��������⵽���ߣ�ǿ�ҹ�
			{ 
				LSpeed = LCar_Spend+165;
				RSpeed = RCar_Spend-15;
				Line_Flag=0;
			}																 // 1111 1110
			else if(gd==0XFE)								 //5�����ұ�1��������⵽���ߣ���ǿ�ҹ�			
			{ 
				 LSpeed = LCar_Spend+170;
				 RSpeed = RCar_Spend-20;			
				 Line_Flag=1;
			}
		}
		
		if(Line_Flag != 1)//ƫ��
		{
			if(gd == 0XEF)     //1110 1111  //ƫ��//����ǰ��+70�����ֺ���-80    -85,-70
			{
				RSpeed = RCar_Spend;
				LSpeed = LCar_Spend;
				Line_Flag = 0;
			}
			else if(gd == 0XCF || gd==0xDF || gd == 0X8F)						//6���м�6��5��������⵽���ߣ�΢���		 // 1100 1111	  1101 1111   1000 1111
			{ 
				 RSpeed = RCar_Spend+140;
				 LSpeed = LCar_Spend;
				 Line_Flag = 0;
			}													 				// 1001 1111   1101 1111	 0001 1111
			else if(gd == 0X9F || gd == 0XDF || gd == 0X1F) //7���м�7��6��������⵽���ߣ���΢���		 
			{ 
				 RSpeed = RCar_Spend+145;
				 LSpeed = LCar_Spend-5;
				 Line_Flag = 0;
			} 			  												// 0011 1111 1011 1111				
			else if(gd == 0X3F || gd == 0XBF) //8���м�8��7��������⵽���ߣ�ǿ���		 
			{ 
				 RSpeed = RCar_Spend+150;
				 LSpeed = LCar_Spend-10;
				 Line_Flag = 0;
			} 																// 0111 1111
			else if(gd == 0X7F)									//9������8��������⵽���ߣ���ǿ���		 	
			{ 
				 RSpeed = RCar_Spend+155;
				 LSpeed = LCar_Spend-15;
				 Line_Flag=2;
			}
		}		
	}	
	
	if(Track_Back_Flag != 0)
	{						
		Control(LSpeed, RSpeed);
	}
}


void Track_Check()
{	
	if(Track_Forward_Flag == 1)//ǰ��ѭ��
	{
		//TRACK_H8 Ϊ ��8λѰ������
		Track_Correct(Get_Host_UpTrack(TRACK_H8));//TRACK_H8
	}
	if(Track_Back_Flag == 1)//����ѭ��
	{
		Track_AssistCorrect(Get_Host_UpTrack(TRACK_H8));
	}
}

void Roadway_Check(void)
{
	Track_Check();  				//ѭ�����
	Go_and_Back_Check(); 		//ǰ�����˼��
	wheel_Track_check();  	//����ѭ����ת��(�ж���û�л�����һ��ѭ����)
	trackcheckdistacne();		//����ǰ�����
	turn_left();						//������ת���
}
	

void roadway_check_TimInit(uint16_t arr,uint16_t psc)//168M
{
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);
	
	TIM_InitStructure.TIM_Period = arr;
	TIM_InitStructure.TIM_Prescaler = psc;
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM9,&TIM_InitStructure);

	TIM_ITConfig(TIM9,TIM_IT_Update,ENABLE);
	TIM_ClearFlag(TIM9,TIM_FLAG_Update);//��ֹһ�ϵ�ͽ����ж�
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_TIM9_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM9, ENABLE);
}


void TIM1_BRK_TIM9_IRQHandler(void)  //0.5ms
{
	if(TIM_GetITStatus(TIM9,TIM_IT_Update) == SET)
	{		
		if(RFID_Flag==2)
		{
			RFIDtime++;
			if(RFIDtime>=10000)//5s��û�м�⵽RFID����ǿ���˳�
			{
				RFID_Flag=4;
				RFIDtime=0;
				
			}
		}
		if(voice_flag==1)
		{
			voice_flag_time++;
			if(voice_flag_time>=10000)//6.5s��û�м�⵽������־��ش���ǿ���˳�
			{
				voice_flag=2;
				voice_flag_time=0;
				
			}
		}
		Roadway_Check();								//·����⣬0.5ms���һ��
	}
	TIM_ClearITPendingBit(TIM9,TIM_IT_Update);
}


void SpecialPass_Terrain(uint8_t Spend, uint16_t MP)
{
	Special_Flag = 0;
	Roadway_mp_syn();	//����ͬ��
	Go_Flag = 1;
	temp_MP = MP;
	//Car_Spend = Spend;
	Control(Spend ,Spend);	
	
	while (1)
	{
		CanP_Host_Main();		/*��ȡ����ֵ*/
		Go_and_Back_Check();/*����Ƿ�ǰ������*/	
		if (Stop_Flag == 0x03) /*�˳��������*/
		{
			Special_Star = 0;
			//Car_Spend = 30;
			Control(30 ,30);	
			break;
		}
	}				
}

void Pass_Rfid(uint8_t Spend, uint16_t MP)
{
	Roadway_mp_syn();													//����ͬ��
	Go_Flag = 1;
	temp_MP = MP;
	Control(Spend ,Spend);	
	
	while (1)
	{
		CanP_Host_Main();				 /*��ȡ����ֵ*/
		Go_and_Back_Check();		 /*����Ƿ�ǰ������*/	
		
		if (Stop_Flag == 0x03)	 /*��200����ֵ��ֹͣһ��*/
		{	
			break;
		}							
	 }
}





