#include <stdio.h>
#include "stm32f4xx.h"
#include "delay.h"
#include "infrared.h"
#include "cba.h"
#include "ultrasonic.h"
#include "canp_hostcom.h"
#include "hard_can.h"
#include "bh1750.h"
#include "syn7318.h"
#include "power_check.h"
#include "can_user.h"
#include "data_base.h"
#include "roadway_check.h"
#include "tba.h"
#include "data_base.h"
#include "swopt_drv.h"
#include "uart_a72.h"
#include "Can_check.h"
#include "delay.h"
#include "can_user.h"
#include "Timer.h"
#include "rc522.h"
#include "test.h"
#include "LED.h"
#include "TFT.h"
#include "infrared_marker.h"
#include "garage.h"
#include "mytest.h"
#include "syn7318.h"
#include "AGV.h"
#include "Rotate.h"
#include "daoza.h"
#include "baojing.h"
#include "intelligent_street_lamp.h"
#include "voicebroadcast.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "wirelesscharging.h"

int testtime=0;	             
int d =0,e =0,u=0;
uint16_t ucErrTime;
RCC_ClocksTypeDef RCC_Clocks;
uint16_t main_cont;
uint8_t str[5];
uint16_t gzqd=0;
uint8_t a[]={"A1B2C3\n"};//���ƺ�
int o =0;
uint8_t Infrared[6] = {0x12,0x1e,0x3c,0xd2,0x13,0x18};			//�������ݴ������,��������
uint8_t baojing[6]={0x03,0x05,0x14,0x45,0xDE,0x92};//���ⱨ��Ĭ�Ͽ�����
uint16_t Check_Agv_daoza = 0;
uint8_t daozha_ack_Count = 0;
uint8_t hh1[8]={0x55,0x02,0x21,0x00,0x00,0x00,0x21,0xbb};//hh1+hh2�Ǵӳ����������������Ϻ��ⷢ�䵽������ʾ����
uint8_t hh2[8]={0x55,0x02,0x12,0x00,0x00,0x00,0x12,0xbb};
uint8_t hh3[8]={0x55,0x02,0x63,0x00,0x00,0x00,0x63,0xbb};//hh3�Ǵӳ�����·������

uint8_t infrare_com12[6]={0xFF,0x11,0x00,0x00,0x00,0x00};//���������������ʾ���뵽������ʾ��־������

int to=0;

int qa=0;
int q=0;



uint32_t gradetime=0;//�������ڼ��
uint32_t daozatime = 0;//��բ���ڼ��
uint8_t daozatuichu=0;//��բ�˳�����
uint32_t ETCtime=0;//ETC���ڼ��
uint8_t gradetuic=0;//���ⳤʱ��û��⵽��ǿ���˳�
/**
�������ܣ�Ӳ����ʼ��
��    ������
�� �� ֵ����
*/
void Hardware_Init()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);			//�жϷ���
	delay_init(168);																		//�δ�ʱ����ʼ��
	Tba_Init();																					//������ʼ��
	Infrared_Init();																		//�����ʼ��
	Cba_Init();																					//���İ��ʼ��
	Ultrasonic_Init();																	//��������ʼ��
	Hard_Can_Init();																		//CAN���߳�ʼ��
	BH1750_Configure();																	//BH1750��ʼ��
	BKRC_Voice_Init();																  //С������ģ���ʼ��
	Electricity_Init();																	//��������ʼ��
	UartA72_Init();																			//A72��ʼ��
	Can_check_Init(7,83);																//CAN���߶�ʱ����ʼ��
	roadway_check_TimInit(499,167);										  //·������ʼ��
	Timer_Init(167,999);																//��������ͨѶʱ��֡��ʼ��
	Readcard_daivce_Init();                         	  // RFID��ʼ��
}


void KEY_Check() 
{
	if(S1 == 0)  
	{
        delay_ms(20);
        while(S1==0);
				delay_ms(20);
				LED1 = !LED1;
				d++;
				delay_ms(10);
				if(d > 50)
				{
					d = 1;
				}
	
	}
	
	if(S2 == 0)  
	{
					delay_ms(20);
					while(S2 == 0);
					delay_ms(20);
					LED2 = !LED2;
					u++;
					if(u > 50)
					{
						u = 1;
					}
	}
	
	if(S3 == 0)  //�Ѱ���1����2��ֵd��ֵ��e��Ȼ��e�Ϳ���ִ�а��������ݣ�����һ��ִ��һ��
	{
				delay_ms(20);
        while(S3==0);
				delay_ms(20);
	
	
				Light_match(4);
	}
	
	if(S4 == 0)//�޹���
    {
				delay_ms(20);
				while(S4 == 0);
				delay_ms(20);
		}
}

