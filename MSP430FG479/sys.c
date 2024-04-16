#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//系统函数代码	   								  
////////////////////////////////////////////////////////////////////////////////// 	   


void sys_init(void)
{
  u16 i;
  WDTCTL = WDTPW + WDTHOLD;//关闭看门狗
  FLL_CTL0 |= XCAP14PF+DCOPLUS;                     // Configure load caps
  SCFQCTL = 121;//F=D(N+1)*fcrystal.  D=2  N=121  f=32768hz   MCLK=8Mhz 
  SCFI0 |= FN_3;//MCLK = SMCLK = 8MHz
  for (i = 0; i < 10000; i++);              // Delay for 32 kHz crystal to
}

#if OS_CRITICAL_METHOD

volatile u16 TaskEvent = 0;//定义事件变量
volatile u8 TaskEventNum = 0;//定义事件变量个数
volatile u16 EventTime[TaskEventNumMax] = {0};		//事件延时时间
volatile u16 EventTimeSave[TaskEventNumMax] = {0};//事件延时时间缓存
//----------------------------------------------------//
//任务事件定义
OS_TCB_Func Task_Func;
//---------------------------------------------------//

//清除所有任务
void OS_evt_clean_all(void)
{
	//printf("OS EVT CNT = %d\r\n",TaskEventNum);
	TaskEventNum = 0;
}

//创建事件
//EventFunc() 事件函数，time定时时间单位5ms
//返回: 0:创建任务失败 1:创建任务成功
u8 OS_evt_set(void EventFunc(),u16 time)
{
	u8 i;
        u16 data;
	if(time) time = time/(1000/OS_TICKS_PER_SEC);
	//判断是否存在任务
	for(i=0;i<TaskEventNum;i++)
	{
		if(Task_Func[i] == EventFunc)
		{
			_DINT();//关中断
			EventTime[i] = time;
			EventTimeSave[i] = time;
			if(time==0)
			{
				TaskEvent |= 0x0001<<i;
			}
			_EINT();//开中断
			return 1;
		}
	}
	//第一次创建任务
	if(TaskEventNum < TaskEventNumMax)
	{
		_DINT();//关中断
		Task_Func[TaskEventNum] = EventFunc;//指向一个函数
		EventTime[TaskEventNum] = time;
		EventTimeSave[TaskEventNum] = time;
		if(time==0)
		{
			data = 0x0001<<TaskEventNum;
                        TaskEvent |= data;
		}
		TaskEventNum++;
		_EINT();//开中断
		return 1;
	}
	else
	{
		return 0;
	}
}
//删除事件
//EventFunc() 事件函数
//返回: 0:删除任务失败 1:删除任务成功
u8 OS_evt_delete(void EventFunc())
{
	u8 i;
	_DINT();//关中断
	for(i=0;i<TaskEventNum;i++)
	{
		if(Task_Func[i] == EventFunc)
		{
			//TaskEvent = (TaskEvent & (~(0xFFFF<<i))) | ((TaskEvent>>1) & (0xFFFF<<i));
			for(;i<(TaskEventNum-1);i++)
			{
				Task_Func[i] = Task_Func[i+1];
				EventTime[i] = EventTime[i+1];
				EventTimeSave[i] = EventTimeSave[i+1];
			}
			TaskEventNum--;
			EventTime[i] = 0;
			EventTimeSave[i] = 0;
			_EINT();//开中断
			return 1;
		}
	}
	_EINT();//开中断
	return 0;
}

//每1000/OS_TICKS_PER_SEC ms进行事件扫描一次
void OS_evt_timer(void)
{
	u8 i;
	for(i=0;i<TaskEventNum;i++)
	{
		if(EventTime[i])
		{
			EventTime[i]--;
			if(EventTime[i]==0)
			{
				TaskEvent |= 0x0001<<i;//定时时间到标定事件
			}
		}
		else if(EventTimeSave[i])//判断防止EventTime[i]时间丢失,通过EventTimeSave[i]恢复
		{
			EventTime[i] = EventTimeSave[i];
		}
	}
}

//系统开始运行
void OS_Run(void)
{
	u8 i;
	for(i=0;i<TaskEventNum;i++)//最大可以创建16个事件
	{
		if(TaskEvent&(0x0001<<i))//判断事件
		{
			TaskEvent &= ~(0x0001<<i);//清除事件
			Task_Func[i]();
		}
	}
}

#endif




