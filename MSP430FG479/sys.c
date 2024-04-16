#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//ϵͳ��������	   								  
////////////////////////////////////////////////////////////////////////////////// 	   


void sys_init(void)
{
  u16 i;
  WDTCTL = WDTPW + WDTHOLD;//�رտ��Ź�
  FLL_CTL0 |= XCAP14PF+DCOPLUS;                     // Configure load caps
  SCFQCTL = 121;//F=D(N+1)*fcrystal.  D=2  N=121  f=32768hz   MCLK=8Mhz 
  SCFI0 |= FN_3;//MCLK = SMCLK = 8MHz
  for (i = 0; i < 10000; i++);              // Delay for 32 kHz crystal to
}

#if OS_CRITICAL_METHOD

volatile u16 TaskEvent = 0;//�����¼�����
volatile u8 TaskEventNum = 0;//�����¼���������
volatile u16 EventTime[TaskEventNumMax] = {0};		//�¼���ʱʱ��
volatile u16 EventTimeSave[TaskEventNumMax] = {0};//�¼���ʱʱ�仺��
//----------------------------------------------------//
//�����¼�����
OS_TCB_Func Task_Func;
//---------------------------------------------------//

//�����������
void OS_evt_clean_all(void)
{
	//printf("OS EVT CNT = %d\r\n",TaskEventNum);
	TaskEventNum = 0;
}

//�����¼�
//EventFunc() �¼�������time��ʱʱ�䵥λ5ms
//����: 0:��������ʧ�� 1:��������ɹ�
u8 OS_evt_set(void EventFunc(),u16 time)
{
	u8 i;
        u16 data;
	if(time) time = time/(1000/OS_TICKS_PER_SEC);
	//�ж��Ƿ��������
	for(i=0;i<TaskEventNum;i++)
	{
		if(Task_Func[i] == EventFunc)
		{
			_DINT();//���ж�
			EventTime[i] = time;
			EventTimeSave[i] = time;
			if(time==0)
			{
				TaskEvent |= 0x0001<<i;
			}
			_EINT();//���ж�
			return 1;
		}
	}
	//��һ�δ�������
	if(TaskEventNum < TaskEventNumMax)
	{
		_DINT();//���ж�
		Task_Func[TaskEventNum] = EventFunc;//ָ��һ������
		EventTime[TaskEventNum] = time;
		EventTimeSave[TaskEventNum] = time;
		if(time==0)
		{
			data = 0x0001<<TaskEventNum;
                        TaskEvent |= data;
		}
		TaskEventNum++;
		_EINT();//���ж�
		return 1;
	}
	else
	{
		return 0;
	}
}
//ɾ���¼�
//EventFunc() �¼�����
//����: 0:ɾ������ʧ�� 1:ɾ������ɹ�
u8 OS_evt_delete(void EventFunc())
{
	u8 i;
	_DINT();//���ж�
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
			_EINT();//���ж�
			return 1;
		}
	}
	_EINT();//���ж�
	return 0;
}

//ÿ1000/OS_TICKS_PER_SEC ms�����¼�ɨ��һ��
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
				TaskEvent |= 0x0001<<i;//��ʱʱ�䵽�궨�¼�
			}
		}
		else if(EventTimeSave[i])//�жϷ�ֹEventTime[i]ʱ�䶪ʧ,ͨ��EventTimeSave[i]�ָ�
		{
			EventTime[i] = EventTimeSave[i];
		}
	}
}

//ϵͳ��ʼ����
void OS_Run(void)
{
	u8 i;
	for(i=0;i<TaskEventNum;i++)//�����Դ���16���¼�
	{
		if(TaskEvent&(0x0001<<i))//�ж��¼�
		{
			TaskEvent &= ~(0x0001<<i);//����¼�
			Task_Func[i]();
		}
	}
}

#endif




