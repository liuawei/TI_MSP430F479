#include "delay.h"
#include "uart.h"
#include "gpio.h"
//////////////////////////////////////////////////////////////////////////////////	 
//ʹ��SysTick����ͨ����ģʽ���ӳٽ��й���
//����delay_us,delay_ms
//********************************************************************************
////////////////////////////////////////////////////////////////////////////////// 	 
//static u8  fac_us=0;//us��ʱ������
//static u16 fac_ms=0;//ms��ʱ������
u16 temp=0;
u16 timeout_1ms=0;
extern u32 SystemCoreClock;
//systick�жϷ�����,ʹ��ucosʱ�õ�
// Basic Timer Interrupt Service Routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=BASICTIMER_VECTOR
__interrupt void basic_timer_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(BASICTIMER_VECTOR))) basic_timer_ISR (void)
#else
#error Compiler not supported!
#endif
{
  //P2OUT ^= 0x02;                            // Toggle P4.6
  temp++;
  if(temp>=256)
  {
    temp=0;
	timeout_1ms=1;
  }
  IFG2 &=~BTIFG;//���жϱ�־
}


//��ʼ���ӳٺ���
//SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
//SYSCLK:ϵͳʱ��
void delay_init()	 
{
   BTCTL = BTSSEL + BT_fCLK2_DIV32 ;            // ACLK/(256*16)
   IE2 |= BTIE;                              // Enable BT interrupt
}								    

//��ʱnus
//nusΪҪ��ʱ��us��.		    								   
void delay_us(u32 nus)
{		
 
}
//nms:Ҫ��ʱ��ms��
void delay_ms(u16 nms)
{	
   for(u16 i=0; i<200; i++)
   {
      for(u16 j=0; j<1000; j++)
      {
        _NOP();
      }
   }
}




