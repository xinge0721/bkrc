#ifndef __VOICEBROADCAST_H
#define __VOICEBROADCAST_H
#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"
extern int tempwendu;
void Voice_BroadCast_SuiJi(void);					//语音标志物播放随机指令
void Voice_BroadCast_ZhiDing(uint8_t num);//语音标志物播放指定指令
void Voice_BroadCast_wendu(void);


extern uint8_t voice_flag;
extern int  voice_flag_time;
extern uint8_t uart6_data;			// USART6 接收数据缓存
extern uint8_t uart6_flag;			// USART6 接收数据时序
extern uint8_t UART6_RxData[8];		// USART6 接收数据缓存
extern uint8_t voice_falg;			// 语音模块返回状态
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void USART6_Config_Lib(void);
void SendData_USART6(uint8_t *Data, uint16_t length);
uint8_t Voice_Drive(void);
void YY_Play_Zigbee(char *p);		// 语音播报标志物播报指定文本信息
void YY_Comm_Zigbee(uint8_t Primary, uint8_t Secondary); // 播报语音控制命令
uint8_t BKRC_Voice_Extern(uint8_t yy_mode)	;	// 语音识别
void BKRC_Voice_Init(void);// 语音识别初始化
uint8_t BKRC_Key(uint8_t num);
#endif
