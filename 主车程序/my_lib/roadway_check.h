#ifndef __ROADWAY_H
#define __ROADWAY_H
#include "stm32f4xx.h"

/*��������*/
extern uint8_t Pass_Flag;
extern unsigned char Special_Flag;
void Track_AssistCorrect(uint8_t gd);												//���˸�������
void Pass_Rfid(uint8_t Spend, uint16_t MP);									//ͨ��RFID����
void SpecialPass_Terrain(uint8_t Spend, uint16_t MP); 			//ͨ��������κ���
void Track_Back_Correct(uint8_t gd);												//���ѭ������
/*********/

void Track_Correct1(uint8_t gd);
void Track_Correct(uint8_t gd);
void Back_Track(uint8_t gd);

void Track_Check(void);
void Roadway_Check(void);
void Roadway_Flag_clean(void);
void Roadway_mp_syn(void);
void Roadway_nav_syn(void);
uint16_t Roadway_mp_Get(void);
	
extern uint8_t wheel_L_Flag,wheel_R_Flag;
extern uint8_t wheel_Nav_Flag;
extern uint8_t Go_Flag,Back_Flag;
extern uint8_t Track_Forward_Flag;
extern uint8_t Track_Back_Flag;
extern int LCar_Spend;
extern int RCar_Spend;
extern uint16_t temp_MP;
extern uint16_t temp_Nav;
extern uint8_t Stop_Flag;
extern uint8_t turnflag;
extern uint8_t Line_Flag;					 //��״̬��־λ
extern uint8_t Line_Stop;
extern uint8_t Track_go;
extern uint8_t RFID_Flag;          	           // RFID����־λ
void Control(int L_Spend,int R_Spend);
void trackcheckdistacne(void);
extern uint8_t Roadway_GoBack_Check(void);
void roadway_check_TimInit(uint16_t arr,uint16_t psc);
#endif


