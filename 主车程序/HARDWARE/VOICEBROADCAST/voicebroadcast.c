/* Includes ------------------------------------------------------------------*/
#include "voicebroadcast.h"
#include "canP_HostCom.h"
#include "stdlib.h"
#include "can_user.h"
#include "stdio.h"
#include "string.h"
#include <stdio.h>
#include "stm32f4xx.h"
#include "delay.h"
#include "canp_hostcom.h"
#include "cba.h"

int tempwendu=0;//智能公交站回传温度数据(十进制)
uint8_t voice_flag;
int voice_flag_time;
/* 变量定义 ---------------------------------------------------------*/
uint8_t uart6_data = 0;			// USART6 接收数据缓存
uint8_t uart6_flag = 0;			// USART6 接收数据时序
uint8_t UART6_RxData[8];		// USART6 接收数据缓存
uint8_t voice_falg = 0;		// 语音模块返回状态
uint8_t YY_Init[5] = {0xFD, 0x00, 0x00, 0x01, 0x01};
uint8_t Zigbee[8];           // Zigbee发送数据缓存

uint8_t start_voice_dis[5]= {0xFA,0xFA,0xFA,0xFA,0xA1};
uint8_t bkrc_voice_Flag = 0;           // SYN7318语音识别命令ID编号
/*******************************************************
功　能：初始化串口
参　数：无
返回值：无
********************************************************/
static void USART6_Hardware_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* 使能 GPIOC 外设时钟 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    /* 使能 USART6 外设时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

    /* 配置 PC6/PC7 引脚复用映射 */
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);

    /* 将 PC6/PC7 引脚配置为复用功能模式（上拉） */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;		// 端口模式 -> 复用功能模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		// 端口输出类型 -> 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	// 端口输出速度 -> 高速 100MHz(30pF)
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  		// 端口上拉/下拉 -> 上拉
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* USART6 初始化配置 */
    USART_InitStructure.USART_BaudRate = 115200;					// 波特率设置
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// 硬件流设置 -> 无硬件流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	// 收发模式设置 -> 接收+发送
    USART_InitStructure.USART_Parity = USART_Parity_No;				// 奇偶校验位设置 -> 无奇偶校验位
    USART_InitStructure.USART_StopBits = USART_StopBits_1;			// 停止位设置 -> 1位停止位
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;		// 数据位长度 -> 8位数据格式
    USART_Init(USART6, &USART_InitStructure);

    /* 使能 USART6 中断 */
    USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);		// 接收数据寄存器不为空中断

    /* 设置 USART6 中断优先级 */
    NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;				// 选择 IRQ 通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	// 抢占优先级设置
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;			// 响应优先级设置
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					// 启用 USART6 IRQ 通道
    NVIC_Init(&NVIC_InitStructure);

    /* 使能 USART6 */
    USART_Cmd(USART6, ENABLE);
}

/*******************************************************
功　能：串口中断函数
参　数：无
返回值：无
********************************************************/
void USART6_IRQHandler(void)
{
    /* 判断 USART6 是否触发指定中断 -> 接收数据寄存器不为空中断 */
    if (USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)
    {
        uart6_data = USART_ReceiveData(USART6);		// 读取 USART6 数据寄存器

        if (uart6_flag == 0x00)
        {
            if (uart6_data == 0x55)				// 自定义数据帧头
            {
                uart6_flag = 0x01;
                UART6_RxData[0] = uart6_data;	// 帧头
                UART6_RxData[1] = 0x00;
                UART6_RxData[2] = 0x00;
                UART6_RxData[3] = 0x00;
            }
        }
        else if (uart6_flag == 0x01)
        {
            uart6_flag = 0x02;
            UART6_RxData[1] = uart6_data;		// 数据类型
        }
        else if(uart6_flag == 0x02)
        {
            uart6_flag = 0x03;
            UART6_RxData[2] = uart6_data;		// 状态标志
        }
        else if(uart6_flag == 0x03)
        {
            uart6_flag = 0x00;
            UART6_RxData[3] = uart6_data;		// 数据位
            voice_falg = 0x01;					// 自定义数据帧接收完毕
        }
        else
        {
            uart6_flag = 0x00;
            voice_falg = 0x00;
            UART6_RxData[0] = 0x00;
        }

    }
    //清除串口中断接收标志位
    USART_ClearITPendingBit(USART6,USART_IT_RXNE);
}

/*******************************************************
功　能：通过串口1发送一个字节，如0x12、0xff等
参　数：hex -> 字节
返回值：无
********************************************************/
void USART6_Send_Byte(uint8_t byte)
{
    USART_SendData(USART6,byte);
    while(USART_GetFlagStatus(USART6,USART_FLAG_TXE) == RESET);
}

