#include <stdio.h>
#include "stm32f4xx.h"
#include "LED.h"
#include "canP_HostCom.h"
#include "ultrasonic.h"
#include "delay.h"
int sizedis[3] = {0,0,0};//超声波数值

/***************************************************************
** 功能：     LED显示标志物开始计时    
** 参数：	  	无
** 返回值：   无
** 说明：     主车通过zigbee控制LED显示标志物的计时功能开启
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
		Send_zigbee[0]=0;//清除数组,因为有时候只发了几位
		Send_zigbee[1]=0;
		Send_zigbee[2]=0;
		Send_zigbee[3]=0;
		Send_zigbee[4]=0;
		Send_zigbee[5]=0;
		Send_zigbee[6]=0;
		Send_zigbee[7]=0;
}
/***************************************************************
** 功能：     LED显示标志物计时结束 
** 参数：	 	 无
** 返回值：   无
** 说明：     主车通过zigbee控制LED显示标志物的计时功能关闭
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
		Send_zigbee[0]=0;//清除数组,因为有时候只发了几位
		Send_zigbee[1]=0;
		Send_zigbee[2]=0;
		Send_zigbee[3]=0;
		Send_zigbee[4]=0;
		Send_zigbee[5]=0;
		Send_zigbee[6]=0;
		Send_zigbee[7]=0;
}
/***************************************************************
** 功能：     LED显示标志物计时清零  
** 参数：	  	无
** 返回值：   无
** 说明：     主车通过zigbee控制LED显示标志物的计时清零
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
		Send_zigbee[0]=0;//清除数组,因为有时候只发了几位
		Send_zigbee[1]=0;
		Send_zigbee[2]=0;
		Send_zigbee[3]=0;
		Send_zigbee[4]=0;
		Send_zigbee[5]=0;
		Send_zigbee[6]=0;
		Send_zigbee[7]=0;
}
/***************************************************************
** 功能：     LED显示标志物第二行清零
** 参数：	  	无
** 返回值：   无
** 说明：     主车通过zigbee控制LED显示标志物的第二行清零
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
		Send_zigbee[0]=0;//清除数组,因为有时候只发了几位
		Send_zigbee[1]=0;
		Send_zigbee[2]=0;
		Send_zigbee[3]=0;
		Send_zigbee[4]=0;
		Send_zigbee[5]=0;
		Send_zigbee[6]=0;
		Send_zigbee[7]=0;
}

/***************************************************************
** 功能：     LED显示标志物数据写入第一排数码管
** 参数：	  	num1 num2 num3  写入的数据，用十六进制表示
** 返回值：   无
** 说明：     主车通过zigbee控制LED显示标志物数据写入第一排数码管
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
		Send_zigbee[0]=0;//清除数组,因为有时候只发了几位
		Send_zigbee[1]=0;
		Send_zigbee[2]=0;
		Send_zigbee[3]=0;
		Send_zigbee[4]=0;
		Send_zigbee[5]=0;
		Send_zigbee[6]=0;
		Send_zigbee[7]=0;
}

/***************************************************************
** 功能：     LED显示标志物数据写入第二排数码管
** 参数：	  	num1 num2 num3  写入的数据，用十六进制表示
** 返回值：   无
** 说明：     主车通过zigbee控制LED显示标志物数据写入第二排数码管
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
		Send_zigbee[0]=0;//清除数组,因为有时候只发了几位
		Send_zigbee[1]=0;
		Send_zigbee[2]=0;
		Send_zigbee[3]=0;
		Send_zigbee[4]=0;
		Send_zigbee[5]=0;
		Send_zigbee[6]=0;
		Send_zigbee[7]=0;
}

///***************************************************************
//** 功能：     LED显示标志物第二排显示距离
//** 参数：	  	distance 显示的距离 用十进制表示
//** 返回值：   无
//** 说明：     主车通过zigbee控制LED显示标志物第二排显示距离
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
 * 函 数 名 ：  LED显示标志物显示测距信息(上面的是官方的，这个是新写的，用这个)
 * 参    数 ：  dis 距离值
 * 返 回 值 ：  无
 * 简    例 ：  LED_Dis_Zigbee(351);//测距351 误差1，单位毫米
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
    Send_ZigbeeData_To_Fifo(Send_zigbee, 8);  //发送Zigbee数据
    delay_ms(100);
		Send_zigbee[0]=0;//清除数组,因为有时候只发了几位
		Send_zigbee[1]=0;
		Send_zigbee[2]=0;
		Send_zigbee[3]=0;
		Send_zigbee[4]=0;
		Send_zigbee[5]=0;
		Send_zigbee[6]=0;
		Send_zigbee[7]=0;
}


/**
寻找最大值
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
 * 函 数 名 ：  LED显示标志物显示测距信息—寻找最大值
 * 参    数 ：  dis 距离值
 * 返 回 值 ：  无
Ultrasonic_Ranging();//超声波

**********************************************************************/
void LED_Ult_dis(void)
{
	int max;
	for(int j=0;j<3;j++)
	{
		Ultrasonic_Ranging();//超声波
		delay_ms(100);
		sizedis[j] = (int)dis;
	}
	max=Find_Max(sizedis,3);
	LED_Dis_Zigbee(max);//显示到LED
	delay_ms(10);
	LED_Dis_Zigbee(max);
	delay_ms(10);
}
