#ifndef __TIMER_H
#define __TIMER_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//timer驱动代码	   							  
////////////////////////////////////////////////////////////////////////////////// 

#define ONESECOND                      1
#define ONEMINUTE                      60*ONESECOND
#define ONEHOUR                        60*ONEMINUTE
#define MOTORINTERVAL                  1*ONEMINUTE
#define SAMPLEINTERVAL                 1*ONEMINUTE

#define SAMPLERUN                      (0)
#define MOTORRUN                       (1)

//#define LEFTTORIGHT                    (0)
//#define RIGHTTOLEFT                    (1)


extern s16 MotorIntervalTime ;
extern s16 MotorIntervalTimeBuf ;
extern s16 SampleIntervalTime;

extern u8 MotorState;
extern u8 taskswitch;
extern u8 Sequencenum;
void Timer_Init(void);
void takeSample(void);
void MotorTask(void);
void SampleTask(void);


/***********进入高频状态**************/
void Clock_Set(u8 num);
#endif




