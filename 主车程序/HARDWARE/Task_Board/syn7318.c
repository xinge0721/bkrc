#include "stm32f4xx.h"
#include "delay.h"
#include <string.h>
#include "cba.h"
#include "infrared.h"
#include "data_base.h"
#include "canP_HostCom.h"
#include "syn7318.h"

uint8_t uart6_data = 0;			// USART6 �������ݻ���
uint8_t uart6_flag = 0;			// USART6 ��������ʱ��
uint8_t UART6_RxData[9];		// USART6 �������ݻ���
uint8_t voice_falg = 0;		// ����ģ�鷵��״̬

uint8_t Syn7318_Init_Count = 0;
unsigned char Back[4] = {0};   //��������ش�������
unsigned char ASR[6] = {0};    //����ʶ�����ش�������
unsigned char S[4] = {0};      //����ģ�鵱ǰ����״̬�ش�������
unsigned char shibie_shibai_flag=2;  //����ʶ������ָ���Ƿ�ʧ�ܵı�־������ʼֵ2������ɹ���Ϊ0�����ʧ�ܣ�Ϊ1

char Wake_Up[] = {0xfd,0x00,0x02,0x51,0x1F};
char Wake_Up_Off[] = {0xfd,0x00,0x02,0x52};//�رջ���
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
//	//���ڸ�λ,��ӵģ�����
//	USART_DeInit(USART6);
	
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6);
	
	//PC6->Tx 
	GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_TypeDefStructure.GPIO_Mode = GPIO_Mode_AF;			//���ù���
	GPIO_TypeDefStructure.GPIO_OType = GPIO_OType_PP;   //�������
	GPIO_TypeDefStructure.GPIO_PuPd = GPIO_PuPd_UP;     
	GPIO_TypeDefStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC,&GPIO_TypeDefStructure);
	
//	//PC7-RX
//	GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_7;
//	GPIO_TypeDefStructure.GPIO_Mode = GPIO_Mode_IN;		//����
//	GPIO_TypeDefStructure.GPIO_PuPd = GPIO_PuPd_UP; 	//����
//	
//	GPIO_Init(GPIOC,&GPIO_TypeDefStructure);
	
	
	USART_TypeDefStructure.USART_BaudRate = baudrate;					   				//������
	USART_TypeDefStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None; //��Ӳ��������
	USART_TypeDefStructure.USART_Mode = USART_Mode_Tx|USART_Mode_Rx; 		//�����뷢��ģʽ
	USART_TypeDefStructure.USART_Parity = USART_Parity_No; 		       		//��У��λ
	USART_TypeDefStructure.USART_StopBits = USART_StopBits_1;        		//ֹͣλ1
	USART_TypeDefStructure.USART_WordLength = USART_WordLength_8b;   		//����λ8λ
	USART_Init(USART6,&USART_TypeDefStructure);
	
	USART_Cmd(USART6,ENABLE);
	USART_ClearFlag(USART6, USART_FLAG_TC);						//���������ɱ�־λ
	USART_ClearFlag(USART6, USART_FLAG_RXNE);
}


/*********************************************************************
���� �� ������SYN7318_Put_Char----����һ���ֽں���
������˵������txd---�����͵��ֽڣ�8λ��
����    ������SYN7318_Put_Char('d');  ���͡�d��
*********************************************************************/
void SYN7318_Put_Char(uint16_t Data)
{
		USART_SendData(USART6,Data);
	  while(USART_GetFlagStatus(USART6,USART_FLAG_TXE)!=SET);
}



/*********************************************************************
���� �� ������SYN7318_Put_String----�����ַ�������
������˵������Pst������ַ�����������
              Length���ַ�������
����    ������uchar d[4] = {0x00,0x01,0x02,0x03};
              SYN7318_Put_String(d,4); ---��������d�е�Ԫ��
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
���� �� ������SYN7318_Get_char----����һ���ֽں���
������˵�������޲�
���� �� ֵ�������յ����ֽ�
����    ������uchar d;
              d = SYN7318_Get_char();
              SYN7318_Get_char(d); -----������յ����ֽ�
*********************************************************************/

uint16_t SYN7318_Get_char(void) 
{
  uint16_t return_data;	
	while(USART_GetFlagStatus(USART6,USART_FLAG_RXNE) != SET);
	return_data =USART_ReceiveData(USART6);
	return return_data;	
}

