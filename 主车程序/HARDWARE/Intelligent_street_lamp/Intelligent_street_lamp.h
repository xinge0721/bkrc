#ifndef __INTELLIGENTSTREETLAMP_H
#define __INTELLIGENTSTREETLAMP_H
#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"

extern uint8_t adjustment_Count;//记录智能路灯最大亮度值

uint8_t Light_Inf_Start(void);
void light_bepp(void);
void Light_match(uint8_t ma);
#endif
