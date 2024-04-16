#ifndef __ADC_H
#define __ADC_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//DAC驱动代码	   							  
////////////////////////////////////////////////////////////////////////////////// 

//////////////////////////////////////////////////////////////////////
#define  Num_of_Results 	33 //连续转换次数

///////////////////////////////////////////////////////////////////////
void ADC_Init(void);//初始化
float ADC_Get_A0_Data(void);
float ADC_Get_A1_Data(void);
float ADC_Get_A4_Data(void);
void Setchannel(u8 ch);
void DisSD16IE(void);
void ENSD16IE(void);
void value_printf(void);
void value(void);
void MotorDirectControl(u8 state, u8 direct);
void MotorDirectPeriodControl(u8 state, u8 direct, u16 period);
void MotorSet(u8 state) ;
void MotorInit(void);
u16 Max_value(u16 * data);

double TRUE_RMS(u32 *arr,u32 num);
double PEAK_RMS(u32 *arr,u32 num);
extern u8 ChannelNum;
extern u8 ChannelNumbuf;
extern u16 results[3][11]; 
extern u16 resultsbuf[2][Num_of_Results];
extern u8 index;   
extern u16 PeriodCnt;
extern u16 EndValue;

#endif




