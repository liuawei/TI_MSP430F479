#include "modbus.h"
#include "uart.h"
#include "flash.h"
//////////////////////////////////////////////////////////////////////////////////	 
//__MODBUS_H					  
////////////////////////////////////////////////////////////////////////////////// 
u8 Modbus_485_Addr = 1;

MODBUS_OBJ MODBUS;


union ANALYDATA
{
  float temp;
  u32 res;
  u16 TBUF[2];
  u8 BUF[4];
}ANALYDATA;//

//////////////////////////////////////////////////////
void Modbus_init(void)
{
  //u8 i,temp[16];//
  UART_485_TX;
  printf("dddd = %d\n",sizeof(MODBUS));
  UART_485_RX;
  //从FLASH中获取数据
  //FLASH_Read(0,MODBUS.H0500,sizeof(MODBUS));
  //for(i=0;i<4;i++)MODBUS.H0500[i] = temp[i];
}



/////////////////////////////////////////
u16 Modbus_CRC16(u8 *pcData, u8 iDatalen)
{
    u8 i,j;
    u16 crc = 0xFFFF;
    for (j = 0; j < iDatalen; j++)
    {
        crc = crc ^ pcData[j];
        for (i = 0; i < 8; i++)
        {
            if ((crc & 0x0001) > 0)
            {
                crc = crc >> 1;
                crc = crc ^ 0xa001;
            }
            else
                crc = crc >> 1;
        }
    }
    return crc;
}

