#ifndef __AT_H
#define __AT_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//AT   								  
//////////////////////////////////////////////////////////////////////////////////   	 

char HexToChar(u8 temp);
//串口接收到AT命令解析
void AT_CMD_Analysis(u8 *temp);
//MM32复位
void System_PowerRestart(void);

void VcmpInit(void);
char CmpRst(void);
#endif


