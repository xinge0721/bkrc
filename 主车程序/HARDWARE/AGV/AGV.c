#include <stdio.h>
#include "stm32f4xx.h"
#include "AGV.h"
#include "canP_HostCom.h"
#include "delay.h"
#include "roadway_check.h"
#include "can_user.h"
#include "tba.h"
#include "data_base.h"

uint8_t Check_Agv_Ask = 0;
/***************************************************************
** ���ܣ�     �ƶ�������ǰ��
** ������	  	speed ���ͣ�С���ٶȣ�mapan��u16�ͣ�����ֵ  
** ����ֵ��   ��
** ˵����     ����ͨ��zigbee�������䳵ǰ��
yunshuche_go(90,310);
				while(1)
				{
					Can_ZigBeeRx_Check();
					if(uustop ==2)   //ǰ�����
					{
						uustop=0;
						break;
					}
				}
****************************************************************/
void yunshuche_go(uint8_t speed,uint16_t mapan)
{
    Send_zigbee[0]=0x55;
    Send_zigbee[1]=0x02;
    Send_zigbee[2]=0x02;
    Send_zigbee[3]=speed;
    Send_zigbee[4]=mapan%256; //�Ͱ�λ
    Send_zigbee[5]=mapan/256; //�߰�λ
    Send_zigbee[6]=(Send_zigbee[2]+Send_zigbee[3]+Send_zigbee[4]+Send_zigbee[5])%256;
    Send_zigbee[7]=0xbb;
    Send_ZigbeeData_To_Fifo(Send_zigbee,8);
    delay_ms(5);
}


/***************************************************************
** ���ܣ�     �ƶ������˺���
** ������	  speed ���ͣ�С���ٶȣ�mapan��u16�ͣ�����ֵ
** ����ֵ��   ��
** ˵����     ����ͨ��zigbee�������䳵����
****************************************************************/
void yunshuche_back(uint8_t speed,uint16_t mapan)
{	
    Send_zigbee[0]=0x55;
    Send_zigbee[1]=0x02;
    Send_zigbee[2]=0x03;
    Send_zigbee[3]=speed;
    Send_zigbee[4]=mapan%256;
    Send_zigbee[5]=mapan/256;
    Send_zigbee[6]=(Send_zigbee[2]+Send_zigbee[3]+Send_zigbee[4]+Send_zigbee[5])%256;
    Send_zigbee[7]=0xbb;
    Send_ZigbeeData_To_Fifo(Send_zigbee,8);
    delay_ms(5);   	
}

/***************************************************************
** ���ܣ�     �ƶ���������ת45
** ������	  	speed ���ͣ�С���ٶȣ�mapan��u16�ͣ�����ֵ  
** ����ֵ��   ��
** ˵����     ����ͨ��zigbee�������䳵ǰ��
yunshuche_turnright();//��ת45���
				while(1)
				{
					Can_ZigBeeRx_Check();
					if(uustop ==4)    //��ת45���
					{
						uustop=0;
						break;
					}
				}
****************************************************************/
void yunshuche_turnright(void)
{
    Send_zigbee[0]=0x55;
    Send_zigbee[1]=0x02;
    Send_zigbee[2]=0x18;
    Send_zigbee[3]=0x00;
    Send_zigbee[4]=0x00;
    Send_zigbee[5]=0x00;
    Send_zigbee[6]=(Send_zigbee[2]+Send_zigbee[3]+Send_zigbee[4]+Send_zigbee[5])%256;
    Send_zigbee[7]=0xbb;
    Send_ZigbeeData_To_Fifo(Send_zigbee,8);
    delay_ms(5);
}

//��45��
//yunshuche_turnleft();//��ת45���
//				while(1)
//				{
//					Can_ZigBeeRx_Check();
//					if(uustop ==5)    //��ת45���
//					{
//						uustop=0;
//						break;
//					}
//				}
void yunshuche_turnleft(void)
{
    Send_zigbee[0]=0x55;
    Send_zigbee[1]=0x02;
    Send_zigbee[2]=0x19;
    Send_zigbee[3]=0x00;
    Send_zigbee[4]=0x00;
    Send_zigbee[5]=0x00;
    Send_zigbee[6]=(Send_zigbee[2]+Send_zigbee[3]+Send_zigbee[4]+Send_zigbee[5])%256;
    Send_zigbee[7]=0xbb;
    Send_ZigbeeData_To_Fifo(Send_zigbee,8);
    delay_ms(5);
}

