#ifndef __SYS_H
#define __SYS_H	
#include <msp430.h>
#include <stdint.h>
//////////////////////////////////////////////////////////////////////////////////	 
//STM32������		   
////////////////////////////////////////////////////////////////////////////////// 	 



typedef long  s32;
typedef int s16;
typedef char  s8;

typedef unsigned long u32;
typedef unsigned int u16;
typedef unsigned char  u8;

//0,��֧��ucos
//1,֧��ucos
#define OS_CRITICAL_METHOD		1		//����ϵͳ�ļ����Ƿ�֧��UCOS


#if OS_CRITICAL_METHOD
#define OS_TICKS_PER_SEC			1000	// 1000/1000 = 1ms ���1000
#define TaskEventNumMax				16	//���������¼�����
//
typedef void (*OS_TCB_Func[TaskEventNumMax])(void);//���庯��ָ��
extern OS_TCB_Func Task_Func;


//�����������
void OS_evt_clean_all(void);
///
void OS_evt_timer(void);
//�����¼�
u8 OS_evt_set(void EventFunc(),u16 time);
//ɾ���¼�
u8 OS_evt_delete(void EventFunc());
//�������п�ʼ
void OS_Run(void);

#endif

void sys_init(void);
#endif
