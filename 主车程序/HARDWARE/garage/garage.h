#ifndef __GARAGE_h
#define __GARAGE_h
#include "stm32f4xx.h"
#include "sys.h"

void litiGarageF1(char Device);					//���峵�⵽���һ�㺯������		
void litiGarageF2(char Device);					//���峵�⵽��ڶ��㺯������	
void litiGarageF3(char Device);					//���峵�⵽������㺯������	
void litiGarageF4(char Device);					//���峵�⵽����Ĳ㺯������
void Request_Location(char Device);			    //���󷵻س���λ�ڵڼ��㺯������
void Request_Infrared(char Device);			    //���󷵻�ǰ������״̬��������
#endif
