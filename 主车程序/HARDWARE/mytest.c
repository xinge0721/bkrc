#include "TFT.h"
#include "LED.h"
#include "AGV.h"
#include "cba.h"
#include "tba.h"
#include "delay.h"
#include "Timer.h"
#include "RC522.h"
#include "daoza.h"
#include "syn7318.h"
#include "garage.h"
#include "mytest.h"
#include "syn7318.h"
#include "can_user.h"
#include "stm32f4xx.h"
#include "ultrasonic.h"
#include "CanP_Hostcom.h"
#include "trafficlight.h"
#include "roadway_check.h"
#include "Infrared_marker.h"
#include "wirelesscharging.h"


extern int LSpeed, RSpeed;;
uint8_t Special_Flag = 0;	//特殊地形标志位


/***************************************************************
** 功能：     竞赛平台向前行驶一段距离
** 参数：	  	Speed	整型，小车速度  MP 码盘
** 返回值：   无
** 说明：     竞赛平台向前行驶
Forward(70,95,10000);
****************************************************************/
void Forward(uint8_t LSpeed,uint8_t RSpeed,uint16_t MP) //Wifi_Rx_Buf[3] Wifi_Rx_Buf[5],这里的Speed只能写正数
{
	Roadway_mp_syn();	//码盘同步
	Stop_Flag = 0;
	Go_Flag = 1;
	wheel_L_Flag = 0;
	wheel_R_Flag = 0;
	wheel_Nav_Flag = 0;
	Back_Flag = 0;
	Track_Forward_Flag = 0;
	Track_Back_Flag = 0;
	temp_MP = MP;
	LCar_Spend = LSpeed;
	RCar_Spend = RSpeed;
	Control(LCar_Spend ,RCar_Spend);
	while (Stop_Flag != 3);
	delay_ms(50);
	Send_UpMotor(0 ,0);
	Roadway_Flag_clean();	//清除标志位状态	
}

/***************************************************************
** 功能：     竞赛平台向后行驶一段距离
** 参数：	  	Speed	整型，小车速度  MP 码盘
** 返回值：   无
** 说明：     竞赛平台向后行驶
Back_1(85,70,5000);
****************************************************************/
void Back_1(uint8_t LSpend,uint8_t RSpend,uint16_t MP)	//Wifi_Rx_Buf[3] Wifi_Rx_Buf[5],这里的Speed只能写正数
{
	Roadway_mp_syn();	//码盘同步
	Stop_Flag = 0;
	Go_Flag = 0;
	wheel_L_Flag = 0;
	wheel_R_Flag = 0;
	wheel_Nav_Flag = 0;
	Back_Flag = 1;
	Track_Forward_Flag = 0;
	Track_Back_Flag = 0;
	temp_MP = MP;
	LCar_Spend = -LSpend;
	RCar_Spend = -RSpend;
	Control(LCar_Spend ,RCar_Spend);
	while (Stop_Flag != 3);
	delay_ms(50);	
	Send_UpMotor(0 ,0);
	Roadway_Flag_clean();	//清除标志位状态	
}
	
/***************************************************************
** 功能：     竞赛平台左转
** 参数：	  	Speed	整型，小车速度 
** 返回值：   无
** 说明：     竞赛平台向左转到另一个循迹线则停,使用这个可以测出左右电机哪边强弱，左右速度给一样
****************************************************************/
void Turn_Left(int LSpeed,int RSpeed)//这里我的车子左轮速度要比右轮小
{
	Stop_Flag = 0;
	Go_Flag = 0;
	wheel_L_Flag = 1;
	wheel_R_Flag = 0;
	wheel_Nav_Flag = 0;
	Back_Flag = 0;
	Track_Forward_Flag = 0;
	Track_Back_Flag = 0;
	LCar_Spend = LSpeed;
	RCar_Spend = RSpeed;
	Control(-LCar_Spend ,RCar_Spend);
	while (Stop_Flag != 2);
	delay_ms(10);
	Send_UpMotor(0 ,0);
	Roadway_Flag_clean();	//清除标志位状态	
}

/***************************************************************
** 功能：     竞赛平台右转
** 参数：	  	Speed	整型，小车速度 
** 返回值：   无
** 说明：     竞赛平台向右转到另一个循迹线则停,使用这个可以测出左右电机哪边强弱，左右速度给一样
****************************************************************/
void Turn_Right(int LSpeed,int RSpeed)
{
	Stop_Flag = 0;
	Go_Flag = 0;
	wheel_L_Flag = 0;
	wheel_R_Flag = 1;
	wheel_Nav_Flag = 0;
	Back_Flag = 0;
	Track_Forward_Flag = 0;
	Track_Back_Flag = 0;
	LCar_Spend = LSpeed;
	RCar_Spend = RSpeed;	
	Control(LCar_Spend ,-RCar_Spend);
	while (Stop_Flag != 2);      //如果还没有转弯完成，运行标志位则一直卡着，直到等于2，则跳出循环代表转弯完成
	delay_ms(10);
	Stop_Flag = 2;
	Send_UpMotor(0 ,0);
	Roadway_Flag_clean();	//清除标志位状态
}



