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
uint8_t a[]={"A1B2C3\n"};//车牌号
int o =0;
uint8_t Infrared[6] = {0x12,0x1e,0x3c,0xd2,0x13,0x18};			//红外数据存放数组,测试数组
uint8_t baojing[6]={0x03,0x05,0x14,0x45,0xDE,0x92};//红外报警默认开启码
uint16_t Check_Agv_daoza = 0;
uint8_t daozha_ack_Count = 0;
uint8_t hh1[8]={0x55,0x02,0x21,0x00,0x00,0x00,0x21,0xbb};//hh1+hh2是从车超声波测距离完加上红外发射到立体显示上面
uint8_t hh2[8]={0x55,0x02,0x12,0x00,0x00,0x00,0x12,0xbb};
uint8_t hh3[8]={0x55,0x02,0x63,0x00,0x00,0x00,0x63,0xbb};//hh3是从车智能路灯三档

uint8_t infrare_com12[6]={0xFF,0x11,0x00,0x00,0x00,0x00};//这个是主车发送显示距离到立体显示标志物上面

int to=0;

int qa=0;
int q=0;



uint32_t gradetime=0;//车库周期检测
uint32_t daozatime = 0;//道闸周期检测
uint8_t daozatuichu=0;//道闸退出机制
uint32_t ETCtime=0;//ETC周期检测
uint8_t gradetuic=0;//车库长时间没检测到则强制退出
/**
函数功能：硬件初始化
参    数：无
返 回 值：无
*/
void Hardware_Init()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);			//中断分组
	delay_init(168);																		//滴答定时器初始化
	Tba_Init();																					//任务板初始化
	Infrared_Init();																		//红外初始化
	Cba_Init();																					//核心板初始化
	Ultrasonic_Init();																	//超声波初始化
	Hard_Can_Init();																		//CAN总线初始化
	BH1750_Configure();																	//BH1750初始化
	BKRC_Voice_Init();																  //小创语音模块初始化
	Electricity_Init();																	//电量检测初始化
	UartA72_Init();																			//A72初始化
	Can_check_Init(7,83);																//CAN总线定时器初始化
	roadway_check_TimInit(499,167);										  //路况检测初始化
	Timer_Init(167,999);																//串行数据通讯时间帧初始化
	Readcard_daivce_Init();                         	  // RFID初始化
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
	
	
	if(S3 == 0)  //把按键1或者2的值d赋值给e，然后e就可以执行按键的内容，按下一次执行一次
	{
				delay_ms(20);
        while(S3==0);
				delay_ms(20);
	
	
				Light_match(4);

//				Tracking_Forward(70,95);
//				Forward(70,95,350);  //十字路口前进一点

//				turn_ppmd(90,90,820);
//				turn_llmd(90,90,1650);
//				delay_ms(20);
//				Back_1(85,70,200);
//				delay_ms(20);
//				Tracking_Forward(70,95);
//				Forward(70,95,1300);  //十字路口前进一点
//				Tracking_Forward(70,95);
//				Forward(70,95,350);  //十字路口前进一点
				
				
//				gradetime = gt_get()+100;
//				while (1)		//判断车有没有到第一层
//				{
//					if (Stop_Flag == 0x09)
//					{
//						Forward(35,75,1200);  //十字路口前进一点，出车库
//						Stop_Flag = 0;
//						break;
//					}
//					else
//					{
//						if(gt_get_sub(gradetime) == 0)//100ms检测一次
//						{
//							gradetime = gt_get()+100;
//							litiGarageF1('A');//让他下降到一楼
//							Request_Location('A');
//							gradetuic++;
//							if(gradetuic ==210)//21s后无反应强制出库，车库肯定会下降，但是有时候车库没有反应，车不会出库
//							{
//								gradetuic =0;
//								Forward(55,85,1200);  //十字路口前进一点，出车库
//								break;
//							}
//						}
//						Can_ZigBeeRx_Check();
//					}
//				}


				


//				Forward(45,100,310);  //十字路口前进一点
//				Turn_Left(85,85);//左转
//				Tracking_Forward(45,100);//70速度循迹前进
//				Forward(45,100,220);  //十字路口前进一点
//				Turn_Right(85,85);
//				Tracking_Forward(85,85);//70速度循迹前进
//				Forward(45,100,220);  //十字路口前进一点
		

//				Tracking_Forward(85,85);//70速度循迹前进
//				Forward(45,100,325);  //十字路口前进一点
//				Turn_Right_Angle90(80+u);//向左90度，这里的d可以调值，通过按键1和2调
//				delay_ms(500);
//				delay_ms(500);
//				delay_ms(500);
//				delay_ms(500);
//				Turn_Left_Angle(152+d);//向左180度，这里的d可以调值，通过按键1和2调
//				Forward(45,100,1520);//冲特殊标志物
//				Tracking_Forward(85,85);//70速度循迹前进
//				Forward(45,85,325);  //十字路口前进一点


//				yunshuche_start();//从车第一次启动
//				
//				while(1)//从车已经完成全自动任务
//				{
//					if(uustop==6)
//					{
//						uustop = 0;
//						break;
//					}
//					Can_ZigBeeRx_Check();
//				}
//				Tracking_Forward(85,85);//70速度循迹前进
//				Forward(55,100,250);  //十字路口前进一点

		//Turn_Left(100,100);
			
//			Tracking_Forward(85,85);//70速度循迹前进
//			Forward(55,85,280);  //十字路口前进一点	


//			Forward(45,100,310);  //十字路口前进一点	
//			turn_llmd(100,100,830);//旋转90度
	}
	
	if(S4 == 0)//无功能
    {
				delay_ms(20);
				while(S4 == 0);
				delay_ms(20);
				daozha_open();
				TFT_Picture_Down('A');
				TFT_Picture_Down('B');
				TFT_Picture_Down('C');
			
//				Send_ZigbeeData_To_Fifo(SMG_JSK,8);//打开LED计时
//				delay_ms(50);
//				Send_ZigbeeData_To_Fifo(SMG_JSK,8);
//				delay_ms(50);
//				Send_ZigbeeData_To_Fifo(SMG_JSK,8);
//				delay_ms(50);
//				
//				gradetime = gt_get()+100;
//				while (1)		//判断车有没有到第一层
//				{
//					if (Stop_Flag == 0x09)
//					{
//						Forward(55,85,1200);  //十字路口前进一点，出车库
//						Stop_Flag = 0;
//						break;
//					}
//					else
//					{
//						if(gt_get_sub(gradetime) == 0)//100ms检测一次
//						{
//							gradetime = gt_get()+100;
//							litiGarageF1('A');//让他下降到一楼
//							Request_Location('A');
//							gradetuic++;
//							if(gradetuic ==210)//21s后无反应强制出库，车库肯定会下降，但是有时候车库没有反应，车不会出库
//							{
//								gradetuic =0;
//								Forward(55,85,1200);  //十字路口前进一点，出车库
//								break;
//							}
//						}
//						Can_ZigBeeRx_Check();
//					}
//				}
//				
//				Turn_Right(85,85);//右转
//		
//				
//				
//				
//				ETCtime = gt_get()+100;
//				while(1)//等待ETC打开
//				{
//					if(Stop_Flag ==0x06)
//					{
//						Tracking_Forward(85,85);//70速度循迹前进
//						Forward(45,100,250);  //十字路口前进一点
//						break;
//					}
//					else
//					{
//						if(gt_get_sub(ETCtime) == 0)//100ms检测一次
//						{
//							ETCtime = gt_get()+100;
//							Forward(45,100,50); 
//							Back_1(45,100,50);  //让他摇晃一下，可以使ETC更好检测到
//						}
//						Can_ZigBeeRx_Check();
//					}
//				}
//				
//				
//				
//				Voice_BroadCast_SuiJi();//让语言播报标志物随机播报语音
//				delay_ms(500);
//				
//				
//				Turn_Left(85,85);//左转
//				
//				Car_Time_Track(85,85,700);//循迹6cm到达二维码处
//				Turn_Right_Angle90(80);//右转90
//				
//				
//					LED_Ult_dis();
//				
//				Voice_BroadCast_wendu();
//				delay_ms(500);
//				delay_ms(500);
//				
//				Turn_Left(85,85);//左转到循迹线上
//				
//				
//				Tracking_Forward(85,85);//70速度循迹前进
//				Forward(55,100,250);  //十字路口前进一点
//				
//				Turn_Right_Angle90(79);//对准TFT
//				Send_ZigbeeData_To_Fifo(TFT_PageAUp , 8);
//				delay_ms(100);
//				Send_ZigbeeData_To_Fifo(TFT_PageAUp , 8);
//				delay_ms(100);
//				Send_ZigbeeData_To_Fifo(TFT_PageAUp , 8);
//				delay_ms(500);
//				delay_ms(500);
//				delay_ms(500);
//				delay_ms(500);
//				Turn_Left(85,85);//左转到循迹线上
//				
//				Tracking_Forward(85,85);//70速度循迹前进
//				Forward(45,100,325);  //十字路口前进一点
//				Turn_Right_Angle90(80+u);//向左90度，这里的d可以调值，通过按键1和2调
//				delay_ms(500);
//				delay_ms(500);
//				delay_ms(500);
//				delay_ms(500);
//				Turn_Left_Angle(152+d);//向左180度，这里的d可以调值，通过按键1和2调
//				Forward(45,100,1520);//冲特殊标志物
//				Tracking_Forward(85,85);//70速度循迹前进
//				Forward(45,85,325);  //十字路口前进一点
//				
//				
//				Turn_Left(85,85);
//				
//				daozatime = gt_get()+50;
//				while(1)
//				{
//					
//					if(Stop_Flag == 0x05)//检测道闸是否打开
//					{
//						Tracking_Forward(74,70);//70速度循迹前进
//						Forward(55,100,250);  //十字路口前进一点
//						Stop_Flag = 0;
//						break;
//					}
//					else
//					{
//						if(gt_get_sub(daozatime) == 0)//50ms检测一次
//						{
//							daozatime = gt_get()+50;
//							daozha_open();//打开道闸
//							delay_ms(50);
//							daozha_ack();
//							delay_ms(50);
//							daozatuichu++;//积累时间，如果道闸长时间没反应则冲出去
//							if(daozatuichu ==80)
//							{
//								daozatuichu =0;
//								Tracking_Forward(74,70);//70速度循迹前进
//								Forward(55,100,250);  //十字路口前进一点
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
//					while (1)		//判断车库有没有到第一层,因为是用主车控制车库，原本A车库已经下降就不需要这里，有可能让从车去B车库
//					{
//						if (Stop_Flag == 0x09)//一楼就结束循环
//						{
//							Stop_Flag = 0;
//							break;
//						}
//					
//						else
//						{
//							if(gt_get_sub(gradetime) == 0)//50ms检测一次
//							{
//								gradetime = gt_get()+100;
//								litiGarageF1('A');//让他下降到一楼
//								Request_Location('A');
//							}
//							Can_ZigBeeRx_Check();
//						}
//					}	
//				
//				yunshuche_start();//从车第一次启动
//				
//				while(1)//从车已经完成全自动任务
//				{
//					if(uustop==6)
//					{
//						uustop = 0;
//						break;
//					}
//					Can_ZigBeeRx_Check();
//				}
//				Tracking_Forward(85,85);//70速度循迹前进
//				Forward(55,100,250);  //十字路口前进一点
//				
//				Turn_Right(85,85);
//				
//				Tracking_Forward(85,85);//70速度循迹前进
//				Forward(55,100,250);   //十字路口前进一点,这里到报警台附近
//				
//				Turn_Left(85,85);//报警台
//				Infrared_Send(baojing,6);  
//				delay_ms(200);
//				
//				Turn_Right(85,85);
//				Turn_Right(85,85);
//				
//				Tracking_Forward(85,85);//70速度循迹前进
//				Forward(55,100,250);  //十字路口前进一点,这里到LCD附近
//				
//				Tracking_Forward(85,85);//70速度循迹前进
//				Forward(55,100,250);  //十字路口前进一点,这里到路灯附近
//				
//				Turn_Left_Angle(84);//向左90度到路灯面前
//				
//				
//				returnvalue =Light_Inf_Start();
//				mubiaovalue = 4;//智能路灯最终挡位，具体数值可以修改
//				if(returnvalue == 1 || returnvalue == 2 || returnvalue == 3 || returnvalue == 4)//当前智能路灯挡位
//				{
//					if(mubiaovalue==1)//智能路灯最终挡位为1则不需要修改
//					{
//						
//					}
//					else if(mubiaovalue==2)//智能路灯最终挡位为2,则加1档
//					{
//						Street_light_one();
//						delay_ms(500);
//					}
//					else if(mubiaovalue==3)//智能路灯最终挡位为3,则加2档
//					{
//						Street_light_two();
//						delay_ms(500);
//					}
//					else if(mubiaovalue==4)//智能路灯最终挡位为4,则加3档
//					{
//						Street_light_three();
//						delay_ms(500);
//					}
//				}

//				
//				Turn_Left_Angle(84);//向左90度
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
//				to =(((tempwendu + tempwendu)%4) +1);//题目计算
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
//				while (1)		//判断从车库有没有到那一层，到了那一层才停止下面的LED计时
//				{
//					if (Stop_Flag == 0x0B || Stop_Flag == 0x09 || Stop_Flag == 0x0A || Stop_Flag == 0x0C)//无论上哪层都结束循环
//					{
//						Stop_Flag = 0;
//						break;
//					}
//					else
//					{
//						if(gt_get_sub(gradetime) == 0)//50ms检测一次
//						{
//							gradetime = gt_get()+100;
//							Request_Location('A');
//						}
//						Can_ZigBeeRx_Check();
//					}
//				}
//				
//				Send_ZigbeeData_To_Fifo(SMG_JSG,8);//关闭LED计时
//				delay_ms(10);
//				Send_ZigbeeData_To_Fifo(SMG_JSG,8);
//				delay_ms(10);
//				Send_ZigbeeData_To_Fifo(SMG_JSG,8);//关闭LED计时
//				delay_ms(10);
					
		}
}

