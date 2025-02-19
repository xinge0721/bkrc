
#ifndef __POWER_CHECK_H
#define __POWER_CHECK_H

#include "sys.h"
void Electricity_Init(void);
void Parameter_Init(void) ;
uint16_t  Get_Electricity(uint8_t times );
void Power_Check(void);

extern uint8_t Electric_Buf[2];

#endif //__POWER_CHECK_H__


