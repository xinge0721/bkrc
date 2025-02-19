#include "stm32f4xx.h"                  // Device header
#include "infrared.h"
#include "delay.h"
#include "ultrasonic.h"
#include "canP_HostCom.h"

int size4 = 0;//��������ֵ
int size5 = 0;//��������ֵ
int size6 = 0;//��������ֵ
int max_size1 = 0;//���������ֵ

static uint8_t Infrared[6];         // ���ⷢ�����ݻ���



//����ʹ�ó�����������ά��ľ��룬���͸��ӳ����ôӳ������������־������
void stereo_u(void)
{
				Ultrasonic_Ranging();//������
				delay_ms(100);
				size4 = (int)dis;
		
				Ultrasonic_Ranging();//������
				delay_ms(100);
				size5 = (int)dis;
				
				Ultrasonic_Ranging();//������
				delay_ms(100);
				size6 = (int)dis;
		
				max_size1 = size4; // ���� size1 �����ĳ�ʼֵ

				if (size5 > max_size1) 
				{
					max_size1 = size5; // ��� size2 �ȵ�ǰ���ֵ����������ֵ
				}

				if (size6 > max_size1)
				{
					max_size1 = size6; // ��� size3 �ȵ�ǰ���ֵ����������ֵ
				}
			Send_zigbee[0]=0x55;
			Send_zigbee[1]=0x02;
			Send_zigbee[2]=0x22;
			Send_zigbee[3]=(max_size1/100%10)+0x30;
			Send_zigbee[4]=(max_size1/10%10)+0x30;
			Send_zigbee[5]=0x00;
			Send_zigbee[6]=(Send_zigbee[2]+Send_zigbee[3]+Send_zigbee[4]+Send_zigbee[5])%256;
			Send_zigbee[7]=0xbb;
			delay_ms(1);
			Send_ZigbeeData_To_Fifo(Send_zigbee,8);
			delay_ms(100);
			Send_ZigbeeData_To_Fifo(Send_zigbee,8);
			delay_ms(100);
			Send_zigbee[0]=0;//�������,��Ϊ��ʱ��ֻ���˼�λ
			Send_zigbee[1]=0;
			Send_zigbee[2]=0;
			Send_zigbee[3]=0;
			Send_zigbee[4]=0;
			Send_zigbee[5]=0;
			Send_zigbee[6]=0;
			Send_zigbee[7]=0;

}


/**********************************************************************
 * �� �� �� ��  ������ʾ��־����ʾ��������
 * ��    �� ��  *src    �������ݣ�ASICC��
                x,y     ������Ϣ
 * �� �� ֵ ��  ��
 * ��    �� ��  Rotate_show_Inf("A123B4",'C','5');
**********************************************************************/
void Rotate_show_Inf(char* src, char x, char y)
{
    Infrared[0] = 0xFF;			// ��ʼλ
    Infrared[1] = 0x20;			// ģʽ
    Infrared[2] = *(src + 0);	// ���ݡ�1��
    Infrared[3] = *(src + 1);	// ���ݡ�2��
    Infrared[4] = *(src + 2);	// ���ݡ�3��
    Infrared[5] = *(src + 3);	// ���ݡ�4��
    Infrared_Send(Infrared, 6);
    delay_ms(500);
    Infrared[1] = 0x10;			// ģʽ
    Infrared[2] = *(src + 4);	// ���ݡ�1��
    Infrared[3] = *(src + 5);	// ���ݡ�2��
    Infrared[4] = x;			// ���ݡ�3��
    Infrared[5] = y;			// ���ݡ�4��
    Infrared_Send(Infrared, 6);
    delay_ms(10);
}

/**********************************************************************
 * �� �� �� ��  ������ʾ��־����ʾ��������
 * ��    �� ��  *src    �������ݣ�ASICC��
                x,y     ������Ϣ
 * �� �� ֵ ��  ��
 * ��    �� ��  Rotate_show_Inf("A123B4",'C','5');
**********************************************************************/
void Bjt_show_Inf(char* src, char x, char y)
{
    Infrared[0] = 0xFF;			// ��ʼλ
    Infrared[1] = 0x07;			// ģʽ
    Infrared[2] = *(src + 0);	// ���ݡ�1��
    Infrared[3] = *(src + 1);	// ���ݡ�2��
    Infrared[4] = *(src + 2);	// ���ݡ�3��
    Infrared[5] = *(src + 3);	// ���ݡ�4��
    Infrared_Send(Infrared, 6);
    delay_ms(500);
    Infrared[1] = 0x10;			// ģʽ
    Infrared[2] = *(src + 4);	// ���ݡ�1��
    Infrared[3] = *(src + 5);	// ���ݡ�2��
    Infrared[4] = x;			// ���ݡ�3��
    Infrared[5] = y;			// ���ݡ�4��
    Infrared_Send(Infrared, 6);
    delay_ms(10);
}


