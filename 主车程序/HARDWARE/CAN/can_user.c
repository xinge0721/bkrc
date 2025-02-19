#include <stdint.h>
#include "stm32f4xx.h"
#include "canP_HostCom.h"
#include "delay.h"
#include "data_channel.h"
#include "roadway_check.h"
#include "mytest.h"
#define __CAN_USER_C__
#include "can_user.h"
#include "tba.h"
#include "data_base.h"
#include "cba.h"
#include "infrared.h"
#include "syn7318.h"
#include <string.h>
#include "Timer.h"
#include "mytest.h"
#include "garage.h"
#include "daoza.h"
#include "AGV.h"
#include "TFT.h"
uint8_t erweima[6];
uint8_t tp[21], i;
uint8_t Traffic_Flag = 0;
uint8_t Start_First = 1;	//A72һ��������־λ
uint8_t Wifi_Rx_Buf[WIFI_MAX_NUM];
uint8_t Zigb_Rx_Buf[ZIGB_RX_MAX];
uint8_t Wifi_Rx_num ;   //wifi�������ݵĴ���
uint8_t Wifi_Rx_flag ;  //������ɱ�־λ
uint8_t Zigbee_Rx_num ;   //zigbee�������ݵĴ���
uint8_t Zigbee_Rx_flag ;  //������ɱ�־λ

uint8_t uustop =0;

uint8_t traficred1[8]={0x55,0x02,0X02,0X01,0x00,0x00,0x03,0xBB};

uint8_t Host_AGV_Return_Flag = RESET;
uint8_t AGV_data_Falg = RESET;

uint32_t canu_wifi_rxtime = 0;
uint32_t canu_zibe_rxtime = 0;


void Can_WifiRx_Save(uint8_t res) //��can���߽��յ�wifi���ݱ���
{
	if(Wifi_Rx_flag == 0)
	{
		canu_wifi_rxtime = gt_get()+10;
		Wifi_Rx_num =0;
		Wifi_Rx_Buf[Wifi_Rx_num]=res;
		Wifi_Rx_flag = 1;
	}
	else if(Wifi_Rx_num < WIFI_MAX_NUM )	
	{
		Wifi_Rx_Buf[++Wifi_Rx_num]=res;	 
	}
}

uint8_t Rx_Flag ;

void Normal_data(void)	      // ��������8�ֽڿ���ָ�wifi
{

	if(Wifi_Rx_Buf[7] == 0xBB)	  // �жϰ�β
	{					
		//��ָ������λ��ָ�����У��
		//ע�⣺��������ʱӦ�öԺ���256ȡ�ࡣ
		//sum��У��λ
		uint8_t sum = (Wifi_Rx_Buf[2]+Wifi_Rx_Buf[3]+Wifi_Rx_Buf[4]+Wifi_Rx_Buf[5])%256;
		
		if(sum == Wifi_Rx_Buf[6])//Wifi_Rx_Buf[6]��ƽ��������������ߴӳ����͵ĵ���λУ��λ��У��λ�����ȣ�����1�������͸��������ߴӳ���ָ����ȷ
		{
			Send_WifiData_To_Fifo(Wifi_Rx_Buf,8);
			Rx_Flag =1;
		}
		else
		{
			 Rx_Flag =0;
		}
	}
}

void Abnormal_data(void)	  //�����쳣����
{
	u8 i,j;
	u8 sum=0;
	
	if(Wifi_Rx_num <9)			// �쳣�����ֽ���С��8�ֽڲ�������
	{
	   Rx_Flag =0;
	}
	else
	{
		for(i=0; i<=(Wifi_Rx_num -8); i++)  
		{
			if(Wifi_Rx_Buf[i]==0x55)	   // Ѱ�Ұ�ͷ
			{
			   if(Wifi_Rx_Buf[i+7]==0xbb)	// �жϰ�β
			   {
					 sum = (Wifi_Rx_Buf[i+2]+Wifi_Rx_Buf[i+3]+Wifi_Rx_Buf[i+4]+Wifi_Rx_Buf[i+5])%256;

					 if(sum == Wifi_Rx_Buf[i+6])	 // �ж����
					 {
						 for(j=0;j<8;j++)
						 {
								Wifi_Rx_Buf[j]=Wifi_Rx_Buf[j+i];	 // ���ݰ���
						 }
					 	 Rx_Flag =1;
					 }
					 else
					 {
							Rx_Flag =0;
					 }
		     }
			}
	  }
	}
}