/***************************************************************
** ���ܣ�     �ƶ���������ת
** ������	 	  speed	���ͣ�С���ٶ�
** ����ֵ��   ��
** ˵����     ����ͨ��zigbee�������䳵��ת
****************************************************************/
void yunshuche_left(uint8_t speed)
{   
    Send_zigbee[0]=0x55;
    Send_zigbee[1]=0x02;
    Send_zigbee[2]=0x04;
    Send_zigbee[3]=speed;
    Send_zigbee[4]=0x00;
    Send_zigbee[5]=0x00;
    Send_zigbee[6]=(Send_zigbee[2]+Send_zigbee[3]+Send_zigbee[4]+Send_zigbee[5])%256;
    Send_zigbee[7]=0xbb;
    Send_ZigbeeData_To_Fifo(Send_zigbee,8);
    delay_ms(5);  
}


/***************************************************************
** ���ܣ�     �ƶ���������ת
** ������	  	speed	���ͣ�С���ٶ�
** ����ֵ��   ��
** ˵����     ����ͨ��zigbee�������䳵��ת
****************************************************************/
void yunshuche_right(uint8_t speed)
{	
    Send_zigbee[0]=0x55;
    Send_zigbee[1]=0x02;
    Send_zigbee[2]=0x05;
    Send_zigbee[3]=speed;
    Send_zigbee[4]=0x00;
    Send_zigbee[5]=0x00;
    Send_zigbee[6]=(Send_zigbee[2]+Send_zigbee[3]+Send_zigbee[4]+Send_zigbee[5])%256;
    Send_zigbee[7]=0xbb;
    Send_ZigbeeData_To_Fifo(Send_zigbee,8);
    delay_ms(5);   
}


/***************************************************************
** ���ܣ�     �ƶ�������ֹͣ
** ������	  	��
** ����ֵ��   ��
** ˵����     ����ͨ��zigbee�������䳵ֹͣ

****************************************************************/
void yunshuche_stop(void)
{
    Send_zigbee[0]=0x55;
    Send_zigbee[1]=0x02;
    Send_zigbee[2]=0x01;
    Send_zigbee[3]=0x00;
    Send_zigbee[4]=0x00;
    Send_zigbee[5]=0x00;
    Send_zigbee[6]=(Send_zigbee[2]+Send_zigbee[3]+Send_zigbee[4]+Send_zigbee[5])%256;
    Send_zigbee[7]=0xbb;
    Send_ZigbeeData_To_Fifo(Send_zigbee,8);
    delay_ms(5);
}


/***************************************************************
** ���ܣ�     �ƶ�������ѭ��
** ������	  	speed	���ͣ�С���ٶ�
** ����ֵ��   ��
** ˵����     ����ͨ��zigbee�������䳵ѭ��
yunshuche_xj(70);//ѭ��
				while(1)
				{
					Can_ZigBeeRx_Check();
					if(uustop ==1)    //ѭ�����
					{
						uustop=0;
						break;
					}
				}
****************************************************************/
void yunshuche_xj(uint8_t speed)
{   
    Send_zigbee[0]=0x55;
    Send_zigbee[1]=0x02;
    Send_zigbee[2]=0x06;
    Send_zigbee[3]=speed;
    Send_zigbee[4]=0x00;
    Send_zigbee[5]=0x00;
    Send_zigbee[6]=(Send_zigbee[2]+Send_zigbee[3]+Send_zigbee[4]+Send_zigbee[5])%256;
    Send_zigbee[7]=0xbb;
    Send_ZigbeeData_To_Fifo(Send_zigbee,8);
    delay_ms(5);
		
}

/***************************************************************
** ���ܣ�     ���������ӳ�����Э��
** ������	  	��
** ����ֵ��   ��
** ˵����     
****************************************************************/
void yunshuche_xieyi(void)
{
		Send_zigbee[0]=0x55;
		Send_zigbee[1]=0x02;
		Send_zigbee[2]=0x99;
		Send_zigbee[3]=0x01;
		Send_zigbee[4]=0x00;
		Send_zigbee[5]=0x00;
		Send_zigbee[6]=(Send_zigbee[2]+Send_zigbee[3]+Send_zigbee[4]+Send_zigbee[5])%256;
		Send_zigbee[7]=0xbb;
		Send_ZigbeeData_To_Fifo(Send_zigbee,8);
		delay_ms(50);
}



