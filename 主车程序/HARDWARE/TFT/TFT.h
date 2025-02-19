#ifndef __TFT_h
#define __TFT_h
#include "stm32f4xx.h"
#include "sys.h"

void TFT_send_chepai(char Device,char *Licence);   //向TFT显示车牌信息
void TFT_jishi_open(char Device);		   		//TFT标志物计时开始函数声明
void TFT_jishi_close(char Device);		    	//TFT标志物计时结束函数声明
void TFT_jishi_clear(char Device);    //TFT标志物计时清零函数声明
void TFT_Picture_Show(char Device,uint8_t Num); //TFT标志物显示图片
void TFT_Picture_Up(char Device);					//TFT标志物图片向上翻页
void TFT_Picture_Down(char Device);				//TFT标志物图片向下翻页
void TFT_Picture_AutomaticDown(char Device);//TFT标志物图片自动翻页
void TFT_Picture_DistanceShow(char Device,uint8_t dis); //TFT标志物距离显示模式

#endif
