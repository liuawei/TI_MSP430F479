#ifndef __AT_H
#define __AT_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//AT   								  
//////////////////////////////////////////////////////////////////////////////////   	 

char HexToChar(u8 temp);
//���ڽ��յ�AT�������
void AT_CMD_Analysis(u8 *temp);
//MM32��λ
void System_PowerRestart(void);

void VcmpInit(void);
char CmpRst(void);
#endif