/////////////////////////////////////////////////////////
void Modbus_Read_Send(u8 addr,u8 cmd,u8 len,u8 *data)
{
    u8 i,j,temp[16];
    u16 crc;
    UART_485_TX;
    i = 0;
    temp[i++] = addr;
    temp[i++] = cmd;
    temp[i++] = len;
    for(j=0;j<len;j++)
    {
      temp[i++] = data[j];
    }
    crc = Modbus_CRC16(temp,i);
    temp[i++] = crc&0xFF;
    temp[i++] = crc>>8;
    for(j=0;j<i;j++)
    {
      Uart_Send_Data(temp[j]);   
    }
    while(UCA0STAT & UCBUSY);
    UART_485_RX;
}
/////////////////////////////////////////////////////////
void Modbus_Write_Send(u8 *data)
{
    u8 i,j,temp[16];
    u16 crc;
    UART_485_TX;
    i = 0;
    for(j=0;j<6;j++)
    {
      temp[i++] = data[j];
    }
    crc = Modbus_CRC16(temp,i);
    temp[i++] = crc&0xFF;
    temp[i++] = crc>>8;
    for(j=0;j<i;j++)
    {
      Uart_Send_Data(temp[j]);   
    }
    //while(UCA0STAT & UCBUSY);
    ////////////////////////////////////////////////
    FLASH_Write(0,MODBUS.H0500,sizeof(MODBUS));
    UART_485_RX;
}
/////////////////////////////////////////////////////////
void Modbus_CMD_Analysis(u8 *temp)
{
  u8 i,addr,cmd,len;
  u16 MessAddr;
  
  addr = temp[0]; cmd = temp[1]; len = temp[5]*2;
  MessAddr = ((u16)temp[2]<<8) | temp[3]; //报文地址
  /////////////////////////////////////////////////////////
  switch(MessAddr)
  {
    case 0x0500: //0,寄存器1：turb LED值，与365LED相关 寄存器2：cod LED值，与254LED相关
      if(len > 4)len = 4;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H0500); break;
        case 0x10: for(i=0;i<len;i++)MODBUS.H0500[i] = temp[i+7]; //数据更新
                    Modbus_Write_Send(temp); break;//整体数据全部重写
      }
      break; 
    case 0x0601: //Float,原始光强信号，4个。
      if(len > 16)len = 16;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H0601); break;
      }
      break; 
    case 0x0700: //,寄存器1：硬件版本  寄存器2：软件版本
      if(len > 8)len = 8;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H0700); break;
        case 0x10: for(i=0;i<len;i++)MODBUS.H0700[i] = temp[i+7];
                    Modbus_Write_Send(temp); break;
      }
      break; 
    case 0x0800: //Float,单独获取温度值(oC)
      if(len > 4)len = 4;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H0800); break;
      }
      break; 
    case 0x0900: //,获取探头的序列号SN
      if(len > 14)len = 14;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H0900); break;
        case 0x10: for(i=0;i<len;i++)MODBUS.H0900[i] = temp[i+7]; 
                    Modbus_Write_Send(temp); break;
      }
      break; 
    case 0x0D00: //Float,内部标定系数K。待研究
      if(len > 4)len = 4;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H0D00); break;
      }
      break; 
    case 0x0F00: //Float, 浊度吸光度给COD补偿的计算系数
      if(len > 8)len = 8;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H0F00); break;
        case 0x10: for(i=0;i<len;i++)MODBUS.H0F00[i] = temp[i+7];
                    Modbus_Write_Send(temp); break;
      }
      break; 
    case 0x1000: //Float,浊度温度补偿系数：a、b、c 
      if(len > 12)len = 12;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H1000); break;
        case 0x10: for(i=0;i<len;i++)MODBUS.H1000[i] = temp[i+7]; 
                    Modbus_Write_Send(temp); break;
      }
      break; 
    case 0x1100: //Float,COD外部校准参数 
      if(len > 8)len = 8;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H1100); break;
        case 0x10: for(i=0;i<len;i++)MODBUS.H1100[i] = temp[i+7]; 
                    Modbus_Write_Send(temp); break;
      }
      break; 
    case 0x1200: //Float,单独获取浊度值(NTU) 
      if(len > 4)len = 4;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H1200); break;
      }
      break;
    case 0x1700: //Float,温度外部校准系数
      if(len > 8)len = 8;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H1700); break;
        case 0x10: for(i=0;i<len;i++)MODBUS.H1700[i] = temp[i+7];
                    Modbus_Write_Send(temp); break;
      }
      break; 
    case 0x1901: //Float,寄存器1、2：温度值(oC)；寄存器3、4：Water IN值
      if(len > 8)len = 8;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H1901); break;
      }
      break;
    case 0x1903: //Float,寄存器1、2：温度值(oC)；寄存器3、4：Water IO值
      if(len > 8)len = 8;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H1903); break;
      }
      break;
    case 0x1A00: //Float,pt100信号值 
      if(len > 4)len = 4;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H1A00); break;
      }
      break;
    case 0x1B00: //,获取/设置转刷上电自动转动flag
      if(len > 2)len = 2;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H1B00); break;
        case 0x10: for(i=0;i<len;i++)MODBUS.H1B00[i] = temp[i+7];
                    Modbus_Write_Send(temp); break;//全部数据更新
      }
    case 0x1D00: //Float,单独获取254nm吸光度值，无量纲或m-1
      if(len > 4)len = 4;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H1D00); break;
      }
      break;
    case 0x1F00: //Float,读取/设置365吸光度和浊度的换算系数
      if(len > 8)len = 8;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H1F00); break;
        case 0x10: for(i=0;i<len;i++)MODBUS.H1F00[i] = temp[i+7];
                    Modbus_Write_Send(temp); break;
      }
      break; 
    case 0x2300: //Float,设置/读取IN值。与TURB 365LED相关
      if(len > 4)len = 4;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H2300); break;
        case 0x10: for(i=0;i<len;i++)MODBUS.H2300[i] = temp[i+7];
                    Modbus_Write_Send(temp); break;
      }
      break; 
    case 0x2400: //Float,设置/读取IO值。与COD 254LED相关
      if(len > 4)len = 4;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H2400); break;
        case 0x10: for(i=0;i<len;i++)MODBUS.H2400[i] = temp[i+7];
                    Modbus_Write_Send(temp); break;
      }
      break;
    case 0x2500: //,开始测量(默认上电自动测量)
      if(len > 2)len = 2;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H2500); break;
      }
      break;
    case 0x2600: //Float,寄存器1、2：获取温度值(oC)。寄存器3、4：获取COD值(mg/L)
      if(len > 16)len = 16;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H2600); break;
      }
      break;
    case 0x2700: //Float,COD温度补偿系数：a、b、c
      if(len > 12)len = 12;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H2700); break;
        case 0x10: for(i=0;i<len;i++)MODBUS.H2700[i] = temp[i+7];
                    Modbus_Write_Send(temp); break;
      }
      break;
    case 0x2E00: //,停止测量
      if(len > 2)len = 2;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H2E00); break;
      }
    case 0x2F00: //Float,COD吸光度和浓度值换算系数
      if(len > 8)len = 8;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H2F00); break;
        case 0x10: for(i=0;i<len;i++)MODBUS.H2F00[i] = temp[i+7]; 
                    Modbus_Write_Send(temp); break;
      }
      break;
    case 0x3000: //,获取/设置设备地址(获取时以0xFF作为固定地址域)
      if(len > 2)len = 2;
      switch(cmd)
      {
        case 0x03: MODBUS.H3000[0] = Modbus_485_Addr;Modbus_Read_Send(addr,cmd,len,MODBUS.H3000); break;
        case 0x10: for(i=0;i<len;i++)MODBUS.H3000[i] = temp[i+7]; Modbus_485_Addr = MODBUS.H3000[0]; 
                    Modbus_Write_Send(temp); break;
      }
      break;
    case 0x3100: //,启动一次转刷清洗
      switch(cmd)
      {
        case 0x10: Modbus_Write_Send(temp); break;
      }
      break;
    case 0x3200: //,获取/设置转刷转动间隔时间(min)
      if(len > 2)len = 2;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H3200); break;
        case 0x10: for(i=0;i<len;i++)MODBUS.H3200[i] = temp[i+7];
                    Modbus_Write_Send(temp); break;
      }
      break;
    case 0x3300: //Float,设置/获取COD浊度补偿系数Q
      if(len > 4)len = 4;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H3300); break;
        case 0x10: for(i=0;i<len;i++)MODBUS.H3300[i] = temp[i+7];
                    Modbus_Write_Send(temp); break;
      }
      break;
    case 0x3400: //Float,寄存器1、2：浊度外部校准参数 K 值，寄存器3、4：浊度外部校准参数 B 值
      if(len > 8)len = 8;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H3400); break;
        case 0x10: for(i=0;i<len;i++)MODBUS.H3400[i] = temp[i+7]; 
                    Modbus_Write_Send(temp); break;
      }
      break;
    
  }

}

