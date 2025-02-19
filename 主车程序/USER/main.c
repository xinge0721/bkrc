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

//				Tracking_Forward(70,95);
//				Forward(70,95,350);  //ʮ��·��ǰ��һ��

//				turn_ppmd(90,90,820);
//				turn_llmd(90,90,1650);
//				delay_ms(20);
//				Back_1(85,70,200);
//				delay_ms(20);
//				Tracking_Forward(70,95);
//				Forward(70,95,1300);  //ʮ��·��ǰ��һ��
//				Tracking_Forward(70,95);
//				Forward(70,95,350);  //ʮ��·��ǰ��һ��
				
				
//				gradetime = gt_get()+100;
//				while (1)		//�жϳ���û�е���һ��
//				{
//					if (Stop_Flag == 0x09)
//					{
//						Forward(35,75,1200);  //ʮ��·��ǰ��һ�㣬������
//						Stop_Flag = 0;
//						break;
//					}
//					else
//					{
//						if(gt_get_sub(gradetime) == 0)//100ms���һ��
//						{
//							gradetime = gt_get()+100;
//							litiGarageF1('A');//�����½���һ¥
//							Request_Location('A');
//							gradetuic++;
//							if(gradetuic ==210)//21s���޷�Ӧǿ�Ƴ��⣬����϶����½���������ʱ�򳵿�û�з�Ӧ�����������
//							{
//								gradetuic =0;
//								Forward(55,85,1200);  //ʮ��·��ǰ��һ�㣬������
//								break;
//							}
//						}
//						Can_ZigBeeRx_Check();
//					}
//				}


				


//				Forward(45,100,310);  //ʮ��·��ǰ��һ��
//				Turn_Left(85,85);//��ת
//				Tracking_Forward(45,100);//70�ٶ�ѭ��ǰ��
//				Forward(45,100,220);  //ʮ��·��ǰ��һ��
//				Turn_Right(85,85);
//				Tracking_Forward(85,85);//70�ٶ�ѭ��ǰ��
//				Forward(45,100,220);  //ʮ��·��ǰ��һ��
		

//				Tracking_Forward(85,85);//70�ٶ�ѭ��ǰ��
//				Forward(45,100,325);  //ʮ��·��ǰ��һ��
//				Turn_Right_Angle90(80+u);//����90�ȣ������d���Ե�ֵ��ͨ������1��2��
//				delay_ms(500);
//				delay_ms(500);
//				delay_ms(500);
//				delay_ms(500);
//				Turn_Left_Angle(152+d);//����180�ȣ������d���Ե�ֵ��ͨ������1��2��
//				Forward(45,100,1520);//�������־��
//				Tracking_Forward(85,85);//70�ٶ�ѭ��ǰ��
//				Forward(45,85,325);  //ʮ��·��ǰ��һ��


//				yunshuche_start();//�ӳ���һ������
//				
//				while(1)//�ӳ��Ѿ����ȫ�Զ�����
//				{
//					if(uustop==6)
//					{
//						uustop = 0;
//						break;
//					}
//					Can_ZigBeeRx_Check();
//				}
//				Tracking_Forward(85,85);//70�ٶ�ѭ��ǰ��
//				Forward(55,100,250);  //ʮ��·��ǰ��һ��

		//Turn_Left(100,100);
			
//			Tracking_Forward(85,85);//70�ٶ�ѭ��ǰ��
//			Forward(55,85,280);  //ʮ��·��ǰ��һ��	


