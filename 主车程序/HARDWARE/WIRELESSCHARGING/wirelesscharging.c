#include "wirelesscharging.h"
#include "canP_HostCom.h"
#include "delay.h"

/***************************************************************
** ���ܣ�    ���߳���־�￪�����̶�ָ��
** ������	   ��
** ����ֵ��  ��
** ˵����    ����ͨ��zigbee�������߳���־�￪��
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
** ���ܣ�    ���߳���־��ر�
** ������	 	 ��
** ����ֵ��  ��
** ˵����    ����ͨ��zigbee�������߳���־��ر�
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
** ���ܣ�    ���߳���־�￪���뿪����Ĭ��ָ��
** ������	   ��
** ����ֵ��  ��
** ˵����    ����ͨ��zigbee�������߳���־�￪��
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
** ���ܣ�    ���߳���־�￪���뿪������������ָ�����ݿ���
** ������	   ��
** ����ֵ��  ��
** ˵����    ����ͨ��zigbee�������߳���־�￪��
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
		Send_zigbee[0]=0;//�������,��Ϊ��ʱ��ֻ���˼�λ
		Send_zigbee[1]=0;
		Send_zigbee[2]=0;
		Send_zigbee[3]=0;
		Send_zigbee[4]=0;
		Send_zigbee[5]=0;
		Send_zigbee[6]=0;
		Send_zigbee[7]=0;
}
