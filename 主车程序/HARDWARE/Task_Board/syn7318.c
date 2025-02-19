#include "stm32f4xx.h"
#include "delay.h"
#include <string.h>
#include "cba.h"
#include "infrared.h"
#include "data_base.h"
#include "canP_HostCom.h"
#include "syn7318.h"

uint8_t uart6_data = 0;			// USART6 接收数据缓存
uint8_t uart6_flag = 0;			// USART6 接收数据时序
uint8_t UART6_RxData[9];		// USART6 接收数据缓存
uint8_t voice_falg = 0;		// 语音模块返回状态

uint8_t Syn7318_Init_Count = 0;
unsigned char Back[4] = {0};   //接收命令回传的数组
unsigned char ASR[6] = {0};    //接收识别结果回传的数组
unsigned char S[4] = {0};      //接收模块当前工作状态回传的数组
unsigned char shibie_shibai_flag=2;  //表明识别语音指令是否失败的标志量，初始值2；如果成功，为0；如果失败，为1

char Wake_Up[] = {0xfd,0x00,0x02,0x51,0x1F};
char Wake_Up_Off[] = {0xfd,0x00,0x02,0x52};//关闭唤醒
char Stop_Wake_Up[] = {0xFD,0x00,0x01,0x52};

unsigned char YY_suiji[8] = {0x55,0x06,0x20,0x01,0x00,0x00,0x01,0xBB};

char Start_ASR_Buf[] = {0xFD,0x00,0x02,0x10,0x03};
char Stop_ASR_Buf[] = {0xFD,0x00,0x01,0x11};

char Play_MP3[] ={ 0xFD,0x00,0x1E,0x01,0x01,0xC6,0xF4,0xB6,0xAF,0xD3, 0xEF ,0xD2, 0xF4,
                            	0xBF, 0xD8, 0xD6 ,0xC6 ,0xBC ,0xDD ,0xCA ,0xBB ,0xA3 ,0xAC, 0xC7, 0xEB,
                             	0xB7, 0xA2, 0xB3, 0xF6 ,0xD6, 0xB8, 0xC1, 0xEE };
unsigned char Ysn7813_flag=0;

#define SYN7318_RST_H GPIO_SetBits(GPIOB,GPIO_PinSource9)
#define SYN7318_RST_L GPIO_ResetBits(GPIOB,GPIO_PinSource9)

void Yu_Yin_Asr(void);

void USART6_Init(uint32_t baudrate)
{
	GPIO_InitTypeDef  GPIO_TypeDefStructure;
	USART_InitTypeDef USART_TypeDefStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);
//	//串口复位,后加的，调试
//	USART_DeInit(USART6);
	
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6);
	
	//PC6->Tx 
	GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_TypeDefStructure.GPIO_Mode = GPIO_Mode_AF;			//复用功能
	GPIO_TypeDefStructure.GPIO_OType = GPIO_OType_PP;   //推挽输出
	GPIO_TypeDefStructure.GPIO_PuPd = GPIO_PuPd_UP;     
	GPIO_TypeDefStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC,&GPIO_TypeDefStructure);
	
//	//PC7-RX
//	GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_7;
//	GPIO_TypeDefStructure.GPIO_Mode = GPIO_Mode_IN;		//输入
//	GPIO_TypeDefStructure.GPIO_PuPd = GPIO_PuPd_UP; 	//上拉
//	
//	GPIO_Init(GPIOC,&GPIO_TypeDefStructure);
	
	
	USART_TypeDefStructure.USART_BaudRate = baudrate;					   				//波特率
	USART_TypeDefStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None; //无硬件控制流
	USART_TypeDefStructure.USART_Mode = USART_Mode_Tx|USART_Mode_Rx; 		//接收与发送模式
	USART_TypeDefStructure.USART_Parity = USART_Parity_No; 		       		//无校验位
	USART_TypeDefStructure.USART_StopBits = USART_StopBits_1;        		//停止位1
	USART_TypeDefStructure.USART_WordLength = USART_WordLength_8b;   		//数据位8位
	USART_Init(USART6,&USART_TypeDefStructure);
	
	USART_Cmd(USART6,ENABLE);
	USART_ClearFlag(USART6, USART_FLAG_TC);						//清除发送完成标志位
	USART_ClearFlag(USART6, USART_FLAG_RXNE);
}


/*********************************************************************
【函 数 名】：SYN7318_Put_Char----发送一个字节函数
【参数说明】：txd---待发送的字节（8位）
【简    例】：SYN7318_Put_Char('d');  发送‘d’
*********************************************************************/
void SYN7318_Put_Char(uint16_t Data)
{
		USART_SendData(USART6,Data);
	  while(USART_GetFlagStatus(USART6,USART_FLAG_TXE)!=SET);
}



