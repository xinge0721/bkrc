#ifndef __AGV_h
#define __AGV_h
#include "stm32f4xx.h"
#include "sys.h"

extern uint16_t ucErrTime;
void yunshuche_xieyi(void);
void zhuche_Ack(void);
void zhuche_Ask_congche(void);								//�������ظ��ӳ���Ӧ���ź�
void yunshuche_go(uint8_t speed,uint16_t mapan);		    //��Zigbee�����ƶ�������ǰ����������
void yunshuche_back(uint8_t speed,uint16_t mapan);		  //��Zigbee�����ƶ������˺��˺�������
void yunshuche_left(uint8_t speed);			          //��Zigbee�����ƶ���������ת��������
void yunshuche_right(uint8_t speed);		            //��Zigbee�����ƶ���������ת��������
void yunshuche_stop(void);			     	     		//��Zigbee�����ƶ�������ֹͣ��������
void yunshuche_xj(uint8_t speed);			         		//��Zigbee�����ƶ�������Ѱ����������
void yunshuche_start(void);
void yunshuche_start2(void);
void yunshuche_start3(void);
void yunshuche_turnright(void);
void yunshuche_turnleft(void);
void yunshuche_xieyi2(void);
#endif

