#include "dac.h"
#include "adc.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//DAC驱动代码	   								  
////////////////////////////////////////////////////////////////////////////////// 	   
	

//DAC初始化
void DAC_Init(void)
{
  P5DIR |=(BIT3+BIT4);           //输出模式     
  DAC_SetChannle(ChannelNum);
  
  P1DIR |=BIT4;           //输出模式
  P1OUT &= ~BIT4;     
  P1OUT |= BIT4;
  
  SD16CTL |= SD16REFON;				  // Internal 1.2V ref onDAC12IR 
  DAC12_1CTL = DAC12SREF_3 + DAC12AMP_5 + 	
  		DAC12OPS + DAC12ENC + DAC12CALON; // SD16 Ref,   DAC12IR + 
//  DAC12_1DAT = 0x0FF; // 0.6V  0x7FF; 
  //__bis_SR_register(LPM4_bits);  	          //Enter Low Power Mode 4    
  //__bic_SR_register_on_exit(LPM4_bits);         //Exit Low Power Mode 4 
}
 
// ox7ff-1.8V 
//设置DAC参数
void DAC_Set(u16 temp)
{
  DAC12_1DAT = temp;  // 0.6V	
}


void DAC_SetChannle(u8 data)
{
  if(data==CHANNELC38)
  {
    P5OUT &= ~(BIT3+BIT4);     
    P5OUT |= BIT3;
  }
  else if(data==CHANNELC46)
  {
    P5OUT &= ~(BIT3+BIT4);     
    P5OUT |= BIT4;
  }
}