/*******************************************************
功　能：通过串口1发送一个数组
参　数：*buf -> 指针指向一个数组
		 length -> 数组的长度
返回值：无
********************************************************/
void USART6_Send_Length(uint8_t *buf,uint8_t length)
{
    uint8_t len = 0;
    for(len = 0; len < length; len++)
    {
        USART6_Send_Byte(buf[len]);
    }
}

//按键启动识别并播报指定内容
uint8_t BKRC_Key(uint8_t num)
{
	USART6_Send_Length(start_voice_dis,5);//发送开启语音识别指令，唤醒指令
    
	while(bkrc_voice_Flag!=0x40)//等待进入识别模式
	{
		voice_flag=1;//开启计时
		bkrc_voice_Flag = Voice_Drive();//接收返回状态
		if(voice_flag==2)
		{
			break;
		}
	}
	USART6_Send_Byte(num);//发送播报指令
	bkrc_voice_Flag=0;
	voice_flag=1;//开启计时
	while(1)
	{
        
        bkrc_voice_Flag = Voice_Drive();

        if (bkrc_voice_Flag != 0x00||voice_flag==2)   //判断传回数据或者超时退出
        {
            return bkrc_voice_Flag;// 退出循环并返回 bkrc_voice_Flag
        }
	}
}


/**************************************************
功  能：语音识别函数
参  数：	0控制语音播报随即播报指令语音识别测试，2-6播报指定命令进行语音识别测试
返回值：	语音词条ID    词条内容
        0x02 -> 富强路站
			  0x03 -> 民主路站
			  0x04 -> 文明路站
			  0x05 -> 和谐路站
			  0x06 -> 爱国路站
			  0x07 -> 敬业路站
			  0x08 -> 友善路站
		
**************************************************/
uint8_t BKRC_Voice_Extern(uint8_t yy_mode)		// 语音识别
{
    USART6_Send_Length(start_voice_dis,5);//发送开启语音识别指令，唤醒指令
    
		while(bkrc_voice_Flag!=0x40)//等待进入识别模式
		{
			bkrc_voice_Flag = Voice_Drive();//接收返回状态
		}

    
	if(yy_mode==0)
	{
	    YY_Comm_Zigbee(0x20,0x01);			//语音播报随机语音命令
	}else
	{
		YY_Comm_Zigbee(0x10,yy_mode);			//语音播报随机语音命令
	}

    bkrc_voice_Flag=0;
		voice_flag=1;//开启计时
    while (1)
    {
        
        bkrc_voice_Flag = Voice_Drive();

        if (bkrc_voice_Flag != 0x00||voice_flag==2)   //判断传回数据或者超时退出
        {
            return bkrc_voice_Flag;// 退出循环并返回 bkrc_voice_Flag
        }
    }
}


/**************************************************
功  能：语音识别回传命令解析函数
参  数：	无
返回值：	语音词条ID /小创语音识别模块状态
**************************************************/
uint8_t Voice_Drive(void)
{
    uint8_t status = 0;
    if ((voice_falg == 0x01) && (UART6_RxData[0] == 0x55))			// 自定义数据帧接收完毕
    {
        if (UART6_RxData[1] == 0x01)//对应config文件
        {
            status = 0x00;
            switch (UART6_RxData[2])
            {
            case 0x01: {
                //                printf("* 初始化完成 *");
                status |= 0x80;
                break;
            }
            case 0x02: {
                //                printf("* 进入识别模式 *");
                status |= 0x40;
                break;
            }
            case 0x03: {
                //                printf("* 退出识别模式 *");
                status &= 0xB0;
                break;
            }
            case 0x04: {
                //                printf("* 进入休眠模式 *");
                status = 0x00;
                break;
            }
            default  :
                break;
            }
        }

        else if (UART6_RxData[1] == 0x02)//对应asr文件
        {
            status &= 0xF0;
            switch (UART6_RxData[2])
            {
            case 0x01: {
//                USART6_Send_Byte(0x01);
				Send_InfoData_To_Fifo("ID: 2\n", 9);
//				printf("* 富强路站 *");
                status |= 0x02;
                break;
            }
            case 0x02: {
//                USART6_Send_Byte(0x02);
				Send_InfoData_To_Fifo("ID: 3\n", 9);
//                printf("* 民主路站 *");
                status |= 0x03;
                break;
            }
            case 0x03: {
//                USART6_Send_Byte(0x03);
				Send_InfoData_To_Fifo("ID: 4\n", 9);
//                printf("* 文明路站 *");
                status |= 0x04;
                break;
            }
            case 0x04: {
//                USART6_Send_Byte(0x04);
				Send_InfoData_To_Fifo("ID: 5\n", 9);
//                printf("* 和谐路站 *");
                status |= 0x05;
                break;
            }
            case 0x05: {
//                USART6_Send_Byte(0x05);
				Send_InfoData_To_Fifo("ID: 6\n", 9);
//                printf("* 爱国路站 *");
                status |= 0x06;
                break;
            }
			case 0x06: {
//                USART6_Send_Byte(0x06);
				Send_InfoData_To_Fifo("ID: 7\n", 9);
//                printf("* 敬业路站 *");
                status |= 0x07;
                break;
            }
			case 0x07: {
//                USART6_Send_Byte(0x07);
				Send_InfoData_To_Fifo("ID: 8\n", 9);
//                printf("* 友善路站 *");
                status |= 0x08;
                break;
            }
			case 0x08: {
//                USART6_Send_Byte(0x07);
				Send_InfoData_To_Fifo("ID: 8\n", 9);
//                printf("* 友善路站 *");
                status |= 0x09;
                break;
            }
            default  :
                break;
            }
        }
        else if (UART6_RxData[1] == 0x03)//对应serialTTS文件
        {
            switch (UART6_RxData[2])
            {
            case 0x01:
						{
							status |= 0xA0;
						}
                break;
            case 0x02:
						{
							status |= 0xA1;
						}
                break;
            case 0x03:
						{
							status |= 0xA2;
						}
                break;
            case 0x04:
						{
							status |= 0xA3;
						}
                break;
            case 0x05:
						{
							status |= 0xA4;
						}
                break;
						case 0x06:
						{
							status |= 0xB5;
						}
            case 0x10:
						{
							status |= 0xA5;
						}
                break;
            case 0x11:
						{
							status |= 0xA6;
						}
                break;
            case 0x12:
						{
							status |= 0xA7;
						}
                break;
            default  :
                break;
            }
        }
        voice_falg = 0x00;
    }
    return status;
}

