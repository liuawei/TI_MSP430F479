#include "uart.h"	
#include "at.h"
#include "modbus.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//���ڳ�ʼ��		   
//////////////////////////////////////////////////////////////////
u32 app_bin_len = 0;
u32 app_bin_add = 0;



int putchar(int c)
{
  if(c == '\n')
  {
    while(UCA0STAT & UCBUSY);
    UCA0TXBUF = '\r';
  }
  while(UCA0STAT & UCBUSY);
  UCA0TXBUF = c;
  return c;
}

//���ڷ������ݺ���
void Uart_Send_Data(u8 ch)
{
  while(UCA0STAT & UCBUSY);
  UCA0TXBUF = ch;
}
//����0�������ݺ���
void Uart_Send_Str(char ch[])
{
  u8 i;
  UART_485_TX;
  for(i=0;i<200;i++)
  {
    if(ch[i]==0)break;
    Uart_Send_Data(ch[i]);
  }
  UART_485_RX;
}
//���ڳ�ʼ��
void uart_init(void)
{
  P2SEL |= BIT4+BIT5;                       // P2.4,5 = USCI_A0 RXD/TXD
  UCA0CTL1 |= UCSSEL_2;                     // CLK = ACLK
  UCA0BR0 = 0x41;                           // 8000000/9600 - 341
  UCA0BR1 = 0x03;                           //
  UCA0MCTL = 0x06;                          // Modulation
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
  UART_485_RX;
  _EINT();
}


/////////////////////////////////////////////////////////////////////////////////////
u8 UART_RX_BUF[UART_REC_LEN];     //���ջ���,���UART_REC_LEN���ֽ�.
u16 UART_RX_CNT=0; 

//  Echo back RXed character, confirm TX buffer is ready first
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCIA0RX_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0RX_VECTOR))) USCIA0RX_ISR (void)
#else
#error Compiler not supported!
#endif
{
  u8 Res;
  //if(UART_GetITStatus(UART1, UART_IT_RXIEN)  != RESET) 
  {
    Res = UCA0RXBUF;	//��ȡ���յ�������
    if(UART_RX_CNT < UART_REC_LEN)
    {
      UART_RX_BUF[UART_RX_CNT] = Res;
      OS_evt_set(Uart_Receive_Data_Task,50);//��������
      UART_RX_CNT++;
    }
    else
    {
      UART_RX_CNT = 0;
    }
  } 
}


/////////////////////////////////////�����û�����///////////////////////////////////////////////////////////////////////
//���ڽ������ݴ�������
void Uart_Receive_Data_Task(void)
{
  u16 i;
  u8 check;

  IE2 &= 0xFE;    //�رմ��ڽ���
  OS_evt_delete(Uart_Receive_Data_Task);
  //���յ�ATָ��
  if((UART_RX_BUF[0]=='A')&&(UART_RX_BUF[1]=='T'))
  {
    AT_CMD_Analysis(UART_RX_BUF);
  }
  else if(((UART_RX_BUF[0] == Modbus_485_Addr) || (UART_RX_BUF[0] == 0xFF))&&
          ((UART_RX_BUF[1] == 0x03) || (UART_RX_BUF[1] == 0x10)))
  {
    check = UART_RX_CNT - 2;//���ݳ���
    if(Modbus_CRC16(UART_RX_BUF,check) == ((u16)UART_RX_BUF[check+1]<<8 | UART_RX_BUF[check]))
    {
      Modbus_CMD_Analysis(UART_RX_BUF);
    }
  }
  //�������յ���������ϵͳ��λ
  else if((UART_RX_BUF[0]==0x55)&&(UART_RX_BUF[1]==0x55)&&
            (UART_RX_BUF[2]=='#')&&(UART_RX_BUF[3]=='#')&&
            (UART_RX_BUF[13]=='*')&&(UART_RX_BUF[14]=='*')&&
            (UART_RX_BUF[15]==0xAA)&&(UART_RX_BUF[16]==0xAA))//
  {
    //����У���
    for(i=0,check=0;i<8;i++)
    {
      check += UART_RX_BUF[4+i];
    }
    //�ж�У���Ƿ���ȷ
    if(check==UART_RX_BUF[12])
    {
      //System_PowerRestart();
    }
  }
  //��ս��ջ���
  for(i=0;i<UART_REC_LEN;i++)UART_RX_BUF[i]=0;
  UART_RX_CNT = 0;
  IE2 |= UCA0RXIE;   // Enable USCI_A0 RX interrupt	
}