/*********************************************************************
【函 数 名】：SYN7318_Put_String----发送字符串函数
【参数说明】：Pst：存放字符串的数组名
              Length：字符串长度
【简    例】：uchar d[4] = {0x00,0x01,0x02,0x03};
              SYN7318_Put_String(d,4); ---发送数组d中的元素
*********************************************************************/
void SYN7318_Put_String(char * Pst,uint16_t Length)
{ 
   uint16_t i;
	for(i = 0; i < Length; i++)
	{
		  SYN7318_Put_Char( Pst[i]);
	}
}


/*********************************************************************
【函 数 名】：SYN7318_Get_char----接收一个字节函数
【参数说明】：无参
【返 回 值】：接收到的字节
【简    例】：uchar d;
              d = SYN7318_Get_char();
              SYN7318_Get_char(d); -----输出接收到的字节
*********************************************************************/

uint16_t SYN7318_Get_char(void) 
{
  uint16_t return_data;	
	while(USART_GetFlagStatus(USART6,USART_FLAG_RXNE) != SET);
	return_data =USART_ReceiveData(USART6);
	return return_data;	
}

/*********************************************************************
【函 数 名】：SYN7318_Get_String----接收字符串函数
【参数说明】：Pst：存放接收到的字符串的数组名
              Length：字符串长度
【简    例】：uchar d[4] = {0};
              SYN7318_Get_String(d,4);
              SYN7318_Get_String(d,4);----输出接收到的字符串 
*********************************************************************/
void SYN7318_Get_String(unsigned char *Pst,uint16_t Length)
{
  uint16_t i;
  for(i=0;i<Length;i++)
  {
     Pst[i] = SYN7318_Get_char();
  }
}


void SYN7318_Init(void)//语音模块初始化
{
	USART6_Init(115200);//串口初始化
	
	GPIO_InitTypeDef  GPIO_TypeDefStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);

	//PB9 -- SYN7318_RESET—语音模块的复位键
	GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_TypeDefStructure.GPIO_Mode = GPIO_Mode_OUT;	//输出模式
	GPIO_TypeDefStructure.GPIO_OType = GPIO_OType_PP;   //推挽输出
	GPIO_TypeDefStructure.GPIO_PuPd = GPIO_PuPd_UP;			// 上拉
	GPIO_TypeDefStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB,&GPIO_TypeDefStructure);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_9);						//默认为高电平
	
}
/*********************************************************************
【函 数 名】：SYN_TTS----语音合成播放函数
【参数说明】：Pst：存放要合成播放的文本的数组名
【简    例】：uchar Data[] = {"北京龙邱"};
              SYN_TTS(Data); -----合成播放北京龙邱
Back数组(是见7通信协议及通信控制的7.3的模块通用回传)
*********************************************************************/
void SYN_TTS(char *Pst)
{
	uint8_t Length;
	char Frame[5];   //保存发送命令的数组
	
	Length = strlen(Pst); 
	Frame[0] = 0xFD;      //帧头
	Frame[1] = 0x00;
	Frame[2] = Length+2;
	Frame[3] = 0x01;      //语音合成播放命令
	Frame[4] = 0x00;      //播放编码格式为“GB2312”
	
	SYN7318_Put_String(Frame, 5);	 //发送命令
	SYN7318_Put_String(Pst, Length);
}

static uint8_t YY_SJ [8]={0x55 ,0x06 ,0x20 ,0x01 ,0x00 ,0x00 ,0x21 ,0xbb};  // 发给语音播报标志物—随机指令(播报)
//static uint8_t YY_SJ [8]={0x55 ,0x06 ,0x20 ,0x01 ,0x00 ,0x00 ,0x21 ,0xbb};  // 发给语音播报标志物—随机指令(播报)
/*********************************************************************
【函 数 名】：Start_ASR----开始语音识别函数
【参数说明】：Dict：词典编号
【简    例】：Start_ASR(0x00); ----识别0x00词典中的词条
识别后，会先回传命令结果，告诉接收成功或者失败—Back数组，然后把词表更新到语音模块的词条，然后语音模块识别回传到单片机上—ASR数组，ASR[0]是语音识别命令回传(见7通信协议及通信控制的7.3的开始语音识别0x10的七个回传)
ASR[3]是语音模块的词条，比如词典编号0，第一个音量加就是0x00
Back数组(是见7通信协议及通信控制的7.3的模块通用回传)
*********************************************************************/
void Start_ASR(char Dict)
{

	char Frame[5];   //保存发送命令的数组
	
	Frame[0] = 0xFD;      //帧头
	Frame[1] = 0x00;
	Frame[2] = 0x02;   
	Frame[3] = 0x10;      //开始语音识别命令
	Frame[4] = Dict;      //词典编号  在这里修改想要识别的词典编号
	
	ASR[3]=0;
	
	SYN7318_Put_String(Frame, 5);	//发送命令
	SYN7318_Get_String(Back,4); 	//接收回传命令
	if(Back[3] == 0x41) 					//命令回传数组，接收成功
	{
		Send_ZigbeeData_To_Fifo(YY_SJ,8);
		Send_ZigbeeData_To_Fifo(YY_SJ,8);
		delay_ms(100);
		 SYN7318_Get_String(Back,3);  //语音识别命令回传结果(识别成功或者识别失败)
		if(Back[0] == 0xfc)						//语言识别的帧头
		{
			 SYN7318_Get_String(ASR,Back[2]);//ASR是接收识别结果回传的数组
		}
	}
}

