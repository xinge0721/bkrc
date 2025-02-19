#ifndef __SYN7318_H
#define __SYN7318_H
#include "stm32f4xx.h"
#include "sys.h"

#define SYN7318RESET PBout(5)

void Syn7318_Up(uint8_t num);
void SYN7318_Init(void);
void SYN_TTS(char *Pst);
void Start_ASR(char Dict);
//void SYN_7318_One_test(uint8_t mode,uint8_t num);  // mode 模式测试 1 随机指令 0 指定指令
void Test(void);

#endif

