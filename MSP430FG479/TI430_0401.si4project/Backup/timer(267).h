#ifndef __TIMER_H
#define __TIMER_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//timer��������	   							  
////////////////////////////////////////////////////////////////////////////////// 

#define ONESECOND                      1
#define ONEMINUTE                      60*ONESECOND
#define ONEHOUR                        60*ONEMINUTE
#define MOTORINTERVAL                  5*ONEMINUTE;
extern u8 MotorState;
extern u8 switchflag;
extern u8 Sequencenum;
void Timer_Init(void);
void task_1s(void);
void MotorTask(void);

/***********�����Ƶ״̬**************/
void Clock_Set(u8 num);
#endif




