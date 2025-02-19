#include "stm32f4xx.h"
#include "tba.h"
#include "delay.h"
/**
函数功能：任务板光敏电阻配置
参    数：无
返 回 值：无
*/

void Tba_Photoresistance_Config(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
	GPIO_InitTypeDef  GPIO_TypeDefStructure;
	GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_TypeDefStructure.GPIO_Mode = GPIO_Mode_IN;			//输入模式
	GPIO_TypeDefStructure.GPIO_PuPd = GPIO_PuPd_UP;   	//上拉输入
	GPIO_Init(GPIOG,&GPIO_TypeDefStructure);
}

uint8_t Get_tba_phsis_value(void)//读引脚电平
{
	return GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_8);
}

/**
函数功能：任务板蜂鸣器配置
参    数：无
返 回 值：无
*/
void Tba_Beep_Config(void)
{
	GPIO_InitTypeDef  GPIO_TypeDefStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	
	GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_TypeDefStructure.GPIO_Mode = GPIO_Mode_OUT;  //通用输出
	GPIO_TypeDefStructure.GPIO_OType = GPIO_OType_PP; //推挽输出 
	GPIO_TypeDefStructure.GPIO_PuPd = GPIO_PuPd_UP;   //上拉
	GPIO_TypeDefStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOC,&GPIO_TypeDefStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);//不响，低电平响，高电平不响，这里高电平
}


/**
函数功能：任务板转向灯配置
参    数: 无
返 回 值：无
*/
void Tba_WheelLED_Config(void)
{
	GPIO_InitTypeDef  GPIO_TypeDefStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
	
	// LED_L -- PH10  LED_R --PH11
	GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
	GPIO_TypeDefStructure.GPIO_Mode = GPIO_Mode_OUT;  //通用输出
	GPIO_TypeDefStructure.GPIO_OType = GPIO_OType_PP; //推挽输出 
	GPIO_TypeDefStructure.GPIO_PuPd = GPIO_PuPd_UP;   //上拉
	GPIO_TypeDefStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOH,&GPIO_TypeDefStructure);
	GPIO_SetBits(GPIOH,GPIO_Pin_10 | GPIO_Pin_11);//低电平亮灯，高电平不亮，这里高电平
}

/**
函数功能: 任务板初始化
参    数：无
返 回 值: 无
*/
void Tba_Init(void)
{
	Tba_Photoresistance_Config();
	Tba_Beep_Config();
	Tba_WheelLED_Config();
}

