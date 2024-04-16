#ifndef __UART_H
#define __UART_H
#include "stdio.h"	
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//串口2初始化		   
////////////////////////////////////////////////////////////////////////////////// 	

#define UART_485_RX    P2OUT &= ~(BIT6)
#define UART_485_TX    P2OUT |= BIT6


//串口2初始化		   
#define UART_REC_LEN  			64  					//定义最大接收字节数 32
extern u8  UART_RX_BUF[UART_REC_LEN]; 	//接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 UART_RX_CNT;         					//接收状态标记	





//如果想串口中断接收，请不要注释以下宏定义
void uart_init(void);
void Uart_Send_Data(u8 ch);
//串口0发送数据函数
void Uart_Send_Str(char ch[]);
//串口接收数据处理任务
void Uart_Receive_Data_Task(void);
#endif


