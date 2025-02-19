#ifndef __Infrared_marker_h
#define __Infrared_marker_h
#include "stm32f4xx.h"
#include "sys.h"

/*****新增函数*****/
uint8_t Read_Street_light(void);
/*****************/

void qian_4chepai(void);
void hou_2chepai(void);
void Street_light_one(void);        //智能路灯光源档位+1
void Street_light_two(void);        //智能路灯光源档位+2
void Street_light_three(void);      //智能路灯光源档位+3

void liti_shape(uint8_t shape);    //立体显示形状信息
void liti_color(uint8_t color);    //立体显示颜色信息


//static u8 H_1[4]={0x00,0xFF,0x0C,~(0x0C)};	 //光源档位加1
//static u8 H_2[4]={0x00,0xFF,0x18,~(0x18)};	 //光源档位加2
//static u8 H_3[4]={0x00,0xFF,0x5E,~(0x5E)};	 //光源档位加3






#endif