/*********************************************************************
���� �� ������SYN7318_Get_String----�����ַ�������
������˵������Pst����Ž��յ����ַ�����������
              Length���ַ�������
����    ������uchar d[4] = {0};
              SYN7318_Get_String(d,4);
              SYN7318_Get_String(d,4);----������յ����ַ��� 
*********************************************************************/
void SYN7318_Get_String(unsigned char *Pst,uint16_t Length)
{
  uint16_t i;
  for(i=0;i<Length;i++)
  {
     Pst[i] = SYN7318_Get_char();
  }
}


void SYN7318_Init(void)//����ģ���ʼ��
{
	USART6_Init(115200);//���ڳ�ʼ��
	
	GPIO_InitTypeDef  GPIO_TypeDefStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);

	//PB9 -- SYN7318_RESET������ģ��ĸ�λ��
	GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_TypeDefStructure.GPIO_Mode = GPIO_Mode_OUT;	//���ģʽ
	GPIO_TypeDefStructure.GPIO_OType = GPIO_OType_PP;   //�������
	GPIO_TypeDefStructure.GPIO_PuPd = GPIO_PuPd_UP;			// ����
	GPIO_TypeDefStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB,&GPIO_TypeDefStructure);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_9);						//Ĭ��Ϊ�ߵ�ƽ
	
}
/*********************************************************************
���� �� ������SYN_TTS----�����ϳɲ��ź���
������˵������Pst�����Ҫ�ϳɲ��ŵ��ı���������
����    ������uchar Data[] = {"��������"};
              SYN_TTS(Data); -----�ϳɲ��ű�������
Back����(�Ǽ�7ͨ��Э�鼰ͨ�ſ��Ƶ�7.3��ģ��ͨ�ûش�)
*********************************************************************/
void SYN_TTS(char *Pst)
{
	uint8_t Length;
	char Frame[5];   //���淢�����������
	
	Length = strlen(Pst); 
	Frame[0] = 0xFD;      //֡ͷ
	Frame[1] = 0x00;
	Frame[2] = Length+2;
	Frame[3] = 0x01;      //�����ϳɲ�������
	Frame[4] = 0x00;      //���ű����ʽΪ��GB2312��
	
	SYN7318_Put_String(Frame, 5);	 //��������
	SYN7318_Put_String(Pst, Length);
}

static uint8_t YY_SJ [8]={0x55 ,0x06 ,0x20 ,0x01 ,0x00 ,0x00 ,0x21 ,0xbb};  // ��������������־����ָ��(����)
//static uint8_t YY_SJ [8]={0x55 ,0x06 ,0x20 ,0x01 ,0x00 ,0x00 ,0x21 ,0xbb};  // ��������������־����ָ��(����)
/*********************************************************************
���� �� ������Start_ASR----��ʼ����ʶ����
������˵������Dict���ʵ���
����    ������Start_ASR(0x00); ----ʶ��0x00�ʵ��еĴ���
ʶ��󣬻��Ȼش������������߽��ճɹ�����ʧ�ܡ�Back���飬Ȼ��Ѵʱ���µ�����ģ��Ĵ�����Ȼ������ģ��ʶ��ش�����Ƭ���ϡ�ASR���飬ASR[0]������ʶ������ش�(��7ͨ��Э�鼰ͨ�ſ��Ƶ�7.3�Ŀ�ʼ����ʶ��0x10���߸��ش�)
ASR[3]������ģ��Ĵ���������ʵ���0����һ�������Ӿ���0x00
Back����(�Ǽ�7ͨ��Э�鼰ͨ�ſ��Ƶ�7.3��ģ��ͨ�ûش�)
*********************************************************************/
void Start_ASR(char Dict)
{

	char Frame[5];   //���淢�����������
	
	Frame[0] = 0xFD;      //֡ͷ
	Frame[1] = 0x00;
	Frame[2] = 0x02;   
	Frame[3] = 0x10;      //��ʼ����ʶ������
	Frame[4] = Dict;      //�ʵ���  �������޸���Ҫʶ��Ĵʵ���
	
	ASR[3]=0;
	
	SYN7318_Put_String(Frame, 5);	//��������
	SYN7318_Get_String(Back,4); 	//���ջش�����
	if(Back[3] == 0x41) 					//����ش����飬���ճɹ�
	{
		Send_ZigbeeData_To_Fifo(YY_SJ,8);
		Send_ZigbeeData_To_Fifo(YY_SJ,8);
		delay_ms(100);
		 SYN7318_Get_String(Back,3);  //����ʶ������ش����(ʶ��ɹ�����ʶ��ʧ��)
		if(Back[0] == 0xfc)						//����ʶ���֡ͷ
		{
			 SYN7318_Get_String(ASR,Back[2]);//ASR�ǽ���ʶ�����ش�������
		}
	}
}

