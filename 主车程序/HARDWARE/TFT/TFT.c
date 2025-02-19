#include <stdio.h>
#include "stm32f4xx.h"
#include "TFT.h"
#include "canP_HostCom.h"
#include "delay.h"


/**********************************************************************
 * �� �� �� ��  TFT��ʾ����ʾ����
 * ��    �� ��  *Licence -> �������ݣ�ASICC��
 * �� �� ֵ ��  ��
 * ��    �� ��  TFT_send_chepai('A',"A123B4");
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
    Send_zigbee[3] = *(Licence + 0);     // �������ݡ�1��
    Send_zigbee[4] = *(Licence + 1);     // �������ݡ�2��
    Send_zigbee[5] = *(Licence + 2);     // �������ݡ�3��
    Send_zigbee[6] = (Send_zigbee[2] + Send_zigbee[3] + Send_zigbee[4] + Send_zigbee[5]) % 256;
    Send_zigbee[7] = 0xBB;
    Send_ZigbeeData_To_Fifo(Send_zigbee, 8);     // ����Zigbee����
    delay_ms(500);
    Send_zigbee[2] = 0x21;
    Send_zigbee[3] = *(Licence + 3);     // �������ݡ�4��
    Send_zigbee[4] = *(Licence + 4);     // �������ݡ�5��
    Send_zigbee[5] = *(Licence + 5);     // �������ݡ�6��
    Send_zigbee[6] = (Send_zigbee[2] + Send_zigbee[3] + Send_zigbee[4] + Send_zigbee[5]) % 256;
    Send_zigbee[7] = 0xBB;
    Send_ZigbeeData_To_Fifo(Send_zigbee, 8);     // ����Zigbee����
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
** ���ܣ�     TFT��־���ʱ���ܿ�ʼ   
** ������	    Device��TFT���ͺ�
** ����ֵ��   ��
** ˵����     ����ͨ��zigbee����TFT��־��ļ�ʱ���ܿ���
��������ABC��TFT��־��
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
** ���ܣ�     TFT��־���ʱ���ܽ���   
** ������	    Device��TFT���ͺ�
** ����ֵ��   ��
** ˵����     ����ͨ��zigbee����TFT��־��ļ�ʱ���ܽ���
��������ABC��TFT��־��
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
** ���ܣ�     TFT��־���ʱ��������   
** ������	    Device��TFT���ͺ�
** ����ֵ��   ��
** ˵����     ����ͨ��zigbee����TFT��־��ļ�ʱ��������
��������ABC��TFT��־��
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
** ���ܣ�     TFT��־��ͼƬ��ʾ   
** ������	   Device��TFT���ͺţ� Num ��ʾ�ڼ���ͼƬ
** ����ֵ��   	��
** ˵����     ����ͨ��zigbee����TFT��־���ͼƬ��ʾ
������TFT_Picture_Show('A',1);
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
** ���ܣ�     TFT��־��ͼƬ���Ϸ�ҳ   
** ������	    Device��TFT���ͺ�
** ����ֵ��   	��
** ˵����     ����ͨ��zigbee����TFT��־��ͼƬ���Ϸ�ҳ
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
** ���ܣ�     TFT��־��ͼƬ���·�ҳ   
** ������	    Device��TFT���ͺ�
** ����ֵ��   	��
** ˵����     ����ͨ��zigbee����TFT��־��ͼƬ���·�ҳ
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
** ���ܣ�     TFT��־��ͼƬ�Զ����·�ҳ   
** ������	    Device��TFT���ͺ�
** ����ֵ��   	��
** ˵����     ����ͨ��zigbee����TFT��־��ͼƬ�Զ����·�ҳ
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
** ���ܣ�     TFT��־�������ʾģʽ   
** ������	    Device��TFT���ͺ�,distance ��ʮ���Ʊ�ʾ�ľ���
** ����ֵ��   	��
** ˵����     ����ͨ��zigbee����TFT��־��ͼƬ������ʾģʽ
����;TFT_Picture_DistanceShow('A',dis);//�����dis�ǳ�������dis
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
		Send_zigbee[0]=0;//�������,��Ϊ��ʱ��ֻ���˼�λ
		Send_zigbee[1]=0;
		Send_zigbee[2]=0;
		Send_zigbee[3]=0;
		Send_zigbee[4]=0;
		Send_zigbee[5]=0;
		Send_zigbee[6]=0;
		Send_zigbee[7]=0;
}

