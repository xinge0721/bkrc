#ifndef __TBA_H
#define __TBA_H
#include "stm32f4xx.h"
#include "sys.h"

#define L_LED 	PHout(10)//低电平点亮
#define R_LED 	PHout(11)//低电平点亮
#define Beep    PCout(13)//低电平则响

void Tba_Init(void);//任务板初始化
uint8_t Get_tba_phsis_value(void);//读任务板光敏电阻电平


#endif

