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
uint8_t Special_Flag = 0;	//������α�־λ


/***************************************************************
** ���ܣ�     ����ƽ̨��ǰ��ʻһ�ξ���
** ������	  	Speed	���ͣ�С���ٶ�  MP ����
** ����ֵ��   ��
** ˵����     ����ƽ̨��ǰ��ʻ
Forward(70,95,10000);
****************************************************************/
void Forward(uint8_t LSpeed,uint8_t RSpeed,uint16_t MP) //Wifi_Rx_Buf[3] Wifi_Rx_Buf[5],�����Speedֻ��д����
{
	Roadway_mp_syn();	//����ͬ��
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
	Roadway_Flag_clean();	//�����־λ״̬	
}

/***************************************************************
** ���ܣ�     ����ƽ̨�����ʻһ�ξ���
** ������	  	Speed	���ͣ�С���ٶ�  MP ����
** ����ֵ��   ��
** ˵����     ����ƽ̨�����ʻ
Back_1(85,70,5000);
****************************************************************/
void Back_1(uint8_t LSpend,uint8_t RSpend,uint16_t MP)	//Wifi_Rx_Buf[3] Wifi_Rx_Buf[5],�����Speedֻ��д����
{
	Roadway_mp_syn();	//����ͬ��
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
	Roadway_Flag_clean();	//�����־λ״̬	
}
	
/***************************************************************
** ���ܣ�     ����ƽ̨��ת
** ������	  	Speed	���ͣ�С���ٶ� 
** ����ֵ��   ��
** ˵����     ����ƽ̨����ת����һ��ѭ������ͣ,ʹ��������Բ�����ҵ���ı�ǿ���������ٶȸ�һ��
****************************************************************/
void Turn_Left(int LSpeed,int RSpeed)//�����ҵĳ��������ٶ�Ҫ������С
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
	Roadway_Flag_clean();	//�����־λ״̬	
}

/***************************************************************
** ���ܣ�     ����ƽ̨��ת
** ������	  	Speed	���ͣ�С���ٶ� 
** ����ֵ��   ��
** ˵����     ����ƽ̨����ת����һ��ѭ������ͣ,ʹ��������Բ�����ҵ���ı�ǿ���������ٶȸ�һ��
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
	while (Stop_Flag != 2);      //�����û��ת����ɣ����б�־λ��һֱ���ţ�ֱ������2��������ѭ������ת�����
	delay_ms(10);
	Stop_Flag = 2;
	Send_UpMotor(0 ,0);
	Roadway_Flag_clean();	//�����־λ״̬
}



/***************************************************************
** ���ܣ�     ����ƽ̨��ǰѭ��ǰ��
** ������	  	Speed	���ͣ�С���ٶ�  
** ����ֵ��   ��	
** ˵����     ����ƽ̨ѭ��ǰ��
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
	while (Stop_Flag != 0x01) ;  //�ȴ�ѭ�����
	delay_ms(20);
	Send_UpMotor(0,0);
	Roadway_Flag_clean();	//�����־λ״̬		
}



/***************************************************************
** ���ܣ�     ����ƽ̨��ǰѭ��ǰ�����پ���
** ������	  	С���ٶ�  ,����
** ����ֵ��   ��	
****************************************************************/
void Car_Time_Track(uint8_t LSpeed,uint8_t RSpeed, uint16_t tims)		// С��ѭ�� �������ٶȣ�����
{
	Roadway_mp_syn();	//����ͬ��
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
	Roadway_Flag_clean();	//�����־λ״̬	
	Send_UpMotor(0,0);
}

/***************************************************************
** ���ܣ�     ����ƽ̨���ѭ��ǰ��
** ������	  	Speed	���ͣ�С���ٶ�  
** ����ֵ��   ��
** ˵����     ����ƽ̨���ѭ��ǰ��
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
//	while (Stop_Flag != 0x01) ;  //�ȴ�ѭ�����
//	Roadway_Flag_clean();	//�����־λ״̬	
//	Send_UpMotor(0,0);
}

//������ת����
void turn_ppmd(uint8_t LSpend,uint8_t RSpend,uint16_t MP)	
{
	Roadway_mp_syn();	//����ͬ��
	Stop_Flag = 0;
	Go_Flag = 0;
	wheel_L_Flag = 0;
	wheel_R_Flag = 0;
	wheel_Nav_Flag = 0;
	Back_Flag = 0;
	turnflag=1;//ת��
	Track_Forward_Flag = 0;
	Track_Back_Flag = 0;
	temp_MP = MP;
	LCar_Spend = LSpend;
	RCar_Spend = RSpend;
	Control(-LCar_Spend ,RCar_Spend);
	while (Stop_Flag != 20);
	Roadway_Flag_clean();	//�����־λ״̬		
}

//������ת����
//turn_llmd(90,90,850);
void turn_llmd(uint8_t LSpend,uint8_t RSpend,uint16_t MP)	
{
	Roadway_mp_syn();	//����ͬ��
	Stop_Flag = 0;
	Go_Flag = 0;
	wheel_L_Flag = 0;
	wheel_R_Flag = 0;
	wheel_Nav_Flag = 0;
	Back_Flag = 0;
	turnflag=1;//ת��
	Track_Forward_Flag = 0;
	Track_Back_Flag = 0;
	temp_MP = MP;
	LCar_Spend = LSpend;
	RCar_Spend = RSpend;
	Control(LCar_Spend ,-RCar_Spend);
	while (Stop_Flag != 20);
	Roadway_Flag_clean();	//�����־λ״̬	
	Send_UpMotor(0,0);
}

void Track_duoy_RFID(int LSpeed,int RSpeed)   // ѭ��Ѱ������
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
	Roadway_Flag_clean();	//�����־λ״̬	
}


/*
//������ѭ���ϰ���
ѭ������Ȼ��ǰ��һ��ľ����Ӱ����ת���˶�׼�����ϰ����λ��(ѭ���Ƶ�λ�ã�����1011 1111),
�������ں����ڽ���ѭ���Ļ������͵Ĵ�գ��������ǰ��֮����ת��λ����1110 1111�����ӵģ��ں�����ѭ���ڳ壬�ٷ�֮�ٹ�ȥ��

<---   --->
		^
		|
		|
Tracking_Forward(70,95);
Forward(70,95,350);  //ʮ��·��ǰ��һ��

turn_ppmd(90,90,820);
turn_llmd(90,90,1650);
delay_ms(20);
Back_1(85,70,200);
delay_ms(20);
Tracking_Forward(70,95);
Forward(70,95,2000);  //ʮ��·��ǰ��һ��


*/
