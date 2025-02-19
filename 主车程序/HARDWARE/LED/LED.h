#ifndef __LED_h
#define __LED_h
#include "stm32f4xx.h"
#include "sys.h"

void LED_Timer_open(void);		    	  														//LED��ʾ��־���ʱ��ʼ��������
void LED_Timer_close(void);			 	  															//LED��ʾ��־���ʱ������������
void LED_Timer_clear(void);			 	  															//LED��ʾ��־���ʱ���㺯������
void LED_secondline_clear(void);																	//LED����ܵڶ������㺯��˵��
void LED_xianshi_juli(void);			  															//LED����ܵڶ�����ʾ���뺯��˵��
void LED_Firstline_Data(uint8_t num1, uint8_t num2, uint8_t num3);//LED��ʾ��־������д���һ��
void LED_secondline_Data(uint8_t num1, uint8_t num2, uint8_t num3);//LED��ʾ��־������д��ڶ���
void LED_secondline_ShowDistance(int distance); 									//LED��ʾ��־��ڶ�����ʾ����(�ٷ�)
void LED_Dis_Zigbee(uint16_t dis);                                //LED��ʾ��־����ʾ�����Ϣ(��д��)
void LED_Ult_dis(void);

#endif
