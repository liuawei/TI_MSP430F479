#ifndef __UART_H
#define __UART_H
#include "stdio.h"	
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//����2��ʼ��		   
////////////////////////////////////////////////////////////////////////////////// 	

#define UART_485_RX    P2OUT &= ~(BIT6)
#define UART_485_TX    P2OUT |= BIT6


//����2��ʼ��		   
#define UART_REC_LEN  			64  					//�����������ֽ��� 32
extern u8  UART_RX_BUF[UART_REC_LEN]; 	//���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 UART_RX_CNT;         					//����״̬���	





//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(void);
void Uart_Send_Data(u8 ch);
//����0�������ݺ���
void Uart_Send_Str(char ch[]);
//���ڽ������ݴ�������
void Uart_Receive_Data_Task(void);
#endif


