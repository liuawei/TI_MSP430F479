#ifndef __MODBUS_H
#define __MODBUS_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//AT   								  
//////////////////////////////////////////////////////////////////////////////////   	 

//OBD_RT结构体定义
typedef struct
{
  u8   H0500[4];    //0,寄存器1：turb LED值，与365LED相关 寄存器2：cod LED值，与254LED相关
  u8   H0601[16];   //4,Float,原始光强信号，4个。
  u8   H0700[8];    //20,寄存器1：硬件版本  寄存器2：软件版本
  u8   H0800[4];    //28,Float,单独获取温度值(oC)
  u8   H0900[14];   //32,获取探头的序列号SN
  u8   H0D00[4];    //46,Float,内部标定系数K。待研究
  u8   H0F00[8];    //50,Float, 浊度吸光度给COD补偿的计算系数
  u8   H1000[12];   //58,Float,浊度温度补偿系数：a、b、c 
  u8   H1100[8];    //70,Float,COD外部校准参数 
  u8   H1200[4];    //78,Float,单独获取浊度值(NTU) 
  u8   H1700[8];    //82,Float,温度外部校准系数
  u8   H1901[8];    //90,Float,寄存器1、2：温度值(oC)；寄存器3、4：Water IN值
  u8   H1903[8];    //98,Float,寄存器1、2：温度值(oC)；寄存器3、4：Water IO值
  u8   H1A00[4];    //106,Float,pt100信号值 
  u8   H1B00[2];    //110,获取/设置转刷上电自动转动flag
  u8   H1D00[4];    //112,Float,单独获取254nm吸光度值，无量纲或m-1
  u8   H1F00[8];    //116,Float,读取/设置365吸光度和浊度的换算系数
  u8   H2300[4];    //124,Float,设置/读取IN值。与TURB 365LED相关
  u8   H2400[4];    //128,Float,设置/读取IO值。与COD 254LED相关
  u8   H2500[2];    //132,开始测量(默认上电自动测量)
  u8   H2600[16];   //134,Float,寄存器1、2：获取温度值(oC)。寄存器3、4：获取COD值(mg/L)
  u8   H2700[12];   //150,Float,COD温度补偿系数：a、b、c
  u8   H2E00[2];    //162,停止测量
  u8   H2F00[8];    //164,Float,COD吸光度和浓度值换算系数
  u8   H3000[2];    //172,获取/设置设备地址(获取时以0xFF作为固定地址域)
 // u8   H3100[];    //,启动一次转刷清洗
  u8   H3200[2];    //174,获取/设置转刷转动间隔时间(min)
  u8   H3300[4];    //176,Float,设置/获取COD浊度补偿系数Q
  u8   H3400[8];    //180,Float,寄存器1、2：浊度外部校准参数 K 值，寄存器3、4：浊度外部校准参数 B 值
}MODBUS_OBJ;
extern MODBUS_OBJ MODBUS;//188个字节


extern u8 Modbus_485_Addr;

/////////////////////////////////////////////////////////////////
void Modbus_init(void);
u16 Modbus_CRC16(u8 *pcData, u8 iDatalen);
void Modbus_CMD_Analysis(u8 *temp);
void FLASH_Init(void);

#endif


