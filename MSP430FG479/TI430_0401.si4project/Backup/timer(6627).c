#include "timer.h"
#include "uart.h"
#include "adc.h"
#include "dac.h"
#include <string.h>
//////////////////////////////////////////////////////////////////////////////////	 
//timer驱动代码	   								  
////////////////////////////////////////////////////////////////////////////////// 	   
u8 MotorState=1;	
s16 MotorIntervalTime = MOTORINTERVAL;
u32 timecnt=0;
u16 actualvalue[2]={0,0};

u16 Sinewave[2][11]=
{
  {1792,2845,3496,3496,2845,1792,738,87,87,738,1792},
  {2167,2359,2478,2478,2359,2167,1974,1855,1855,1974,2167},//c46
};
float Wavefactor=0.5;
u16 FinalWave=0;
u8 Sequencenum=0;
u8 switchflag=0;
//    FinalWave=(u16)(Sinewave[Sequencenum]*Wavefactor);
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A(void) 
{
  if(TA0CTL&TAIFG)
  {
    Sequencenum++;
    TA0CTL&=~TAIFG; //清楚中断标志位    
//    SD16CCTL0 |= SD16IE;
  }
}

void Timer_Init()
{      
//  TA0CCR0 = 8000;                        // 定义中断计数周期1s,时钟频率为8MHZ,8000 / 8M = 1ms
  TA0CCR0 = 104;                           // 定义中断计数周期1s,时钟频率为8MHZ,104/ 8M = 13us
 // TA0CCTL0 = CCIE;                        // TA0CCR0捕获/比较中断寄存器中断使能
  TA0CTL = TASSEL_2 + MC_1 + TACLR;       // TASSEL_2, SMCLK时钟源  MC_1,增计数模式
//   _BIS_SR(LPM3_bits + GIE);               // 进入LPM3低功耗模式,开启总中断
}

void MotorTask(void)
{
  if (MotorIntervalTime >= 0)
  {
      MotorIntervalTime -= ONESECOND;
      if (MotorIntervalTime <= 0)
      {
          MotorIntervalTime = -1;
      }
  }
  if(MotorIntervalTime<0)
  {
//    MotorState = 1;
    PeriodCnt =0;
    MotorIntervalTime = MOTORINTERVAL;
  }
}
u16 storecnt=0;
u8  revnum=0;
void task_1s(void)
{
  if(TA0CTL&TAIFG)
  {
  results[revnum][Sequencenum] = SD16MEM0;
  DAC_Set(Sinewave[ChannelNum][Sequencenum]);
  TA0CTL&=~TAIFG;
  Sequencenum++;
  if(Sequencenum >= 11)
    {
      Sequencenum=0;
      revnum++;
      if(revnum==3)
      revnum=0;
      storecnt++;
      if(storecnt>=(Num_of_Results/11)*1000)
      {
        storecnt=0;
        memcpy(resultsbuf[ChannelNum],&results,66);
        actualvalue[ChannelNum]=Max_value(&results[ChannelNum][0]);
        if(ChannelNum==CHANNELC38)
        {
          ChannelNum=CHANNELC46;    
        }
         
        else if(ChannelNum==CHANNELC46)
        {
          ChannelNum=CHANNELC38;
          switchflag=1;
        }
         
        DAC_SetChannle(ChannelNum);
        Setchannel(ChannelNum);
      }

      //TA0CCTL0 &= ~CCIE;
    }
//  printf("interval time test");
 //   MotorTask();
}
}