/**************************************************
功  能：控制语音播报标志物播报指定文本信息
参  数：	*p  --> 需要发送的数据
返回值：	无
**************************************************/
void YY_Play_Zigbee(char *p)
{
    uint16_t p_len = strlen(p);             // 文本长度

    YY_Init[1] = 0xff & ((p_len + 2) >> 8); // 数据区长度高八位
    YY_Init[2] = 0xff & (p_len + 2);        // 数据区长度低八位
    Send_ZigbeeData_To_Fifo(YY_Init, 5);
    Send_ZigbeeData_To_Fifo((uint8_t *)p, p_len);
    delay_ms(100);
}

/**********************************************************************
 * 函 数 名 ：  控制语音播报标志物播报语音控制命令
 * 参    数 ：  Primary   -> 主指令
                Secondary -> 副职令
                详见附录1
 * 返 回 值 ：  无
 * 简    例 ：  YY_Comm_Zigbee(0x20, 0x01);     // 语音播报随机语音命令

附录1：
-----------------------------------------------------------------------
| Primary | Secondary | 说明
|---------|-----------|------------------------------------------------
|  0x10   |  0x01     | 富强路站
|         |  0x02     | 民主路站
|         |  0x03     | 文明路站
|         |  0x04     | 和谐路站
|         |  0x05     | 爱国路站
|         |  0x06     | 敬业路站
|         |  0x07     | 友善路站
|---------|-----------|------------------------------------------------
|  0x20   |  0x01     | 随机指令
|---------|-----------|------------------------------------------------
***********************************************************************/
void YY_Comm_Zigbee(uint8_t Primary, uint8_t Secondary)
{
    Zigbee[0] = 0x55;
    Zigbee[1] = 0x06;
    Zigbee[2] = Primary;
    Zigbee[3] = Secondary;
    Zigbee[4] = 0x00;
    Zigbee[5] = 0x00;
    Zigbee[6] = (Zigbee[2] + Zigbee[3] + Zigbee[4] + Zigbee[5]) % 256;
    Zigbee[7] = 0xBB;
    Send_ZigbeeData_To_Fifo(Zigbee, 8);
}
/*******************************************************
功　能：语音识别初始化函数
参　数：无
返回值：无
********************************************************/
void BKRC_Voice_Init(void)
{
    USART6_Hardware_Init();
}

//										endfile


