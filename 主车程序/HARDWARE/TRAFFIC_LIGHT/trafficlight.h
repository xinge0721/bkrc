#ifndef __TRAFFICLIGHT_H
#define __TRAFFICLIGHT_H
#include "stm32f4xx.h"
#include "sys.h"

void Recognition_Erweima(void);							//请求A72识别二维码
void Ask_Traffic_Light(void);								//交通灯进入识别模式
void Traffic_Light_Ask_A72(void);						//请求A72识别交通灯
void Recognition_Traffic_Light(void);				//识别交通灯
void Traffic_Light_result(uint8_t result);	//交通灯请求确认识别结果  0x01 红灯 0x02 绿灯 0x03 黄灯
#endif
