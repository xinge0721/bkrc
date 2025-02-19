#ifndef __LED_h
#define __LED_h
#include "stm32f4xx.h"
#include "sys.h"

void LED_Timer_open(void);		    	  														//LED显示标志物计时开始函数声明
void LED_Timer_close(void);			 	  															//LED显示标志物计时结束函数声明
void LED_Timer_clear(void);			 	  															//LED显示标志物计时清零函数声明
void LED_secondline_clear(void);																	//LED数码管第二行清零函数说明
void LED_xianshi_juli(void);			  															//LED数码管第二行显示距离函数说明
void LED_Firstline_Data(uint8_t num1, uint8_t num2, uint8_t num3);//LED显示标志物数据写入第一排
void LED_secondline_Data(uint8_t num1, uint8_t num2, uint8_t num3);//LED显示标志物数据写入第二排
void LED_secondline_ShowDistance(int distance); 									//LED显示标志物第二行显示距离(官方)
void LED_Dis_Zigbee(uint16_t dis);                                //LED显示标志物显示测距信息(新写的)
void LED_Ult_dis(void);

#endif
