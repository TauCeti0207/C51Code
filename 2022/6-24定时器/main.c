#include <REGX52.H>
#include "INTRINS.H"
#include "DelayXms.h"
#include "LCD1602.H"
#include "MatrixKey.h"
#include "Timer0.h"
#include "Key.h"

// void test1()
// {
// 	LCD_Init();
// 	LCD_ShowChar(1, 1, 'A');
// 	LCD_ShowChar(2, 1, 'b');
// 	LCD_ShowString(1, 3, "hello");
// 	LCD_ShowNum(2, 3, 556, 3);
// 	LCD_ShowSignedNum(2, 6, -66, 2);
// 	LCD_ShowHexNum(2, 10, 0xA8, 2);
// 	LCD_ShowBinNum(1, 8, 0xAA, 8);
// }

// unsigned char KeyNum;
// void test2()
// {
// 	LCD_Init();
// 	LCD_ShowString(1, 1, "MatrixKey");
// 	while (1)
// 	{
// 		KeyNum = MatrixKey();
// 		if (KeyNum != 0)
// 		{
// 			LCD_ShowNum(2, 1, KeyNum, 2);
// 		}
// 	}
// }

// // 定时器控制LED灯每一秒亮灭交替
// void Timer0_Routine() interrupt 1 // 定时1s
// {
// 	static unsigned int T0Count;
// 	TL0 = 0x66; //设置定时初值
// 	TH0 = 0xFC; //设置定时初值
// 	T0Count++;
// 	if (T0Count >= 1000)
// 	{
// 		T0Count = 0;
// 		P2_0 = ~P2_0;
// 	}
// }

// void test3()
// {
// 	Timer0Init();
// 	while (1)
// 	{
// 	}
// }

// unsigned char KeyNum, LEDMode;
// void test4()
// {
// 	P2 = 0xFE;
// 	Timer0Init();
// 	while (1)
// 	{
// 		KeyNum = Key(); //获取独立按键键码
// 		if (KeyNum)		//如果按键按下
// 		{
// 			if (KeyNum == 1) //如果K1按键按下
// 			{
// 				LEDMode++; //模式切换
// 				if (LEDMode >= 2)
// 					LEDMode = 0;
// 			}
// 		}
// 	}
// }

// // LED每500ms移动一次
// void Timer0_Routine() interrupt 1
// {
// 	static unsigned int T0Count;
// 	TL0 = 0x66; //设置定时初值
// 	TH0 = 0xFC; //设置定时初值
// 	T0Count++;
// 	if (T0Count >= 500)
// 	{
// 		T0Count = 0;
// 		if (LEDMode == 0)		//模式判断
// 			P2 = _crol_(P2, 1); // LED输出
// 		if (LEDMode == 1)
// 			P2 = _cror_(P2, 1);
// 	}
// }

unsigned char Sec = 55, Min = 59, Hour = 23;

void test5()
{
	LCD_Init();
	Timer0Init();

	LCD_ShowString(1, 1, "Clock:"); //上电显示静态字符串
	LCD_ShowString(2, 1, "  :  :");

	while (1)
	{
		LCD_ShowNum(2, 1, Hour, 2); //显示时分秒
		LCD_ShowNum(2, 4, Min, 2);
		LCD_ShowNum(2, 7, Sec, 2);
	}
}

// 时钟
void Timer0_Routine() interrupt 1
{
	static unsigned int T0Count;
	TL0 = 0x66; //设置定时初值
	TH0 = 0xFC; //设置定时初值
	T0Count++;
	if (T0Count >= 1000)
	{
		T0Count = 0;
		Sec++; // 1秒到，Sec自增
		if (Sec >= 60)
		{
			Sec = 0; // 60秒到，Sec清0，Min自增
			Min++;
			if (Min >= 60)
			{
				Min = 0; // 60分钟到，Min清0，Hour自增
				Hour++;
				if (Hour >= 24)
				{
					Hour = 0; // 24小时到，Hour清0
				}
			}
		}
	}
}

void main()
{
	test5();
}