uint8_t Infrared_Tab[6];			//�������ݴ������



void Can_WifiRx_Check(void)  //wifi�������ݴ���
{
	if(Wifi_Rx_flag)						//��Э������ж�,Wifi_Rx_flag�ǽ�����ɱ�־λ��
	{	
		
		 if(gt_get_sub(canu_wifi_rxtime) == 0)
		 {
			 if(Wifi_Rx_Buf[0]==0xFD)  
			 {			
				 Send_ZigbeeData_To_Fifo( Wifi_Rx_Buf , (Wifi_Rx_num +1)); //�����ݷ�����ZigBeeģ��
			 }
			 else if(Wifi_Rx_Buf[0]==0x55)  //ƽ��������������ߴӳ����͵����ݰ�ͷ
			 { 
				 Normal_data();  // ��������8�ֽڿ���ָ��(ƽ����������ӳ����͵�8�ֽ�ָ��)�����ж��Ƿ�ָ��һ��
			 }
//			 else
//			 {
//				 Abnormal_data();    //�����쳣����(ƽ������������ӷ���ָ��)
//			 } 	
			 Wifi_Rx_flag = 0;    //������ɱ�־λ��0
		}
	}
	
	if(Rx_Flag ==1)	//Rx_Flag=1����ƽ����Է��͸�������ָ����ȷ��������
	{    

		if(Wifi_Rx_Buf[1] == 0xAA) 	  //��ͷ��2λ���ڶ�λ��ͷ�����0xAA�ʹ�����ƽ����Է����������� 
		{	
			switch(Wifi_Rx_Buf[2]) 			//�ж���ָ��
			{
				case 0x01:              	//����ֹͣ
					Send_UpMotor(0, 0);			//�ٶ�Ϊ0
					Roadway_Flag_clean();	  //�����־λ״̬	
					break;
				case 0x02:                //����ǰ��
					Roadway_mp_syn();				//����ͬ�������̿��������
					Track_Forward_Flag = 0;	//ǰ��ѭ����־λ
					Track_Back_Flag = 0;		//����ѭ����־λ
					wheel_Nav_Flag = 0;			//������ת��־λ
					wheel_L_Flag = 0;				//��ת��־λ
					wheel_R_Flag = 0;				//��ת��־λ
					Back_Flag = 0;					//���˱�־λ
					Stop_Flag = 0;					//����״̬��־λ���ж�ƽ��������������͵�ǰ��ָ�����cm�Ƿ���ɣ�����0��ѭ��״̬������ѭ����
					Go_Flag = 1;						//ǰ����־λ
					temp_MP = Wifi_Rx_Buf[6];		//ƽ��������������͵ĸ�ָ�����λ�����̸߰�λ
					temp_MP <<=  8;								
					temp_MP |= Wifi_Rx_Buf[5];	//ƽ��������������͵ĸ�ָ�����λ�����̵Ͱ�λ��temp_MP������ֵ
					LCar_Spend = Wifi_Rx_Buf[3];	//ƽ��������������͵ĸ�ָ�����λ���ٶ�ֵ
					RCar_Spend = Wifi_Rx_Buf[4];	//ƽ��������������͵ĸ�ָ�����λ���ٶ�ֵ
					Control(LCar_Spend ,RCar_Spend);//����ٶȿ��ƺ���(��⣬�Ҳ�)����ƽ�������wifiģ�鷢�͵��ٶ�Ȼ�����ú��İ巢�������
					break;
				case 0x03:              //����
					Roadway_mp_syn();			//����ͬ�������̿��������
					Track_Forward_Flag = 0;
					wheel_Nav_Flag = 0;
					wheel_L_Flag = 0;				
					wheel_R_Flag = 0;				
					Stop_Flag = 0;					//ѭ����
					Back_Flag = 1;					//���˱�־λ
					Go_Flag = 0;
					Track_Back_Flag = 0;
					temp_MP = Wifi_Rx_Buf[5];
					temp_MP <<=  8;
					temp_MP |= Wifi_Rx_Buf[4];
					LCar_Spend = Wifi_Rx_Buf[3]; //ƽ��������������͵ĸ�ָ�����λ���ٶ�ֵ
					Control(-LCar_Spend ,-LCar_Spend);		//����д����			
					break;
				case 0x04:              //��ת�����ﲻ��Ҫ����ͬ������Ϊת��90�ȣ��൱��û�о���
					Track_Forward_Flag = 0;
					Track_Back_Flag = 0;
					wheel_Nav_Flag = 0;
					wheel_L_Flag = 1;								//��ת��־λ
					wheel_R_Flag = 0;
					Stop_Flag = 0;									//ѭ����
					Back_Flag = 0;
					Go_Flag = 0;
					LCar_Spend = Wifi_Rx_Buf[3];			//ƽ��������������͵ĸ�ָ�����λ���ٶ�ֵ	
					Control(-LCar_Spend ,LCar_Spend);//��ת�Ļ������ֺ��ˣ�����ǰ��
					break;
				case 0x05:              //��ת�����ﲻ��Ҫ����ͬ������Ϊת��90�ȣ��൱��û�о���
					Track_Forward_Flag = 0;
					Track_Back_Flag = 0;
					wheel_Nav_Flag = 0;
					wheel_L_Flag = 0;
					wheel_R_Flag = 1;							//��ת��־λ
					Stop_Flag = 0;								//ѭ����
					Back_Flag = 0;
					Go_Flag = 0;
					LCar_Spend = Wifi_Rx_Buf[3];		//ƽ��������������͵ĸ�ָ�����λ���ٶ�ֵ
					Control(LCar_Spend,-LCar_Spend);//��ת�Ļ�������ǰ�������ֺ���
					break;
				case 0x06:              //ѭ��
					Track_Forward_Flag = 1;								//ǰ��ѭ����־λ
					Track_Back_Flag = 0;
					wheel_Nav_Flag = 0;
					wheel_L_Flag = 0;
					wheel_R_Flag = 0;
					Stop_Flag = 0;             //ѭ����
					Back_Flag = 0;
					Go_Flag = 0;
					LCar_Spend = Wifi_Rx_Buf[3]; //ƽ��������������͵���ָ�����λ���ٶ�
					break;
				case 0x07:				//��������					
					break;
				case 0x08:									//��ת��--�Ƕ�											
					Roadway_nav_syn();				//�Ƕ�ͬ��
					Roadway_mp_syn();					//����ͬ��
					Track_Forward_Flag = 0;
					Track_Back_Flag = 0;
					wheel_Nav_Flag = 1;				//������ת��־λ
					wheel_L_Flag = 0;
					wheel_R_Flag = 0;
					Stop_Flag = 0;
					Back_Flag = 0;
					Go_Flag = 0;
					temp_Nav = Wifi_Rx_Buf[5];
					temp_Nav <<=  8;
					temp_Nav |= Wifi_Rx_Buf[4];
					LCar_Spend = Wifi_Rx_Buf[3];				
					Send_UpMotor(-LCar_Spend ,LCar_Spend);					
					break;
				case 0x09:						//��ת��--�Ƕ�			
					Roadway_nav_syn();	//�Ƕ�ͬ��
					Roadway_mp_syn();		//����ͬ��
					Track_Forward_Flag = 0;
					Track_Back_Flag = 0;
					wheel_Nav_Flag = 1;		//������ת��־λ
					wheel_L_Flag = 0;
					wheel_R_Flag = 0;
					Stop_Flag = 0;
					Back_Flag = 0;
					Go_Flag = 0;
					temp_Nav = Wifi_Rx_Buf[5];
					temp_Nav <<=  8;
					temp_Nav |= Wifi_Rx_Buf[4];
					LCar_Spend = Wifi_Rx_Buf[3];
					Send_UpMotor(LCar_Spend,-LCar_Spend);
					break;
				case 0x10:												//����ǰ��λ����
					Infrared_Tab[0]=Wifi_Rx_Buf[3];
					Infrared_Tab[1]=Wifi_Rx_Buf[4];
					Infrared_Tab[2]=Wifi_Rx_Buf[5];
					break;
				case 0x11:												//�������λ����
					Infrared_Tab[3]=Wifi_Rx_Buf[3];	//���ݵ���λ
					Infrared_Tab[4]=Wifi_Rx_Buf[4];	//��λУ����
					Infrared_Tab[5]=Wifi_Rx_Buf[5];	//��λУ����
					break;
				case 0x12:												//֪ͨС����Ƭ�����ͺ�����
					Infrared_Send(Infrared_Tab,6);					
					break;									
				case 0x20:												//ת��ƿ���
					L_LED = ~Wifi_Rx_Buf[3];
					R_LED = ~Wifi_Rx_Buf[4];
					break;					
				case 0x30:
					Beep =  ~Wifi_Rx_Buf[3];				//������	
					break;
				case 0x40:												//��δʹ��					
					
					break;
					case 0x50:  										//���ⷢ�������Ƭ�Ϸ� 
					Infrared_Send(H_S,4);
					break;
				case 0x51:    										//���ⷢ�������Ƭ�·� 
					Infrared_Send(H_X,4);
					break;
				case 0x61:    										//���ⷢ����ƹ�Դǿ�ȵ�λ��1 
					Infrared_Send(H_1,4);				
					break;
				case 0x62:   	 										//���ⷢ����ƹ�Դǿ�ȵ�λ��2 
					Infrared_Send(H_2,4);
					break;
				case 0x63:    										//���ⷢ����ƹ�Դǿ�ȵ�λ��3 
					Infrared_Send(H_3,4);
					break;
			   case 0x80:												//�˶���־�����ݷ�������λ
					Host_AGV_Return_Flag = Wifi_Rx_Buf[3];   //SET ���� / RESET ��ֹ
					break;
				 case 0x9D:												//A72���͸�����Э��
				 {
						if (Wifi_Rx_Buf[2] == 0x9D && Wifi_Rx_Buf[3] == 0x00)		//����A72һ������
						{
							 Stop_Flag = 0x0E;
						}
						
						//A72�ش���ͨ��ʶ����
						if ((Wifi_Rx_Buf[3] == 0x01) || (Wifi_Rx_Buf[3] == 0x02) || (Wifi_Rx_Buf[3] == 0x03))
						{
							Traffic_Flag  = Wifi_Rx_Buf[3];
							zhuche_Ack();
						}

						//A72�ش���ά��ʶ����
						if (Wifi_Rx_Buf[2] == 0x75)
						{
							erweima[0] = Wifi_Rx_Buf[3];
							erweima[1] = Wifi_Rx_Buf[4];
							erweima[2] = Wifi_Rx_Buf[5];							
							Traffic_Flag  = 0x75;
							zhuche_Ack();
						}

						if (Wifi_Rx_Buf[2] == 0x76)
						{
							erweima[3] = Wifi_Rx_Buf[3];
							erweima[4] = Wifi_Rx_Buf[4];
							erweima[5] = Wifi_Rx_Buf[5];							
							Traffic_Flag  = 0x76;
							zhuche_Ack();
						}
						
						// A72����ָ��  ֻ����һ��
						if (Stop_Flag == 0x0E && Start_First == 1)
						{
							Start_First = 0;
							zhuche_Ack();
							//autorun();
						}	
				 }
					break;
					
			   default:
					break;
			}
		}
		
		else if(Wifi_Rx_Buf[1] == 0x0B)
		{
				if(Wifi_Rx_Buf[2] == 0x10) 			//�ж���ָ��
			{
					if(Wifi_Rx_Buf[3] == 0x00)
					{
					
					}
					else if (Wifi_Rx_Buf[3] == 0x01)
					{
						LED1 = !LED1;
						TFT_Picture_Down('A');
						TFT_Picture_Down('B');
						TFT_Picture_Down('C');
					}
					else if (Wifi_Rx_Buf[3] == 0x02)
					{
							LED1 = !LED1;
						TFT_Picture_Down('A');
						TFT_Picture_Down('B');
						TFT_Picture_Down('C');
					}
					else if (Wifi_Rx_Buf[3] == 0x03)
					{
					
					}
						
			}
		}
		
		else if(Wifi_Rx_Buf[1] == 0x01)
		{
			LED1 = !LED1;
			if(Wifi_Rx_Buf[3] == 0x00 == 0 && Wifi_Rx_Buf[4] == 0x00)
			{
				if(Wifi_Rx_Buf[2] == 0x01)//��
				{
					daozha_open();
				}
				else if(Wifi_Rx_Buf[2] == 0x02)//�ر�
				{
					daozha_close();
				}
			}
			
		}
		
		else  	//ƽ����Է���������wifiģ��Ҫ�ɵ����飬����ִ�в�������ĳ���(��ͷ�ڶ�λ��0x20���Ǵӳ�)���Ͱ�wifiģ����յ�Ҫ�ɵ����鷢��zigbeeģ�飬����zigbee�ڷ�������zignbee��־����ߴӳ�
		{				//���������˼���Ǳ���ƽ����Է����ӳ�ִ�еĶ����������������������Ȼ������ִ�в��ˣ���ͨ��zigbee�����ӳ���zigbee���ôӳ�ִ�в���
			Send_ZigbeeData_To_Fifo(Wifi_Rx_Buf,8); //�����ݷ�����ZigBeeģ��
		}
		Rx_Flag = 0;
	}
}




