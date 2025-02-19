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
** 功能：     移动机器人前进
** 参数：	  	speed 整型，小车速度；mapan，u16型，码盘值  
** 返回值：   无
** 说明：     主车通过zigbee控制运输车前进
yunshuche_go(90,310);
				while(1)
				{
					Can_ZigBeeRx_Check();
					if(uustop ==2)   //前进完成
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
    Send_zigbee[4]=mapan%256; //低八位
    Send_zigbee[5]=mapan/256; //高八位
    Send_zigbee[6]=(Send_zigbee[2]+Send_zigbee[3]+Send_zigbee[4]+Send_zigbee[5])%256;
    Send_zigbee[7]=0xbb;
    Send_ZigbeeData_To_Fifo(Send_zigbee,8);
    delay_ms(5);
}


/***************************************************************
** 功能：     移动机器人后退
** 参数：	  speed 整型，小车速度；mapan，u16型，码盘值
** 返回值：   无
** 说明：     主车通过zigbee控制运输车后退
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
** 功能：     移动机器人右转45
** 参数：	  	speed 整型，小车速度；mapan，u16型，码盘值  
** 返回值：   无
** 说明：     主车通过zigbee控制运输车前进
yunshuche_turnright();//右转45完成
				while(1)
				{
					Can_ZigBeeRx_Check();
					if(uustop ==4)    //右转45完成
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

//左45度
//yunshuche_turnleft();//左转45完成
//				while(1)
//				{
//					Can_ZigBeeRx_Check();
//					if(uustop ==5)    //左转45完成
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
** 功能：     移动机器人左转
** 参数：	 	  speed	整型，小车速度
** 返回值：   无
** 说明：     主车通过zigbee控制运输车左转
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
** 功能：     移动机器人右转
** 参数：	  	speed	整型，小车速度
** 返回值：   无
** 说明：     主车通过zigbee控制运输车右转
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
** 功能：     移动机器人停止
** 参数：	  	无
** 返回值：   无
** 说明：     主车通过zigbee控制运输车停止

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
** 功能：     移动机器人循迹
** 参数：	  	speed	整型，小车速度
** 返回值：   无
** 说明：     主车通过zigbee控制运输车循迹
yunshuche_xj(70);//循迹
				while(1)
				{
					Can_ZigBeeRx_Check();
					if(uustop ==1)    //循迹完成
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
** 功能：     主车发给从车启动协议
** 参数：	  	无
** 返回值：   无
** 说明：     
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
** 功能：     移动机器人一键启动
** 参数：	  	无
** 返回值：   无
** 说明：     主车通过zigbee控制从车启动 ,第一次启动
整个流程就是主车先发送第一次数据让从车启动全自动函数，如果从车没有接收到全启动协议，主车在4s后会重新发送，然后主车以50ms速度检测，
如果从车接收到了就会返回应答信号给主车，如果从车返回的应答信号主车没有接收到，从车在1s后则重新发送，如果还是没有接收到则再过1s后继续发送一次，从车则跳出循环，
开始执行从车全自动任务，这里一般最少第二次也成功了，主车收到了从车发的应答信号，zigbee检测置Stop_Flag == 0x0D，则跳出循环，则主车发送全自动函数完成
这里需要注意，主车发送全自动函数隔4s最好，让从车那些应答任务先完成，4s后没什么事情了，如果没有启动起来，主车再发全自动函数



//启动全自动函数以后，从车请求主车开道闸
yunshuche_start();//让从车成功启动
			while (Stop_Flag != 0x0F)	//如果从车过了道闸，Stop_Flag = 0x0F，在zigbee检测里面
			{
				if(Check_Agv_daoza % 5 == 0)//时间可以根据情况修改
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
			
从车启动成功，等待从车完成全部任务主车在执行剩下的任务
yunshuche_start();//从车第一次启动
				
				while(1)//从车已经完成全自动任务
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
		yunshuche_xieyi();//发完启动协议后
		//从车返回0x55,0x02,0x99,0x03,0x00,0x00,0x9C,0xBB应答信号，Can_ZigBeeRx_Check检测会置Stop_Flag = 0x0D;
		while (Stop_Flag != 0x0D)		//等待从车返回应答信号,从车发0x55,0x02,0x99,0x03,0x00,0x00,0x9C,0xBB应答信号，如果没有收到从车的应答信号则进入再一次发一次启动
		{														//退出机制，以防收不到应答信号卡死
			if(Check_Agv_Ask % 80 == 0)//时间可以根据情况修改，Check_Agv_Ask等于80取余80才等于0,这里是每50ms，Check_Agv_Ask+1，刚刚好4s以后进入这个条件也就是80，每+80进入一次，但是=160结束，最多进入两次，也就是全文一共发2次启动协议
			{
				yunshuche_xieyi();//再一次发送启动协议
				if (Check_Agv_Ask == 160)//这里是时间，可以更改
				{
					Check_Agv_Ask = 0;
					break;					//这里break是结束循环，就是如果是50了，立马不执行下面的代码，跳出循环
				}
			}
			Check_Agv_Ask++;
			//CanP_Host_Main();//判断指定自定义Fifo中获取数据并保存
			delay_ms(50);
			Can_ZigBeeRx_Check();//zigbee检测
		}
		Check_Agv_Ask = 0;
		Stop_Flag = 0;	
		delay_ms(50);
}


/***************************************************************
** 功能：     主车回传安卓信号
** 参数：	  	无
** 返回值：   无
** 说明：     
****************************************************************/
void zhuche_Ack(void)
{   
		Principal_Tab[2] = 0x7E;				  //运行状态
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
** 功能：     主车发给从车的应答信号
** 参数：	  	无
** 返回值：   无
** 说明：     
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
		Send_zigbee[0]=0;//清除数组,因为有时候只发了几位
		Send_zigbee[1]=0;
		Send_zigbee[2]=0;
		Send_zigbee[3]=0;
		Send_zigbee[4]=0;
		Send_zigbee[5]=0;
		Send_zigbee[6]=0;
		Send_zigbee[7]=0;
}

