#ifndef __HX711_H__
#define __HX711_H__



#include <REGX52.H>
#include <INTRINS.H>
//IO设置
sbit HX711_DOUT=P2^0; 
sbit HX711_SCK=P2^1; 

//函数或者变量声明
extern void Delay__hx711_us(void);
extern unsigned long HX711_Read(void);

#endif