/**
�������ܣ�����ZigBee����
��    ��: ��
�� �� ֵ����
*/
void Can_ZigBeeRx_Save(uint8_t res)
{
	if(Zigbee_Rx_flag == 0)
	{
		Zigb_Rx_Buf[Zigbee_Rx_num]=res;
		Zigbee_Rx_num++;
		if(Zigbee_Rx_num > ZIGB_RX_MAX )	
		{
			Zigbee_Rx_num = 0;
			Zigbee_Rx_flag = 1;
		} 
	}

	if(Zigbee_Rx_flag == 0)
	{
		canu_zibe_rxtime = gt_get()+10;
		Zigbee_Rx_num =0;
		Zigb_Rx_Buf[Zigbee_Rx_num]=res;
		Zigbee_Rx_flag = 1;
	}
	else if(Zigbee_Rx_num < ZIGB_RX_MAX )	
	{
		Zigb_Rx_Buf[++Zigbee_Rx_num]=res;	 
	}
		
}


/**
�������ܣ�ZigBee���ݼ��(zigbeeģ���յ���־����ߴӳ��Ļش�����)
��    ������
�� �� ֵ����
*/
void Can_ZigBeeRx_Check(void)
{
	if(Zigbee_Rx_flag)
	{
		if(gt_get_sub(canu_zibe_rxtime) == 0)//canu_zibe_rxtime��ʼֵΪ0��ֱ�ӽ������
		{
			if((Zigb_Rx_Buf[0] == 0x55)&&(Zigb_Rx_Buf[1] == 0x02))		//�ӳ���������������
			{
					if (Zigb_Rx_Buf[2] == 0xAA)
					{
						for (i = 0; i < 8; i++)
						{
							tp[i] = (Zigb_Rx_Buf[4] & 0x01) + '0';
							Zigb_Rx_Buf[4] >>= 1;
						}
						
						for (i = 0; i < 8; i++)
						{
							tp[i] = (Zigb_Rx_Buf[3] & 0x01) + '0';
							Zigb_Rx_Buf[4] >>= 1;
						}						
						Send_InfoData_To_Fifo("tp", 21);
					}
					if(Zigb_Rx_Buf[2] ==0x01)//ѭ�����
					{
						uustop=1;
					}
					if(Zigb_Rx_Buf[2] ==0x03)//ǰ�����ߺ������
					{
						uustop=2;
					}
					if(Zigb_Rx_Buf[2] ==0x02)//ת�����
					{
						uustop=3;
					}
					if(Zigb_Rx_Buf[2] ==0xCC)//��ת45���
					{
						uustop=4;
					}
					if(Zigb_Rx_Buf[2] ==0xAB)//��ת45���
					{
						uustop=5;
					}
					//0x55,0x02,0xCC,0x05,0x00,0x00,0xD1,0xbb
					if(Zigb_Rx_Buf[2] ==0xCC && Zigb_Rx_Buf[3] ==0x05)//�ӳ��Ѿ�����ȫ�Զ�����
					{
						uustop=6;
					}
					
					
					//����Ƿ��յ��ӳ�ȫ�Զ���Ӧ���ź�
					if ((Zigb_Rx_Buf[2] == 0x99) && (Zigb_Rx_Buf[3] == 0x03))//0x55,0x02,0x99,0x03,0x00,0x00,0x9C,0xBB
					{
						Stop_Flag = 0x0D;
						//zhuche_Ask_congche();/
					}	
					//�ӳ�������������բ���ӳ���������0x55,0x02,0x99,0x04,0x00,0x00,0x9D,0xBB������򿪵�բ
					if ((Zigb_Rx_Buf[2] == 0x99) && (Zigb_Rx_Buf[3] == 0x04))
					{
//						daozha_open();
//						delay_ms(100);
//						daozha_open();
//						delay_ms(100);
//						daoza_open_ack();//0x55,0x02,0x99,0x02,0x00,0x00,0x9B,0xbb
//						delay_ms(100);
					}
					
					//�ӳ��Ѿ�ͨ����բ,�ӳ�����������0x55,0x02,0x99,0x05,0x00,0x00,0x9E,0xBB��Stop_Flag = 0x0F
					if ((Zigb_Rx_Buf[2] == 0x99) && (Zigb_Rx_Buf[3] == 0x05))
					{
						Stop_Flag = 0x0F;//��բ������ɱ�־λ
					}				

					memcpy(Follower_Tab,Zigb_Rx_Buf,50);
					AGV_data_Falg = SET;
				}
			
			else if(Zigb_Rx_Buf[1] == 0x03) 				// ��բ����բ�ش�������������
			{
			   if(Zigb_Rx_Buf[2]==0x01)							//��բ�ش�����������ָ��
			    {				
						Stop_Flag=Zigb_Rx_Buf[4];	  			 		//Zigb_Rx_Buf[4]�ǵ�բ�ش���������״̬��ָ������0x05����բ���ѿ�����Ȼ�����б�־λ=0x05Ҳ��բ���Ѿ�����														
			    }		
			} 
			else if(Zigb_Rx_Buf[1]==0x0c) 		// ETC��ETC�ش�������������
			{
				if(Zigb_Rx_Buf[2]==0x01)
			    {
						if(Zigb_Rx_Buf[3]==0x01)
						{
							Stop_Flag=Zigb_Rx_Buf[4] ;	  	//Zigb_Rx_Buf[4]��ETC�ش���������״̬��ָ������0x06����բ���ѿ�����Ȼ�����б�־λ=0x06Ҳ��ETC�Ѿ�����		 											
						}							
			    }		
			}
			else if((Zigb_Rx_Buf[1]==0x0D)|| (Zigb_Rx_Buf[1]==0x05)) //���峵���־��AB�����峵���־��ش�¥�������������
			{
				if(Zigb_Rx_Buf[2]==0x03)
				{
				   if(Zigb_Rx_Buf[3] == 0x01)														//��ȡ���峵�⵱ǰ����
				  {
					 switch(Zigb_Rx_Buf[4])							
					 {
						 case 1:
							 Stop_Flag = 0x09;						//��һ��
							 break;
						 case 2:
							 Stop_Flag = 0x0A;						//�ڶ���
							 break;
						 case 3:
							 Stop_Flag = 0x0B;						//������
							 break;
						 case 4:
							 Stop_Flag = 0x0C;						//���Ĳ�
							 break;
					  }
				  }
				}
			}
			else if((Zigb_Rx_Buf[1]==0x0D)|| (Zigb_Rx_Buf[1]==0x05)) //���峵���־��AB,��ȡ���峵�⵱ǰ����״̬
			{
					if(Zigb_Rx_Buf[3] == 0x02)  
					{
						  if((Zigb_Rx_Buf[4] == 0x01) && (Zigb_Rx_Buf[5] == 0x01))
					  {
						  Stop_Flag = 0x11;							//ǰ����ⴥ���������ⴥ��
					  } 
					  else if((Zigb_Rx_Buf[4] == 0x02) && (Zigb_Rx_Buf[5] == 0x02))
					  {
						  Stop_Flag = 0x22;							//ǰ�����δ������������δ����
					  }
					  else if((Zigb_Rx_Buf[4] == 0x01) && (Zigb_Rx_Buf[5] == 0x02))
					  {
						  Stop_Flag = 0x12;							//ǰ����ⴥ����������δ����
					  }
					  else if((Zigb_Rx_Buf[4] == 0x02) && (Zigb_Rx_Buf[5] == 0x01))
					  {
						  Stop_Flag = 0x21;							//ǰ�����δ�����������ⴥ��
					  }
					}
			}
			else if(Zigb_Rx_Buf[1]==0x06)	 //�����������أ����������ش�������������
			{
				if(Zigb_Rx_Buf[2]==0x01)
				{
					Stop_Flag=Zigb_Rx_Buf[3] ;	  // ��������״̬���أ������0x4E����æµ�У������0x4F���������
				}
				if(Zigb_Rx_Buf[2]==0x04)
				{
					Stop_Flag=0xEF;	  // ��������״̬��ѯ�������¶ȷ���
				}
			}
			else if((Zigb_Rx_Buf[1]==0x10) && (Zigb_Rx_Buf[2]==0x01)) //������α�־��
			{
				if(Zigb_Rx_Buf[3]==0x01)
				{
					Stop_Flag = Zigb_Rx_Buf[4];
				}
			}
			Zigbee_Rx_flag = 0;
		}	
	}
}


/** ��δʹ��
�������ܣ�����ѭ���ϴ�����ʱ��
��    ������
�� �� ֵ����
*/
void Canuser_upTrackTime(void)
{
	static uint8_t run_mode = 0;
	
	if(gt_get() < 1000)
		run_mode = 0;
	else if(run_mode == 0)
	{
		run_mode = 1;
		Host_Set_UpTrack(50);
	}
}

#define UART_or_CAN    0   //  0---UART  1---CAN

/**
�������ܣ�CAN��ѯ�����ͽ��ռ��
��    ������
�� �� ֵ����
*/
void Canuser_main(void)
{
	CanP_Host_Main();
	//CanP_CanTx_Check();				//CAN���߷������ݼ��
	CanP_CanTx_Check_fIrq();
}



//end file

