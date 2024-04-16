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

  printf("OS EVT Test!");
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
  OS_evt_set(MotorTask,20);
//  OS_evt_set(SampleTask,1000);
//  OS_evt_set(TEST,1000);
  
  for(;;)
  {
    if(timeout_1ms==1)
    {
        timeout_1ms=0;
        OS_evt_timer();
        OS_Run();
    }
	if(MotorState==1)
  {
	MotorDirectPeriodControl(&MotorState,0,5);
  }
//   if(taskswitch==SAMPLERUN)
//   	{
//       takeSample();
//   	}
  }
}





































