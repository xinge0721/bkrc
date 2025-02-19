#include "daoza.h"
#include "delay.h"
#include "canP_HostCom.h"

/***************************************************************
** 功能：     打开道闸 
** 参数：	    无
** 返回值：   无
** 说明：     主车通过zigbee控制道闸开启
				daozatime = gt_get()+50;
				while(1)
				{
					
					if(Stop_Flag == 0x05)//检测道闸是否打开
					{
						Tracking_Forward(74,70);//70速度循迹前进
						Forward(55,100,250);  //十字路口前进一点
						Stop_Flag = 0;
						break;
					}
					else
					{
						if(gt_get_sub(daozatime) == 0)//50ms检测一次
						{
							daozatime = gt_get()+50;
							daozha_open();//打开道闸
							delay_ms(50);
							daozha_ack();
							delay_ms(50);
							daozatuichu++;//积累时间，如果道闸长时间没反应则冲出去
							if(daozatuichu ==80)
							{
								daozatuichu =0;
								Tracking_Forward(74,70);//70速度循迹前进
								Forward(55,100,250);  //十字路口前进一点
								break;
							}
						}
						Can_ZigBeeRx_Check();
					}
				}
****************************************************************/
 void daozha_open(void)
{	
    Send_zigbee[0]=0x55;
    Send_zigbee[1]=0x03;
    Send_zigbee[2]=0x01;
    Send_zigbee[3]=0x01;
    Send_zigbee[4]=0x00;
    Send_zigbee[5]=0x00;
    Send_zigbee[6]=(Send_zigbee[2]+Send_zigbee[3]+Send_zigbee[4]+Send_zigbee[5])%256;
    Send_zigbee[7]=0xbb;
    delay_ms(1);   
		Send_ZigbeeData_To_Fifo(Send_zigbee,8);
    delay_ms(50);
    Send_ZigbeeData_To_Fifo(Send_zigbee,8);
		delay_ms(50);
		Send_ZigbeeData_To_Fifo(Send_zigbee,8);
		delay_ms(50);
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
** 功能：     关闭道闸 
** 参数：	    无
** 返回值：   无
** 说明：     主车通过zigbee控制道闸关闭
****************************************************************/
 void daozha_close(void)
{	
    Send_zigbee[0]=0x55;
    Send_zigbee[1]=0x03;
    Send_zigbee[2]=0x01;
    Send_zigbee[3]=0x02;
    Send_zigbee[4]=0x00;
    Send_zigbee[5]=0x00;
    Send_zigbee[6]=(Send_zigbee[2]+Send_zigbee[3]+Send_zigbee[4]+Send_zigbee[5])%256;
    Send_zigbee[7]=0xbb;
    delay_ms(1);   
		Send_ZigbeeData_To_Fifo(Send_zigbee,8);
    delay_ms(100);
    delay_ms(250);	
}

/***************************************************************
** 功能：     道闸状态回传 
** 参数：	    无
** 返回值：   无
** 说明：     主车通过zigbee请求道闸返回当前状态
****************************************************************/
 void daozha_ack(void)
{
    Send_zigbee[0]=0x55;
    Send_zigbee[1]=0x03;
    Send_zigbee[2]=0x20;
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
** 功能：     向道闸标志物发送车牌 
** 参数：	    车牌6位的ASCll值
** 返回值：   无
** 说明：     主车通过zigbee向道闸发送车牌信息
****************************************************************/
void daozha_send_chepai(u8 one,u8 two,u8 three,u8 four,u8 five,u8 six)
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
}

/***************************************************************
** 功能：     主车返回给从车道闸已打开协议
** 参数：	  	无
** 返回值：   无
** 说明：     无
****************************************************************/
void daoza_open_ack(void)//0x55,0x02,0x99,0x02,0x00,0x00,0x9B,0xbb
{   
	Send_zigbee[0]=0x55;
	Send_zigbee[1]=0x02;
	Send_zigbee[2]=0x99;
	Send_zigbee[3]=0x02;
	Send_zigbee[4]=0x00;
	Send_zigbee[5]=0x00;
	Send_zigbee[6]=(Send_zigbee[2]+Send_zigbee[3]+Send_zigbee[4]+Send_zigbee[5])%256;
	Send_zigbee[7]=0xbb;
	Send_ZigbeeData_To_Fifo(Send_zigbee,8);
	delay_ms(10);
	delay_ms(250);
	Send_zigbee[0]=0;//清除数组,因为有时候只发了几位
  Send_zigbee[1]=0;
  Send_zigbee[2]=0;
  Send_zigbee[3]=0;
  Send_zigbee[4]=0;
  Send_zigbee[5]=0;
  Send_zigbee[6]=0;
  Send_zigbee[7]=0;
}
