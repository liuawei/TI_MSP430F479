#include "motor.h"
//////////////////////////////////////////////////////////////////////////////////	 
//GPIO驱动代码	   								  
////////////////////////////////////////////////////////////////////////////////// 	   
	



//GPIO初始化
void Motor_Init(void)
{
  P1DIR &= ~BIT1;      // Set P1.1 input   正向堵转
  P2DIR &= ~BIT1;      // Set P2.1 input   反向堵转
  
  P4DIR |= BIT0;      // Set P4.0 output  电机传感器开关
  
  P5DIR |= BIT5 + BIT6 + BIT7;      // Set P5.5 P5.6  P5.7 as output  电机控制
	
}



//电机转动 0：停止 1：正转 2：反转
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

//电机转动 次数
u16 Motor_Rot_Num = 0;
void Motor_Rotation_Num(u16 num)
{
  Motor_Rot_Num = num;
  
  if(Motor_Rot_Num)
  {
    if((P1IN & BIT1) || (P2IN & BIT1))
    {
      
    }
    else  //没有归位 强制归位
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




