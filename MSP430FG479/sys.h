#ifndef __SYS_H
#define __SYS_H	
#include <msp430.h>
#include <stdint.h>
//////////////////////////////////////////////////////////////////////////////////	 
//STM32开发板		   
////////////////////////////////////////////////////////////////////////////////// 	 



typedef long  s32;
typedef int s16;
typedef char  s8;

typedef unsigned long u32;
typedef unsigned int u16;
typedef unsigned char  u8;

//0,不支持ucos
//1,支持ucos
#define OS_CRITICAL_METHOD		1		//定义系统文件夹是否支持UCOS


#if OS_CRITICAL_METHOD
#define OS_TICKS_PER_SEC			1000	// 1000/1000 = 1ms 最大1000
#define TaskEventNumMax				16	//定义最大的事件数量
//
typedef void (*OS_TCB_Func[TaskEventNumMax])(void);//定义函数指针
extern OS_TCB_Func Task_Func;


//清除所有任务
void OS_evt_clean_all(void);
///
void OS_evt_timer(void);
//创建事件
u8 OS_evt_set(void EventFunc(),u16 time);
//删除事件
u8 OS_evt_delete(void EventFunc());
//任务运行开始
void OS_Run(void);

#endif

void sys_init(void);
#endif
