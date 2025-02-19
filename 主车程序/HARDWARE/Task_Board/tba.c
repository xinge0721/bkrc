#include "stm32f4xx.h"
#include "tba.h"
#include "delay.h"
/**
�������ܣ�����������������
��    ������
�� �� ֵ����
*/

void Tba_Photoresistance_Config(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
	GPIO_InitTypeDef  GPIO_TypeDefStructure;
	GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_TypeDefStructure.GPIO_Mode = GPIO_Mode_IN;			//����ģʽ
	GPIO_TypeDefStructure.GPIO_PuPd = GPIO_PuPd_UP;   	//��������
	GPIO_Init(GPIOG,&GPIO_TypeDefStructure);
}

uint8_t Get_tba_phsis_value(void)//�����ŵ�ƽ
{
	return GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_8);
}

/**
�������ܣ���������������
��    ������
�� �� ֵ����
*/
void Tba_Beep_Config(void)
{
	GPIO_InitTypeDef  GPIO_TypeDefStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	
	GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_TypeDefStructure.GPIO_Mode = GPIO_Mode_OUT;  //ͨ�����
	GPIO_TypeDefStructure.GPIO_OType = GPIO_OType_PP; //������� 
	GPIO_TypeDefStructure.GPIO_PuPd = GPIO_PuPd_UP;   //����
	GPIO_TypeDefStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOC,&GPIO_TypeDefStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);//���죬�͵�ƽ�죬�ߵ�ƽ���죬����ߵ�ƽ
}


/**
�������ܣ������ת�������
��    ��: ��
�� �� ֵ����
*/
void Tba_WheelLED_Config(void)
{
	GPIO_InitTypeDef  GPIO_TypeDefStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
	
	// LED_L -- PH10  LED_R --PH11
	GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
	GPIO_TypeDefStructure.GPIO_Mode = GPIO_Mode_OUT;  //ͨ�����
	GPIO_TypeDefStructure.GPIO_OType = GPIO_OType_PP; //������� 
	GPIO_TypeDefStructure.GPIO_PuPd = GPIO_PuPd_UP;   //����
	GPIO_TypeDefStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOH,&GPIO_TypeDefStructure);
	GPIO_SetBits(GPIOH,GPIO_Pin_10 | GPIO_Pin_11);//�͵�ƽ���ƣ��ߵ�ƽ����������ߵ�ƽ
}

/**
��������: ������ʼ��
��    ������
�� �� ֵ: ��
*/
void Tba_Init(void)
{
	Tba_Photoresistance_Config();
	Tba_Beep_Config();
	Tba_WheelLED_Config();
}

