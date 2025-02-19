#include <stdio.h>
#include "stm32f4xx.h"
#include "garage.h"
#include "canP_HostCom.h"
#include "delay.h"
#include "can_user.h"
#include "tba.h"
#include "Timer.h"
#include "mytest.h"
#include "roadway_check.h"
#include "LED.h"

/*********新增变量******/
uint8_t Ack_Traffic_Light_Count;
uint8_t Back_Count = 0;
uint8_t CK_flag = 0;
uint32_t canu_zibe_ck;
/**********************/

/***************************************************************
** 功能：   立体车库到达第一层
** 参数：	  Device是车库A或者B
** 返回值： 无
litiGarageF1('A');
****************************************************************/
void litiGarageF1(char Device)
{
    Send_zigbee[0]=0x55;
    if(Device == 'A')
		{
			Send_zigbee[1] = 0x0D;
		}
		else if(Device == 'B')
		{
			Send_zigbee[1] = 0x05;
		}
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
** 功能：   立体车库到达第二层
** 参数：	  Device是车库A或者B
** 返回值： 无 
litiGarageF2('A');
****************************************************************/
void litiGarageF2(char Device)
{
	  Send_zigbee[0]=0x55;
    if(Device == 'A')
		{
			Send_zigbee[1] = 0x0D;
		}
		else if(Device == 'B')
		{
			Send_zigbee[1] = 0x05;
		}
    Send_zigbee[2]=0x01;
    Send_zigbee[3]=0x02;
    Send_zigbee[4]=0x00;
    Send_zigbee[5]=0x00;
    Send_zigbee[6]=(Send_zigbee[2]+Send_zigbee[3]+Send_zigbee[4]+Send_zigbee[5])%256;
    Send_zigbee[7]=0xbb;
    delay_ms(1);   
	  Send_ZigbeeData_To_Fifo(Send_zigbee,8);
    delay_ms(150);
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
** 功能：   立体车库到达第三层
** 参数：	  Device是车库A或者B
** 返回值： 无 
litiGarageF3('A');
****************************************************************/
void litiGarageF3(char Device)
{
		Send_zigbee[0]=0x55;
    if(Device == 'A')
		{
			Send_zigbee[1] = 0x0D;
		}
		else if(Device == 'B')
		{
			Send_zigbee[1] = 0x05;
		}
    Send_zigbee[2]=0x01;
    Send_zigbee[3]=0x03;
    Send_zigbee[4]=0x00;
    Send_zigbee[5]=0x00;
    Send_zigbee[6]=(Send_zigbee[2]+Send_zigbee[3]+Send_zigbee[4]+Send_zigbee[5])%256;;
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
** 功能：   立体车库到达第四层
** 参数：	  Device是车库A或者B
** 返回值： 无 
litiGarageF4('A');
****************************************************************/
void litiGarageF4(char Device)
{
		Send_zigbee[0]=0x55;
    if(Device == 'A')
		{
			Send_zigbee[1] = 0x0D;
		}
		else if(Device == 'B')
		{
			Send_zigbee[1] = 0x05;
		}
    Send_zigbee[2]=0x01;
    Send_zigbee[3]=0x04;
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
** 功能：   请求返回车库位于第几层
** 参数：	  Device是车库A或者B
** 返回值： 无 
功能1：车库为出发点，等待车库下降在出发
gradetime = gt_get()+100;
				while (1)		//判断车有没有到第一层
				{
					if (Stop_Flag == 0x09)
					{
						Forward(55,85,1200);  //十字路口前进一点，出车库
						Stop_Flag = 0;
						break;
					}
					else
					{
						if(gt_get_sub(gradetime) == 0)//100ms检测一次
						{
							gradetime = gt_get()+100;
							litiGarageF1('A');//让他下降到一楼
							Request_Location('A');
							gradetuic++;
							if(gradetuic ==210)//21s后无反应强制出库，车库肯定会下降，但是有时候车库没有反应，车不会出库
							{
								gradetuic =0;
								Forward(55,85,1200);  //十字路口前进一点，出车库
								break;
							}
						}
						Can_ZigBeeRx_Check();
					}
				}	
功能2：等待车库下降,然后倒退回车库
gradetime = gt_get()+100;
				while (1)		//判断车有没有到第一层
				{
					if (Stop_Flag == 0x09)
					{
						Back_1(45,100,1200);
						Stop_Flag = 0;
						break;
					}
					else
					{
						if(gt_get_sub(gradetime) == 0)//50ms检测一次
						{
							gradetime = gt_get()+100;
							litiGarageF1('A');//让他下降到一楼
							Request_Location('A');
							gradetuic++;
							if(gradetuic ==210)//21s后无反应强制回库，车库肯定会下降，但是有时候车库没有反应，车不会回库
							{
								gradetuic =0;
								Back_1(45,100,1200);  //十字路口前进一点，出车库
								break;
							}
						}
						Can_ZigBeeRx_Check();
					}
				}

功能3：题目里面车库上升是最后一题，车库上到指定层数才结束LED计时
gradetime = gt_get()+100;
				while (1)		//判断从车库有没有到那一层，到了那一层才停止下面的LED计时
				{
					if (Stop_Flag == 0x0B || Stop_Flag == 0x09 || Stop_Flag == 0x0A || Stop_Flag == 0x0C)//无论上哪层都结束循环
					{
						Stop_Flag = 0;
						break;
					}
					else
					{
						if(gt_get_sub(gradetime) == 0)//50ms检测一次
						{
							gradetime = gt_get()+100;
							Request_Location('A');
						}
						Can_ZigBeeRx_Check();
					}
				}

功能4：要求某个车库下降到一楼，比如从车去的车库
while (1)		//判断车库有没有到第一层,因为是用主车控制车库，原本A车库已经下降就不需要这里，有可能让从车去B车库
					{
						if (Stop_Flag == 0x09)//一楼就结束循环
						{
							Stop_Flag = 0;
							break;
						}
					
						else
						{
							if(gt_get_sub(gradetime) == 0)//50ms检测一次
							{
								gradetime = gt_get()+100;
								litiGarageF1('A');//让他下降到一楼
								Request_Location('A');
							}
							Can_ZigBeeRx_Check();
						}
					}
****************************************************************/
//查询车库位于几层
void Request_Location(char Device)//0x05
{
		Send_zigbee[0]=0x55;
    if(Device == 'A')
		{
			Send_zigbee[1] = 0x0D;
		}
		else if(Device == 'B')
		{
			Send_zigbee[1] = 0x05;
		}
    Send_zigbee[2]=0x02;
    Send_zigbee[3]=0x01;
    Send_zigbee[4]=0x00;
    Send_zigbee[5]=0x00;
    Send_zigbee[6]=(Send_zigbee[2]+Send_zigbee[3]+Send_zigbee[4]+Send_zigbee[5])%256;
    Send_zigbee[7]=0xbb;  
		Send_ZigbeeData_To_Fifo(Send_zigbee,8);
    delay_ms(10);
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
** 功能：   请求返回前后侧红外状态
** 参数：	  Device是车库A或者B
** 返回值： 无 
****************************************************************/
void Request_Infrared(char Device)
{
		Send_zigbee[0]=0x55;
    if(Device == 'A')
		{
			Send_zigbee[1] = 0x0D;
		}
		else if(Device == 'B')
		{
			Send_zigbee[1] = 0x05;
		}
    Send_zigbee[2]=0x02;
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

