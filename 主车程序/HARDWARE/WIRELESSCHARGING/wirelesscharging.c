#include "wirelesscharging.h"
#include "canP_HostCom.h"
#include "delay.h"

/***************************************************************
** 功能：    无线充电标志物开启—固定指令
** 参数：	   无
** 返回值：  无
** 说明：    主车通过zigbee控制无线充电标志物开启
****************************************************************/
void Wireless_charging_open(void)
{
		Send_zigbee[0]=0x55;
    Send_zigbee[1]=0x0A;
    Send_zigbee[2]=0x01;
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
** 功能：    无线充电标志物关闭
** 参数：	 	 无
** 返回值：  无
** 说明：    主车通过zigbee控制无线充电标志物关闭
****************************************************************/
void Wireless_charging_close(void)
{
		Send_zigbee[0]=0x55;
    Send_zigbee[1]=0x0A;
    Send_zigbee[2]=0x01;
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
** 功能：    无线充电标志物开启码开启—默认指令
** 参数：	   无
** 返回值：  无
** 说明：    主车通过zigbee控制无线充电标志物开启
****************************************************************/
void Wireless_charging_open_start(void)
{
		Send_zigbee[0]=0x55;
    Send_zigbee[1]=0x0A;
    Send_zigbee[2]=0x02;
    Send_zigbee[3]=0xA1;
    Send_zigbee[4]=0x23;
    Send_zigbee[5]=0xB4;
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
** 功能：    无线充电标志物开启码开启—做任务获得指令数据开启
** 参数：	   无
** 返回值：  无
** 说明：    主车通过zigbee控制无线充电标志物开启
****************************************************************/
void Wireless_charging_ap(uint8_t a,uint8_t b,uint8_t c)
{
		Send_zigbee[0]=0x55;
    Send_zigbee[1]=0x0A;
    Send_zigbee[2]=0x02;
    Send_zigbee[3]=a;
    Send_zigbee[4]=b;
    Send_zigbee[5]=c;
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
