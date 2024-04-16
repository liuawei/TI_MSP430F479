#include "motor.h"
//////////////////////////////////////////////////////////////////////////////////	 
//GPIO��������	   								  
////////////////////////////////////////////////////////////////////////////////// 	   
	



//GPIO��ʼ��
void Motor_Init(void)
{
  P1DIR &= ~BIT1;      // Set P1.1 input   �����ת
  P2DIR &= ~BIT1;      // Set P2.1 input   �����ת
  
  P4DIR |= BIT0;      // Set P4.0 output  �������������
  
  P5DIR |= BIT5 + BIT6 + BIT7;      // Set P5.5 P5.6  P5.7 as output  �������
	
}



//���ת�� 0��ֹͣ 1����ת 2����ת
void Motor_Rotation(u8 falg)
{
  switch(falg)
  {
    case 1: 
      P5OUT |= 0x40; //PH 
      P5OUT |= 0x20; //nsleep
      P5OUT |= 0x80; //EN
      break;
    case 2: 
      P5OUT &= ~0x40; //PH 
      P5OUT |= 0x20; //nsleep
      P5OUT |= 0x80; //EN
      break;
    default: 
      P5OUT &= ~0xA0; //nsleep EN OFF
      break;
  }
}

//���ת�� ����
u16 Motor_Rot_Num = 0;
void Motor_Rotation_Num(u16 num)
{
  Motor_Rot_Num = num;
  
  if(Motor_Rot_Num)
  {
    if((P1IN & BIT1) || (P2IN & BIT1))
    {
      
    }
    else  //û�й�λ ǿ�ƹ�λ
      Motor_Rotation(1);
  }
  
}

void Motor_Run_Task(void)
{
  if(Motor_Rot_Num)
  {
    if(P1IN & BIT1)
    {
      Motor_Rot_Num--;
      Motor_Rotation(1);
    }
    else if(P2IN & BIT1)
    {
      Motor_Rot_Num--;
      Motor_Rotation(2);
    }
  }
  else
  {
    Motor_Rotation(0);
  }
}