/***************************************************************
** 功能：    语音播报标志物播放特定指令
** 参数：	   num:
**								0x01 民主路站		0x04 左侧行驶
**								0x02 禁止右转	  0x05 左行被禁
**								0x03 左侧行驶		0x06 原地掉头
** 返回值：  无
** 说明：    主车通过zigbee控制语音播报标志物播放特定指令
****************************************************************/
void Voice_BroadCast_ZhiDing(uint8_t num)
{
		Send_zigbee[0]=0x55;
    Send_zigbee[1]=0x06;
    Send_zigbee[2]=0x10;
    Send_zigbee[3]=num;
    Send_zigbee[4]=0x00;
    Send_zigbee[5]=0x00;
    Send_zigbee[6]=(Send_zigbee[2]+Send_zigbee[3]+Send_zigbee[4]+Send_zigbee[5])%256;
    Send_zigbee[7]=0xbb;
    delay_ms(1);   
		Send_ZigbeeData_To_Fifo(Send_zigbee,8);
    delay_ms(100);
		Send_ZigbeeData_To_Fifo(Send_zigbee,8);
    delay_ms(100);
		Send_zigbee[0]=0;//清除数组,因为有时候只发了几位
		Send_zigbee[1]=0;
		Send_zigbee[2]=0;
		Send_zigbee[3]=0;
		Send_zigbee[4]=0;
		Send_zigbee[5]=0;
		Send_zigbee[6]=0;
		Send_zigbee[7]=0;
}

/***************************************************************
** 功能：    语音播报标志物播放随机指令
** 参数：	   无
** 返回值：  无
** 说明：    主车通过zigbee控制语音播报标志物播放随机指令
****************************************************************/
void Voice_BroadCast_SuiJi(void)
{
		Send_zigbee[0]=0x55;
    Send_zigbee[1]=0x06;
    Send_zigbee[2]=0x20;
    Send_zigbee[3]=0x01;
    Send_zigbee[4]=0x00;
    Send_zigbee[5]=0x00;
    Send_zigbee[6]=(Send_zigbee[2]+Send_zigbee[3]+Send_zigbee[4]+Send_zigbee[5])%256;
    Send_zigbee[7]=0xbb;
		delay_ms(1);  
		Send_ZigbeeData_To_Fifo(Send_zigbee,8);
    delay_ms(100);
		Send_ZigbeeData_To_Fifo(Send_zigbee,8);
		delay_ms(100);
		Send_zigbee[0]=0;//清除数组,因为有时候只发了几位
		Send_zigbee[1]=0;
		Send_zigbee[2]=0;
		Send_zigbee[3]=0;
		Send_zigbee[4]=0;
		Send_zigbee[5]=0;
		Send_zigbee[6]=0;
		Send_zigbee[7]=0;
}


int HexToDec(uint8_t hexValue)
{ 
	return (int)hexValue;// 强制转为十进制
}


/***************************************************************
** 功能：    语音播报标志物请求回传天气数据与温度数据
** 参数：	   无
** 返回值：  无
下面第四位是把回传的温度转成十进制
Voice_BroadCast_wendu();//使用这个函数即可
				if(tempwendu ==25)//这里只是测试十六进制转十进制的对不对
				{
					MP_SPK = 1;
				}

功能：回传回来的温度根据题目计算车库到指定层
while(1)
			{
				Voice_BroadCast_wendu();
				delay_ms(500);
				Can_ZigBeeRx_Check();
				if(Stop_Flag==0xEF)
				{
					Stop_Flag = 0;
					break;
				}
			}
				to =(((tempwendu + tempwendu)%4) +1);//题目计算
				
				if(to == 1)
				{
					litiGarageF1('A');
					delay_ms(10);
					litiGarageF1('A');
					delay_ms(10);
					litiGarageF1('A');
					delay_ms(10);
				}
				if(to == 2)
				{
					litiGarageF2('A');
					delay_ms(10);
					litiGarageF2('A');
					delay_ms(10);
					litiGarageF2('A');
					delay_ms(10);
				}
				if(to == 3)
				{
					litiGarageF3('A');
					delay_ms(10);
					litiGarageF3('A');
					delay_ms(10);
					litiGarageF3('A');
					delay_ms(10);
				}
				if(to == 4)
				{
					litiGarageF4('A');
					delay_ms(10);
					litiGarageF4('A');
					delay_ms(10);
					litiGarageF4('A');
					delay_ms(10);
				}
****************************************************************/
void Voice_BroadCast_wendu(void)
{
		Send_zigbee[0]=0x55;
    Send_zigbee[1]=0x06;
    Send_zigbee[2]=0x43;
    Send_zigbee[3]=0x00;
    Send_zigbee[4]=0x00;
    Send_zigbee[5]=0x00;
    Send_zigbee[6]=(Send_zigbee[2]+Send_zigbee[3]+Send_zigbee[4]+Send_zigbee[5])%256;
    Send_zigbee[7]=0xbb;
		delay_ms(1);  
		Send_ZigbeeData_To_Fifo(Send_zigbee,8);
		delay_ms(100);
		tempwendu = HexToDec(Zigb_Rx_Buf[4]);
		Send_zigbee[0]=0;
    Send_zigbee[1]=0;
    Send_zigbee[2]=0;
    Send_zigbee[3]=0;
    Send_zigbee[4]=0;
    Send_zigbee[5]=0;
    Send_zigbee[6]=0;
    Send_zigbee[7]=0;
}
