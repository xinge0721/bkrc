#ifndef __DAOZA_H__
#define __DAOZA_H__
#include "sys.h"
#include "stm32f4xx.h"

void daozha_open(void);	//�򿪵�բ
void daozha_close(void);//�رյ�բ
void daozha_ack(void);	//��բ״̬�ش�
void daozha_send_chepai(uint8_t one, uint8_t two,uint8_t three,uint8_t four,uint8_t five,uint8_t six);  
												//���բ���ͳ���
void daoza_open_ack(void);
#endif
