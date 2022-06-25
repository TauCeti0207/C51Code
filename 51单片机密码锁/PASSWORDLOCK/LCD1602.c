#include "LCD1602.h"


void LCMDelay(int ms)								//LCD1206 delay.
{
	unsigned int i,cnt;
	cnt = 140 * ms;
	for(i=0;i<cnt;i++);
}

unsigned char LCMReadState(void)		//read LCD1206 busy flage bit.1:busy.0:idle.
{
	unsigned char state;
	LCM_E = 0;
	LCM_RS = 0;
	LCM_RW = 1;
	LCM_E = 1;
	_nop_();
	_nop_();
	state = LCM_DB;
	LCM_E = 0;
	return state;
}

void LCMClear(void)									//LCD1602 clean all screen. 
{
	LCMDelay(1);
	LCM_E = 0;
	LCM_RS = 0;
	LCM_RW = 0;
	LCM_DB = 0x01;
	LCM_E = 1;
	_nop_();
	_nop_();
	LCM_E = 0;
	LCMDelay(1);
}
void LCMWriteCmd(unsigned char cmd)	//write command to LCD1206.
{
	LCMDelay(1);
	LCM_E = 0;
	LCM_RS = 0;
	LCM_RW = 0;
	LCM_DB = cmd;
	LCM_E = 1;
	_nop_();
	_nop_();
	LCM_E = 0;
}
void LCMWriteData(unsigned char dc)	//write datas to LCD1206.
{
	while(LCMReadState() & BUSYFLAG);
	LCM_RS = 1;
	LCM_RW = 0;
	LCM_DB = dc;
	LCM_E = 1;
	_nop_();
	_nop_();
	LCM_E = 0;
}
void LCMInit(void)									//initialize LCD1206
{
	LCMDelay(60); 
	LCMWriteCmd(0x38); 								
	LCMDelay(5);
	LCMWriteCmd(0x38);
	LCMDelay(1); 
	LCMWriteCmd(0x38);
	while(LCMReadState() & BUSYFLAG);
	LCMWriteCmd(0x08); 
	while(LCMReadState() & BUSYFLAG);
	LCMWriteCmd(0x01); 
	while(LCMReadState() & BUSYFLAG);														 
	LCMWriteCmd(0x06); 
	while(LCMReadState() & BUSYFLAG);
	LCMWriteCmd(0x0c); 
	while(LCMReadState() & BUSYFLAG);
}
void LCMGotoXY(unsigned char x,unsigned char y) 													//set position of cursor.
{
	unsigned char cmd	;
	if(x==0)
	{
		cmd = 0x80 | y;
	}
	else
	{
		cmd = 0x80 | 0x40 | y;
	}
	LCMWriteCmd(cmd);
	while(LCMReadState() & BUSYFLAG);
}
void LCMDisplayChar(unsigned char x,unsigned char y,unsigned char ch)			//diaplay a char(ASCII or 'a char') on appointed position.
{
	LCMGotoXY(x,y);
	LCMWriteData(ch);
}
void LCMDisplayString(unsigned char x,unsigned char y,unsigned char* str)	//display a string on oppointed position.
{
	unsigned char ptr;
	ptr = 0;
	while(*(str+ptr) != 0)
		{
			LCMDisplayChar(x,(y+ptr),*(str+ptr));
			ptr++;
		}
}
void LCMBlink(unsigned char x,unsigned char y,unsigned char cmd)					//set cursor blink on oppointed position.cmd=BLINK,blinking.cmd=NOBLINK,cursor no blinking
{
	LCMGotoXY(x,y);
	if(cmd == BLINK)
	{
		LCMWriteCmd(0x0d);
	}
	else
	{
		LCMWriteCmd(0x0c);
	}
}