#ifndef __test_h
#define __test_h
#include "stm32f4xx.h"
#include "sys.h"

void zhuche_xunji(int speed);  		  	//����ѭ������
void go_ahead(int speed,u16 mapan);   //ǰ����������
void back_off(int speed,u16 mapan);   //���˺�������
void turn_left(int speed);            //��ת��������
void turn_right(int speed);           //��ת��������
void stop(void);                      //ֹͣ��������


#endif


