#include "adc.h"
#include "math.h"
#include "uart.h"
#include "delay.h"
#include "dac.h"
#include "timer.h"
//////////////////////////////////////////////////////////////////////////////////	 
//DAC驱动代码	   								  
////////////////////////////////////////////////////////////////////////////////// 	   	



float ADC_A0_Data = 0;        
float ADC_A1_Data = 0;        
float ADC_A4_Data = 0;        
float adcval=0;        
u16 adc_cnt=0;        
u32 A0_data=0, A1_data=0, A4_data=0;        
u16 results[3][11];  
u16 resultsbuf[2][Num_of_Results];
u16 j;        
u32 res=0;      
u8 DirectFlag;
u16 PeriodCnt=0;
u16 NotUsed;
u16 Max_value(u16* data);  
u8 ChannelNum=CHANNELC38;
u8 ChannelNumbuf=CHANNELC38;

//ADC初始化
void ADC_Init(void)
{
  volatile unsigned int i;         
  P6SEL |= BIT0+BIT1+BIT3+BIT4;        		// Config SD16 inputs     
  SD16CTL = SD16REFON+SD16SSEL0;                    // 1.2V ref, SMCLK+SD16DIV_1
  SD16INCTL0 = SD16INTDLY_3;
  Setchannel(ChannelNum);
//  SD16INCTL0 |= SD16INCH_0;
//  SD16CCTL0 |= SD16OSR_64 +SD16UNI+SD16BUF_3;                 // Enable interrupt
//  SD16CCTL0 |= SD16OSR_64 +SD16DF+SD16BUF_3;                 // Enable interrupt
//  for(i=0;        i<0x3600;        i++);        
//  _EINT();         
  SD16CCTL0 |= SD16SC;
  SD16CCTL0 &= ~SD16IE;
  // Enable general interrupts
//  __bis_SR_register(LPM0_bits+GIE);                 // Enter LPM0 SD16CCTL0 |= SD16SC;        
    
}


float ADC_Get_A0_Data(void)//信号测量  365nm
{
  return ADC_A0_Data;        
}

float ADC_Get_A1_Data(void)//信号测量  254nm
{
  return ADC_A1_Data;        
}
float ADC_Get_A4_Data(void)//热敏电阻 测量温度
{
  return ADC_A4_Data;        
}


//#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
//#pragma vector=SD16A_VECTOR
//__interrupt void SD16ISR(void)
//#elif defined(__GNUC__)
//void __attribute__ ((interrupt(SD16A_VECTOR))) SD16ISR (void)
//#else
//#error Compiler not supported!
//#endif
u8 index = 0;        
u16 EndValue = 0;        
#pragma vector=SD16A_VECTOR
__interrupt void SD16ISR(void)
{
  //ch = (SD16INCTL0&0x07);        
  switch (SD16IV)
  {
    case 2: break;              // SD16MEM Overflow
    case 4:             // SD16MEM0 IFG      
      break;        
  default: break;        
  }
//  SD16CCTL0 &= ~(SD16IE+SD16SC);                 // Enable interrupt
  SD16CCTL0 &= ~SD16IE;                 // Enable interrupt 
 __bic_SR_register_on_exit(LPM0_bits);        
}

#pragma vector = PORT1_VECTOR     //固定格式，声明中断向量地址，不能变
__interrupt void P1ISR(void) //函数名，可以任意定义
{
  u8 sensor1=(P1IN & BIT1);        
  u8 sensor2=(P2IN & BIT1);  
  if(sensor1==0 && sensor2==0)
  {  
  }
  else if(sensor1==0 && sensor2==2)
  {
  	if(DirectFlag!=2)
  	{
	    DirectFlag=2;   
  	}
  }
  else if(sensor1==2 && sensor2==0)
  {
  	if(DirectFlag!=1)
  	{
	    DirectFlag=1;
  	}
  }
  else if(sensor1==2 && sensor2==2)
  { 
  }
	P1IFG &= ~BIT1;                //初始化清空中断标志位	
}

#pragma vector = PORT2_VECTOR     //固定格式，声明中断向量地址，不能变
__interrupt void P2ISR(void) //函数名，可以任意定义
{
  u8 sensor1=(P1IN & BIT1);        
  u8 sensor2=(P2IN & BIT1);  
  if(sensor1==0 && sensor2==0)
  {  
  }
  else if(sensor1==0 && sensor2==2)
  {
  	if(DirectFlag!=2)
  	{
	    DirectFlag=2;  
  	}
  }
  else if(sensor1==2 && sensor2==0)
  {
  	if(DirectFlag!=1)
  	{
	    DirectFlag=1;
  	}
  }
  else if(sensor1==2 && sensor2==2)
  { 
  }
  P1IFG &= ~BIT1;                //初始化清空中断标志位
}

