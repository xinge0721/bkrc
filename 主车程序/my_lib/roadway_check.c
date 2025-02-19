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
uint16_t RFIDtime = 0;               // 计数值2
/*新增变量*/
int ReadSpecial_Count = 0;
int ReadWhite_Count = 0;
uint8_t Pass_Flag = 0;    //读白卡标志位 1：在循迹线中间读到白卡 2：在十字路口读到白卡
uint8_t ok = 0;
uint8_t Pass_two = 0;
uint8_t Pass_One = 1;
uint8_t Track_Back_Flag = 0;
uint8_t Go_First = 0;
unsigned char	Special_Star = 0; //开启特殊地形标志位 1：进入特殊地形 0：没有进入特殊地形 
uint8_t Rt_Tmp = 0;
/*********/

uint8_t wheel_L_Flag =0, wheel_R_Flag = 0;
uint8_t wheel_Nav_Flag = 0;
uint8_t Go_Flag = 0, Back_Flag = 0;
uint8_t Track_Forward_Flag = 0;
uint8_t Track_go=0;
uint8_t turnflag=1;
uint8_t Line_Flag = 0;					 //线状态标志位
uint8_t Line_Stop =0;
uint8_t Stop_Flag = 0;//各个标志物运行状态标志位
int LSpeed = 0,RSpeed = 0;
int LCar_Spend = 0;
int RCar_Spend = 0;
uint16_t temp_MP = 0;
uint16_t temp_Nav = 0;
uint8_t stop_delay=0;
uint32_t Wheel_flag = 0;
uint8_t Ok = 0;
uint8_t stop_delayfalg=0;
uint8_t RFID_Flag = 0;          	           // RFID检测标志位
//_________________________________________________________
int16_t Roadway_cmp;
extern int16_t CanHost_Mp;

/*
	码盘同步
**/
void Roadway_mp_syn(void)
{
	Roadway_cmp = CanHost_Mp;
}

/*
	码盘获取，获取走了多少
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
	角度同步
**/
void Roadway_nav_syn(void)
{
	Roadway_Navig = CanHost_Navig;
}

/*
	获取角度差值
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
//清除标志位函数
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
	前进和后退距离监测，是使用mytest文件里面的向前(向后)走多远函数Forward(Back_1)之后进行检测的
  下面写了定时器进行检测
*/
void Go_and_Back_Check(void)
{	
	if(Go_Flag == 1)		//前进标志位
	{
		//temp_MP是平板电脑(或者手动设置)发送给主车的码盘值，平板电脑发的码盘值(手动设置的码盘值)如果小于等于主车自己走了的码盘值(主车走了多远)
		//就是平板电脑发的指令(手动设置的指令)走多远，如果小于等于车自己走了多远，就判定完成了前进任务
		//比如我让它前进40cm，因为外部因素可能前进超过了40cm，然后它前进了45cm，也判定它前进完成，然后别让他走了，就立马设置速度为0
		if(temp_MP <= Roadway_mp_Get()) 
		{
			Go_Flag = 0;         
			Stop_Flag = 3;       //Stop_Flag是运行状态标志位，等于3的时候是代表平板电脑发送给主车前进多少cm的任务完成了
			RFID_Flag=2;
			Send_UpMotor(0,0);  //核心板发送给电机驱动板，速度为0
		}
	} 
	else if(Back_Flag == 1)  //后退标志位
	{
		//temp_MP是平板电脑(或者手动设置)发送给主车的码盘值，平板电脑发的码盘值(手动设置的码盘值)如果小于等于主车自己走了的码盘值(主车走了多远)
		//就是平板电脑发的指令(手动设置的指令)走多远，如果小于等于车自己走了多远，就判定完成了后退任务
		//比如我让它后退40cm，因为外部因素可能后退超过了40cm，然后它后退了45cm，也判定它后退完成，然后别让他走了，就立马设置速度为0
		if(temp_MP <= Roadway_mp_Get()) //temp_MP是平板电脑发送给主车的码盘值，发的码盘值如果小于等于主车现在的码盘值
		{
			Back_Flag = 0;
			Stop_Flag=3;           //Stop_Flag是运行状态标志位，等于3的时候是代表平板电脑发送给主车后退多少cm的任务完成了
			Send_UpMotor(0,0);		//核心板发送给电机驱动板，速度为0
		}
	}
}
/**
	码盘转弯
*/

void turn_left(void)
{	
	if(turnflag == 1)		//前进标志位
	{
		if(temp_MP <= Roadway_mp_Get()) 
		{
			turnflag = 0;         
			Stop_Flag = 20;       //Stop_Flag是运行状态标志位，等于3的时候是代表平板电脑发送给主车前进多少cm的任务完成了
			Send_UpMotor(0,0);  //核心板发送给电机驱动板，速度为0
			temp_MP =0;
		}
	} 
}

