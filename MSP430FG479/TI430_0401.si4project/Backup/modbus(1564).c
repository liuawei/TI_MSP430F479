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
  //��FLASH�л�ȡ����
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
  MessAddr = ((u16)temp[2]<<8) | temp[3]; //���ĵ�ַ
  /////////////////////////////////////////////////////////
  switch(MessAddr)
  {
    case 0x0500: //0,�Ĵ���1��turb LEDֵ����365LED��� �Ĵ���2��cod LEDֵ����254LED���
      if(len > 4)len = 4;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H0500); break;
        case 0x10: for(i=0;i<len;i++)MODBUS.H0500[i] = temp[i+7]; //���ݸ���
                    Modbus_Write_Send(temp); break;//��������ȫ����д
      }
      break; 
    case 0x0601: //Float,ԭʼ��ǿ�źţ�4����
      if(len > 16)len = 16;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H0601); break;
      }
      break; 
    case 0x0700: //,�Ĵ���1��Ӳ���汾  �Ĵ���2������汾
      if(len > 8)len = 8;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H0700); break;
        case 0x10: for(i=0;i<len;i++)MODBUS.H0700[i] = temp[i+7];
                    Modbus_Write_Send(temp); break;
      }
      break; 
    case 0x0800: //Float,������ȡ�¶�ֵ(oC)
      if(len > 4)len = 4;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H0800); break;
      }
      break; 
    case 0x0900: //,��ȡ̽ͷ�����к�SN
      if(len > 14)len = 14;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H0900); break;
        case 0x10: for(i=0;i<len;i++)MODBUS.H0900[i] = temp[i+7]; 
                    Modbus_Write_Send(temp); break;
      }
      break; 
    case 0x0D00: //Float,�ڲ��궨ϵ��K�����о�
      if(len > 4)len = 4;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H0D00); break;
      }
      break; 
    case 0x0F00: //Float, �Ƕ�����ȸ�COD�����ļ���ϵ��
      if(len > 8)len = 8;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H0F00); break;
        case 0x10: for(i=0;i<len;i++)MODBUS.H0F00[i] = temp[i+7];
                    Modbus_Write_Send(temp); break;
      }
      break; 
    case 0x1000: //Float,�Ƕ��¶Ȳ���ϵ����a��b��c 
      if(len > 12)len = 12;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H1000); break;
        case 0x10: for(i=0;i<len;i++)MODBUS.H1000[i] = temp[i+7]; 
                    Modbus_Write_Send(temp); break;
      }
      break; 
    case 0x1100: //Float,COD�ⲿУ׼���� 
      if(len > 8)len = 8;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H1100); break;
        case 0x10: for(i=0;i<len;i++)MODBUS.H1100[i] = temp[i+7]; 
                    Modbus_Write_Send(temp); break;
      }
      break; 
    case 0x1200: //Float,������ȡ�Ƕ�ֵ(NTU) 
      if(len > 4)len = 4;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H1200); break;
      }
      break;
    case 0x1700: //Float,�¶��ⲿУ׼ϵ��
      if(len > 8)len = 8;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H1700); break;
        case 0x10: for(i=0;i<len;i++)MODBUS.H1700[i] = temp[i+7];
                    Modbus_Write_Send(temp); break;
      }
      break; 
    case 0x1901: //Float,�Ĵ���1��2���¶�ֵ(oC)���Ĵ���3��4��Water INֵ
      if(len > 8)len = 8;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H1901); break;
      }
      break;
    case 0x1903: //Float,�Ĵ���1��2���¶�ֵ(oC)���Ĵ���3��4��Water IOֵ
      if(len > 8)len = 8;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H1903); break;
      }
      break;
    case 0x1A00: //Float,pt100�ź�ֵ 
      if(len > 4)len = 4;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H1A00); break;
      }
      break;
    case 0x1B00: //,��ȡ/����תˢ�ϵ��Զ�ת��flag
      if(len > 2)len = 2;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H1B00); break;
        case 0x10: for(i=0;i<len;i++)MODBUS.H1B00[i] = temp[i+7];
                    Modbus_Write_Send(temp); break;//ȫ�����ݸ���
      }
    case 0x1D00: //Float,������ȡ254nm�����ֵ�������ٻ�m-1
      if(len > 4)len = 4;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H1D00); break;
      }
      break;
    case 0x1F00: //Float,��ȡ/����365����Ⱥ��ǶȵĻ���ϵ��
      if(len > 8)len = 8;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H1F00); break;
        case 0x10: for(i=0;i<len;i++)MODBUS.H1F00[i] = temp[i+7];
                    Modbus_Write_Send(temp); break;
      }
      break; 
    case 0x2300: //Float,����/��ȡINֵ����TURB 365LED���
      if(len > 4)len = 4;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H2300); break;
        case 0x10: for(i=0;i<len;i++)MODBUS.H2300[i] = temp[i+7];
                    Modbus_Write_Send(temp); break;
      }
      break; 
    case 0x2400: //Float,����/��ȡIOֵ����COD 254LED���
      if(len > 4)len = 4;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H2400); break;
        case 0x10: for(i=0;i<len;i++)MODBUS.H2400[i] = temp[i+7];
                    Modbus_Write_Send(temp); break;
      }
      break;
    case 0x2500: //,��ʼ����(Ĭ���ϵ��Զ�����)
      if(len > 2)len = 2;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H2500); break;
      }
      break;
    case 0x2600: //Float,�Ĵ���1��2����ȡ�¶�ֵ(oC)���Ĵ���3��4����ȡCODֵ(mg/L)
      if(len > 16)len = 16;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H2600); break;
      }
      break;
    case 0x2700: //Float,COD�¶Ȳ���ϵ����a��b��c
      if(len > 12)len = 12;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H2700); break;
        case 0x10: for(i=0;i<len;i++)MODBUS.H2700[i] = temp[i+7];
                    Modbus_Write_Send(temp); break;
      }
      break;
    case 0x2E00: //,ֹͣ����
      if(len > 2)len = 2;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H2E00); break;
      }
    case 0x2F00: //Float,COD����Ⱥ�Ũ��ֵ����ϵ��
      if(len > 8)len = 8;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H2F00); break;
        case 0x10: for(i=0;i<len;i++)MODBUS.H2F00[i] = temp[i+7]; 
                    Modbus_Write_Send(temp); break;
      }
      break;
    case 0x3000: //,��ȡ/�����豸��ַ(��ȡʱ��0xFF��Ϊ�̶���ַ��)
      if(len > 2)len = 2;
      switch(cmd)
      {
        case 0x03: MODBUS.H3000[0] = Modbus_485_Addr;Modbus_Read_Send(addr,cmd,len,MODBUS.H3000); break;
        case 0x10: for(i=0;i<len;i++)MODBUS.H3000[i] = temp[i+7]; Modbus_485_Addr = MODBUS.H3000[0]; 
                    Modbus_Write_Send(temp); break;
      }
      break;
    case 0x3100: //,����һ��תˢ��ϴ
      switch(cmd)
      {
        case 0x10: Modbus_Write_Send(temp); break;
      }
      break;
    case 0x3200: //,��ȡ/����תˢת�����ʱ��(min)
      if(len > 2)len = 2;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H3200); break;
        case 0x10: for(i=0;i<len;i++)MODBUS.H3200[i] = temp[i+7];
                    Modbus_Write_Send(temp); break;
      }
      break;
    case 0x3300: //Float,����/��ȡCOD�ǶȲ���ϵ��Q
      if(len > 4)len = 4;
      switch(cmd)
      {
        case 0x03: Modbus_Read_Send(addr,cmd,len,MODBUS.H3300); break;
        case 0x10: for(i=0;i<len;i++)MODBUS.H3300[i] = temp[i+7];
                    Modbus_Write_Send(temp); break;
      }
      break;
    case 0x3400: //Float,�Ĵ���1��2���Ƕ��ⲿУ׼���� K ֵ���Ĵ���3��4���Ƕ��ⲿУ׼���� B ֵ
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

