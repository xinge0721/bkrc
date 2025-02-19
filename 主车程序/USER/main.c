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
	}
	
	if(S4 == 0)//无功能
    {
				delay_ms(20);
				while(S4 == 0);
				delay_ms(20);
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
#if 0		
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




