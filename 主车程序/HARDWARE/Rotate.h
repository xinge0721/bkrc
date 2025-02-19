#ifndef __ROTATE_H
#define __ROTATE_H
#include "stm32f4xx.h"                  // Device header

void Rotate_show_Inf(char* src, char x, char y);//立体显示标志物显示车牌数据
void Rotate_Dis_Inf(uint16_t dis);//立体显示标志物显示距离信息（单位：ms）
void Stereo_ColorDisplay(uint8_t color);//立体显示标志物显示颜色
void Stereo_ShapeDisplay(uint8_t shape);//立体显示标志物显示形状
void Stereo_LicensePlateDisplay(const uint8_t *num,const uint8_t *location);//立体显示标志物显示车牌和坐标
void Stereo_DistanceDisplay(uint8_t distance);//立体显示标志物显示距离
void Stereo_TrafficDisplay(uint8_t n);//立体显示标志物显示路况
void stereo_u(void);

#endif
