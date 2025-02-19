#include "stm32f4xx.h"
#include "Timer.h"

volatile uint32_t global_times = 0;
volatile uint32_t delay_ms_const = 0;

void Timer_Init(uint16_t arr,uint16_t psc)//�ڲ�ʱ��168MHZ��������APB2
{
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10,ENABLE);
	
	TIM_InitStructure.TIM_Period = arr;
	TIM_InitStructure.TIM_Prescaler = psc;
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM10,&TIM_InitStructure);
	
	TIM_ITConfig(TIM10,TIM_IT_Update,ENABLE);
	TIM_ClearFlag(TIM10,TIM_FLAG_Update);//��ֹһ�ϵ�ͽ����ж�
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	
	TIM_Cmd(TIM10, ENABLE);
}


void TIM1_UP_TIM10_IRQHandler(void)//1ms����һ�Σ�����ֵglobal_times+1��global_times=1�ʹ���Ϊ1ms
{
	if(TIM_GetITStatus(TIM10,TIM_IT_Update) == SET)
	{
			global_times++;
	}
	TIM_ClearITPendingBit(TIM10,TIM_IT_Update);
}

uint32_t gt_get(void)//��ȡ����ֵglobal_times
{
	return global_times;
}

uint32_t gt_get_sub(uint32_t c)
{
	if(c > global_times)//���Ŀ��ʱ��c���ڼ�ʱʱ��global_times����c=c-global_times��Ҳ����������ǵĲ�ֵ
		c -= global_times;
	else											//���Ŀ��ʱ��cС�ڵ��ڼ�ʱʱ��global_times�����÷���ֵΪ0
		c = 0;
	return c;
}



