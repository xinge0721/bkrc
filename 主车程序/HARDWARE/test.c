
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

void autorun(void);     			  //ȫ�Զ���������
void zhuche_xunji(int speed);  		  //����ѭ������
void go_ahead(int speed,u16 mapan);   //ǰ����������
void back_off(int speed,u16 mapan);   //���˺�������
void turn_left(int speed);            //��ת��������
void turn_right(int speed);           //��ת��������
void stop(void);                      //ֹͣ��������



/***************************************************************
** ���ܣ�     ����ǰ��
** ������	  speed�����ͣ������ٶȣ�mapan��u16�ͣ�����ֵ
** ����ֵ��   ��
** ˵����     ����ֵԽ��ǰ������Խ��������ֵ���Ϊ65535 
****************************************************************/
void go_ahead(int speed,u16 mapan)
{
	Roadway_mp_syn();	//����ͬ��
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
** ���ܣ�     ��������
** ������	  speed�����ͣ������ٶȣ�mapan��u16�ͣ�����ֵ
** ����ֵ��   ��
** ˵����     ����ֵԽ��ǰ������Խ��������ֵ���Ϊ65535 
****************************************8************************/
void back_off(int speed,u16 mapan)
{
  Roadway_mp_syn();	//����ͬ��
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
** ���ܣ�     ������ת
** ������	  speed�����ͣ������ٶ�
** ����ֵ��   �� 
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
** ���ܣ�     ������ת
** ������	  speed�����ͣ������ٶ�
** ����ֵ��   �� 
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
** ���ܣ�     ����ֹͣ
** ������	  �޲���
** ����ֵ��   �� 
****************************************************************/
void stop(void)
{
   Roadway_Flag_clean();	//�����־λ״̬	
}
/***************************************************************
** ���ܣ�   ����Ѱ��
** ������	  speed�����ͣ������ٶ�
** ����ֵ��   �� 
****************************************************************/
void zhuche_xunji(int speed)
{	
	 Go_Flag=1;
	 Stop_Flag=0;
   Car_Spend =speed;				      //�����ٶ�		
	 Track_Flag=1;				  
	 TIM_Cmd(TIM2, ENABLE);
}


