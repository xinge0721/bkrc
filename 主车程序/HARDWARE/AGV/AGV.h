#ifndef __AGV_h
#define __AGV_h
#include "stm32f4xx.h"
#include "sys.h"

extern uint16_t ucErrTime;
void yunshuche_xieyi(void);
void zhuche_Ack(void);
void zhuche_Ask_congche(void);								//主车返回给从车的应答信号
void yunshuche_go(uint8_t speed,uint16_t mapan);		    //用Zigbee控制移动机器人前进函数声明
void yunshuche_back(uint8_t speed,uint16_t mapan);		  //用Zigbee控制移动机器人后退函数声明
void yunshuche_left(uint8_t speed);			          //用Zigbee控制移动机器人左转函数声明
void yunshuche_right(uint8_t speed);		            //用Zigbee控制移动机器人右转函数声明
void yunshuche_stop(void);			     	     		//用Zigbee控制移动机器人停止函数声明
void yunshuche_xj(uint8_t speed);			         		//用Zigbee控制移动机器人寻迹函数声明
void yunshuche_start(void);
void yunshuche_start2(void);
void yunshuche_start3(void);
void yunshuche_turnright(void);
void yunshuche_turnleft(void);
void yunshuche_xieyi2(void);
#endif

