#ifndef __LCD1602_H__
#define __LCD1602_H__


#include <REGX52.h>

//LCD1602 IO设置
#define LCD1602_PORT P0
sbit LCD1602_RS = P2^6;
sbit LCD1602_RW = P2^5;
sbit LCD1602_EN = P2^7;

//函数或者变量声明
extern void LCD1602_delay_ms(unsigned int n);
extern void LCD1602_write_com(unsigned char com);
extern void LCD1602_write_data(unsigned char dat);
extern void LCD1602_write_word(unsigned char *s);
extern void Init_LCD1602();
extern void LCD_Init();
extern void LCD_ShowChar(unsigned char Line, unsigned char Column, char Char);
extern void LCD_ShowString(unsigned char Line, unsigned char Column, char *String);
extern void LCD_ShowNum(unsigned char Line, unsigned char Column, unsigned int Number, unsigned char Length);
extern void LCD_ShowSignedNum(unsigned char Line, unsigned char Column, int Number, unsigned char Length);
extern void LCD_ShowHexNum(unsigned char Line, unsigned char Column, unsigned int Number, unsigned char Length);
extern void LCD_ShowBinNum(unsigned char Line, unsigned char Column, unsigned int Number, unsigned char Length);
#endif