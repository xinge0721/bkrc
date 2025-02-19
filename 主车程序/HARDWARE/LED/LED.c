#include <stdio.h>
#include "stm32f4xx.h"
#include "LED.h"
#include "canP_HostCom.h"
#include "ultrasonic.h"
#include "delay.h"
int sizedis[3] = {0,0,0};//��������ֵ

/***************************************************************
** ���ܣ�     LED��ʾ��־�￪ʼ��ʱ    
** ������	  	��
** ����ֵ��   ��
** ˵����     ����ͨ��zigbee����LED��ʾ��־��ļ�ʱ���ܿ���
****************************************************************/
void LED_Timer_open(void)
{
    Send_zigbee[0]=0x55;
    Send_zigbee[1]=0x04;
    Send_zigbee[2]=0x03;
    Send_zigbee[3]=0x01;
    Send_zigbee[4]=0x00;
    Send_zigbee[5]=0x00;
    Send_zigbee[6]=(Send_zigbee[2]+Send_zigbee[3]+Send_zigbee[4]+Send_zigbee[5])%256;
    Send_zigbee[7]=0xbb;
    delay_ms(1);
		Send_ZigbeeData_To_Fifo(Send_zigbee,8);
    delay_ms(100);
		Send_zigbee[0]=0;//�������,��Ϊ��ʱ��ֻ���˼�λ
		Send_zigbee[1]=0;
		Send_zigbee[2]=0;
		Send_zigbee[3]=0;
		Send_zigbee[4]=0;
		Send_zigbee[5]=0;
		Send_zigbee[6]=0;
		Send_zigbee[7]=0;
}
/***************************************************************
** ���ܣ�     LED��ʾ��־���ʱ���� 
** ������	 	 ��
** ����ֵ��   ��
** ˵����     ����ͨ��zigbee����LED��ʾ��־��ļ�ʱ���ܹر�
****************************************************************/
void LED_Timer_close(void)
{
		Send_zigbee[0]=0x55;
    Send_zigbee[1]=0x04;
    Send_zigbee[2]=0x03;
    Send_zigbee[3]=0x00;
    Send_zigbee[4]=0x00;
    Send_zigbee[5]=0x00;
    Send_zigbee[6]=(Send_zigbee[2]+Send_zigbee[3]+Send_zigbee[4]+Send_zigbee[5])%256;
    Send_zigbee[7]=0xbb;
    delay_ms(1);
		Send_ZigbeeData_To_Fifo(Send_zigbee,8);
    delay_ms(100);
		Send_zigbee[0]=0;//�������,��Ϊ��ʱ��ֻ���˼�λ
		Send_zigbee[1]=0;
		Send_zigbee[2]=0;
		Send_zigbee[3]=0;
		Send_zigbee[4]=0;
		Send_zigbee[5]=0;
		Send_zigbee[6]=0;
		Send_zigbee[7]=0;
}
/***************************************************************
** ���ܣ�     LED��ʾ��־���ʱ����  
** ������	  	��
** ����ֵ��   ��
** ˵����     ����ͨ��zigbee����LED��ʾ��־��ļ�ʱ����
****************************************************************/
void LED_Timer_clear(void)
{
		Send_zigbee[0]=0x55;
    Send_zigbee[1]=0x04;
    Send_zigbee[2]=0x03;
    Send_zigbee[3]=0x02;
    Send_zigbee[4]=0x00;
    Send_zigbee[5]=0x00;
    Send_zigbee[6]=(Send_zigbee[2]+Send_zigbee[3]+Send_zigbee[4]+Send_zigbee[5])%256;
    Send_zigbee[7]=0xbb;
    delay_ms(1);
		Send_ZigbeeData_To_Fifo(Send_zigbee,8);
    delay_ms(100);
		Send_zigbee[0]=0;//�������,��Ϊ��ʱ��ֻ���˼�λ
		Send_zigbee[1]=0;
		Send_zigbee[2]=0;
		Send_zigbee[3]=0;
		Send_zigbee[4]=0;
		Send_zigbee[5]=0;
		Send_zigbee[6]=0;
		Send_zigbee[7]=0;
}
/***************************************************************
** ���ܣ�     LED��ʾ��־��ڶ�������
** ������	  	��
** ����ֵ��   ��
** ˵����     ����ͨ��zigbee����LED��ʾ��־��ĵڶ�������
****************************************************************/
void LED_secondline_clear(void)
{
		Send_zigbee[0]=0x55;
    Send_zigbee[1]=0x04;
    Send_zigbee[2]=0x02;
    Send_zigbee[3]=0x00;
    Send_zigbee[4]=0x00;
    Send_zigbee[5]=0x00;
    Send_zigbee[6]=(Send_zigbee[2]+Send_zigbee[3]+Send_zigbee[4]+Send_zigbee[5])%256;
    Send_zigbee[7]=0xbb;
    delay_ms(1);
		Send_ZigbeeData_To_Fifo(Send_zigbee,8);
    delay_ms(100);
		Send_zigbee[0]=0;//�������,��Ϊ��ʱ��ֻ���˼�λ
		Send_zigbee[1]=0;
		Send_zigbee[2]=0;
		Send_zigbee[3]=0;
		Send_zigbee[4]=0;
		Send_zigbee[5]=0;
		Send_zigbee[6]=0;
		Send_zigbee[7]=0;
}

