#ifndef __GARAGE_h
#define __GARAGE_h
#include "stm32f4xx.h"
#include "sys.h"

void litiGarageF1(char Device);					//立体车库到达第一层函数声明		
void litiGarageF2(char Device);					//立体车库到达第二层函数声明	
void litiGarageF3(char Device);					//立体车库到达第三层函数声明	
void litiGarageF4(char Device);					//立体车库到达第四层函数声明
void Request_Location(char Device);			    //请求返回车库位于第几层函数声明
void Request_Infrared(char Device);			    //请求返回前后侧红外状态函数声明
#endif