/*********************************************************************
���� �� ������Stop_ASR----ֹͣ����ʶ����
������˵�������޲�
*********************************************************************/
void Stop_ASR()
{
	char Frame[4];   //���淢�����������
	
	Frame[0] = 0xFD;      //֡ͷ
	Frame[1] = 0x00;
	Frame[2] = 0x01;   
	Frame[3] = 0x11;      //ֹͣ����ʶ������
	
  SYN7318_Put_String(Frame, 4);	 //��������
  SYN7318_Get_String(Back,4);    //���ջش�������ճɹ�����ʧ�ܵȵ�

}

/***************************************************************************
���� �� ������Status_Query----ģ��״̬��ѯ����
������˵�������޲�
����    ������Status_Query(); 
��ѯ�󣬻��Ȼش������������߽��ճɹ�����ʧ�ܡ�Back���飬Ȼ���ڷ���ģ�鵱ǰ״̬��S����
S������7ͨ��Э�鼰ͨ�ſ���7.9.1��״̬��ѯ����ش��������
***************************************************************************/
void Status_Query()
{
	char Frame[4];   //���淢�����������
	
	Frame[0] = 0xFD;    //֡ͷ
	Frame[1] = 0x00;
	Frame[2] = 0x01;   
	Frame[3] = 0x21;   //״̬��ѯ����
	
	SYN7318_Put_String(Frame, 4);	//������������
	SYN7318_Get_String(Back,4);   //���ջش�������ճɹ�����ʧ�ܵȵ�
	if(Back[3] == 0x41)         //����ش����飬���ճɹ�
	{
		 SYN7318_Get_String(S,4);   //ģ�鵱ǰ����״̬�Ļش���������浽S��������
	}
}


void Test(void)//stn7318,ʹ��֮���Ӽǵø�λ��ֻ��ʶ��һ�Σ�������������������־��������������ظ�����
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
			switch(ASR[5]) // ����ID�����Ǵʵ�����Ĵ����������ID�Ż᷵����һ������ID
					{
            case 25:   	 //�ӳ����������Զ�����ϵͳ0x19
						{														  	
							SYN_TTS("����·վ");
							delay_ms(500);
							delay_ms(500);
							delay_ms(250);
							delay_ms(500);
							Stop_ASR();
							break;													
						}
						case 33:   	//0x21
						{														  	
							SYN_TTS("��ǿ·վ");
							delay_ms(500);
							delay_ms(500);
							delay_ms(250);
							delay_ms(500);
							Stop_ASR();
							break;													
						}
						case 34:   	 //0x22
						{														  	
							SYN_TTS("����·վ");
							delay_ms(500);
							delay_ms(500);
							delay_ms(250);
							delay_ms(500);
							Stop_ASR();
							break;													
						}
						case 35:   	//0x23
						{														  	
							SYN_TTS("��г·վ");
							delay_ms(500);
							delay_ms(500);
							delay_ms(250);
							delay_ms(500);
							Stop_ASR();
							break;													
						}
						case 36:   	 //0x24
						{														  	
							SYN_TTS("����·վ");
							delay_ms(500);
							delay_ms(500);
							delay_ms(250);
							delay_ms(500);
							Stop_ASR();
							break;													
						}
						case 37:   	 //0x25
						{														  	
							SYN_TTS("��ҵ·վ");
							delay_ms(500);
							delay_ms(500);
							delay_ms(250);
							delay_ms(500);
							Stop_ASR();
							break;													
						}
						case 38:   	 //0x26
						{														  	
							SYN_TTS("����·վ");
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
** ���ܣ�     ʶ�����ϴ���̨(�Զ������ϴ�ϵͳ)
** ������	 	  ��
** ����ֵ��   ��
** ˵����     
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
