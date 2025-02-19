#include "intelligent_street_lamp.h"
#include "infrared.h"
#include "data_base.h"
#include "BH1750.h"
#include "cba.h"
/***新增变量***/
uint8_t adjustment_Check = 0;
uint8_t adjustment_Count = 0;//记录智能路灯最大亮度值
uint8_t gzqdvalue = 0;
uint16_t tmp_0 = 0;///+1档后的光度值
uint16_t tmp_1 = 0;//初始光度值

uint8_t returnvalue = 0;//智能路灯当前挡位
uint8_t mubiaovalue =0;//智能路灯最终挡位，测试用的，具体数值可以修改
/***********/

/***************************************************************
** 功能：     智能路灯光源档位+1 
** 参数：	  	无
** 返回值：   无
** 说明：     主车通过红外控制智能路灯标志物光源档位+1
****************************************************************/
void Street_light_one(void)
{
    Infrared_Send(H_1,4);
    delay_ms(100);
}
/***************************************************************
** 功能：     智能路灯光源档位+2 
** 参数：	  	无
** 返回值：   无
** 说明：     主车通过红外控制智能路灯标志物光源档位+2
****************************************************************/
void Street_light_two(void)
{   
    Infrared_Send(H_2,4);
    delay_ms(100);  
}
/***************************************************************
** 功能：     智能路灯光源档位+3
** 参数：	  	无
** 返回值：   无
** 说明：     主车通过红外控制智能路灯标志物光源档位+3
****************************************************************/
void Street_light_three(void)
{    
    Infrared_Send(H_3,4);
    delay_ms(100);   
}


/*****************************************************************
 * 函 数 名 ：	获取初始光度挡位
 * 参    数 ：	无
 * 返 回 值 ：	gear_init -> 初始挡位值
 * 简     例：

*****************************************************************/
uint8_t Light_Inf_Start(void)
{
	uint8_t i;
	uint8_t gear_init = 0;	// 初始挡位值
	uint16_t array[2];		//缓存自学习的光档位数组
	
		delay_ms(100);
		array[0] = Get_Bh_Value();		//光强度传感器	,初始光度值
		for(i=0; i<4; i++)
		{
			gear_init++;
			Street_light_one();//智能路灯+1档
			delay_ms(500);
			delay_ms(500);
			array[1] = Get_Bh_Value();		//光强度传感器,+1光度值
			if (array[0] < array[1]) //+1的光度值大于初始光度值则不断替换初始光度值进行下一次光度测试
			{
				array[0] = array[1];
				array[1] = 0;
			}
			else                    //+1的光度值小于等于初始光度值则结束光度测试
			{
				gear_init = 5 - gear_init;
				break;
			}
		}
	return gear_init;
}

//功能：获取初始挡位并蜂鸣器响几声，用于调试
void light_bepp(void)
{
			returnvalue=Light_Inf_Start();
				if(returnvalue == 1)
				{
					MP_SPK = 1;
					delay_ms(500);
					MP_SPK = 0;
				}
				if(returnvalue == 2)
				{
					MP_SPK = 1;
					delay_ms(500);
					MP_SPK = 0;
					delay_ms(500);
					MP_SPK = 1;
					delay_ms(500);
					MP_SPK = 0;
				}
				if(returnvalue == 3)
				{
					MP_SPK = 1;
					delay_ms(500);
					MP_SPK = 0;
					delay_ms(500);
					MP_SPK = 1;
					delay_ms(500);
					MP_SPK = 0;
					delay_ms(500);
					MP_SPK = 1;
					delay_ms(500);
					MP_SPK = 0;
				}
				if(returnvalue == 4)
				{
					MP_SPK = 1;
					delay_ms(500);
					MP_SPK = 0;
					delay_ms(500);
					MP_SPK = 1;
					delay_ms(500);
					MP_SPK = 0;
					delay_ms(500);
					MP_SPK = 1;
					delay_ms(500);
					MP_SPK = 0;
					delay_ms(500);
					MP_SPK = 1;
					delay_ms(500);
					MP_SPK = 0;
				}
}




//功能：获取初始挡位并调到指定挡位
void Light_match(uint8_t ma)
{
				returnvalue =Light_Inf_Start();
				mubiaovalue = ma;//智能路灯最终挡位，具体数值可以修改
				if(returnvalue == 1 || returnvalue == 2 || returnvalue == 3 || returnvalue == 4)//当前智能路灯挡位
				{
					if(mubiaovalue==1)//智能路灯最终挡位为1则不需要修改
					{
						
					}
					else if(mubiaovalue==2)//智能路灯最终挡位为2,则加1档
					{
						Street_light_one();
						delay_ms(500);
					}
					else if(mubiaovalue==3)//智能路灯最终挡位为3,则加2档
					{
						Street_light_two();
						delay_ms(500);
					}
					else if(mubiaovalue==4)//智能路灯最终挡位为4,则加3档
					{
						Street_light_three();
						delay_ms(500);
					}
				}
}