void Setchannel(u8 ch)
{
    u16 i;
   SD16CCTL0 |=  SD16IE ;        
   SD16CCTL0 |=  SD16SC;        // ADC开始转换
   SD16INCTL0 &= 0xF8;        //清除ADC通道
   switch(ch)
   {
   case 0: SD16INCTL0 |= SD16INCH_0;        break;        //选择通道0 
   case 1: SD16INCTL0 |= SD16INCH_1;        break;        //选择通道1
   case 2: SD16INCTL0 |= SD16INCH_4;        break;        //选择通道4
   default: break;        
   }
   ChannelNumbuf=ChannelNum;  
   index = 0; 
   SD16CCTL0 |= SD16OSR_64 + SD16IE+SD16UNI+SD16BUF_3;                 // Enable interrupt
    for(i=0;        i<0x3600;        i++);        
    _EINT();         
}
void ENSD16IE()
{
   SD16CCTL0 |=  SD16IE ;        
   SD16CCTL0 |=  SD16SC;        // ADC开始转换
}
void DisSD16IE()
{
  SD16CCTL0 &=~SD16SC;        // ADC停止转换
  //SD16INCTL0 &= 0xF8;        //清除ADC通道
  SD16CCTL0 &=~SD16IE;        // ADC关闭中断 
}

//void value_printf()
//{
//  if(adc_cnt>=Num_of_Results)
//     {
//        
//        for(j=0;        j<Num_of_Results;        j++)
//         {
//           printf("j=%d,adc=%5.1f \r\n",j,((float)temp123[j]*1200/65536+600));        //+600adc测量范围  1.2V/2 ~ 1.2V 之间    
//          }
//        adc_cnt = 0;              
//     }
//}
void value()
{
//  if(adc_cnt>=Num_of_Results)
//            {
//              for(j=0;        j<Num_of_Results;        j++)
//              {
//                 if(results[j]>results[j+1])
//                   results[j+1]=results[j];        
//              }
//              printf("adc=%5.1f \r\n",((float)results[Num_of_Results-2]*1200/65536));        //+600adc测量范围  1.2V/2 ~ 1.2V 之间
//            }

}
 u16 tmp[Num_of_Results];        
u16 Max_value(u16 * data)
{
  u32 temp=0;        
  u16 ret,i=0;        

  while(i<Num_of_Results)
  {
    tmp[i]=(u16)(data[i]*(float)600/32768);        
    temp=((data[i]>temp) ? data[i]:temp);           
    i++;        
  }
  ret =temp*(float)1200/65536;        
  return ret;        
}
/*****************************************************
*   函数功能：峰峰值计算有效值
*   作者版权：klaus 邮箱：xcf2016a@outlook.com
*   功能介绍：传入完整一次周期采样数据和采样点数
*           返回一次有效值计算结果
*   版本：1.0（无改动）
******************************************************/
double PEAK_RMS(u32 *arr,u32 num)
{
    u32 Peak_Max,Peak_Min;        
    double Peak_To_Peak;        
    double Ture_Peak;        
    int i;        
    Peak_Max=Peak_Min=arr[0];        
    for(i=0;        i<num;        i++){
        if(arr[i]>=Peak_Max)Peak_Max=arr[i];        
        if(arr[i]<=Peak_Min)Peak_Min=arr[i];        
    }
    Peak_To_Peak=Peak_Max-Peak_Min;        
    Ture_Peak=3.3*sqrt(Peak_To_Peak)/4096;        
    return Ture_Peak;        
}
/*****************************************************
*   函数功能：真有效值计算
*   作者版权：klaus 邮箱：xcf2016a@outlook.com
*   功能介绍：传入完整一次周期采样数据和采样点数
*           返回一次真有效值计算结果
*   版本：1.0（无改动）
******************************************************/
double TRUE_RMS(u32 *arr,u32 num)
{
    u32 sum_pre,sum_lat;        
    double ave_pre;        
    double ture_value;        
    int i;        
    for(i=0;        i<num;        i++)
            sum_pre+=arr[i];            
    ave_pre=sum_pre/num;        
    for(i=0;        i<num;        i++)
            sum_lat+=pow((double)(arr[i]-ave_pre),2);        
    ture_value=3.3*sqrt(sum_lat/num)/4096;        
    return ture_value;        
}
void MotorInit(void)
{
  P5DIR |=(BIT5+BIT6+BIT7);           //输出模式
  P5OUT &= ~(BIT5+BIT6+BIT7);     
  P4DIR |=BIT0;                  //输出模式
  P4OUT |=BIT0;        
//    P4OUT &= ~BIT0;        
  P1DIR &= ~(BIT1);              //输入模式
//  P1OUT |= BIT1;                 //上拉模式
//  P1OUT &= ~(BIT1); 
  P2DIR &= ~(BIT1);              //输入模式 
//  P2OUT |= BIT1;                 //上拉模式
//  P2OUT &= ~(BIT1); 
//  P1IFG &= ~BIT1;                //初始化清空中断标志位
//  P1IE  |= BIT1;                 //P1.1中断使能
//  P1IES |= BIT1;                 //下降沿产生中断
//  P2IFG &= ~BIT1;                //初始化清空中断标志位
//  P2IE  |= BIT1;                 //P2.1中断使能
//  P2IES |= BIT1;                 //下降沿产生中断
//  _EINT();
    
}

