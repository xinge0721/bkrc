#include <stdio.h>
#include "stm32f4xx.h"
#include "TFT.h"
#include "canP_HostCom.h"
#include "delay.h"


/**********************************************************************
 * 函 数 名 ：  TFT显示器显示车牌
 * 参    数 ：  *Licence -> 车牌数据（ASICC）
 * 返 回 值 ：  无
 * 简    例 ：  TFT_send_chepai('A',"A123B4");
**********************************************************************/
void TFT_send_chepai (char Device,char *Licence)
{
    Send_zigbee[0] = 0x55;
		if(Device == 'A')
		{
			Send_zigbee[1] = 0x0B;
		}
		else if(Device == 'B')
		{
			Send_zigbee[1] = 0x08;
		}
		else if(Device == 'C')
		{
			Send_zigbee[1] = 0x12;
		}
    Send_zigbee[2] = 0x20;
    Send_zigbee[3] = *(Licence + 0);     // 车牌数据【1】
    Send_zigbee[4] = *(Licence + 1);     // 车牌数据【2】
    Send_zigbee[5] = *(Licence + 2);     // 车牌数据【3】
    Send_zigbee[6] = (Send_zigbee[2] + Send_zigbee[3] + Send_zigbee[4] + Send_zigbee[5]) % 256;
    Send_zigbee[7] = 0xBB;
    Send_ZigbeeData_To_Fifo(Send_zigbee, 8);     // 发送Zigbee数据
    delay_ms(500);
    Send_zigbee[2] = 0x21;
    Send_zigbee[3] = *(Licence + 3);     // 车牌数据【4】
    Send_zigbee[4] = *(Licence + 4);     // 车牌数据【5】
    Send_zigbee[5] = *(Licence + 5);     // 车牌数据【6】
    Send_zigbee[6] = (Send_zigbee[2] + Send_zigbee[3] + Send_zigbee[4] + Send_zigbee[5]) % 256;
    Send_zigbee[7] = 0xBB;
    Send_ZigbeeData_To_Fifo(Send_zigbee, 8);     // 发送Zigbee数据
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
** 功能：     TFT标志物计时功能开始   
** 参数：	    Device是TFT的型号
** 返回值：   无
** 说明：     主车通过zigbee控制TFT标志物的计时功能开启
参数代表ABC型TFT标志物
****************************************************************/
void TFT_jishi_open(char Device)
{
    Send_zigbee[0]=0x55;
    if(Device == 'A')
		{
			Send_zigbee[1] = 0x0B;
		}
		else if(Device == 'B')
		{
			Send_zigbee[1] = 0x08;
		}
		else if(Device == 'C')
		{
			Send_zigbee[1] = 0x12;
		}
    Send_zigbee[2]=0x30;
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
** 功能：     TFT标志物计时功能结束   
** 参数：	    Device是TFT的型号
** 返回值：   无
** 说明：     主车通过zigbee控制TFT标志物的计时功能结束
参数代表ABC型TFT标志物
****************************************************************/
void TFT_jishi_close(char Device)
{
    Send_zigbee[0]=0x55;
    if(Device == 'A')
		{
			Send_zigbee[1] = 0x0B;
		}
		else if(Device == 'B')
		{
			Send_zigbee[1] = 0x08;
		}
		else if(Device == 'C')
		{
			Send_zigbee[1] = 0x12;
		}
    Send_zigbee[2]=0x30;
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
** 功能：     TFT标志物计时功能清零   
** 参数：	    Device是TFT的型号
** 返回值：   无
** 说明：     主车通过zigbee控制TFT标志物的计时功能清零
参数代表ABC型TFT标志物
****************************************************************/
void TFT_jishi_clear(char Device)
{
    Send_zigbee[0]=0x55;
    if(Device == 'A')
		{
			Send_zigbee[1] = 0x0B;
		}
		else if(Device == 'B')
		{
			Send_zigbee[1] = 0x08;
		}
		else if(Device == 'C')
		{
			Send_zigbee[1] = 0x12;
		}
    Send_zigbee[2]=0x30;
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
** 功能：     TFT标志物图片显示   
** 参数：	   Device是TFT的型号， Num 显示第几张图片
** 返回值：   	无
** 说明：     主车通过zigbee控制TFT标志物的图片显示
案例：TFT_Picture_Show('A',1);
****************************************************************/
void TFT_Picture_Show(char Device,uint8_t Num)
{
    Send_zigbee[0] = 0x55;
    if(Device == 'A')
		{
			Send_zigbee[1] = 0x0B;
		}
		else if(Device == 'B')
		{
			Send_zigbee[1] = 0x08;
		}
		else if(Device == 'C')
		{
			Send_zigbee[1] = 0x12;
		}
    Send_zigbee[2] = 0x10;
    Send_zigbee[3] = 0x00;
    Send_zigbee[4] = Num;
    Send_zigbee[5] = 0x00;
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
** 功能：     TFT标志物图片向上翻页   
** 参数：	    Device是TFT的型号
** 返回值：   	无
** 说明：     主车通过zigbee控制TFT标志物图片向上翻页
****************************************************************/
void TFT_Picture_Up(char Device)
{
    Send_zigbee[0] = 0x55;
    if(Device == 'A')
		{
			Send_zigbee[1] = 0x0B;
		}
		else if(Device == 'B')
		{
			Send_zigbee[1] = 0x08;
		}
		else if(Device == 'C')
		{
			Send_zigbee[1] = 0x12;
		}
    Send_zigbee[2] = 0x10;
    Send_zigbee[3] = 0x01;
    Send_zigbee[4] = 0x00;
    Send_zigbee[5] = 0x00;
    Send_zigbee[6] = (Send_zigbee[2]+Send_zigbee[3]+Send_zigbee[4]+Send_zigbee[5])%256;;
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
** 功能：     TFT标志物图片向下翻页   
** 参数：	    Device是TFT的型号
** 返回值：   	无
** 说明：     主车通过zigbee控制TFT标志物图片向下翻页
****************************************************************/
void TFT_Picture_Down(char Device)
{
    Send_zigbee[0] = 0x55;
    if(Device == 'A')
		{
			Send_zigbee[1] = 0x0B;
		}
		else if(Device == 'B')
		{
			Send_zigbee[1] = 0x08;
		}
		else if(Device == 'C')
		{
			Send_zigbee[1] = 0x12;
		}
    Send_zigbee[2] = 0x10;
    Send_zigbee[3] = 0x02;
    Send_zigbee[4] = 0x00;
    Send_zigbee[5] = 0x00;
    Send_zigbee[6] = (Send_zigbee[2]+Send_zigbee[3]+Send_zigbee[4]+Send_zigbee[5])%256;;
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
** 功能：     TFT标志物图片自动向下翻页   
** 参数：	    Device是TFT的型号
** 返回值：   	无
** 说明：     主车通过zigbee控制TFT标志物图片自动向下翻页
****************************************************************/
void TFT_Picture_AutomaticDown(char Device)
{
    Send_zigbee[0] = 0x55;
    if(Device == 'A')
		{
			Send_zigbee[1] = 0x0B;
		}
		else if(Device == 'B')
		{
			Send_zigbee[1] = 0x08;
		}
		else if(Device == 'C')
		{
			Send_zigbee[1] = 0x12;
		}
    Send_zigbee[2] = 0x10;
    Send_zigbee[3] = 0x03;
    Send_zigbee[4] = 0x00;
    Send_zigbee[5] = 0x00;
    Send_zigbee[6] = (Send_zigbee[2]+Send_zigbee[3]+Send_zigbee[4]+Send_zigbee[5])%256;;
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
** 功能：     TFT标志物距离显示模式   
** 参数：	    Device是TFT的型号,distance 用十进制表示的距离
** 返回值：   	无
** 说明：     主车通过zigbee控制TFT标志物图片距离显示模式
案例;TFT_Picture_DistanceShow('A',dis);//这里的dis是超声波的dis
****************************************************************/
void TFT_Picture_DistanceShow(char Device,uint8_t dis)
{
    Send_zigbee[0] = 0x55;
    if(Device == 'A')
		{
			Send_zigbee[1] = 0x0B;
		}
		else if(Device == 'B')
		{
			Send_zigbee[1] = 0x08;
		}
		else if(Device == 'C')
		{
			Send_zigbee[1] = 0x12;
		}
    Send_zigbee[2] = 0x50;
    Send_zigbee[3] = 0x00;
    Send_zigbee[4] = dis/100%10;
    Send_zigbee[5] = ((dis/10%10)*16+dis%10);
    Send_zigbee[6] = (Send_zigbee[2]+Send_zigbee[3]+Send_zigbee[4]+Send_zigbee[5])%256;;
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

