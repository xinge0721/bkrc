#ifndef __test_h
#define __test_h
#include "stm32f4xx.h"
#include "sys.h"

void zhuche_xunji(int speed);  		  	//主车循迹函数
void go_ahead(int speed,u16 mapan);   //前进函数声明
void back_off(int speed,u16 mapan);   //后退函数声明
void turn_left(int speed);            //左转函数声明
void turn_right(int speed);           //右转函数声明
void stop(void);                      //停止函数声明


#endif


