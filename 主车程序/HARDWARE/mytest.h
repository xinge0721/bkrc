#ifndef _KEY_H
#define _KEY_H
#include "stm32f4xx.h"


void Forward(uint8_t LSpeed,uint8_t RSpeed,uint16_t MP);
void Back_1(uint8_t LSpend,uint8_t RSpend,uint16_t MP);
void Turn_Left(int LSpeed,int RSpeed);
void Turn_Right(int LSpeed,int RSpeed);
void Tracking_Forward(uint8_t LSpeed,uint8_t RSpeed) ;
void Car_Time_Track(uint8_t LSpeed,uint8_t RSpeed, uint16_t tims)	;
void Tracking_Back(uint8_t LSpeed,uint8_t RSpeed) ;
void turn_ppmd(uint8_t LSpend,uint8_t RSpend,uint16_t MP)	;
void turn_llmd(uint8_t LSpend,uint8_t RSpend,uint16_t MP)	;
void Track_duoy_RFID(int LSpeed,int RSpeed);
#endif


