#ifndef __ROTATE_H
#define __ROTATE_H
#include "stm32f4xx.h"                  // Device header

void Rotate_show_Inf(char* src, char x, char y);//������ʾ��־����ʾ��������
void Rotate_Dis_Inf(uint16_t dis);//������ʾ��־����ʾ������Ϣ����λ��ms��
void Stereo_ColorDisplay(uint8_t color);//������ʾ��־����ʾ��ɫ
void Stereo_ShapeDisplay(uint8_t shape);//������ʾ��־����ʾ��״
void Stereo_LicensePlateDisplay(const uint8_t *num,const uint8_t *location);//������ʾ��־����ʾ���ƺ�����
void Stereo_DistanceDisplay(uint8_t distance);//������ʾ��־����ʾ����
void Stereo_TrafficDisplay(uint8_t n);//������ʾ��־����ʾ·��
void stereo_u(void);

#endif
