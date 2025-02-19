#include "intelligent_street_lamp.h"
#include "infrared.h"
#include "data_base.h"
#include "BH1750.h"
#include "cba.h"
/***��������***/
uint8_t adjustment_Check = 0;
uint8_t adjustment_Count = 0;//��¼����·���������ֵ
uint8_t gzqdvalue = 0;
uint16_t tmp_0 = 0;///+1����Ĺ��ֵ
uint16_t tmp_1 = 0;//��ʼ���ֵ

uint8_t returnvalue = 0;//����·�Ƶ�ǰ��λ
uint8_t mubiaovalue =0;//����·�����յ�λ�������õģ�������ֵ�����޸�
/***********/

/***************************************************************
** ���ܣ�     ����·�ƹ�Դ��λ+1 
** ������	  	��
** ����ֵ��   ��
** ˵����     ����ͨ�������������·�Ʊ�־���Դ��λ+1
****************************************************************/
void Street_light_one(void)
{
    Infrared_Send(H_1,4);
    delay_ms(100);
}
/***************************************************************
** ���ܣ�     ����·�ƹ�Դ��λ+2 
** ������	  	��
** ����ֵ��   ��
** ˵����     ����ͨ�������������·�Ʊ�־���Դ��λ+2
****************************************************************/
void Street_light_two(void)
{   
    Infrared_Send(H_2,4);
    delay_ms(100);  
}
/***************************************************************
** ���ܣ�     ����·�ƹ�Դ��λ+3
** ������	  	��
** ����ֵ��   ��
** ˵����     ����ͨ�������������·�Ʊ�־���Դ��λ+3
****************************************************************/
void Street_light_three(void)
{    
    Infrared_Send(H_3,4);
    delay_ms(100);   
}


/*****************************************************************
 * �� �� �� ��	��ȡ��ʼ��ȵ�λ
 * ��    �� ��	��
 * �� �� ֵ ��	gear_init -> ��ʼ��λֵ
 * ��     ����

*****************************************************************/
uint8_t Light_Inf_Start(void)
{
	uint8_t i;
	uint8_t gear_init = 0;	// ��ʼ��λֵ
	uint16_t array[2];		//������ѧϰ�Ĺ⵵λ����
	
		delay_ms(100);
		array[0] = Get_Bh_Value();		//��ǿ�ȴ�����	,��ʼ���ֵ
		for(i=0; i<4; i++)
		{
			gear_init++;
			Street_light_one();//����·��+1��
			delay_ms(500);
			delay_ms(500);
			array[1] = Get_Bh_Value();		//��ǿ�ȴ�����,+1���ֵ
			if (array[0] < array[1]) //+1�Ĺ��ֵ���ڳ�ʼ���ֵ�򲻶��滻��ʼ���ֵ������һ�ι�Ȳ���
			{
				array[0] = array[1];
				array[1] = 0;
			}
			else                    //+1�Ĺ��ֵС�ڵ��ڳ�ʼ���ֵ�������Ȳ���
			{
				gear_init = 5 - gear_init;
				break;
			}
		}
	return gear_init;
}

//���ܣ���ȡ��ʼ��λ���������켸�������ڵ���
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




//���ܣ���ȡ��ʼ��λ������ָ����λ
void Light_match(uint8_t ma)
{
				returnvalue =Light_Inf_Start();
				mubiaovalue = ma;//����·�����յ�λ��������ֵ�����޸�
				if(returnvalue == 1 || returnvalue == 2 || returnvalue == 3 || returnvalue == 4)//��ǰ����·�Ƶ�λ
				{
					if(mubiaovalue==1)//����·�����յ�λΪ1����Ҫ�޸�
					{
						
					}
					else if(mubiaovalue==2)//����·�����յ�λΪ2,���1��
					{
						Street_light_one();
						delay_ms(500);
					}
					else if(mubiaovalue==3)//����·�����յ�λΪ3,���2��
					{
						Street_light_two();
						delay_ms(500);
					}
					else if(mubiaovalue==4)//����·�����յ�λΪ4,���3��
					{
						Street_light_three();
						delay_ms(500);
					}
				}
}
