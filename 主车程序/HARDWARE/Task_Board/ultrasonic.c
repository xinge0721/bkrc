#include "stm32f4xx.h"
#include "ultrasonic.h"
#include "delay.h"
#include "cba.h"

float Ultrasonic_Value = 0;
uint32_t Ultrasonic_Num=0;						// ����ֵ
uint16_t dis =0 ;

void Ultrasonic_Port()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB,ENABLE);//��ʱ������APB1�ϣ�APB1ʱ��Ƶ��Ϊ84MHZ
	
	
	//GPIOA15---INC--RX(�Գ�����Ϊ������ǽ��գ���Ƭ������)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  //ͨ�����
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //������� 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   //����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//GPIOB4---INT0--TX(�Գ�����Ϊ������Ƿ��ͣ���Ƭ������)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;    
	GPIO_Init(GPIOB,&GPIO_InitStructure);

}


void Ultrasonic_TIM(uint16_t arr,uint16_t psc)
{
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
	
	TIM_InitStructure.TIM_Period = arr;
	TIM_InitStructure.TIM_Prescaler = psc;
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
//�˲����Ի�����ʱ����Ч
//	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//	TIM_InitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM6,&TIM_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 8;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM6, DISABLE);
}

void Ultrasonic_EXTI(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//����IO�����ж��ߵ�ӳ���ϵ
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB,EXTI_PinSource4);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void Ultrasonic_Init()//��Ϊ��ʱ������APB1�ϣ�APB1ʱ��Ƶ��Ϊ84MHZ
{
	Ultrasonic_Port();
	Ultrasonic_TIM(9,83);//10us
	Ultrasonic_EXTI();
}
/*
���ܣ������������β�ȡ���ֵ���͵�LED����ʾ
				Ultrasonic_Ranging();//������
				delay_ms(100);
				size1 = (int)dis;
		
				Ultrasonic_Ranging();//������
				delay_ms(100);
				size2 = (int)dis;
				
				Ultrasonic_Ranging();//������
				delay_ms(100);
				size3 = (int)dis;
		
				max_size = size1; // ���� size1 �����ĳ�ʼֵ

				if (size2 > max_size) 
				{
					max_size = size2; // ��� size2 �ȵ�ǰ���ֵ����������ֵ
				}

				if (size3 > max_size)
				{
					max_size = size3; // ��� size3 �ȵ�ǰ���ֵ����������ֵ
				}
		
				
				LED_Dis_Zigbee(max_size);//��ʾ��LED
				delay_ms(10);
				LED_Dis_Zigbee(max_size);
				delay_ms(10);
				
���ܣ�����������������ʾ�������־������			
				while(1)//����������������ʾ�������־������
			{
				Ultrasonic_Ranging();//������
				delay_ms(100);
				distance123= (int)dis;
				infrare_com12[2]=(distance123/100%10)+0x30;
				infrare_com12[3]=(distance123/10%10)+0x30;
				Infrared_Send(infrare_com12,6);
			}

*/

//���������
void Ultrasonic_Ranging()
{
    INC = 1;    //Trig���ߵ�ƽ10us        
		delay_us(12);
    INC = 0;

	TIM_Cmd(TIM6,ENABLE);
//	TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
	TIM_ClearFlag(TIM6,TIM_FLAG_Update);//��ֹһ�ϵ�ͽ����ж�
	
  Ultrasonic_Num  = 0;			 // ��ʱ������
	delay_ms(30);							 //�ȴ�һ��ʱ�䣬�ȴ����ͳ����������ź�
	INC = 1;
	delay_ms(5);
	TIM_Cmd(TIM6,DISABLE);
}

void TIM6_DAC_IRQHandler()
{
	if(TIM_GetITStatus(TIM6,TIM_IT_Update) == SET)
	{
		Ultrasonic_Num++;
		
	}
	TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
}

void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4) == SET)
	{
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4) == RESET)
		{	
			TIM_Cmd(TIM6,DISABLE);
			Ultrasonic_Value = (float)Ultrasonic_Num;
			Ultrasonic_Value = (float)Ultrasonic_Value*1.72f-20.0f;       // ������붨ʱ10us��S=Vt/2����2��������
			dis = (uint16_t) Ultrasonic_Value;//����
		}
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
}



