#include "at.h"
#include "uart.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "flash.h"
#include "dac.h"
//////////////////////////////////////////////////////////////////////////////////	 
//AT						  
////////////////////////////////////////////////////////////////////////////////// 
char HexToChar(u8 temp)
{
  switch(temp)
  {
    case 0: return '0';
    case 1: return '1';
    case 2: return '2';
    case 3: return '3';
    case 4: return '4';
    case 5: return '5';
    case 6: return '6';
    case 7: return '7';
    case 8: return '8';
    case 9: return '9';
    case 10: return 'A';
    case 11: return 'B';
    case 12: return 'C';
    case 13: return 'D';
    case 14: return 'E';
    case 15: return 'F';
    default: return 0;
  }
}

//MM32复位
void System_PowerRestart(void)
{	    
	
  WDTCTL = WDTPW|0xff00;
}

//串口接收到AT命令解析
void AT_CMD_Analysis(u8 *temp)
{
  u8 valu[8]={0x78,0x56,0x34,0x12};
  u16 data;
  if(!strcmp((char *)temp,"ATWST\r\n"))	
  {
    System_PowerRestart();	//重启设备
  }
  else if(!strncmp((char *)temp,"ATDAC=",6))//ADC校准
  {
    data = atoi((const char *)(temp+6));
    if(data>0x7FF)data=0x7FF;
    DAC_Set(data);//配置DAC
    Uart_Send_Str("ATDAC=OK\r\n");
  }
  else if(!strcmp((char *)temp,"ATSLEEP\r\n"))	//扫描当前ID
  {
    FLASH_Write(2,valu,4);
    Uart_Send_Str("ATSLEEP=OK\r\n");
  }
  else if(!strcmp((char *)temp,"ATSTBY\r\n"))	//扫描当前ID
  {
    
    FLASH_Read(0,valu,4);
    UART_485_TX;
    printf("VLAU=%02X%02X%02X%02X\r\n",valu[0],valu[1],valu[2],valu[3]);
    UART_485_RX;
  }
  else if(!strncmp((char *)temp,"ATBVV=",6))//ADC校准
  {
    data = atoi((const char *)(temp+6));
    valu[0]=0x12;valu[1]=0x34;valu[2]=0x56;valu[3]=0x78;
    FLASH_Write(data,valu,4);
    Uart_Send_Str("ATBVV=OK\r\n");
  }
  else if(!strncmp((char *)temp,"ATADJ=",6))//ADC校准
  {
    data = atoi((const char *)(temp+6));
    FLASH_Read(data,valu,8);
    UART_485_TX;
    printf("VLAU=%02X%02X%02X%02X %02X%02X%02X%02X\r\n",
           valu[0],valu[1],valu[2],valu[3],valu[4],valu[5],valu[6],valu[7]);
    UART_485_RX;
  }
  else if(!strcmp((char *)temp,"ATCOPR\r\n"))	
  {
    Uart_Send_Str("GongXiang\r\n");
  }
}

void VcmpInit(void)
{
  CACTL1 = CAON+CAREF_2+CARSEL;             // Enable Comp, ref = 0.5*Vcc
  CACTL2 = P2CA0;                           // Pin to CA0
  CAPD |= CAPD6;                            // Port pin buffer disable
  P4DIR |= 0x40;                            // Set P4.6 to output direction
}

char CmpRst(void)
{
  char ret=0;
  if (CACTL2&0x01)
  {
    ret=1;
  }
  else
  {
    ret=-1;
  }
  return ret;
}

