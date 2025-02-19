#ifndef __TFT_h
#define __TFT_h
#include "stm32f4xx.h"
#include "sys.h"

void TFT_send_chepai(char Device,char *Licence);   //��TFT��ʾ������Ϣ
void TFT_jishi_open(char Device);		   		//TFT��־���ʱ��ʼ��������
void TFT_jishi_close(char Device);		    	//TFT��־���ʱ������������
void TFT_jishi_clear(char Device);    //TFT��־���ʱ���㺯������
void TFT_Picture_Show(char Device,uint8_t Num); //TFT��־����ʾͼƬ
void TFT_Picture_Up(char Device);					//TFT��־��ͼƬ���Ϸ�ҳ
void TFT_Picture_Down(char Device);				//TFT��־��ͼƬ���·�ҳ
void TFT_Picture_AutomaticDown(char Device);//TFT��־��ͼƬ�Զ���ҳ
void TFT_Picture_DistanceShow(char Device,uint8_t dis); //TFT��־�������ʾģʽ

#endif
