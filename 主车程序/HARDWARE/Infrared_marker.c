#include <stdio.h>
#include "stm32f4xx.h"
#include "Infrared_marker.h"
#include "canP_HostCom.h"
#include "infrared.h"
#include "delay.h"
#include "bh1750.h"
#include "data_base.h"
#include "tba.h"



/***************************************************************
** ���ܣ�     ������ʾ��־����ʾ��ɫ
** ������	  color ���ͣ�Ҫ������ɫ
** ����ֵ��   ��
** ˵����     ����ͨ���������������ʾ��־����ʾ��ɫ
****************************************************************/
void liti_color(u8 color)
{
    Send_Infrared[0]=0xFF;
    Send_Infrared[1]=0x13;
    Send_Infrared[2]=color;
    Send_Infrared[3]=0x00;
    Send_Infrared[4]=0x00;
    Send_Infrared[5]=0x00;
    delay_ms(1);
    Infrared_Send(Send_Infrared,6);
    delay_ms(100); 
}
/***************************************************************
** ���ܣ�     ������ʾ��־����ʾ��״
** ������	  shape ���ͣ�Ҫ������״
** ����ֵ��   ��
** ˵����     ����ͨ���������������ʾ��־����ʾ��״
****************************************************************/
void liti_shape(u8 shape)
{
    Send_Infrared[0]=0xFF;
    Send_Infrared[1]=0x12;
    Send_Infrared[2]=shape;
    Send_Infrared[3]=0x00;
    Send_Infrared[4]=0x00;
    Send_Infrared[5]=0x00;
    delay_ms(1);
    Infrared_Send(Send_Infrared,6);
    delay_ms(100); 
}

/***************************************************************
** ���ܣ�     ������ʾ��־����ʾ����
** ������	  	��
** ����ֵ��   ��
** ˵����     ��
****************************************************************/
void qian_4chepai(void)
{
    Send_Infrared[0]=0xFF;
    Send_Infrared[1]=0x20;
    Send_Infrared[2]='W';
    Send_Infrared[3]=3;
    Send_Infrared[4]=7;
    Send_Infrared[5]=4;
    delay_ms(1);
    Infrared_Send(Send_Infrared,6);
    delay_ms(100); 
}

void hou_2chepai(void)
{
    Send_Infrared[0]=0xFF;
    Send_Infrared[1]=0x10;
    Send_Infrared[2]='X';
    Send_Infrared[3]=9;
    Send_Infrared[4]='F';
    Send_Infrared[5]=2;
    delay_ms(1);
    Infrared_Send(Send_Infrared,6);
    delay_ms(100); 
}