/***************************************************************
** ���ܣ�     LED��ʾ��־������д���һ�������
** ������	  	num1 num2 num3  д������ݣ���ʮ�����Ʊ�ʾ
** ����ֵ��   ��
** ˵����     ����ͨ��zigbee����LED��ʾ��־������д���һ�������
****************************************************************/
void LED_Firstline_Data(uint8_t num1, uint8_t num2, uint8_t num3)
{
		Send_zigbee[0] = 0x55;
    Send_zigbee[1] = 0x04;
    Send_zigbee[2] = 0x01;
    Send_zigbee[3] = num1;
    Send_zigbee[4] = num2;
    Send_zigbee[5] = num3;
    Send_zigbee[6] = (Send_zigbee[2]+Send_zigbee[3]+Send_zigbee[4]+Send_zigbee[5])%256;
    Send_zigbee[7] = 0xbb;
    delay_ms(1);
		Send_ZigbeeData_To_Fifo(Send_zigbee,8);
    delay_ms(100);
		Send_zigbee[0]=0;//�������,��Ϊ��ʱ��ֻ���˼�λ
		Send_zigbee[1]=0;
		Send_zigbee[2]=0;
		Send_zigbee[3]=0;
		Send_zigbee[4]=0;
		Send_zigbee[5]=0;
		Send_zigbee[6]=0;
		Send_zigbee[7]=0;
}

/***************************************************************
** ���ܣ�     LED��ʾ��־������д��ڶ��������
** ������	  	num1 num2 num3  д������ݣ���ʮ�����Ʊ�ʾ
** ����ֵ��   ��
** ˵����     ����ͨ��zigbee����LED��ʾ��־������д��ڶ��������
****************************************************************/
void LED_secondline_Data(uint8_t num1, uint8_t num2, uint8_t num3)
{
		Send_zigbee[0] = 0x55;
    Send_zigbee[1] = 0x04;
    Send_zigbee[2] = 0x02;
    Send_zigbee[3] = num1;
    Send_zigbee[4] = num2;
    Send_zigbee[5] = num3;
    Send_zigbee[6] = (Send_zigbee[2]+Send_zigbee[3]+Send_zigbee[4]+Send_zigbee[5])%256;
    Send_zigbee[7] = 0xbb;
    delay_ms(1);
		Send_ZigbeeData_To_Fifo(Send_zigbee,8);
    delay_ms(100);
		Send_zigbee[0]=0;//�������,��Ϊ��ʱ��ֻ���˼�λ
		Send_zigbee[1]=0;
		Send_zigbee[2]=0;
		Send_zigbee[3]=0;
		Send_zigbee[4]=0;
		Send_zigbee[5]=0;
		Send_zigbee[6]=0;
		Send_zigbee[7]=0;
}

