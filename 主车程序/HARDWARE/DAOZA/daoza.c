#include "daoza.h"
#include "delay.h"
#include "canP_HostCom.h"

/***************************************************************
** ���ܣ�     �򿪵�բ 
** ������	    ��
** ����ֵ��   ��
** ˵����     ����ͨ��zigbee���Ƶ�բ����
				daozatime = gt_get()+50;
				while(1)
				{
					
					if(Stop_Flag == 0x05)//����բ�Ƿ��
					{
						Tracking_Forward(74,70);//70�ٶ�ѭ��ǰ��
						Forward(55,100,250);  //ʮ��·��ǰ��һ��
						Stop_Flag = 0;
						break;
					}
					else
					{
						if(gt_get_sub(daozatime) == 0)//50ms���һ��
						{
							daozatime = gt_get()+50;
							daozha_open();//�򿪵�բ
							delay_ms(50);
							daozha_ack();
							delay_ms(50);
							daozatuichu++;//����ʱ�䣬�����բ��ʱ��û��Ӧ����ȥ
							if(daozatuichu ==80)
							{
								daozatuichu =0;
								Tracking_Forward(74,70);//70�ٶ�ѭ��ǰ��
								Forward(55,100,250);  //ʮ��·��ǰ��һ��
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
** ���ܣ�     �رյ�բ 
** ������	    ��
** ����ֵ��   ��
** ˵����     ����ͨ��zigbee���Ƶ�բ�ر�
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
** ���ܣ�     ��բ״̬�ش� 
** ������	    ��
** ����ֵ��   ��
** ˵����     ����ͨ��zigbee�����բ���ص�ǰ״̬
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
** ���ܣ�     ���բ��־�﷢�ͳ��� 
** ������	    ����6λ��ASCllֵ
** ����ֵ��   ��
** ˵����     ����ͨ��zigbee���բ���ͳ�����Ϣ
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
** ���ܣ�     �������ظ��ӳ���բ�Ѵ�Э��
** ������	  	��
** ����ֵ��   ��
** ˵����     ��
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
	Send_zigbee[0]=0;//�������,��Ϊ��ʱ��ֻ���˼�λ
  Send_zigbee[1]=0;
  Send_zigbee[2]=0;
  Send_zigbee[3]=0;
  Send_zigbee[4]=0;
  Send_zigbee[5]=0;
  Send_zigbee[6]=0;
  Send_zigbee[7]=0;
}
