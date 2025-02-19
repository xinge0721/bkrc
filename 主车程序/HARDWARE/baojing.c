#include "stm32f4xx.h"                  // Device header
#include "delay.h"
#include "canP_HostCom.h"
/*
修改报警台开启指令
*/
void baojing_send(uint8_t one,uint8_t two,uint8_t three,uint8_t four,uint8_t five,uint8_t six)
{          
    Send_zigbee[0]=0x55;
    Send_zigbee[1]=0x03;
    Send_zigbee[2]=0x10;
    Send_zigbee[3]=one;
    Send_zigbee[4]=two;
    Send_zigbee[5]=three;
    Send_zigbee[6]=(0x10+one+two+three)%256;
    Send_zigbee[7]=0xbb;
    delay_ms(1);
    Send_ZigbeeData_To_Fifo(Send_zigbee,8);
    delay_ms(500);
	
    Send_zigbee[0]=0x55;
    Send_zigbee[1]=0x03;
    Send_zigbee[2]=0x11;
    Send_zigbee[3]=four;
    Send_zigbee[4]=five;
    Send_zigbee[5]=six;
    Send_zigbee[6]=(0x11+four+five+six)%256;
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

