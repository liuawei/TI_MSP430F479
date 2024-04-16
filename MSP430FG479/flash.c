#include "flash.h"
//////////////////////////////////////////////////////////////////////////////////	 
//DAC驱动代码	   								  
////////////////////////////////////////////////////////////////////////////////// 	   
	
u8 FLASH_BUF[192];//最多是64字节

//从指定地址开始写入指定长度的数据
//WriteAddr:起始地址
//pBuffer:数据指针
//NumToWrite:字节数(就是要写入的8位数据的个数.)
void FLASH_Write(u8 WriteAddr,u8 *pBuffer,u8 NumToWrite)
{
  unsigned int i,j;
  unsigned char *Flash_ptr;                          // Flash pointer
  
  //if(NumToWrite > 256)NumToWrite = 256;
  Flash_ptr = (unsigned char *)0x1000;               // Initialize Flash pointer
  for (i = 0; i < 192; i++)
  {
    FLASH_BUF[i] = *(Flash_ptr+i);             // Write value to flash
  }
  for (i = 0; i < NumToWrite; i++)
  {
    FLASH_BUF[WriteAddr+i] = pBuffer[i];              // Write value to flash
  }
  /////////////////////////////////////////////////////////////
  for(j=0;j<3;j++)
  {
    switch(j)
    {
      case 0: Flash_ptr = (unsigned char *)0x1000; break;
      case 1: Flash_ptr = (unsigned char *)0x1040; break;
      case 2: Flash_ptr = (unsigned char *)0x1080; break;
      case 3: Flash_ptr = (unsigned char *)0x10C0; break;
    }
    /////////////////////////////////////////////////////////
    FCTL3 = FWKEY;                            // Clear Lock bit
    FCTL1 = FWKEY + ERASE;                    // Set Erase bit
    _DINT();//关中断
    *Flash_ptr = 0;                           // Dummy write to erase Flash seg
    while(FCTL3 & BUSY);                    //如果处于忙状态，则等待
    FCTL1 = FWKEY + WRT;                      // Set WRT bit for write operation

    for (i = 0; i < 64; i++)
    {
      *Flash_ptr++ = FLASH_BUF[i+j*64];            // Write value to flash
    }
    while(FCTL3 & BUSY);                    //如果处于忙状态，则等待
    _EINT();//开中断
    FCTL1 = FWKEY;                            // Clear WRT bit
    FCTL3 = FWKEY + LOCK;                     // Set LOCK bit
  }
}

//读取指定地址的字节(8位数据)
//faddr:读地址
//返回值:对应数据.
u8 FLASH_ReadByte(u16 faddr)
{
  return *(char *)faddr; 
}

//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToWrite:字节数
void FLASH_Read(u8 ReadAddr,u8 *pBuffer,u8 NumToRead)   	
{
  u8 i;
  for(i=0;i<NumToRead;i++)
  {
    pBuffer[i]=FLASH_ReadByte(ReadAddr+0x1000+i);//读取1个字节.
  }
}








