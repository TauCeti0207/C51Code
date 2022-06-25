#include <REGX52.H>
#include "INTRINS.H"
#include "DelayXms.h"
#include "LCD1602.H"
#include "MatrixKey.h"
#include "Timer0.h"
#include "Key.h"
#include "UART.h"

unsigned char Sec; // 全局变量默认是0
void test1()
{
	UART_Init(); //串口初始化
	while (1)
	{
		UART_SendByte(Sec); //串口发送一个字节
		Sec++;				// Sec自增
		Delay(1000);		//延时1秒
	}
}

// 电脑向单片机发送数据，单片机接收后并回传给电脑
void test2()
{
	UART_Init(); //串口初始化

	while (1)
	{
	}
}

void UART_Routine() interrupt 4
{
	// 检测是否是接收中断
	if (RI == 1)
	{
		P2 = ~SBUF;
		UART_SendByte(SBUF); // 向电脑发送单片机接收到的数据
		RI = 0;				 // 必须软件清零
	}
}

void test3()
{
	UART_Init(); //串口初始化
	UART_SendByte('B'); // 文本发送 等价于 UART_SendByte(‘B’);
	while (1)
	{
	}
}
void main()
{
	test3();
}