uint8_t Roadway_GoBack_Check(void)//如果前面这些标志位都是0则返回1，否则返回0
{
	return ((Track_Back_Flag == 0)&&(Go_Flag == 0)&&(Back_Flag == 0)&&(Track_Forward_Flag == 0)&&(wheel_L_Flag == 0)&&(wheel_R_Flag == 0))? 1:0;
}

/**
	码盘循迹前进监测—循迹前进多少m
*/

void trackcheckdistacne(void)
{	
	if(Track_go == 1)		//前进标志位
	{
		if(temp_MP > Roadway_mp_Get()) 
		{
			Track_go = 0;         
			Stop_Flag = 30;       //Stop_Flag是运行状态标志位，等于3的时候是代表平板电脑发送给主车前进多少cm的任务完成了
			Send_UpMotor(0,0);  //核心板发送给电机驱动板，速度为0
			temp_MP =0;
		}
	} 
}


/**
	循迹线转弯监测,是使用mytest文件里面的左右转弯Turn_Left和Turn_Right函数进行检测的
  下面写了定时器2ms进行一次检测
*/
uint32_t Mp_Value = 0;
void wheel_Track_check(void)
{ 	
	uint16_t Track_Value = 0;
	
	if(wheel_L_Flag == 1) //运行标志位—左转
	{
		Track_Value = Get_Host_UpTrack(TRACK_H8);//获取循迹板的高8位
		//循迹板，循迹灯亮了返回1(无黑线)，循迹灯灭了返回0(有黑线)
		if(!(Track_Value & 0X10)) //已经在转弯中,找到循迹线，停止,//1110 1111 & 0001 0000=0000 0000      然后!=1就可以进来    
		{	
			if(Wheel_flag > 50) //Wheel_flag初始值是0，一开始进不去这个if，下面的Wheel_flag++，一直到50才能进去，就可以知道，循迹线换到了另一条
			{
				wheel_L_Flag = 0;
				Wheel_flag=0;						//到了循迹线以后清0
				Stop_Flag=2;            //运行状态标志位，转弯成功
				Send_UpMotor(0,0);
			}
		}
		else if(Track_Value == 0Xff) //循迹灯全亮，还没有到循到黑线
		{			
			Wheel_flag++;
		}
	} 
	else if(wheel_R_Flag == 1)//运行标志位—右转
	{
		Track_Value = Get_Host_UpTrack(TRACK_H8);
		 if(!(Track_Value & 0X08)) //找到循迹线，停止，//1111 0111 & 0000 1000=0000 0000      然后!=1就可以进来
			{	
				if(Wheel_flag > 50)
				{					
					wheel_R_Flag=0;
					Wheel_flag=0;
					Stop_Flag=2;         //运行状态标志位，转弯成功
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
** 功能：     电机控制函数
** 参数：	  	L_Spend：电机左轮速度
**            R_Spend：电机右轮速度
** 返回值：   无	  
****************************************************************/
void Control(int L_Spend,int R_Spend)
{
	if(L_Spend>=0)
	{	
		if(L_Spend>100)L_Spend=100;if(L_Spend<5)L_Spend=5;		//限制速度参数
	}
	else 
	{
		if(L_Spend<-100)L_Spend= -100;if(L_Spend>-5)L_Spend= -5;     //限制速度参数
	}	
	if(R_Spend>=0)
	{	
		if(R_Spend>100)R_Spend=100;if(R_Spend<5)R_Spend=5;		//限制速度参数
	}
	else
	{	
		if(R_Spend<-100)R_Spend= -100;if(R_Spend>-5)R_Spend= -5;		//限制速度参数		
	}
	Send_UpMotor(L_Spend ,R_Spend);	  //发送速度
}


/***************************************************************
** 功能：     前进循迹函数
** 参数：	  参数是传感器的值
** 返回值：   无
右侧电机较弱，所以我们偏左的时候1111 0111 ，我们左侧电机给正转，右侧反转
然后偏右1110 1111给正常的1110 0111 的速度
****************************************************************/
void Track_Correct(uint8_t gd)
{	
	//循迹灯全灭 到十字路口 0000 0000   
	//特殊标志物1111 1111
	//或者白卡加场地全白1111 1111
	if(gd == 0x00 || gd==0xFF)	
	{	
		stop_delay++;
		if(stop_delay==10)//5ms消除某个区域识别成十字路口
		{
			stop_delay=0;
			stop_delayfalg=1;
		}
		if(stop_delayfalg==1)//停车
		{
			Track_Forward_Flag = 0;
			Stop_Flag = 1;  //循迹完成
			if(gd==0xFF)
			{
				RFID_Flag=1;
			}
			Send_UpMotor(0,0);//停止
		}
	}	
	else	
	{
	   Stop_Flag=0;															// 1110 0111 
		 if(gd == 0XE7)//1、中间3/4传感器检测到黑线，全速运行，车的右电机偏弱
		 { 
			 LSpeed=70;
			 RSpeed=95;
		 }	
		 if(Line_Flag != 2)//偏左 1111 0111  
		 {
			 if(gd == 0XF7 )
			 {
					LSpeed=95;		
				  RSpeed=-30;		//正转
				  Line_Flag=0;
			 }
			  else if(gd == 0XF3 || gd == 0XFB || gd==0xF1) 		 //2、中间4、3传感器检测到黑线，微右拐	1111 0011   1111 1011  1111 0001
			  { 
				  LSpeed=95;		
				  RSpeed=-70;		//反转
				  Line_Flag=0;
			  }	
				
			  else if(gd == 0XF9 || gd == 0XFD || gd==0xF8)//3、中间3、2传感器检测到黑线，再微右拐		 1111 1001  1111 1101   1111 1000	
			  { 
				   LSpeed=90;   
				   RSpeed=-80;		//反转
				   Line_Flag=0;
			  }
			  else if(gd == 0XFC )							 //4、中间2、1传感器检测到黑线，强右拐   1111 1100
			  {  
				  LSpeed = 90;    
				  RSpeed = -90;    //反转
				  Line_Flag=0;
			  }																
			  else if(gd==0XFE)								 //5、最右边1传感器检测到黑线，再强右拐			 1111 1110
			  { 
				   LSpeed = 95;      
				   RSpeed = -100;			//反转
				   Line_Flag=1;
			  }
				
		 }
		
		 if(Line_Flag != 1)//偏右
		 { 		
				if(gd == 0XEF)			//1110 1111!!!!!!!!!
				{
					RSpeed = 100;    
				  LSpeed = -20;    //正转
				  Line_Flag = 0;
				}
			  else if(gd == 0XCF || gd == 0XDF || gd==0x8F)						//6、中间6、5传感器检测到黑线，微左拐	1100 1111  1101 1111     1000 1111
			  { 
				   RSpeed = 95;    
				   LSpeed = -70;    //反转
				   Line_Flag = 0;
			  }													 					 
			  else if(gd == 0X9F || gd == 0XBF || gd==0x1F) //7、中间7、6传感器检测到黑线，再微左拐		  /1001 1111  	1011 1111   0001 1111
			  { 
				   RSpeed = 95;       
				   LSpeed = -80;      //反转 
				   Line_Flag = 0;
			  }
			  else if(gd == 0X3F) //8、中间8、7传感器检测到黑线，强左拐		  0011 1111  
			  {  
				   RSpeed = 95;   			
				   LSpeed = -90;					//反转
				   Line_Flag = 0;
			  }
			  else if(gd == 0X7F)									//9、最左8传感器检测到黑线，再强左拐		 0111 1111 	
			  { 
				   RSpeed = 95; 				
				   LSpeed = -100;				//反转
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
** 功能：     后退循迹函数
** 参数：	  参数是传感器的值
** 返回值：   无
****************************************************************/
void Track_AssistCorrect(uint8_t gd)
{
	if(gd == 0x00 || gd == 0xFF)	//循迹灯全灭或者全亮 停止
	{	
		stop_delay++;
		if(stop_delay==60)//30ms消除某个区域识别成十字路口
		{
			stop_delay=0;
			stop_delayfalg=1;
		}
		if(stop_delayfalg==1)
		{
			//Track_Back_Flag = 0;
			//Stop_Flag = 1;  //循迹完成
		//Send_UpMotor(0,0);//停止
		}
				
	}	
	else	
	{
	  Stop_Flag=0;														
		if(gd == 0XE7)//1、中间3/4传感器检测到黑线，全速运行 1110 0111
		{ 
			LSpeed = LCar_Spend;
			RSpeed = RCar_Spend;
		}		
		if(Line_Flag != 2)//偏左 -85,-70,//左轮前进+70,右轮后退-70
				
		{		
			if(gd == 0XF7)				//1111 0111
			{
				LSpeed= LCar_Spend;
				RSpeed=	RCar_Spend;
				Line_Flag=0;
			}
			else if(gd == 0XF3 || gd == 0XFB || gd == 0XF1) 		 //2、中间4、3传感器检测到黑线，微右拐	 // 1111 0011   1111 1011  1111 0001
			{ 
				LSpeed= LCar_Spend+155;
				RSpeed=	RCar_Spend-5;
				Line_Flag=0;
			}																 // 1111 1001  1111 1101				 1111 1000		
			else if(gd == 0XF9 || gd == 0XFD || gd == 0XF8)//3、中间3、2传感器检测到黑线，再微右拐		
			{ 
				 LSpeed= LCar_Spend+160;
				 RSpeed= RCar_Spend-10;				 
				 Line_Flag=0;
			}                   						 // 1111 1100
			else if(gd == 0XFC)							 //4、中间2、1传感器检测到黑线，强右拐
			{ 
				LSpeed = LCar_Spend+165;
				RSpeed = RCar_Spend-15;
				Line_Flag=0;
			}																 // 1111 1110
			else if(gd==0XFE)								 //5、最右边1传感器检测到黑线，再强右拐			
			{ 
				 LSpeed = LCar_Spend+170;
				 RSpeed = RCar_Spend-20;			
				 Line_Flag=1;
			}
		}
		
		if(Line_Flag != 1)//偏右
		{
			if(gd == 0XEF)     //1110 1111  //偏右//右轮前进+70，左轮后退-80    -85,-70
			{
				RSpeed = RCar_Spend;
				LSpeed = LCar_Spend;
				Line_Flag = 0;
			}
			else if(gd == 0XCF || gd==0xDF || gd == 0X8F)						//6、中间6、5传感器检测到黑线，微左拐		 // 1100 1111	  1101 1111   1000 1111
			{ 
				 RSpeed = RCar_Spend+140;
				 LSpeed = LCar_Spend;
				 Line_Flag = 0;
			}													 				// 1001 1111   1101 1111	 0001 1111
			else if(gd == 0X9F || gd == 0XDF || gd == 0X1F) //7、中间7、6传感器检测到黑线，再微左拐		 
			{ 
				 RSpeed = RCar_Spend+145;
				 LSpeed = LCar_Spend-5;
				 Line_Flag = 0;
			} 			  												// 0011 1111 1011 1111				
			else if(gd == 0X3F || gd == 0XBF) //8、中间8、7传感器检测到黑线，强左拐		 
			{ 
				 RSpeed = RCar_Spend+150;
				 LSpeed = LCar_Spend-10;
				 Line_Flag = 0;
			} 																// 0111 1111
			else if(gd == 0X7F)									//9、最左8传感器检测到黑线，再强左拐		 	
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
	if(Track_Forward_Flag == 1)//前进循迹
	{
		//TRACK_H8 为 后8位寻迹数据
		Track_Correct(Get_Host_UpTrack(TRACK_H8));//TRACK_H8
	}
	if(Track_Back_Flag == 1)//后退循迹
	{
		Track_AssistCorrect(Get_Host_UpTrack(TRACK_H8));
	}
}

void Roadway_Check(void)
{
	Track_Check();  				//循迹监测
	Go_and_Back_Check(); 		//前进后退监测
	wheel_Track_check();  	//根据循迹线转弯(判断有没有换到另一条循迹线)
	trackcheckdistacne();		//码盘前进监测
	turn_left();						//码盘左转检测
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
	TIM_ClearFlag(TIM9,TIM_FLAG_Update);//防止一上电就进入中断
	
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
			if(RFIDtime>=10000)//5s内没有检测到RFID卡就强制退出
			{
				RFID_Flag=4;
				RFIDtime=0;
				
			}
		}
		if(voice_flag==1)
		{
			voice_flag_time++;
			if(voice_flag_time>=10000)//6.5s内没有检测到语音标志物回传就强制退出
			{
				voice_flag=2;
				voice_flag_time=0;
				
			}
		}
		Roadway_Check();								//路况检测，0.5ms检测一次
	}
	TIM_ClearITPendingBit(TIM9,TIM_IT_Update);
}


void SpecialPass_Terrain(uint8_t Spend, uint16_t MP)
{
	Special_Flag = 0;
	Roadway_mp_syn();	//码盘同步
	Go_Flag = 1;
	temp_MP = MP;
	//Car_Spend = Spend;
	Control(Spend ,Spend);	
	
	while (1)
	{
		CanP_Host_Main();		/*获取码盘值*/
		Go_and_Back_Check();/*检查是否前进结束*/	
		if (Stop_Flag == 0x03) /*退出特殊地形*/
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
	Roadway_mp_syn();													//码盘同步
	Go_Flag = 1;
	temp_MP = MP;
	Control(Spend ,Spend);	
	
	while (1)
	{
		CanP_Host_Main();				 /*获取码盘值*/
		Go_and_Back_Check();		 /*检查是否前进结束*/	
		
		if (Stop_Flag == 0x03)	 /*走200码盘值就停止一次*/
		{	
			break;
		}							
	 }
}





