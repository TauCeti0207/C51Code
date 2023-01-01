#ifndef __LCD1602_H__
#define __LCD1602_H__
#include <reg52.h>
#include <intrins.h>

#define LCM_DB P0
#define BUSYFLAG 0x80
#define BLINK 0x01
#define NOBLINK 0x00

sbit LCM_RS = P2^0;/*I have #include <REGX51.H>,open REGX51.H i found its use Px_x.however i use Px_x errors happend!!*/
sbit LCM_RW = P2^1;
sbit LCM_E = P2^2;
//sbit LCM_BLC = P2^3;
//sbit key = P1^0;


void LCMDelay(int ms);
unsigned char LCMReadState(void);
void LCMClear(void);
void LCMWriteCmd(unsigned char cmd);
void LCMWriteData(unsigned char dc);
void LCMInit(void);
void LCMGotoXY(unsigned char x,unsigned char y); // x:ÐÐ(0~1) y:ÁÐ(0~F)
void LCMDisplayChar(unsigned char x,unsigned char y,unsigned char ch);
void LCMDisplayString(unsigned char x,unsigned char y,unsigned char* str);
void LCMBlink(unsigned char x,unsigned char y,unsigned char cmd);



#endif