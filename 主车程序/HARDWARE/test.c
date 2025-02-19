
#include <stdio.h>
#include "stm32f4xx.h"
#include "TFT.h"
#include "AGV.h"
#include "delay.h"
#include "infrared.h"
#include "cba.h"
#include "ultrasonic.h"
#include "canp_hostcom.h"
#include "hard_can.h"
#include "bh1750.h"
#include "syn7318.h"
#include "power_check.h"
#include "can_user.h"
#include "data_base.h"
#include "roadway_check.h"
#include "tba.h"
#include "data_base.h"
#include "swopt_drv.h"
#include "uart_a72.h"
#include "Can_check.h"
#include "delay.h"
#include "can_user.h"
#include "Timer.h"
#include "Rc522.h"
#include "test.h"
#include "led.h"
#include "infrared_marker.h"
#include "garage.h"

void autorun(void);     			  //全自动函数声明
void zhuche_xunji(int speed);  		  //主车循迹函数
void go_ahead(int speed,u16 mapan);   //前进函数声明
void back_off(int speed,u16 mapan);   //后退函数声明
void turn_left(int speed);            //左转函数声明
void turn_right(int speed);           //右转函数声明
void stop(void);                      //停止函数声明



/***************************************************************
** 功能：     主车前进
** 参数：	  speed，整型，主车速度；mapan，u16型，码盘值
** 返回值：   无
** 说明：     码盘值越大，前进距离越长，码盘值最大为65535 
****************************************************************/
void go_ahead(int speed,u16 mapan)
{
	Roadway_mp_syn();	//码盘同步
	Stop_Flag = 0;
	Go_Flag = 1;
	wheel_L_Flag = 0;
	wheel_R_Flag = 0;
	wheel_Nav_Flag = 0;
	Back_Flag = 0;
	Track_Flag = 0;
	temp_MP=mapan;
	Car_Spend = speed;
	Control(Car_Spend ,Car_Spend);
}
/***************************************************************
** 功能：     主车后退
** 参数：	  speed，整型，主车速度；mapan，u16型，码盘值
** 返回值：   无
** 说明：     码盘值越大，前进距离越长，码盘值最大为65535 
****************************************8************************/
void back_off(int speed,u16 mapan)
{
  Roadway_mp_syn();	//码盘同步
  Stop_Flag = 0;
	Go_Flag = 0;
	wheel_L_Flag = 0;
	wheel_R_Flag = 0;
	wheel_Nav_Flag = 0;
	Back_Flag = 1;
	Track_Flag = 0;
	temp_MP=mapan;
	Car_Spend = speed;
	Control(-Car_Spend,-Car_Spend);
}

/***************************************************************
** 功能：     主车左转
** 参数：	  speed，整型，主车速度
** 返回值：   无 
****************************************************************/
void turn_left(int speed)
{
  Stop_Flag = 0;
	Go_Flag = 0;
	wheel_L_Flag = 1;
	wheel_R_Flag = 0;
	wheel_Nav_Flag = 0;
	Back_Flag = 0;
	Track_Flag = 0;
	Car_Spend = speed;
	Control(-Car_Spend,Car_Spend);
}

/***************************************************************
** 功能：     主车右转
** 参数：	  speed，整型，主车速度
** 返回值：   无 
****************************************************************/
void turn_right(int speed)
{
  Stop_Flag = 0;
	Go_Flag = 0;
	wheel_L_Flag = 0;
	wheel_R_Flag = 1;
	wheel_Nav_Flag = 0;
	Back_Flag = 0;
	Track_Flag = 0;
	Car_Spend = speed;
	Control(Car_Spend,-Car_Spend);
}

/***************************************************************
** 功能：     主车停止
** 参数：	  无参数
** 返回值：   无 
****************************************************************/
void stop(void)
{
   Roadway_Flag_clean();	//清除标志位状态	
}
/***************************************************************
** 功能：   主车寻迹
** 参数：	  speed，整型，主车速度
** 返回值：   无 
****************************************************************/
void zhuche_xunji(int speed)
{	
	 Go_Flag=1;
	 Stop_Flag=0;
   Car_Spend =speed;				      //主车速度		
	 Track_Flag=1;				  
	 TIM_Cmd(TIM2, ENABLE);
}