/***************************************************************
** ���ܣ�     �ƶ�������һ������
** ������	  	��
** ����ֵ��   ��
** ˵����     ����ͨ��zigbee���ƴӳ����� ,��һ������
�������̾��������ȷ��͵�һ�������ôӳ�����ȫ�Զ�����������ӳ�û�н��յ�ȫ����Э�飬������4s������·��ͣ�Ȼ��������50ms�ٶȼ�⣬
����ӳ����յ��˾ͻ᷵��Ӧ���źŸ�����������ӳ����ص�Ӧ���ź�����û�н��յ����ӳ���1s�������·��ͣ��������û�н��յ����ٹ�1s���������һ�Σ��ӳ�������ѭ����
��ʼִ�дӳ�ȫ�Զ���������һ�����ٵڶ���Ҳ�ɹ��ˣ������յ��˴ӳ�����Ӧ���źţ�zigbee�����Stop_Flag == 0x0D��������ѭ��������������ȫ�Զ��������
������Ҫע�⣬��������ȫ�Զ�������4s��ã��ôӳ���ЩӦ����������ɣ�4s��ûʲô�����ˣ����û�����������������ٷ�ȫ�Զ�����



//����ȫ�Զ������Ժ󣬴ӳ�������������բ
yunshuche_start();//�ôӳ��ɹ�����
			while (Stop_Flag != 0x0F)	//����ӳ����˵�բ��Stop_Flag = 0x0F����zigbee�������
			{
				if(Check_Agv_daoza % 5 == 0)//ʱ����Ը�������޸�
				{
					if (Check_Agv_daoza == 250)
					{
						Check_Agv_daoza = 0;
						break;					
					}
				}
				Check_Agv_daoza++;
				CanP_Host_Main();
				delay_ms(100);
				Can_ZigBeeRx_Check();
			}
			Stop_Flag = 0;
			
�ӳ������ɹ����ȴ��ӳ����ȫ������������ִ��ʣ�µ�����
yunshuche_start();//�ӳ���һ������
				
				while(1)//�ӳ��Ѿ����ȫ�Զ�����
				{
					if(uustop==6)
					{
						uustop = 0;
						break;
					}
					Can_ZigBeeRx_Check();
				}
****************************************************************/
void yunshuche_start(void)
{   
		//0x55,0x02,0x99,0x01,0x00,0x00,0x9A,0xbb
		yunshuche_xieyi();//��������Э���
		//�ӳ�����0x55,0x02,0x99,0x03,0x00,0x00,0x9C,0xBBӦ���źţ�Can_ZigBeeRx_Check������Stop_Flag = 0x0D;
		while (Stop_Flag != 0x0D)		//�ȴ��ӳ�����Ӧ���ź�,�ӳ���0x55,0x02,0x99,0x03,0x00,0x00,0x9C,0xBBӦ���źţ����û���յ��ӳ���Ӧ���ź��������һ�η�һ������
		{														//�˳����ƣ��Է��ղ���Ӧ���źſ���
			if(Check_Agv_Ask % 80 == 0)//ʱ����Ը�������޸ģ�Check_Agv_Ask����80ȡ��80�ŵ���0,������ÿ50ms��Check_Agv_Ask+1���ոպ�4s�Ժ�����������Ҳ����80��ÿ+80����һ�Σ�����=160���������������Σ�Ҳ����ȫ��һ����2������Э��
			{
				yunshuche_xieyi();//��һ�η�������Э��
				if (Check_Agv_Ask == 160)//������ʱ�䣬���Ը���
				{
					Check_Agv_Ask = 0;
					break;					//����break�ǽ���ѭ�������������50�ˣ�����ִ������Ĵ��룬����ѭ��
				}
			}
			Check_Agv_Ask++;
			//CanP_Host_Main();//�ж�ָ���Զ���Fifo�л�ȡ���ݲ�����
			delay_ms(50);
			Can_ZigBeeRx_Check();//zigbee���
		}
		Check_Agv_Ask = 0;
		Stop_Flag = 0;	
		delay_ms(50);
}


/***************************************************************
** ���ܣ�     �����ش���׿�ź�
** ������	  	��
** ����ֵ��   ��
** ˵����     
****************************************************************/
void zhuche_Ack(void)
{   
		Principal_Tab[2] = 0x7E;				  //����״̬
		Principal_Tab[3] = 0xCC;
						
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
** ���ܣ�     ���������ӳ���Ӧ���ź�
** ������	  	��
** ����ֵ��   ��
** ˵����     
****************************************************************/
void zhuche_Ask_congche(void)
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
//		Send_ZigbeeData_To_Fifo(Send_zigbee,8);
//		delay_ms(10);
		Send_zigbee[0]=0;//�������,��Ϊ��ʱ��ֻ���˼�λ
		Send_zigbee[1]=0;
		Send_zigbee[2]=0;
		Send_zigbee[3]=0;
		Send_zigbee[4]=0;
		Send_zigbee[5]=0;
		Send_zigbee[6]=0;
		Send_zigbee[7]=0;
}

