#ifndef __TBA_H
#define __TBA_H
#include "stm32f4xx.h"
#include "sys.h"

#define L_LED 	PHout(10)//�͵�ƽ����
#define R_LED 	PHout(11)//�͵�ƽ����
#define Beep    PCout(13)//�͵�ƽ����

void Tba_Init(void);//������ʼ��
uint8_t Get_tba_phsis_value(void);//���������������ƽ


#endif

