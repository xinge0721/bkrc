#ifndef __TRAFFICLIGHT_H
#define __TRAFFICLIGHT_H
#include "stm32f4xx.h"
#include "sys.h"

void Recognition_Erweima(void);							//����A72ʶ���ά��
void Ask_Traffic_Light(void);								//��ͨ�ƽ���ʶ��ģʽ
void Traffic_Light_Ask_A72(void);						//����A72ʶ��ͨ��
void Recognition_Traffic_Light(void);				//ʶ��ͨ��
void Traffic_Light_result(uint8_t result);	//��ͨ������ȷ��ʶ����  0x01 ��� 0x02 �̵� 0x03 �Ƶ�
#endif