//			Forward(45,100,310);  //ʮ��·��ǰ��һ��	
//			turn_llmd(100,100,830);//��ת90��
	}
	
	if(S4 == 0)//�޹���
    {
				delay_ms(20);
				while(S4 == 0);
				delay_ms(20);
				daozha_open();
				TFT_Picture_Down('A');
				TFT_Picture_Down('B');
				TFT_Picture_Down('C');
			
//				Send_ZigbeeData_To_Fifo(SMG_JSK,8);//��LED��ʱ
//				delay_ms(50);
//				Send_ZigbeeData_To_Fifo(SMG_JSK,8);
//				delay_ms(50);
//				Send_ZigbeeData_To_Fifo(SMG_JSK,8);
//				delay_ms(50);
//				
//				gradetime = gt_get()+100;
//				while (1)		//�жϳ���û�е���һ��
//				{
//					if (Stop_Flag == 0x09)
//					{
//						Forward(55,85,1200);  //ʮ��·��ǰ��һ�㣬������
//						Stop_Flag = 0;
//						break;
//					}
//					else
//					{
//						if(gt_get_sub(gradetime) == 0)//100ms���һ��
//						{
//							gradetime = gt_get()+100;
//							litiGarageF1('A');//�����½���һ¥
//							Request_Location('A');
//							gradetuic++;
//							if(gradetuic ==210)//21s���޷�Ӧǿ�Ƴ��⣬����϶����½���������ʱ�򳵿�û�з�Ӧ�����������
//							{
//								gradetuic =0;
//								Forward(55,85,1200);  //ʮ��·��ǰ��һ�㣬������
//								break;
//							}
//						}
//						Can_ZigBeeRx_Check();
//					}
//				}
//				
//				Turn_Right(85,85);//��ת
//		
//				
//				
//				
//				ETCtime = gt_get()+100;
//				while(1)//�ȴ�ETC��
//				{
//					if(Stop_Flag ==0x06)
//					{
//						Tracking_Forward(85,85);//70�ٶ�ѭ��ǰ��
//						Forward(45,100,250);  //ʮ��·��ǰ��һ��
//						break;
//					}
//					else
//					{
//						if(gt_get_sub(ETCtime) == 0)//100ms���һ��
//						{
//							ETCtime = gt_get()+100;
//							Forward(45,100,50); 
//							Back_1(45,100,50);  //����ҡ��һ�£�����ʹETC���ü�⵽
//						}
//						Can_ZigBeeRx_Check();
//					}
//				}
//				
//				
//				
//				Voice_BroadCast_SuiJi();//�����Բ�����־�������������
//				delay_ms(500);
//				
//				
//				Turn_Left(85,85);//��ת
//				
//				Car_Time_Track(85,85,700);//ѭ��6cm�����ά�봦
//				Turn_Right_Angle90(80);//��ת90
//				
//				
//					LED_Ult_dis();
//				
//				Voice_BroadCast_wendu();
//				delay_ms(500);
//				delay_ms(500);
//				
//				Turn_Left(85,85);//��ת��ѭ������
//				
//				
//				Tracking_Forward(85,85);//70�ٶ�ѭ��ǰ��
//				Forward(55,100,250);  //ʮ��·��ǰ��һ��
//				
//				Turn_Right_Angle90(79);//��׼TFT
//				Send_ZigbeeData_To_Fifo(TFT_PageAUp , 8);
//				delay_ms(100);
//				Send_ZigbeeData_To_Fifo(TFT_PageAUp , 8);
//				delay_ms(100);
//				Send_ZigbeeData_To_Fifo(TFT_PageAUp , 8);
//				delay_ms(500);
//				delay_ms(500);
//				delay_ms(500);
//				delay_ms(500);
//				Turn_Left(85,85);//��ת��ѭ������
//				
//				Tracking_Forward(85,85);//70�ٶ�ѭ��ǰ��
//				Forward(45,100,325);  //ʮ��·��ǰ��һ��
//				Turn_Right_Angle90(80+u);//����90�ȣ������d���Ե�ֵ��ͨ������1��2��
//				delay_ms(500);
//				delay_ms(500);
//				delay_ms(500);
//				delay_ms(500);
//				Turn_Left_Angle(152+d);//����180�ȣ������d���Ե�ֵ��ͨ������1��2��
//				Forward(45,100,1520);//�������־��
//				Tracking_Forward(85,85);//70�ٶ�ѭ��ǰ��
//				Forward(45,85,325);  //ʮ��·��ǰ��һ��
//				
//				
//				Turn_Left(85,85);
//				
//				daozatime = gt_get()+50;
//				while(1)
//				{
//					
//					if(Stop_Flag == 0x05)//����բ�Ƿ��
//					{
//						Tracking_Forward(74,70);//70�ٶ�ѭ��ǰ��
//						Forward(55,100,250);  //ʮ��·��ǰ��һ��
//						Stop_Flag = 0;
//						break;
//					}
//					else
//					{
//						if(gt_get_sub(daozatime) == 0)//50ms���һ��
//						{
//							daozatime = gt_get()+50;
//							daozha_open();//�򿪵�բ
//							delay_ms(50);
//							daozha_ack();
//							delay_ms(50);
//							daozatuichu++;//����ʱ�䣬�����բ��ʱ��û��Ӧ����ȥ
//							if(daozatuichu ==80)
//							{
//								daozatuichu =0;
//								Tracking_Forward(74,70);//70�ٶ�ѭ��ǰ��
//								Forward(55,100,250);  //ʮ��·��ǰ��һ��
//								break;
//							}
//						}
//						Can_ZigBeeRx_Check();
//					}
//				}
//				
//				
//				
//					gradetime = gt_get()+100;
//					while (1)		//�жϳ�����û�е���һ��,��Ϊ�����������Ƴ��⣬ԭ��A�����Ѿ��½��Ͳ���Ҫ����п����ôӳ�ȥB����
//					{
//						if (Stop_Flag == 0x09)//һ¥�ͽ���ѭ��
//						{
//							Stop_Flag = 0;
//							break;
//						}
//					
//						else
//						{
//							if(gt_get_sub(gradetime) == 0)//50ms���һ��
//							{
//								gradetime = gt_get()+100;
//								litiGarageF1('A');//�����½���һ¥
//								Request_Location('A');
//							}
//							Can_ZigBeeRx_Check();
//						}
//					}	
//				
//				yunshuche_start();//�ӳ���һ������
//				
//				while(1)//�ӳ��Ѿ����ȫ�Զ�����
//				{
//					if(uustop==6)
//					{
//						uustop = 0;
//						break;
//					}
//					Can_ZigBeeRx_Check();
//				}
//				Tracking_Forward(85,85);//70�ٶ�ѭ��ǰ��
//				Forward(55,100,250);  //ʮ��·��ǰ��һ��
//				
//				Turn_Right(85,85);
//				
//				Tracking_Forward(85,85);//70�ٶ�ѭ��ǰ��
//				Forward(55,100,250);   //ʮ��·��ǰ��һ��,���ﵽ����̨����
//				
//				Turn_Left(85,85);//����̨
//				Infrared_Send(baojing,6);  
//				delay_ms(200);
//				
//				Turn_Right(85,85);
//				Turn_Right(85,85);
//				
//				Tracking_Forward(85,85);//70�ٶ�ѭ��ǰ��
//				Forward(55,100,250);  //ʮ��·��ǰ��һ��,���ﵽLCD����
//				
//				Tracking_Forward(85,85);//70�ٶ�ѭ��ǰ��
//				Forward(55,100,250);  //ʮ��·��ǰ��һ��,���ﵽ·�Ƹ���
//				
//				Turn_Left_Angle(84);//����90�ȵ�·����ǰ
//				
//				
//				returnvalue =Light_Inf_Start();
//				mubiaovalue = 4;//����·�����յ�λ��������ֵ�����޸�
//				if(returnvalue == 1 || returnvalue == 2 || returnvalue == 3 || returnvalue == 4)//��ǰ����·�Ƶ�λ
//				{
//					if(mubiaovalue==1)//����·�����յ�λΪ1����Ҫ�޸�
//					{
//						
//					}
//					else if(mubiaovalue==2)//����·�����յ�λΪ2,���1��
//					{
//						Street_light_one();
//						delay_ms(500);
//					}
//					else if(mubiaovalue==3)//����·�����յ�λΪ3,���2��
//					{
//						Street_light_two();
//						delay_ms(500);
//					}
//					else if(mubiaovalue==4)//����·�����յ�λΪ4,���3��
//					{
//						Street_light_three();
//						delay_ms(500);
//					}
//				}

//				
//				Turn_Left_Angle(84);//����90��
//				
//				Back_1(85,85,600);
//				
//				Send_ZigbeeData_To_Fifo(Charge_Open, 8);
//        delay_ms(200);
//        Send_ZigbeeData_To_Fifo(Charge_Open, 8);
//        delay_ms(200);
//			while(1)
//			{
//				Voice_BroadCast_wendu();
//				delay_ms(500);
//				Can_ZigBeeRx_Check();
//				if(Stop_Flag==0xEF)
//				{
//					Stop_Flag = 0;
//					break;
//				}
//			}
//				to =(((tempwendu + tempwendu)%4) +1);//��Ŀ����
//				
//				if(to == 1)
//				{
//					litiGarageF1('A');
//					delay_ms(10);
//					litiGarageF1('A');
//					delay_ms(10);
//					litiGarageF1('A');
//					delay_ms(10);
//				}
//				if(to == 2)
//				{
//					litiGarageF2('A');
//					delay_ms(10);
//					litiGarageF2('A');
//					delay_ms(10);
//					litiGarageF2('A');
//					delay_ms(10);
//				}
//				if(to == 3)
//				{
//					litiGarageF3('A');
//					delay_ms(10);
//					litiGarageF3('A');
//					delay_ms(10);
//					litiGarageF3('A');
//					delay_ms(10);
//				}
//				if(to == 4)
//				{
//					litiGarageF4('A');
//					delay_ms(10);
//					litiGarageF4('A');
//					delay_ms(10);
//					litiGarageF4('A');
//					delay_ms(10);
//				}
//						
//				
//				
//				
//				gradetime = gt_get()+100;
//				while (1)		//�жϴӳ�����û�е���һ�㣬������һ���ֹͣ�����LED��ʱ
//				{
//					if (Stop_Flag == 0x0B || Stop_Flag == 0x09 || Stop_Flag == 0x0A || Stop_Flag == 0x0C)//�������Ĳ㶼����ѭ��
//					{
//						Stop_Flag = 0;
//						break;
//					}
//					else
//					{
//						if(gt_get_sub(gradetime) == 0)//50ms���һ��
//						{
//							gradetime = gt_get()+100;
//							Request_Location('A');
//						}
//						Can_ZigBeeRx_Check();
//					}
//				}
//				
//				Send_ZigbeeData_To_Fifo(SMG_JSG,8);//�ر�LED��ʱ
//				delay_ms(10);
//				Send_ZigbeeData_To_Fifo(SMG_JSG,8);
//				delay_ms(10);
//				Send_ZigbeeData_To_Fifo(SMG_JSG,8);//�ر�LED��ʱ
//				delay_ms(10);
					
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
#if 1		
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


//ETCtime = gt_get()+100;
//				while(1)//�ȴ�ETC��
//				{
//					if(Stop_Flag ==0x06)
//					{
//						Tracking_Forward(85,85);//70�ٶ�ѭ��ǰ��
//						Forward(45,100,250);  //ʮ��·��ǰ��һ��
//						break;
//					}
//					else
//					{
//						if(gt_get_sub(ETCtime) == 0)//100ms���һ��
//						{
//							ETCtime = gt_get()+100;
//							Forward(45,100,50); 
//							Back_1(45,100,50);  //����ҡ��һ�£�����ʹETC���ü�⵽
//						}
//						Can_ZigBeeRx_Check();
//					}
//				}


/**
�������ܣ��������
��    ������
�� �� ֵ����
���� ������ͨ���������־��ͨ��
ע�⣺��־��ͨ�����׶������Զ෢����
*/
//void KEY_Check() 
//{
//	if(S1 == 0)  //����һ��,d+1��Ҳ����e+1,����15���1
//	{
//		delay_ms(10);
//		if(S1 == 0)
//		{
//			LED1 = !LED1;
//			d++;
//			delay_ms(10);
//			if(d > 14)
//			{
//				d = 1;
//			}
//			while (!S1);	//�ɿ�����	
//			
//		}
//	}
//	
//	if(S2 == 0)  //����һ��,d-1��Ҳ����e-1,����0���13
//	{
//		delay_ms(10);
//		if(S2 == 0)
//		{
//			LED2 = !LED2;
//			d--;
//			if(d < 0)
//			{
//				d = 13;
//			}
//			while (!S2);
//		}
//	}
//	
//	if(S3 == 0)  //�Ѱ���1����2��ֵd��ֵ��e��Ȼ��e�Ϳ���ִ�а��������ݣ�����һ��ִ��һ��
//	{
//		delay_ms(10);
//		if(S3 == 0)
//		{
//			LED3 = !LED3;
//			e = d;
//			while(!S3);
//		}
//	}
//	
//	if(S4 == 0)//�޹���
//    {
//        delay_ms(10);
//        if(S4 == 0)
//				{
//					while(!S4);					
//				}
//    }
//    
//	
//	switch(e) 
//{
//		case 1:  //�򿪼�ʱ���رռ�ʱ
//		{
//			e = 0;Send_ZigbeeData_To_Fifo(SMG_JSK,8);
//		  for(int i=0;i<5;i++)
//		  {
//				delay_ms(500);
//			}
//			Send_ZigbeeData_To_Fifo(SMG_JSG,8);
//			delay_ms(500);
//			break;
//		}
//		case 2:  //��ಢ��LED��ʾ����
//		{
//			e = 0; 
//			LED_Ult_dis();
//       break;
//		}
//      case 3:   //����ʶ��
//			{
//				e = 0;
//			    Test();//stn7318,ʹ��֮���Ӽǵø�λ��ֻ��ʶ��һ�Σ�������������������־��������������ظ�����
//			}
//       break;
//      case 4:  //��ͨ��ʶ�𣬰���ȷ�Ϸ�^_^ ^_^ ^_^  1��2��3�ƣ�4����(ע��һ����Ҫ����)����������2���Ӻ��ڽ��н�ͨ��ʶ��Ȼ��㸴λ
//			{
//				e = 0; 
//         Send_ZigbeeData_To_Fifo(TrafficA_Open , 8);//��ͨ��A����ʶ��
//         delay_ms(500);
//         delay_ms(500);
//         while(1)
//         {
//             delay_ms(500);
//             delay_ms(500);
//             if(S1 == 0) //����ʶ�����һ�ΰ��°���1��2��3�����߽�ͨ����ʲô��ɫ�ĵ�,
//             {
//                 delay_ms(10);
//                 if(S1 == 0)
//                    while(!S1);
//                  Send_ZigbeeData_To_Fifo(TrafficA_Red , 8);
//              }
//              else if(S2 == 0)
//             {
//                 delay_ms(10);
//                 if(S2 == 0)
//                    while(!S2);
//                  Send_ZigbeeData_To_Fifo(TrafficA_Green , 8);
//              }
//              else if(S3 == 0)
//             {
//                 delay_ms(10);
//                 if(S3 == 0)
//                    while(!S3);
//                  Send_ZigbeeData_To_Fifo(TrafficA_Yellow , 8);
//              }
//              else if(S4 == 0)//Ȼ����������������������֮һһ��֮�󣬽�ͨ�ƻ᷵��״̬���ٰ��°���4����ʶ��
//             {
//                 delay_ms(10);
//                 if(S4 == 0)
//                 while(!S4);
//                 break;
//              }
//           }
//       break;
//			}
//      case 5:   //TFTA�����Ϸ�
//			{
//				e = 0;
//         Send_ZigbeeData_To_Fifo(TFT_PageAUp , 8);
//         for(int i=0;i<5;i++)
//         {     
//            delay_ms(500);
//         }    
//       break;
//			}
//      case 6:  //������ʾ��־����ʾ����Ϊ3
//			{
//				e = 0;
//         Rotate_Dis_Inf(3);
//         delay_ms(500);
//       break;
//			}
//      case 7:   //����·�Ƶ���3��
//			{
//				 e = 0; 
//         Street_light_three();
//         delay_ms(100);
//       break;
//			}
//      case 8:  //��ά��
//			{
//				e = 0;
//         for(int i=0;i<5;i++)
//         {     
//            delay_ms(500);
//         }
//         MP_SPK = 1;
//         delay_ms(500);
//         delay_ms(500);
//         MP_SPK = 0;
//			}
//       break;
//      case 9:  //���ⷢ��
//			{
//				e = 0;
//         delay_ms(200);
//         Infrared_Send(Infrared,6);  
//         delay_ms(200);
//       break;
//			}
//      case 10:    //���Ƽ���ɫʶ��
//			{
//				e = 0;
//         TFT_send_chepai('A',"A12220");
//         delay_ms(500);
//         delay_ms(500);
//         delay_ms(500);
//         TFT_send_chepai('A',"A12220");
//         delay_ms(500);
//       break;
//			}
//      case 11:   //�򿪵�բ
//			{
//				e = 0;
//         daozha_open();
//         delay_ms(500);
//         daozha_open();
//         delay_ms(200);
//       break;
//			
//			}
//      case 12:    //���峵��3��
//			{
//				e = 0; 
//         litiGarageF3('A');  
//         delay_ms(200);
//       break;
//			}
//      case 13:  //���߳�翪��
//			{
//				e = 0; 
//         Send_ZigbeeData_To_Fifo(Charge_Open, 8);
//         delay_ms(500);
//         Send_ZigbeeData_To_Fifo(Charge_Open, 8);
//         delay_ms(200);
//       break;
//			}
//      default:
//         break;
//		}
//}


//Send_ZigbeeData_To_Fifo(ewmTest,8);
				//yunshuche_back(90,1500);
//				Send_ZigbeeData_To_Fifo(hh1,8);
//					delay_ms(500);
//					Send_ZigbeeData_To_Fifo(hh2,8);