static uint32_t LED_twinkle_times;          // LED闪烁周期
static uint32_t Power_check_times;			  //电量检测周期
static uint32_t WIFI_Upload_data_times; 	//通过Wifi上传数据周期
static uint32_t RFID_Init_Check_times;  	//RFID检测周期

int main(void)
{	
	
	uint16_t Light_Value = 0;								//光强度值	
	uint16_t CodedDisk_Value = 0;						//码盘
	uint16_t Nav_Value = 0;									//角度	
	
	Hardware_Init();						    				//硬件初始化 
	LED_twinkle_times = gt_get() + 50;	     
	Power_check_times =  gt_get() + 200;//一开始最少是200ms+gt_get()的时间，所以最低是200ms
	WIFI_Upload_data_times = gt_get() + 200;
	RFID_Init_Check_times = gt_get()+200;
	
	Principal_Tab[0] = 0x55;
	Principal_Tab[1] = 0xAA;	
	
	Follower_Tab[0] = 0x55;
	Follower_Tab[1] = 0x02;
    
  Send_UpMotor(0, 0);
	while(1)
	{				
		KEY_Check();										//按键检测,这里一定要长按一会，不要快速点，否则没反应
		Can_WifiRx_Check();            //wifi交互数据处理—WiFi接收安卓数据
		Can_ZigBeeRx_Check();          //zigbee交互数据处理—zigbee接收所有标志物数据
		
		if(gt_get_sub(LED_twinkle_times) == 0)          // 运行指示灯
        {
            LED_twinkle_times = gt_get() + 50;          // LED4状态取反
            LED4 = !LED4;
        }
		
		
		if(gt_get_sub(Power_check_times) == 0) 	//目标时间200ms，然后定时器计时到了200ms则进来一次	
		{
			Power_check_times =  gt_get() + 200;			//电池电量检测，进来后让目标时间在+200，等程序出去后，就可以等待下一次的200ms测量
			Power_Check();
		}
#if 1		
		if(gt_get_sub(RFID_Init_Check_times) == 0)      // RFID初始化检测,初始化成功的话LED就一直闪烁，失败的话蜂鸣器就开始叫
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
		if(gt_get_sub(WIFI_Upload_data_times) == 0)    //数据上传 
		{
			WIFI_Upload_data_times =  gt_get() + 200;//200ms检测一次
			//Host_AGV_Return_Flag 初始化 为RESET
			// AGV_data_Falg 初始化为 RESET
			if(Host_AGV_Return_Flag == RESET)            //主车数据上传
			{
				Principal_Tab[2] = Stop_Flag;						  //运行状态
				Principal_Tab[3] = Get_tba_phsis_value();	//光敏状态值返回
				
				Ultrasonic_Ranging();										  //超声波数据采集						
				Principal_Tab[4] = dis % 256;        			//超声波数据低八位
				Principal_Tab[5] = dis / 256;							//超声波数据高八位
				
				Light_Value = Get_Bh_Value();						  //光强度传感器数据采集	
				Principal_Tab[6] = Light_Value % 256;	   	//光照数据低八位
				Principal_Tab[7] = Light_Value / 256;			//光照数据高八位
				
				CodedDisk_Value = CanHost_Mp;						  //码盘
				Principal_Tab[8] = CodedDisk_Value % 256;	    	
				Principal_Tab[9] = CodedDisk_Value / 256;
				
				Nav_Value = CanHost_Navig;							  //角度
				Principal_Tab[10] = Nav_Value % 256;	    	
				Principal_Tab[11] = Nav_Value / 256;
				
				Send_WifiData_To_Fifo(Principal_Tab, 12); //通过wifi上传小车数据
				UartA72_TxClear();
				UartA72_TxAddStr(Principal_Tab, 12);      //通过串口上传主车数据
				UartA72_TxStart();
			}
			else if((Host_AGV_Return_Flag == SET) && (AGV_data_Falg == SET))
				{	
					UartA72_TxClear();
					UartA72_TxAddStr(Follower_Tab,50);     //通过串口上传主车数据
					UartA72_TxStart();
					Send_WifiData_To_Fifo(Follower_Tab,50); //通过wifi上传主车数据
					AGV_data_Falg = 0;
				}
		}
		#endif
	}		
}


