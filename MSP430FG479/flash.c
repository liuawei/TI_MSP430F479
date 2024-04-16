#include "flash.h"
//////////////////////////////////////////////////////////////////////////////////	 
//DAC��������	   								  
////////////////////////////////////////////////////////////////////////////////// 	   
	
u8 FLASH_BUF[192];//�����64�ֽ�

//��ָ����ַ��ʼд��ָ�����ȵ�����
//WriteAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:�ֽ���(����Ҫд���8λ���ݵĸ���.)
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
    _DINT();//���ж�
    *Flash_ptr = 0;                           // Dummy write to erase Flash seg
    while(FCTL3 & BUSY);                    //�������æ״̬����ȴ�
    FCTL1 = FWKEY + WRT;                      // Set WRT bit for write operation

    for (i = 0; i < 64; i++)
    {
      *Flash_ptr++ = FLASH_BUF[i+j*64];            // Write value to flash
    }
    while(FCTL3 & BUSY);                    //�������æ״̬����ȴ�
    _EINT();//���ж�
    FCTL1 = FWKEY;                            // Clear WRT bit
    FCTL3 = FWKEY + LOCK;                     // Set LOCK bit
  }
}

//��ȡָ����ַ���ֽ�(8λ����)
//faddr:����ַ
//����ֵ:��Ӧ����.
u8 FLASH_ReadByte(u16 faddr)
{
  return *(char *)faddr; 
}

//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:�ֽ���
void FLASH_Read(u8 ReadAddr,u8 *pBuffer,u8 NumToRead)   	
{
  u8 i;
  for(i=0;i<NumToRead;i++)
  {
    pBuffer[i]=FLASH_ReadByte(ReadAddr+0x1000+i);//��ȡ1���ֽ�.
  }
}








