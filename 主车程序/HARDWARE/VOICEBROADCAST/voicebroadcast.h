#ifndef __VOICEBROADCAST_H
#define __VOICEBROADCAST_H
#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"
extern int tempwendu;
void Voice_BroadCast_SuiJi(void);					//������־�ﲥ�����ָ��
void Voice_BroadCast_ZhiDing(uint8_t num);//������־�ﲥ��ָ��ָ��
void Voice_BroadCast_wendu(void);


extern uint8_t voice_flag;
extern int  voice_flag_time;
extern uint8_t uart6_data;			// USART6 �������ݻ���
extern uint8_t uart6_flag;			// USART6 ��������ʱ��
extern uint8_t UART6_RxData[8];		// USART6 �������ݻ���
extern uint8_t voice_falg;			// ����ģ�鷵��״̬
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void USART6_Config_Lib(void);
void SendData_USART6(uint8_t *Data, uint16_t length);
uint8_t Voice_Drive(void);
void YY_Play_Zigbee(char *p);		// ����������־�ﲥ��ָ���ı���Ϣ
void YY_Comm_Zigbee(uint8_t Primary, uint8_t Secondary); // ����������������
uint8_t BKRC_Voice_Extern(uint8_t yy_mode)	;	// ����ʶ��
void BKRC_Voice_Init(void);// ����ʶ���ʼ��
uint8_t BKRC_Key(uint8_t num);
#endif