/***************************************************************
** 功能：     竞赛平台向前循迹前进
** 参数：	  	Speed	整型，小车速度  
** 返回值：   无	
** 说明：     竞赛平台循迹前进
Tracking_Forward(70,95);
****************************************************************/
void Tracking_Forward(uint8_t LSpeed,uint8_t RSpeed) 
{
	Stop_Flag = 0;
	Go_Flag = 0;
	wheel_L_Flag = 0;
	wheel_R_Flag = 0;
	wheel_Nav_Flag = 0;
	Back_Flag = 0;
	Track_Forward_Flag = 1;
	LCar_Spend = LSpeed;
	RCar_Spend = RSpeed;
	Control(LCar_Spend ,RCar_Spend);
	while (Stop_Flag != 0x01) ;  //等待循迹完成
	delay_ms(20);
	Send_UpMotor(0,0);
	Roadway_Flag_clean();	//清除标志位状态		
}



/***************************************************************
** 功能：     竞赛平台向前循迹前进多少距离
** 参数：	  	小车速度  ,距离
** 返回值：   无	
****************************************************************/
void Car_Time_Track(uint8_t LSpeed,uint8_t RSpeed, uint16_t tims)		// 小车循迹 参数：速度，距离
{
	Roadway_mp_syn();	//码盘同步
	Stop_Flag = 0;
	Go_Flag = 0;
	wheel_L_Flag = 0;
	wheel_R_Flag = 0;
	Back_Flag = 0;
	turnflag=0;
	Track_Forward_Flag = 1;
	Track_go=1;
	Track_Back_Flag = 0;
	temp_MP = tims;
	LCar_Spend = LSpeed;
	RCar_Spend = RSpeed;
	Control(LCar_Spend ,RCar_Spend);
	while (Stop_Flag != 30);
	Roadway_Flag_clean();	//清除标志位状态	
	Send_UpMotor(0,0);
}

/***************************************************************
** 功能：     竞赛平台向后循迹前进
** 参数：	  	Speed	整型，小车速度  
** 返回值：   无
** 说明：     竞赛平台向后循迹前进
Tracking_Back(90,75);
****************************************************************/
void Tracking_Back(uint8_t LSpeed,uint8_t RSpeed) 
{
	Stop_Flag = 0;
	Go_Flag = 0;
	wheel_L_Flag = 0;
	wheel_R_Flag = 0;
	wheel_Nav_Flag = 0;
	Back_Flag = 0;
	Track_Forward_Flag = 0;
	Track_Back_Flag = 1;
	LCar_Spend = -LSpeed;
	RCar_Spend = -RSpeed;
	Control(LCar_Spend ,RCar_Spend);
//	while (Stop_Flag != 0x01) ;  //等待循迹完成
//	Roadway_Flag_clean();	//清除标志位状态	
//	Send_UpMotor(0,0);
}

//码盘旋转向左
void turn_ppmd(uint8_t LSpend,uint8_t RSpend,uint16_t MP)	
{
	Roadway_mp_syn();	//码盘同步
	Stop_Flag = 0;
	Go_Flag = 0;
	wheel_L_Flag = 0;
	wheel_R_Flag = 0;
	wheel_Nav_Flag = 0;
	Back_Flag = 0;
	turnflag=1;//转向
	Track_Forward_Flag = 0;
	Track_Back_Flag = 0;
	temp_MP = MP;
	LCar_Spend = LSpend;
	RCar_Spend = RSpend;
	Control(-LCar_Spend ,RCar_Spend);
	while (Stop_Flag != 20);
	Roadway_Flag_clean();	//清除标志位状态		
}

//码盘旋转向右
//turn_llmd(90,90,850);
void turn_llmd(uint8_t LSpend,uint8_t RSpend,uint16_t MP)	
{
	Roadway_mp_syn();	//码盘同步
	Stop_Flag = 0;
	Go_Flag = 0;
	wheel_L_Flag = 0;
	wheel_R_Flag = 0;
	wheel_Nav_Flag = 0;
	Back_Flag = 0;
	turnflag=1;//转向
	Track_Forward_Flag = 0;
	Track_Back_Flag = 0;
	temp_MP = MP;
	LCar_Spend = LSpend;
	RCar_Spend = RSpend;
	Control(LCar_Spend ,-RCar_Spend);
	while (Stop_Flag != 20);
	Roadway_Flag_clean();	//清除标志位状态	
	Send_UpMotor(0,0);
}

void Track_duoy_RFID(int LSpeed,int RSpeed)   // 循迹寻卡函数
{
	Stop_Flag = 0;
	Go_Flag = 0;
	wheel_L_Flag = 0;
	wheel_R_Flag = 0;
	wheel_Nav_Flag = 0;
	Back_Flag = 0;
	Track_Forward_Flag = 1;
	LCar_Spend = LSpeed;
	RCar_Spend = RSpeed;
	Control(LCar_Spend ,RCar_Spend);
	while(Stop_Flag != 0x01);
	Roadway_Flag_clean();	//清除标志位状态	
}


/*
//过特殊循迹障碍物
循迹完了然后前进一点的距离会影响右转完了对准特殊障碍物的位置(循迹灯的位置，例如1011 1111),
这样子在后退在进行循迹的话它会猛的打拐，所以最好前进之后右转的位置是1110 1111这样子的，在后退在循迹在冲，百分之百过去。

<---   --->
		^
		|
		|
Tracking_Forward(70,95);
Forward(70,95,350);  //十字路口前进一点

turn_ppmd(90,90,820);
turn_llmd(90,90,1650);
delay_ms(20);
Back_1(85,70,200);
delay_ms(20);
Tracking_Forward(70,95);
Forward(70,95,2000);  //十字路口前进一点


*/
