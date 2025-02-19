#ifndef __Infrared_marker_h
#define __Infrared_marker_h
#include "stm32f4xx.h"
#include "sys.h"

/*****��������*****/
uint8_t Read_Street_light(void);
/*****************/

void qian_4chepai(void);
void hou_2chepai(void);
void Street_light_one(void);        //����·�ƹ�Դ��λ+1
void Street_light_two(void);        //����·�ƹ�Դ��λ+2
void Street_light_three(void);      //����·�ƹ�Դ��λ+3

void liti_shape(uint8_t shape);    //������ʾ��״��Ϣ
void liti_color(uint8_t color);    //������ʾ��ɫ��Ϣ


//static u8 H_1[4]={0x00,0xFF,0x0C,~(0x0C)};	 //��Դ��λ��1
//static u8 H_2[4]={0x00,0xFF,0x18,~(0x18)};	 //��Դ��λ��2
//static u8 H_3[4]={0x00,0xFF,0x5E,~(0x5E)};	 //��Դ��λ��3






#endif
