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
uint8_t Start_First = 1;	//A72一键启动标志位
uint8_t Wifi_Rx_Buf[WIFI_MAX_NUM];
uint8_t Zigb_Rx_Buf[ZIGB_RX_MAX];
uint8_t Wifi_Rx_num ;   //wifi接收数据的次数
uint8_t Wifi_Rx_flag ;  //接收完成标志位
uint8_t Zigbee_Rx_num ;   //zigbee接收数据的次数
uint8_t Zigbee_Rx_flag ;  //接收完成标志位

uint8_t uustop =0;

uint8_t traficred1[8]={0x55,0x02,0X02,0X01,0x00,0x00,0x03,0xBB};

uint8_t Host_AGV_Return_Flag = RESET;
uint8_t AGV_data_Falg = RESET;

uint32_t canu_wifi_rxtime = 0;
uint32_t canu_zibe_rxtime = 0;


void Can_WifiRx_Save(uint8_t res) //从can总线接收的wifi数据保存
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

void Normal_data(void)	      // 正常接收8字节控制指令—wifi
{

	if(Wifi_Rx_Buf[7] == 0xBB)	  // 判断包尾
	{					
		//主指令与三位副指令求和校验
		//注意：在求和溢出时应该对和做256取余。
		//sum是校验位
		uint8_t sum = (Wifi_Rx_Buf[2]+Wifi_Rx_Buf[3]+Wifi_Rx_Buf[4]+Wifi_Rx_Buf[5])%256;
		
		if(sum == Wifi_Rx_Buf[6])//Wifi_Rx_Buf[6]是平板电脑向主车或者从车发送的第七位校验位，校验位如果相等，则置1，代表发送给主车或者从车的指令正确
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

void Abnormal_data(void)	  //数据异常处理
{
	u8 i,j;
	u8 sum=0;
	
	if(Wifi_Rx_num <9)			// 异常数据字节数小于8字节不做处理
	{
	   Rx_Flag =0;
	}
	else
	{
		for(i=0; i<=(Wifi_Rx_num -8); i++)  
		{
			if(Wifi_Rx_Buf[i]==0x55)	   // 寻找包头
			{
			   if(Wifi_Rx_Buf[i+7]==0xbb)	// 判断包尾
			   {
					 sum = (Wifi_Rx_Buf[i+2]+Wifi_Rx_Buf[i+3]+Wifi_Rx_Buf[i+4]+Wifi_Rx_Buf[i+5])%256;

					 if(sum == Wifi_Rx_Buf[i+6])	 // 判断求和
					 {
						 for(j=0;j<8;j++)
						 {
								Wifi_Rx_Buf[j]=Wifi_Rx_Buf[j+i];	 // 数据搬移
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


uint8_t Infrared_Tab[6];			//红外数据存放数组



void Can_WifiRx_Check(void)  //wifi交互数据处理
{
	if(Wifi_Rx_flag)						//对协议进行判断,Wifi_Rx_flag是接收完成标志位，
	{	
		
		 if(gt_get_sub(canu_wifi_rxtime) == 0)
		 {
			 if(Wifi_Rx_Buf[0]==0xFD)  
			 {			
				 Send_ZigbeeData_To_Fifo( Wifi_Rx_Buf , (Wifi_Rx_num +1)); //将数据发送至ZigBee模块
			 }
			 else if(Wifi_Rx_Buf[0]==0x55)  //平板电脑向主车或者从车发送的数据包头
			 { 
				 Normal_data();  // 正常接收8字节控制指令(平板电脑向主从车发送的8字节指令)，并判断是否指令一致
			 }
//			 else
//			 {
//				 Abnormal_data();    //数据异常处理(平板电脑向主车从发送指令)
//			 } 	
			 Wifi_Rx_flag = 0;    //接收完成标志位清0
		}
	}
	
	if(Rx_Flag ==1)	//Rx_Flag=1代表平板电脑发送给主车的指令正确则进入程序
	{    

		if(Wifi_Rx_Buf[1] == 0xAA) 	  //包头有2位，第二位包头如果是0xAA就代表是平板电脑发给的是主车 
		{	
			switch(Wifi_Rx_Buf[2]) 			//判断主指令
			{
				case 0x01:              	//主车停止
					Send_UpMotor(0, 0);			//速度为0
					Roadway_Flag_clean();	  //清除标志位状态	
					break;
				case 0x02:                //主车前进
					Roadway_mp_syn();				//码盘同步，码盘可以算距离
					Track_Forward_Flag = 0;	//前进循迹标志位
					Track_Back_Flag = 0;		//后退循迹标志位
					wheel_Nav_Flag = 0;			//码盘旋转标志位
					wheel_L_Flag = 0;				//左转标志位
					wheel_R_Flag = 0;				//右转标志位
					Back_Flag = 0;					//后退标志位
					Stop_Flag = 0;					//运行状态标志位，判断平板电脑向主车发送的前进指令多少cm是否完成，等于0是循迹状态，还在循迹中
					Go_Flag = 1;						//前进标志位
					temp_MP = Wifi_Rx_Buf[6];		//平板电脑向主车发送的副指令第六位—码盘高八位
					temp_MP <<=  8;								
					temp_MP |= Wifi_Rx_Buf[5];	//平板电脑向主车发送的副指令第五位—码盘低八位，temp_MP是码盘值
					LCar_Spend = Wifi_Rx_Buf[3];	//平板电脑向主车发送的副指令第四位—速度值
					RCar_Spend = Wifi_Rx_Buf[4];	//平板电脑向主车发送的副指令第四位—速度值
					Control(LCar_Spend ,RCar_Spend);//电机速度控制函数(左测，右测)，是平板电脑向wifi模块发送的速度然后在让核心板发给电机板
					break;
				case 0x03:              //后退
					Roadway_mp_syn();			//码盘同步，码盘可以算距离
					Track_Forward_Flag = 0;
					wheel_Nav_Flag = 0;
					wheel_L_Flag = 0;				
					wheel_R_Flag = 0;				
					Stop_Flag = 0;					//循迹中
					Back_Flag = 1;					//后退标志位
					Go_Flag = 0;
					Track_Back_Flag = 0;
					temp_MP = Wifi_Rx_Buf[5];
					temp_MP <<=  8;
					temp_MP |= Wifi_Rx_Buf[4];
					LCar_Spend = Wifi_Rx_Buf[3]; //平板电脑向主车发送的副指令第四位—速度值
					Control(-LCar_Spend ,-LCar_Spend);		//这里写正数			
					break;
				case 0x04:              //左转，这里不需要码盘同步，因为转弯90度，相当于没有距离
					Track_Forward_Flag = 0;
					Track_Back_Flag = 0;
					wheel_Nav_Flag = 0;
					wheel_L_Flag = 1;								//左转标志位
					wheel_R_Flag = 0;
					Stop_Flag = 0;									//循迹中
					Back_Flag = 0;
					Go_Flag = 0;
					LCar_Spend = Wifi_Rx_Buf[3];			//平板电脑向主车发送的副指令第四位—速度值	
					Control(-LCar_Spend ,LCar_Spend);//左转的话，左轮后退，右轮前进
					break;
				case 0x05:              //右转，这里不需要码盘同步，因为转弯90度，相当于没有距离
					Track_Forward_Flag = 0;
					Track_Back_Flag = 0;
					wheel_Nav_Flag = 0;
					wheel_L_Flag = 0;
					wheel_R_Flag = 1;							//右转标志位
					Stop_Flag = 0;								//循迹中
					Back_Flag = 0;
					Go_Flag = 0;
					LCar_Spend = Wifi_Rx_Buf[3];		//平板电脑向主车发送的副指令第四位—速度值
					Control(LCar_Spend,-LCar_Spend);//右转的话，左轮前进，右轮后退
					break;
				case 0x06:              //循迹
					Track_Forward_Flag = 1;								//前进循迹标志位
					Track_Back_Flag = 0;
					wheel_Nav_Flag = 0;
					wheel_L_Flag = 0;
					wheel_R_Flag = 0;
					Stop_Flag = 0;             //循迹中
					Back_Flag = 0;
					Go_Flag = 0;
					LCar_Spend = Wifi_Rx_Buf[3]; //平板电脑向主车发送的主指令第四位—速度
					break;
				case 0x07:				//码盘清零					
					break;
				case 0x08:									//左转弯--角度											
					Roadway_nav_syn();				//角度同步
					Roadway_mp_syn();					//码盘同步
					Track_Forward_Flag = 0;
					Track_Back_Flag = 0;
					wheel_Nav_Flag = 1;				//码盘旋转标志位
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
				case 0x09:						//右转弯--角度			
					Roadway_nav_syn();	//角度同步
					Roadway_mp_syn();		//码盘同步
					Track_Forward_Flag = 0;
					Track_Back_Flag = 0;
					wheel_Nav_Flag = 1;		//码盘旋转标志位
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
				case 0x10:												//红外前三位数据
					Infrared_Tab[0]=Wifi_Rx_Buf[3];
					Infrared_Tab[1]=Wifi_Rx_Buf[4];
					Infrared_Tab[2]=Wifi_Rx_Buf[5];
					break;
				case 0x11:												//红外后三位数据
					Infrared_Tab[3]=Wifi_Rx_Buf[3];	//数据第四位
					Infrared_Tab[4]=Wifi_Rx_Buf[4];	//低位校验码
					Infrared_Tab[5]=Wifi_Rx_Buf[5];	//高位校验码
					break;
				case 0x12:												//通知小车单片机发送红外线
					Infrared_Send(Infrared_Tab,6);					
					break;									
				case 0x20:												//转向灯控制
					L_LED = ~Wifi_Rx_Buf[3];
					R_LED = ~Wifi_Rx_Buf[4];
					break;					
				case 0x30:
					Beep =  ~Wifi_Rx_Buf[3];				//蜂鸣器	
					break;
				case 0x40:												//暂未使用					
					
					break;
					case 0x50:  										//红外发射控制相片上翻 
					Infrared_Send(H_S,4);
					break;
				case 0x51:    										//红外发射控制相片下翻 
					Infrared_Send(H_X,4);
					break;
				case 0x61:    										//红外发射控制光源强度档位加1 
					Infrared_Send(H_1,4);				
					break;
				case 0x62:   	 										//红外发射控制光源强度档位加2 
					Infrared_Send(H_2,4);
					break;
				case 0x63:    										//红外发射控制光源强度档位加3 
					Infrared_Send(H_3,4);
					break;
			   case 0x80:												//运动标志物数据返回允许位
					Host_AGV_Return_Flag = Wifi_Rx_Buf[3];   //SET 允许 / RESET 禁止
					break;
				 case 0x9D:												//A72发送给主车协议
				 {
						if (Wifi_Rx_Buf[2] == 0x9D && Wifi_Rx_Buf[3] == 0x00)		//接收A72一键启动
						{
							 Stop_Flag = 0x0E;
						}
						
						//A72回传交通灯识别结果
						if ((Wifi_Rx_Buf[3] == 0x01) || (Wifi_Rx_Buf[3] == 0x02) || (Wifi_Rx_Buf[3] == 0x03))
						{
							Traffic_Flag  = Wifi_Rx_Buf[3];
							zhuche_Ack();
						}

						//A72回传二维码识别结果
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
						
						// A72启动指令  只启动一次
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
				if(Wifi_Rx_Buf[2] == 0x10) 			//判断主指令
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
				if(Wifi_Rx_Buf[2] == 0x01)//打开
				{
					daozha_open();
				}
				else if(Wifi_Rx_Buf[2] == 0x02)//关闭
				{
					daozha_close();
				}
			}
			
		}
		
		else  	//平板电脑发给主车的wifi模块要干的事情，主车执行不了上面的程序(包头第二位是0x20就是从车)，就把wifi模块接收到要干的事情发给zigbee模块，主车zigbee在发给其他zignbee标志物或者从车
		{				//这里大致意思就是本来平板电脑发给从车执行的东西，结果发到主车上来，然后主车执行不了，就通过zigbee发到从车的zigbee，让从车执行操作
			Send_ZigbeeData_To_Fifo(Wifi_Rx_Buf,8); //将数据发送至ZigBee模块
		}
		Rx_Flag = 0;
	}
}




/**
函数功能：保存ZigBee数据
参    数: 无
返 回 值：无
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
函数功能：ZigBee数据监测(zigbee模块收到标志物或者从车的回传数据)
参    数：无
返 回 值：无
*/
void Can_ZigBeeRx_Check(void)
{
	if(Zigbee_Rx_flag)
	{
		if(gt_get_sub(canu_zibe_rxtime) == 0)//canu_zibe_rxtime初始值为0，直接进入程序
		{
			if((Zigb_Rx_Buf[0] == 0x55)&&(Zigb_Rx_Buf[1] == 0x02))		//从车发给主车的数据
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
					if(Zigb_Rx_Buf[2] ==0x01)//循迹完成
					{
						uustop=1;
					}
					if(Zigb_Rx_Buf[2] ==0x03)//前进或者后退完成
					{
						uustop=2;
					}
					if(Zigb_Rx_Buf[2] ==0x02)//转弯完成
					{
						uustop=3;
					}
					if(Zigb_Rx_Buf[2] ==0xCC)//右转45完成
					{
						uustop=4;
					}
					if(Zigb_Rx_Buf[2] ==0xAB)//左转45完成
					{
						uustop=5;
					}
					//0x55,0x02,0xCC,0x05,0x00,0x00,0xD1,0xbb
					if(Zigb_Rx_Buf[2] ==0xCC && Zigb_Rx_Buf[3] ==0x05)//从车已经跑完全自动任务
					{
						uustop=6;
					}
					
					
					//检查是否收到从车全自动的应答信号
					if ((Zigb_Rx_Buf[2] == 0x99) && (Zigb_Rx_Buf[3] == 0x03))//0x55,0x02,0x99,0x03,0x00,0x00,0x9C,0xBB
					{
						Stop_Flag = 0x0D;
						//zhuche_Ask_congche();/
					}	
					//从车请求主车开道闸，从车给主车发0x55,0x02,0x99,0x04,0x00,0x00,0x9D,0xBB，请求打开道闸
					if ((Zigb_Rx_Buf[2] == 0x99) && (Zigb_Rx_Buf[3] == 0x04))
					{
//						daozha_open();
//						delay_ms(100);
//						daozha_open();
//						delay_ms(100);
//						daoza_open_ack();//0x55,0x02,0x99,0x02,0x00,0x00,0x9B,0xbb
//						delay_ms(100);
					}
					
					//从车已经通过道闸,从车给主车发送0x55,0x02,0x99,0x05,0x00,0x00,0x9E,0xBB，Stop_Flag = 0x0F
					if ((Zigb_Rx_Buf[2] == 0x99) && (Zigb_Rx_Buf[3] == 0x05))
					{
						Stop_Flag = 0x0F;//道闸开启完成标志位
					}				

					memcpy(Follower_Tab,Zigb_Rx_Buf,50);
					AGV_data_Falg = SET;
				}
			
			else if(Zigb_Rx_Buf[1] == 0x03) 				// 道闸，道闸回传给主车的数据
			{
			   if(Zigb_Rx_Buf[2]==0x01)							//道闸回传给主车的主指令
			    {				
						Stop_Flag=Zigb_Rx_Buf[4];	  			 		//Zigb_Rx_Buf[4]是道闸回传给主车的状态副指令，如果是0x05就是闸门已开启，然后运行标志位=0x05也是闸门已经开启														
			    }		
			} 
			else if(Zigb_Rx_Buf[1]==0x0c) 		// ETC，ETC回传给主车的数据
			{
				if(Zigb_Rx_Buf[2]==0x01)
			    {
						if(Zigb_Rx_Buf[3]==0x01)
						{
							Stop_Flag=Zigb_Rx_Buf[4] ;	  	//Zigb_Rx_Buf[4]是ETC回传给主车的状态副指令，如果是0x06就是闸门已开启，然后运行标志位=0x06也是ETC已经开启		 											
						}							
			    }		
			}
			else if((Zigb_Rx_Buf[1]==0x0D)|| (Zigb_Rx_Buf[1]==0x05)) //立体车库标志物AB，立体车库标志物回传楼层给主车的数据
			{
				if(Zigb_Rx_Buf[2]==0x03)
				{
				   if(Zigb_Rx_Buf[3] == 0x01)														//获取立体车库当前层数
				  {
					 switch(Zigb_Rx_Buf[4])							
					 {
						 case 1:
							 Stop_Flag = 0x09;						//第一层
							 break;
						 case 2:
							 Stop_Flag = 0x0A;						//第二层
							 break;
						 case 3:
							 Stop_Flag = 0x0B;						//第三层
							 break;
						 case 4:
							 Stop_Flag = 0x0C;						//第四层
							 break;
					  }
				  }
				}
			}
			else if((Zigb_Rx_Buf[1]==0x0D)|| (Zigb_Rx_Buf[1]==0x05)) //立体车库标志物AB,获取立体车库当前红外状态
			{
					if(Zigb_Rx_Buf[3] == 0x02)  
					{
						  if((Zigb_Rx_Buf[4] == 0x01) && (Zigb_Rx_Buf[5] == 0x01))
					  {
						  Stop_Flag = 0x11;							//前侧红外触发、后侧红外触发
					  } 
					  else if((Zigb_Rx_Buf[4] == 0x02) && (Zigb_Rx_Buf[5] == 0x02))
					  {
						  Stop_Flag = 0x22;							//前侧红外未触发、后侧红外未触发
					  }
					  else if((Zigb_Rx_Buf[4] == 0x01) && (Zigb_Rx_Buf[5] == 0x02))
					  {
						  Stop_Flag = 0x12;							//前侧红外触发、后侧红外未触发
					  }
					  else if((Zigb_Rx_Buf[4] == 0x02) && (Zigb_Rx_Buf[5] == 0x01))
					  {
						  Stop_Flag = 0x21;							//前侧红外未触发、后侧红外触发
					  }
					}
			}
			else if(Zigb_Rx_Buf[1]==0x06)	 //语音播报返回，语音播报回传给主车的数据
			{
				if(Zigb_Rx_Buf[2]==0x01)
				{
					Stop_Flag=Zigb_Rx_Buf[3] ;	  // 语音播报状态返回，如果是0x4E，则忙碌中，如果是0x4F，则空闲中
				}
				if(Zigb_Rx_Buf[2]==0x04)
				{
					Stop_Flag=0xEF;	  // 语音播报状态查询天气和温度返回
				}
			}
			else if((Zigb_Rx_Buf[1]==0x10) && (Zigb_Rx_Buf[2]==0x01)) //特殊地形标志物
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


/** 暂未使用
函数功能：设置循迹上传更新时间
参    数：无
返 回 值：无
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
函数功能：CAN查询、发送接收检测
参    数：无
返 回 值：无
*/
void Canuser_main(void)
{
	CanP_Host_Main();
	//CanP_CanTx_Check();				//CAN总线发送数据监测
	CanP_CanTx_Check_fIrq();
}



//end file

