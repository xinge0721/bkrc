

#ifndef __UART_A72_H
#define __UART_A72_H

#ifdef __UART_A72_C__
#define GLOBAL
#else
#define GLOBAL extern
#endif

#include "stm32f4xx.h"                  // Device header

GLOBAL void Uart_A72_PortInit(void);
GLOBAL void Hard_Uart_MurtSel(uint8_t c);
GLOBAL void UartA72_Init(void);
GLOBAL void UartA72_TxClear(void);
GLOBAL void UartA72_TxAddChar(uint8_t d);
GLOBAL void UartA72_TxWhileCheck(void);
GLOBAL void UartA72_TxAddStr(uint8_t *p,uint32_t l);
GLOBAL void UartA72_TxStart(void);



#undef GLOBAL



#endif //__UART_A72_H__

