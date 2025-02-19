#include "trafficlight.h"
#include "canP_HostCom.h"
#include "roadway_check.h"
#include "data_base.h"
#include "can_user.h"
#include "delay.h"
#include "tba.h"

uint8_t Ask_Count = 0;

/***************************************************************
** ���ܣ�   ʶ��ͨ��
** ������	  �޲���
** ����ֵ�� �� 
****************************************************************/
void Recognition_Traffic_Light(void)
{
		Ask_Traffic_Light();
		delay_ms(250);
		Ask_Traffic_Light();
		
		Traffic_Light_Ask_A72();
		
//		while (Stop_Flag != 0x0E)//�ȴ�A72Ӧ��
//		{
//			if (Ask_Count % 5 == 0)
//			{
//				if (Ask_Count == 10) //������λ�û���յ�Ӧ���ź����˳�����A72ʶ��
//				{
//					BEEP = 0;					 //�����һ����ɫֵ�����������
//					delay_ms(500);
//					BEEP = 1;
//					Traffic_Flag = 0x01;//��һ����ɫֵ
//					break;
//				}					
//				Traffic_Light_Ask_A72();
//			}
//			
//			Can_WifiRx_Check();
//			delay_ms(500);
//			delay_ms(200);
//			Ask_Count ++;
//		}
//		Stop_Flag = 0;
//		Ask_Count = 0;

		while (Traffic_Flag == 0)//�ȴ�A72�ش����
		{
			if (Ask_Count % 5 == 0)
			{
				if (Ask_Count == 10) //������λ�û���յ�Ӧ���ź����˳�����A72ʶ��
				{
					delay_ms(500);
					Traffic_Flag = 0x02;//��һ����ɫֵ
					break;
				}					
				Traffic_Light_Ask_A72();
			}
			
			Ask_Count ++;		
			CanP_Host_Main();
			delay_ms(500);
			delay_ms(200);
			//Can_WifiRx_Check();
		}
		
		delay_ms(250);
		
		Traffic_Light_result(Traffic_Flag);		
		delay_ms(250);
		Traffic_Light_result(Traffic_Flag);		
		
		Traffic_Flag = 0;
}

/***************************************************************
** ���ܣ�   ����ͨ�ƽ���ʶ��ģʽ
** ������	  �޲���
** ����ֵ�� �� 
****************************************************************/
void Ask_Traffic_Light(void)
{
    Send_zigbee[0]=0x55;
    Send_zigbee[1]=0x0E;
    Send_zigbee[2]=0x01;
    Send_zigbee[3]=0x00;
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
** ���ܣ�   ����ͨ��ȷ�Ͻ��
** ������	  �޲���
** ����ֵ�� �� 
****************************************************************/
void Traffic_Light_result(uint8_t result)
{
    Send_zigbee[0]=0x55;
    Send_zigbee[1]=0x0E;
    Send_zigbee[2]=0x02;
    Send_zigbee[3]=result;
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
** ���ܣ�   ��������A72ʶ��ͨ��
** ������		��
** ����ֵ�� �� 
****************************************************************/
void Traffic_Light_Ask_A72(void)
{
		Principal_Tab[2] = 0x7E;				  //����״̬
		Principal_Tab[3] = 0x02;
						
		Principal_Tab[4] = 0;        
		Principal_Tab[5] = 0;
		
		Principal_Tab[6] = 0;
		Principal_Tab[7] = 0;
		
		Principal_Tab[8] = 0;	
		Principal_Tab[9] = 0;
		
		Principal_Tab[10] =	0;   	
		Principal_Tab[11] = 0;
		
		Send_WifiData_To_Fifo(Principal_Tab, 12);
}

/***************************************************************
** ���ܣ�   ��������A72ʶ���ά��
** ������		��
** ����ֵ�� �� 
****************************************************************/
void Erweima_Ask_A72(void)
{
		Principal_Tab[2] = 0x7E;				  //����״̬
		Principal_Tab[3] = 0x01;
						
		Principal_Tab[4] = 0;        
		Principal_Tab[5] = 0;
		
		Principal_Tab[6] = 0;
		Principal_Tab[7] = 0;
		
		Principal_Tab[8] = 0;	
		Principal_Tab[9] = 0;
		
		Principal_Tab[10] =	0;   	
		Principal_Tab[11] = 0;
		
		Send_WifiData_To_Fifo(Principal_Tab, 12);
}

/***************************************************************
** ���ܣ�   ʶ���ά��
** ������	  �޲���
** ����ֵ�� �� 
****************************************************************/
void Recognition_Erweima(void)
{
		Erweima_Ask_A72();
		while (Traffic_Flag != 0x75)//�ȴ�A72�ش����
		{
			if (Ask_Count % 5 == 0)
			{
				if (Ask_Count == 10) //������λ�û���յ�Ӧ���ź����˳�����A72ʶ��
				{
					break;
				}					
				Erweima_Ask_A72();
			}
			
			Ask_Count ++;		
			CanP_Host_Main();
			delay_ms(500);
			delay_ms(200);
			//Can_WifiRx_Check();
		}
		
		while (Traffic_Flag != 0x76)//�ȴ�A72�ش����
		{
			if (Ask_Count % 5 == 0)
			{
				if (Ask_Count == 10) //������λ�û���յ�Ӧ���ź����˳�����A72ʶ��
				{
					break;
				}					
				Erweima_Ask_A72();
			}
			
			Ask_Count ++;		
			CanP_Host_Main();
			delay_ms(500);
			delay_ms(200);
			//Can_WifiRx_Check();
		}
		
		delay_ms(250);
		
		Traffic_Flag = 0;
}