/**********************************************************************
 * �� �� �� ��  ������ʾ��־����ʾ������Ϣ����λ��ms��
 * ��    �� ��  dis  �����Ϣ���������룩
 * �� �� ֵ ��  ��
 * ��    �� ��  Rotate_Dis_Inf(123);
**********************************************************************/
void Rotate_Dis_Inf(uint16_t dis)
{
    uint16_t csb = dis; //���泬��������ֵ

    csb += 5;   //��������
    Infrared[0] = 0xFF;
    Infrared[1] = 0x11; //��ʾ����ģʽ
    Infrared[2] = 0x30 + (uint8_t)(csb / 100 % 10); //����ʮλ--cm
    Infrared[3] = 0x30 + (uint8_t)(csb / 10 % 10); //�����λ--cm
    Infrared[4] = 0x00;
    Infrared[5] = 0x00;
    Infrared_Send(Infrared, 6);
    delay_ms(100);
}

/***************************************************************
** ���ܣ�     ������ʾ��־����ʾ��ɫ
** ������	  color ���ͣ�Ҫ������ɫ
** ����ֵ��   ��
** ˵����     ����ͨ���������������ʾ��־����ʾ��ɫ
			  1��ɫ 2��ɫ 3��ɫ 4��ɫ 5��ɫ 6��ɫ 7��ɫ 8��ɫ 
****************************************************************/
void Stereo_ColorDisplay(uint8_t color)//������ʾ��־����ʾ��ɫ
{
    Infrared[0]=0xFF;
    Infrared[1]=0x13;
    Infrared[2]=color;
    Infrared[3]=0x00;
    Infrared[4]=0x00;
    Infrared[5]=0x00;
    delay_ms(1);
    Infrared_Send(Infrared,6);
    delay_ms(100); 
}
/***************************************************************
** ���ܣ�     ������ʾ��־����ʾ��״
** ������	  shape ���ͣ�Ҫ������״
** ����ֵ��   ��
** ˵����     ����ͨ���������������ʾ��־����ʾ��״
			  1���� 2Բ�� 3������ 4���� 5���� 6��ͼ 7��ͼ 8����ͼ
****************************************************************/
void Stereo_ShapeDisplay(uint8_t shape)
{
    Infrared[0]=0xFF;
    Infrared[1]=0x12;
    Infrared[2]=shape;
    Infrared[3]=0x00;
    Infrared[4]=0x00;
    Infrared[5]=0x00;
    delay_ms(1);
    Infrared_Send(Infrared,6);
    delay_ms(100); 
}
/***************************************************************
** ���ܣ�     ������ʾ��־����ʾ���ƺ�����
** ������	  num���ƺ� location����λ��
** ����ֵ��   ��
** ˵����     num�ĸ�ʽ[Axxxbx]  location�ĸ�ʽ[Ax]
****************************************************************/
void Stereo_LicensePlateDisplay(const uint8_t *num,const uint8_t *location)
{
	  Infrared[0]=0xFF;	
    Infrared[1]=0x20;	
    Infrared[2]=num[0];	
    Infrared[3]=num[1]-'0';				
    Infrared[4]=num[2]-'0';			
    Infrared[5]=num[3]-'0';		
	  delay_ms(1);
    Infrared_Send(Infrared,6);
    delay_ms(100); 	
	
	  Infrared[0]=0xFF;
    Infrared[1]=0x10;
    Infrared[2]=num[4];
    Infrared[3]=num[5]-'0';
    Infrared[4]=location[0];
    Infrared[5]=location[1]-'0';
	  delay_ms(1);
    Infrared_Send(Infrared,6);
    delay_ms(100); 	
}
/***************************************************************
** ���ܣ�     ������ʾ��־����ʾ����
** ������	  distance ����
** ����ֵ��   ��
****************************************************************/
void Stereo_DistanceDisplay(uint8_t distance)
{
	  Infrared[0]=0xFF;
    Infrared[1]=0x11;
    Infrared[2]=distance/10;
    Infrared[3]=distance%10;
    Infrared[4]=0x00;
    Infrared[5]=0x00;
	  delay_ms(1);
    Infrared_Send(Infrared,6);
    delay_ms(100); 	
}
/***************************************************************
** ���ܣ�     ������ʾ��־����ʾ·��
** ������	  n=0 ������¹ʣ������� n=1 ǰ��ʩ����������
** ����ֵ��   ��
****************************************************************/
void Stereo_TrafficDisplay(uint8_t n)
{
	  Infrared[0]=0xFF;
    Infrared[1]=0x14;
    Infrared[2]=n;
    Infrared[3]=0x00;
    Infrared[4]=0x00;
    Infrared[5]=0x00;
	  delay_ms(1);
    Infrared_Send(Infrared,6);
    delay_ms(100); 	
}
void Stereo_InitDisplay()
{
	  Infrared[0]=0xFF;
    Infrared[1]=0x15;
    Infrared[2]=0x01;
    Infrared[3]=0x00;
    Infrared[4]=0x00;
    Infrared[5]=0x00;
	  delay_ms(1);
    Infrared_Send(Infrared,6);
    delay_ms(100); 	
}


