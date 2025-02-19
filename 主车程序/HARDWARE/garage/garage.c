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

/*********��������******/
uint8_t Ack_Traffic_Light_Count;
uint8_t Back_Count = 0;
uint8_t CK_flag = 0;
uint32_t canu_zibe_ck;
/**********************/

/***************************************************************
** ���ܣ�   ���峵�⵽���һ��
** ������	  Device�ǳ���A����B
** ����ֵ�� ��
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
** ���ܣ�   ���峵�⵽��ڶ���
** ������	  Device�ǳ���A����B
** ����ֵ�� �� 
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
** ���ܣ�   ���峵�⵽�������
** ������	  Device�ǳ���A����B
** ����ֵ�� �� 
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
** ���ܣ�   ���峵�⵽����Ĳ�
** ������	  Device�ǳ���A����B
** ����ֵ�� �� 
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
** ���ܣ�   ���󷵻س���λ�ڵڼ���
** ������	  Device�ǳ���A����B
** ����ֵ�� �� 
����1������Ϊ�����㣬�ȴ������½��ڳ���
gradetime = gt_get()+100;
				while (1)		//�жϳ���û�е���һ��
				{
					if (Stop_Flag == 0x09)
					{
						Forward(55,85,1200);  //ʮ��·��ǰ��һ�㣬������
						Stop_Flag = 0;
						break;
					}
					else
					{
						if(gt_get_sub(gradetime) == 0)//100ms���һ��
						{
							gradetime = gt_get()+100;
							litiGarageF1('A');//�����½���һ¥
							Request_Location('A');
							gradetuic++;
							if(gradetuic ==210)//21s���޷�Ӧǿ�Ƴ��⣬����϶����½���������ʱ�򳵿�û�з�Ӧ�����������
							{
								gradetuic =0;
								Forward(55,85,1200);  //ʮ��·��ǰ��һ�㣬������
								break;
							}
						}
						Can_ZigBeeRx_Check();
					}
				}	
����2���ȴ������½�,Ȼ���˻س���
gradetime = gt_get()+100;
				while (1)		//�жϳ���û�е���һ��
				{
					if (Stop_Flag == 0x09)
					{
						Back_1(45,100,1200);
						Stop_Flag = 0;
						break;
					}
					else
					{
						if(gt_get_sub(gradetime) == 0)//50ms���һ��
						{
							gradetime = gt_get()+100;
							litiGarageF1('A');//�����½���һ¥
							Request_Location('A');
							gradetuic++;
							if(gradetuic ==210)//21s���޷�Ӧǿ�ƻؿ⣬����϶����½���������ʱ�򳵿�û�з�Ӧ��������ؿ�
							{
								gradetuic =0;
								Back_1(45,100,1200);  //ʮ��·��ǰ��һ�㣬������
								break;
							}
						}
						Can_ZigBeeRx_Check();
					}
				}

����3����Ŀ���泵�����������һ�⣬�����ϵ�ָ�������Ž���LED��ʱ
gradetime = gt_get()+100;
				while (1)		//�жϴӳ�����û�е���һ�㣬������һ���ֹͣ�����LED��ʱ
				{
					if (Stop_Flag == 0x0B || Stop_Flag == 0x09 || Stop_Flag == 0x0A || Stop_Flag == 0x0C)//�������Ĳ㶼����ѭ��
					{
						Stop_Flag = 0;
						break;
					}
					else
					{
						if(gt_get_sub(gradetime) == 0)//50ms���һ��
						{
							gradetime = gt_get()+100;
							Request_Location('A');
						}
						Can_ZigBeeRx_Check();
					}
				}

����4��Ҫ��ĳ�������½���һ¥������ӳ�ȥ�ĳ���
while (1)		//�жϳ�����û�е���һ��,��Ϊ�����������Ƴ��⣬ԭ��A�����Ѿ��½��Ͳ���Ҫ����п����ôӳ�ȥB����
					{
						if (Stop_Flag == 0x09)//һ¥�ͽ���ѭ��
						{
							Stop_Flag = 0;
							break;
						}
					
						else
						{
							if(gt_get_sub(gradetime) == 0)//50ms���һ��
							{
								gradetime = gt_get()+100;
								litiGarageF1('A');//�����½���һ¥
								Request_Location('A');
							}
							Can_ZigBeeRx_Check();
						}
					}
****************************************************************/
//��ѯ����λ�ڼ���
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
** ���ܣ�   ���󷵻�ǰ������״̬
** ������	  Device�ǳ���A����B
** ����ֵ�� �� 
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
		Send_zigbee[0]=0;//�������,��Ϊ��ʱ��ֻ���˼�λ
		Send_zigbee[1]=0;
		Send_zigbee[2]=0;
		Send_zigbee[3]=0;
		Send_zigbee[4]=0;
		Send_zigbee[5]=0;
		Send_zigbee[6]=0;
		Send_zigbee[7]=0;
}