/*********************************************************************
【函 数 名】：Stop_ASR----停止语音识别函数
【参数说明】：无参
*********************************************************************/
void Stop_ASR()
{
	char Frame[4];   //保存发送命令的数组
	
	Frame[0] = 0xFD;      //帧头
	Frame[1] = 0x00;
	Frame[2] = 0x01;   
	Frame[3] = 0x11;      //停止语音识别命令
	
  SYN7318_Put_String(Frame, 4);	 //发送命令
  SYN7318_Get_String(Back,4);    //接收回传命令，接收成功或者失败等等

}

/***************************************************************************
【函 数 名】：Status_Query----模块状态查询函数
【参数说明】：无参
【简    例】：Status_Query(); 
查询后，会先回传命令结果，告诉接收成功或者失败—Back数组，然后在返回模块当前状态—S数组
S数组是7通信协议及通信控制7.9.1的状态查询命令回传结果如下
***************************************************************************/
void Status_Query()
{
	char Frame[4];   //保存发送命令的数组
	
	Frame[0] = 0xFD;    //帧头
	Frame[1] = 0x00;
	Frame[2] = 0x01;   
	Frame[3] = 0x21;   //状态查询命令
	
	SYN7318_Put_String(Frame, 4);	//发送命令数组
	SYN7318_Get_String(Back,4);   //接收回传命令，接收成功或者失败等等
	if(Back[3] == 0x41)         //命令回传数组，接收成功
	{
		 SYN7318_Get_String(S,4);   //模块当前工作状态的回传结果，保存到S数组里面
	}
}


void Test(void)//stn7318,使用之后车子记得复位，只能识别一次，功能是让语音播报标志物随机播报，并重复读出
{
	Status_Query();
	if(S[3]==0x4F)
	{
		MP_SPK = 1;
		delay_ms(500);
		MP_SPK = 0;
		delay_ms(500);
		Start_ASR(0X04);
		if(ASR[0] == 0x01)
		{
			switch(ASR[5]) // 命令ID，就是词典里面的词条后面带的ID号会返回来一个命令ID
					{
            case 25:   	 //从车发过来的自动评分系统0x19
						{														  	
							SYN_TTS("文明路站");
							delay_ms(500);
							delay_ms(500);
							delay_ms(250);
							delay_ms(500);
							Stop_ASR();
							break;													
						}
						case 33:   	//0x21
						{														  	
							SYN_TTS("富强路站");
							delay_ms(500);
							delay_ms(500);
							delay_ms(250);
							delay_ms(500);
							Stop_ASR();
							break;													
						}
						case 34:   	 //0x22
						{														  	
							SYN_TTS("民主路站");
							delay_ms(500);
							delay_ms(500);
							delay_ms(250);
							delay_ms(500);
							Stop_ASR();
							break;													
						}
						case 35:   	//0x23
						{														  	
							SYN_TTS("和谐路站");
							delay_ms(500);
							delay_ms(500);
							delay_ms(250);
							delay_ms(500);
							Stop_ASR();
							break;													
						}
						case 36:   	 //0x24
						{														  	
							SYN_TTS("爱国路站");
							delay_ms(500);
							delay_ms(500);
							delay_ms(250);
							delay_ms(500);
							Stop_ASR();
							break;													
						}
						case 37:   	 //0x25
						{														  	
							SYN_TTS("敬业路站");
							delay_ms(500);
							delay_ms(500);
							delay_ms(250);
							delay_ms(500);
							Stop_ASR();
							break;													
						}
						case 38:   	 //0x26
						{														  	
							SYN_TTS("友善路站");
							delay_ms(500);
							delay_ms(500);
							delay_ms(250);
							delay_ms(500);
							Stop_ASR();
							break;													
						}
					}
	  }
	}
}


/***************************************************************
** 功能：     识别结果上传后台(自动评分上传系统)
** 参数：	 	  无
** 返回值：   无
** 说明：     
****************************************************************/
void Syn7318_Up(uint8_t num)
{
		Send_zigbee[0]=0xAF;
    Send_zigbee[1]=0x06;
    Send_zigbee[2]=num;
    Send_zigbee[3]=0x02;
    Send_zigbee[4]=0x00;
    Send_zigbee[5]=0x00;
    Send_zigbee[6]=0x01;
    Send_zigbee[7]=0xBB;
    delay_ms(1);
		Send_ZigbeeData_To_Fifo(Send_zigbee,8);
    delay_ms(100);
}