//ETCtime = gt_get()+100;
//				while(1)//等待ETC打开
//				{
//					if(Stop_Flag ==0x06)
//					{
//						Tracking_Forward(85,85);//70速度循迹前进
//						Forward(45,100,250);  //十字路口前进一点
//						break;
//					}
//					else
//					{
//						if(gt_get_sub(ETCtime) == 0)//100ms检测一次
//						{
//							ETCtime = gt_get()+100;
//							Forward(45,100,50); 
//							Back_1(45,100,50);  //让他摇晃一下，可以使ETC更好检测到
//						}
//						Can_ZigBeeRx_Check();
//					}
//				}


/**
函数功能：按键检测
参    数：无
返 回 值：无
功能 ：主车通过按键与标志物通信
注意：标志物通信容易丢包可以多发几次
*/
//void KEY_Check() 
//{
//	if(S1 == 0)  //按下一次,d+1，也就是e+1,按到15变回1
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
//			while (!S1);	//松开按键	
//			
//		}
//	}
//	
//	if(S2 == 0)  //按下一次,d-1，也就是e-1,按到0变回13
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
//	if(S3 == 0)  //把按键1或者2的值d赋值给e，然后e就可以执行按键的内容，按下一次执行一次
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
//	if(S4 == 0)//无功能
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
//		case 1:  //打开计时，关闭计时
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
//		case 2:  //测距并在LED显示距离
//		{
//			e = 0; 
//			LED_Ult_dis();
//       break;
//		}
//      case 3:   //文字识别
//			{
//				e = 0;
//			    Test();//stn7318,使用之后车子记得复位，只能识别一次，功能是让语音播报标志物随机播报，并重复读出
//			}
//       break;
//      case 4:  //交通灯识别，按键确认法^_^ ^_^ ^_^  1红2绿3黄，4返回(注意一定需要返回)，主车开机2分钟后在进行交通灯识别，然后点复位
//			{
//				e = 0; 
//         Send_ZigbeeData_To_Fifo(TrafficA_Open , 8);//交通灯A进入识别
//         delay_ms(500);
//         delay_ms(500);
//         while(1)
//         {
//             delay_ms(500);
//             delay_ms(500);
//             if(S1 == 0) //进入识别后，再一次按下按键1，2，3来告诉交通灯是什么颜色的灯,
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
//              else if(S4 == 0)//然后上面三个按键按下其中之一一个之后，交通灯会返回状态，再按下按键4结束识别
//             {
//                 delay_ms(10);
//                 if(S4 == 0)
//                 while(!S4);
//                 break;
//              }
//           }
//       break;
//			}
//      case 5:   //TFTA，向上翻
//			{
//				e = 0;
//         Send_ZigbeeData_To_Fifo(TFT_PageAUp , 8);
//         for(int i=0;i<5;i++)
//         {     
//            delay_ms(500);
//         }    
//       break;
//			}
//      case 6:  //立体显示标志物显示距离为3
//			{
//				e = 0;
//         Rotate_Dis_Inf(3);
//         delay_ms(500);
//       break;
//			}
//      case 7:   //智能路灯调节3档
//			{
//				 e = 0; 
//         Street_light_three();
//         delay_ms(100);
//       break;
//			}
//      case 8:  //二维码
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
//      case 9:  //红外发射
//			{
//				e = 0;
//         delay_ms(200);
//         Infrared_Send(Infrared,6);  
//         delay_ms(200);
//       break;
//			}
//      case 10:    //车牌及颜色识别
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
//      case 11:   //打开道闸
//			{
//				e = 0;
//         daozha_open();
//         delay_ms(500);
//         daozha_open();
//         delay_ms(200);
//       break;
//			
//			}
//      case 12:    //立体车库3层
//			{
//				e = 0; 
//         litiGarageF3('A');  
//         delay_ms(200);
//       break;
//			}
//      case 13:  //无线充电开启
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




