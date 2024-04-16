#include <msp430.h>
#include "sys.h"
#include "delay.h"
#include "uart.h"	
#include "gpio.h"
#include "dac.h"
#include "adc.h"
#include "flash.h"
#include "at.h"
#include "modbus.h"
#include "timer.h"

const  unsigned char cRevisionDate[12] = __DATE__;
const  unsigned char cRevisionTime[16] = __TIME__;

void TEST()
{
  UART_485_TX;

  printf("Test!");
  while(UCA0STAT & UCBUSY);
  UART_485_RX;
}


int main(void)
{
  //Clock_Set(0x79);   //8MHZ
  sys_init();//ʱ�ӳ�ʼ��
  Gpio_Init();//IO��ʼ��
  delay_init(); //��ʱ��ʼ��
  uart_init();  //���ڳ�ʼ��

//  VcmpInit();
  ADC_Init();//ADC��ʼ��  
  MotorInit();
  DAC_Init();//DAC��ʼ��
  UART_485_TX;
  Timer_Init();    
  OS_evt_set(MotorTask,200);
  
  for(;;)
  {
//   if(switchflag==0)
//       task_1s();
//    else
//    {
////      MotorDirectPeriodControl(MotorState,0,5);
//      OS_Run();
//      //delay_nms(100);
//    }   
  }
}





































