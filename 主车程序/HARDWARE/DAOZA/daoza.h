#ifndef __DAOZA_H__
#define __DAOZA_H__
#include "sys.h"
#include "stm32f4xx.h"

void daozha_open(void);	//打开道闸
void daozha_close(void);//关闭道闸
void daozha_ack(void);	//道闸状态回传
void daozha_send_chepai(uint8_t one, uint8_t two,uint8_t three,uint8_t four,uint8_t five,uint8_t six);  
												//向道闸发送车牌
void daoza_open_ack(void);
#endif