void MotorSet(u8 state)  //state: 0 刹车  1 反转  2 正转 
{
  static u8 stateBuf=0;
  if(stateBuf==state)return;
  switch(state)
  {
     case 0:
       P5OUT |= BIT5;
       P5OUT &= ~(BIT7);        
     break;        
     case 1:
       P5OUT |= BIT5;
       P5OUT |= BIT7;       
       P5OUT &= ~(BIT6); 
       if(stateBuf==2)
       {
         PeriodCnt++;
       }
     break;        
     case 2:
       P5OUT |= BIT5;
       P5OUT |= BIT7;        
       P5OUT |= BIT6;  
       if(stateBuf==1)
       {
         PeriodCnt++;
       }
     break;   
     case 3:
       P5OUT &= ~(BIT5);        
     break;  
     default:  
       P5OUT |= BIT5;
       P5OUT &= ~(BIT7);
     break; 
  }
  stateBuf=state;
}

void MotorDirection(u16* period)//2.1低脉冲  右转到头  1.1低脉冲  左转到头
{
  static u16 errcnt=0;
  u8 sensor1=(P1IN & BIT1);        
  u8 sensor2=(P2IN & BIT1);  
  if(sensor1==0 && sensor2==0)
  {  
  
  }
  else if(sensor1==0 && sensor2==2)
  {
  	if(DirectFlag!=2)
  	{
	    DirectFlag=2;
	    *period+=1;   //不能用*p++  因位*和++优先级相同，结合方向自右而左，相当于*（p++）
	    errcnt=0;     //放外面会影响电机检测
  	}
	
  }
  else if(sensor1==2 && sensor2==0)
  {
  	if(DirectFlag!=1)
  	{
	    DirectFlag=1;
	    *period+=1;
		errcnt=0;    //放外面会影响电机检测
  	}
	
  }
  else if(sensor1==2 && sensor2==2)
  { 
  	errcnt++;
  	if(DirectFlag!=2 && errcnt>=4000)
  	{
  		errcnt=0;
	    DirectFlag=2;
  	}
  }
  
  MotorSet(DirectFlag);
}
u8 RunFlag=0;
void MotorDirectControl(u8 state, u8 direct)  //state: 0 停止  1 启动  direct:  0 反转启动  1 正转启动
{
  switch(state)   
  {
     case 0:
       MotorSet(0); 
       RunFlag=0;
     break; 
     case 1:
       if(RunFlag==0)
       {
         MotorSet(direct+1);
        RunFlag=1;
        DirectFlag=direct+1;
       }
       else
       {
        MotorDirection(&NotUsed);
       }
     break; 
     default:  
       P5OUT |= BIT5;
       P5OUT &= ~(BIT7);
     break; 
  }
}
void MotorDirectPeriodControl(u8* state, u8 direct, u16 cycle) //state: 0 停止  1 启动  direct:  0 反转启动  1 正转启动  cycle:来回刷的次数
{
//  if((PeriodCnt%2==0)&&(PeriodCnt/2>=cycle))
//  printf("Rev %d\r\n",PeriodCnt);
  if(PeriodCnt>=2*cycle)
  {
     PeriodCnt=2*cycle;
     MotorDirectControl(0,direct);
	 *state =0;
  }
  else
  {
    MotorDirectControl(1,direct);
  }
}