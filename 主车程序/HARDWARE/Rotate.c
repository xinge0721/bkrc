#include "stm32f4xx.h"                  // Device header
#include "infrared.h"
#include "delay.h"
#include "ultrasonic.h"
#include "canP_HostCom.h"

int size4 = 0;//超声波数值
int size5 = 0;//超声波数值
int size6 = 0;//超声波数值
int max_size1 = 0;//超声波最大值

static uint8_t Infrared[6];         // 红外发送数据缓存



//主车使用超声波测量二维码的距离，发送给从车，让从车来发到立体标志物上面
void stereo_u(void)
{
				Ultrasonic_Ranging();//超声波
				delay_ms(100);
				size4 = (int)dis;
		
				Ultrasonic_Ranging();//超声波
				delay_ms(100);
				size5 = (int)dis;
				
				Ultrasonic_Ranging();//超声波
				delay_ms(100);
				size6 = (int)dis;
		
				max_size1 = size4; // 假设 size1 是最大的初始值

				if (size5 > max_size1) 
				{
					max_size1 = size5; // 如果 size2 比当前最大值大，则更新最大值
				}

				if (size6 > max_size1)
				{
					max_size1 = size6; // 如果 size3 比当前最大值大，则更新最大值
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
			Send_zigbee[0]=0;//清除数组,因为有时候只发了几位
			Send_zigbee[1]=0;
			Send_zigbee[2]=0;
			Send_zigbee[3]=0;
			Send_zigbee[4]=0;
			Send_zigbee[5]=0;
			Send_zigbee[6]=0;
			Send_zigbee[7]=0;

}


/**********************************************************************
 * 函 数 名 ：  立体显示标志物显示车牌数据
 * 参    数 ：  *src    车牌数据（ASICC）
                x,y     坐标信息
 * 返 回 值 ：  无
 * 简    例 ：  Rotate_show_Inf("A123B4",'C','5');
**********************************************************************/
void Rotate_show_Inf(char* src, char x, char y)
{
    Infrared[0] = 0xFF;			// 起始位
    Infrared[1] = 0x20;			// 模式
    Infrared[2] = *(src + 0);	// 数据【1】
    Infrared[3] = *(src + 1);	// 数据【2】
    Infrared[4] = *(src + 2);	// 数据【3】
    Infrared[5] = *(src + 3);	// 数据【4】
    Infrared_Send(Infrared, 6);
    delay_ms(500);
    Infrared[1] = 0x10;			// 模式
    Infrared[2] = *(src + 4);	// 数据【1】
    Infrared[3] = *(src + 5);	// 数据【2】
    Infrared[4] = x;			// 数据【3】
    Infrared[5] = y;			// 数据【4】
    Infrared_Send(Infrared, 6);
    delay_ms(10);
}

/**********************************************************************
 * 函 数 名 ：  立体显示标志物显示车牌数据
 * 参    数 ：  *src    车牌数据（ASICC）
                x,y     坐标信息
 * 返 回 值 ：  无
 * 简    例 ：  Rotate_show_Inf("A123B4",'C','5');
**********************************************************************/
void Bjt_show_Inf(char* src, char x, char y)
{
    Infrared[0] = 0xFF;			// 起始位
    Infrared[1] = 0x07;			// 模式
    Infrared[2] = *(src + 0);	// 数据【1】
    Infrared[3] = *(src + 1);	// 数据【2】
    Infrared[4] = *(src + 2);	// 数据【3】
    Infrared[5] = *(src + 3);	// 数据【4】
    Infrared_Send(Infrared, 6);
    delay_ms(500);
    Infrared[1] = 0x10;			// 模式
    Infrared[2] = *(src + 4);	// 数据【1】
    Infrared[3] = *(src + 5);	// 数据【2】
    Infrared[4] = x;			// 数据【3】
    Infrared[5] = y;			// 数据【4】
    Infrared_Send(Infrared, 6);
    delay_ms(10);
}


/**********************************************************************
 * 函 数 名 ：  立体显示标志物显示距离信息（单位：ms）
 * 参    数 ：  dis  测距信息（四舍五入）
 * 返 回 值 ：  无
 * 简    例 ：  Rotate_Dis_Inf(123);
**********************************************************************/
void Rotate_Dis_Inf(uint16_t dis)
{
    uint16_t csb = dis; //缓存超声波数据值

    csb += 5;   //四舍五入
    Infrared[0] = 0xFF;
    Infrared[1] = 0x11; //显示距离模式
    Infrared[2] = 0x30 + (uint8_t)(csb / 100 % 10); //距离十位--cm
    Infrared[3] = 0x30 + (uint8_t)(csb / 10 % 10); //距离个位--cm
    Infrared[4] = 0x00;
    Infrared[5] = 0x00;
    Infrared_Send(Infrared, 6);
    delay_ms(100);
}

/***************************************************************
** 功能：     立体显示标志物显示颜色
** 参数：	  color 整型，要发送颜色
** 返回值：   无
** 说明：     主车通过红外控制立体显示标志物显示颜色
			  1红色 2绿色 3蓝色 4黄色 5紫色 6青色 7黑色 8白色 
****************************************************************/
void Stereo_ColorDisplay(uint8_t color)//立体显示标志物显示颜色
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
** 功能：     立体显示标志物显示形状
** 参数：	  shape 整型，要发送形状
** 返回值：   无
** 说明：     主车通过红外控制立体显示标志物显示形状
			  1矩形 2圆形 3三角形 4菱形 5梯形 6饼图 7靶图 8条形图
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
** 功能：     立体显示标志物显示车牌和坐标
** 参数：	  num车牌号 location坐标位置
** 返回值：   无
** 说明：     num的格式[Axxxbx]  location的格式[Ax]
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
** 功能：     立体显示标志物显示距离
** 参数：	  distance 距离
** 返回值：   无
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
** 功能：     立体显示标志物显示路况
** 参数：	  n=0 隧道有事故，请绕行 n=1 前方施工，请绕行
** 返回值：   无
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


