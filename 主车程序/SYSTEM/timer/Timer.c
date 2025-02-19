#include "stm32f4xx.h"
#include "Timer.h"

volatile uint32_t global_times = 0;
volatile uint32_t delay_ms_const = 0;

void Timer_Init(uint16_t arr,uint16_t psc)//内部时钟168MHZ，挂载在APB2
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
	TIM_ClearFlag(TIM10,TIM_FLAG_Update);//防止一上电就进入中断
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	
	TIM_Cmd(TIM10, ENABLE);
}


void TIM1_UP_TIM10_IRQHandler(void)//1ms进来一次，计数值global_times+1，global_times=1就代表为1ms
{
	if(TIM_GetITStatus(TIM10,TIM_IT_Update) == SET)
	{
			global_times++;
	}
	TIM_ClearITPendingBit(TIM10,TIM_IT_Update);
}

uint32_t gt_get(void)//获取计数值global_times
{
	return global_times;
}

uint32_t gt_get_sub(uint32_t c)
{
	if(c > global_times)//如果目标时间c大于计时时间global_times，则c=c-global_times，也就是算出它们的差值
		c -= global_times;
	else											//如果目标时间c小于等于计时时间global_times，则让返回值为0
		c = 0;
	return c;
}