///***************************************************************
//** ���ܣ�     LED��ʾ��־��ڶ�����ʾ����
//** ������	  	distance ��ʾ�ľ��� ��ʮ���Ʊ�ʾ
//** ����ֵ��   ��
//** ˵����     ����ͨ��zigbee����LED��ʾ��־��ڶ�����ʾ����
//****************************************************************/
//void LED_secondline_ShowDistance(int distance)
//{
//		Send_zigbee[0] = 0x55;
//    Send_zigbee[1] = 0x04;
//    Send_zigbee[2] = 0x04;
//    Send_zigbee[3] = 0x00;
//    Send_zigbee[4] = (distance / 100);
//    Send_zigbee[5] = ((distance / 10 % 10) << 4) + (distance % 10);
//    Send_zigbee[6] = (Send_zigbee[2]+Send_zigbee[3]+Send_zigbee[4]+Send_zigbee[5])%256;
//    Send_zigbee[7] = 0xbb;
//    delay_ms(1);
//		Send_ZigbeeData_To_Fifo(Send_zigbee,8);
//    delay_ms(100);
//}

/**********************************************************************
 * �� �� �� ��  LED��ʾ��־����ʾ�����Ϣ(������ǹٷ��ģ��������д�ģ������)
 * ��    �� ��  dis ����ֵ
 * �� �� ֵ ��  ��
 * ��    �� ��  LED_Dis_Zigbee(351);//���351 ���1����λ����
**********************************************************************/
void LED_Dis_Zigbee(uint16_t dis)
{
    Send_zigbee[0] = 0x55;
    Send_zigbee[1] = 0x04;
    Send_zigbee[2] = 0x04;
    Send_zigbee[3] = 0x00;
    Send_zigbee[4] = dis / 100 % 10;
    Send_zigbee[5] = ((dis / 10 % 10) * 16 + dis % 10);
    Send_zigbee[6] = (Send_zigbee[2] + Send_zigbee[3] + Send_zigbee[4] + Send_zigbee[5]) % 256;
    Send_zigbee[7] = 0xBB;
    Send_ZigbeeData_To_Fifo(Send_zigbee, 8);  //����Zigbee����
    delay_ms(100);
		Send_zigbee[0]=0;//�������,��Ϊ��ʱ��ֻ���˼�λ
		Send_zigbee[1]=0;
		Send_zigbee[2]=0;
		Send_zigbee[3]=0;
		Send_zigbee[4]=0;
		Send_zigbee[5]=0;
		Send_zigbee[6]=0;
		Send_zigbee[7]=0;
}


/**
Ѱ�����ֵ
**/
int Find_Max(int *dis,int count)
{
	int max;
	max=dis[0];
	for(int i=0;i<count;i++)
	{
		if(dis[i]>max)
		{
			max=dis[i];
		}
	}
	return max;
}

/**********************************************************************
 * �� �� �� ��  LED��ʾ��־����ʾ�����Ϣ��Ѱ�����ֵ
 * ��    �� ��  dis ����ֵ
 * �� �� ֵ ��  ��
Ultrasonic_Ranging();//������

**********************************************************************/
void LED_Ult_dis(void)
{
	int max;
	for(int j=0;j<3;j++)
	{
		Ultrasonic_Ranging();//������
		delay_ms(100);
		sizedis[j] = (int)dis;
	}
	max=Find_Max(sizedis,3);
	LED_Dis_Zigbee(max);//��ʾ��LED
	delay_ms(10);
	LED_Dis_Zigbee(max);
	delay_ms(10);
}