static uint32_t LED_twinkle_times;          // LED��˸����
static uint32_t Power_check_times;			  //�����������
static uint32_t WIFI_Upload_data_times; 	//ͨ��Wifi�ϴ���������
static uint32_t RFID_Init_Check_times;  	//RFID�������

int main(void)
{	
	
	uint16_t Light_Value = 0;								//��ǿ��ֵ	
	uint16_t CodedDisk_Value = 0;						//����
	uint16_t Nav_Value = 0;									//�Ƕ�	
	
	Hardware_Init();						    				//Ӳ����ʼ�� 
	LED_twinkle_times = gt_get() + 50;	     
	Power_check_times =  gt_get() + 200;//һ��ʼ������200ms+gt_get()��ʱ�䣬���������200ms
	WIFI_Upload_data_times = gt_get() + 200;
	RFID_Init_Check_times = gt_get()+200;
	
	Principal_Tab[0] = 0x55;
	Principal_Tab[1] = 0xAA;	
	
	Follower_Tab[0] = 0x55;
	Follower_Tab[1] = 0x02;

  Send_UpMotor(0, 0);
	while(1)
	{				
		KEY_Check();										//�������,����һ��Ҫ����һ�ᣬ��Ҫ���ٵ㣬����û��Ӧ
		Can_WifiRx_Check();            //wifi�������ݴ���WiFi���հ�׿����
		Can_ZigBeeRx_Check();          //zigbee�������ݴ���zigbee�������б�־������
		
		if(gt_get_sub(LED_twinkle_times) == 0)          // ����ָʾ��
        {
            LED_twinkle_times = gt_get() + 50;          // LED4״̬ȡ��
            LED4 = !LED4;
        }
		
		
		if(gt_get_sub(Power_check_times) == 0) 	//Ŀ��ʱ��200ms��Ȼ��ʱ����ʱ����200ms�����һ��	
		{
			Power_check_times =  gt_get() + 200;			//��ص�����⣬��������Ŀ��ʱ����+200���ȳ����ȥ�󣬾Ϳ��Եȴ���һ�ε�200ms����
			Power_Check();
		}
#if 1		
		if(gt_get_sub(RFID_Init_Check_times) == 0)      // RFID��ʼ�����,��ʼ���ɹ��Ļ�LED��һֱ��˸��ʧ�ܵĻ��������Ϳ�ʼ��
      {
            RFID_Init_Check_times =  gt_get() + 200;
            if(Rc522_GetLinkFlag() == 0)
            {
                Readcard_daivce_Init();
                MP_SPK = !MP_SPK;
            }
            else
            {
								MP_SPK = 0;
								LED3 = !LED3;
								Rc522_LinkTest();
            }
       }
#endif		
#if 0		
		if(gt_get_sub(WIFI_Upload_data_times) == 0)    //�����ϴ� 
		{
			WIFI_Upload_data_times =  gt_get() + 200;//200ms���һ��
			//Host_AGV_Return_Flag ��ʼ�� ΪRESET
			// AGV_data_Falg ��ʼ��Ϊ RESET
			if(Host_AGV_Return_Flag == RESET)            //���������ϴ�
			{
				Principal_Tab[2] = Stop_Flag;						  //����״̬
				Principal_Tab[3] = Get_tba_phsis_value();	//����״ֵ̬����
				
				Ultrasonic_Ranging();										  //���������ݲɼ�						
				Principal_Tab[4] = dis % 256;        			//���������ݵͰ�λ
				Principal_Tab[5] = dis / 256;							//���������ݸ߰�λ
				
				Light_Value = Get_Bh_Value();						  //��ǿ�ȴ��������ݲɼ�	
				Principal_Tab[6] = Light_Value % 256;	   	//�������ݵͰ�λ
				Principal_Tab[7] = Light_Value / 256;			//�������ݸ߰�λ
				
				CodedDisk_Value = CanHost_Mp;						  //����
				Principal_Tab[8] = CodedDisk_Value % 256;	    	
				Principal_Tab[9] = CodedDisk_Value / 256;
				
				Nav_Value = CanHost_Navig;							  //�Ƕ�
				Principal_Tab[10] = Nav_Value % 256;	    	
				Principal_Tab[11] = Nav_Value / 256;
				
				Send_WifiData_To_Fifo(Principal_Tab, 12); //ͨ��wifi�ϴ�С������
				UartA72_TxClear();
				UartA72_TxAddStr(Principal_Tab, 12);      //ͨ�������ϴ���������
				UartA72_TxStart();
			}
			else if((Host_AGV_Return_Flag == SET) && (AGV_data_Falg == SET))
				{	
					UartA72_TxClear();
					UartA72_TxAddStr(Follower_Tab,50);     //ͨ�������ϴ���������
					UartA72_TxStart();
					Send_WifiData_To_Fifo(Follower_Tab,50); //ͨ��wifi�ϴ���������
					AGV_data_Falg = 0;
				}
		}
		#endif
	}		
}




