#include <REGX52.H>
#include "INTRINS.H"
#include "DelayXms.h"
#include "LCD1602.H"
#include "Key.h"
#include "MatrixKey.h"

void test1()
{
	LCD_Init();
	LCD_ShowChar(1, 1, 'A');
	LCD_ShowChar(2, 1, 'b');
	LCD_ShowString(1, 3, "hello");
	LCD_ShowNum(2, 3, 556, 3);
	LCD_ShowSignedNum(2, 6, -66, 2);
	LCD_ShowHexNum(2, 10, 0xA8, 2);
	LCD_ShowBinNum(1, 8, 0xAA, 8);
}

unsigned char KeyNum;

void test2()
{
	LCD_Init();
	LCD_ShowString(1, 1, "MatrixKey");
	while (1)
	{
		KeyNum = MatrixKey();
		if (KeyNum != 0)
		{
			LCD_ShowNum(2, 1, KeyNum, 2);
		}
	}
}

void test3()
{
	LCD_Init();
	// LCD_ShowString(1, 1, "MatrixKey");
	LCD_ShowChar(1, 1, 'A');
	while (1)
	{
		KeyNum = MatrixKey();
		// if (KeyNum != 0)
		// {
			LCD_ShowNum(2, 1, KeyNum, 2);
		// }
	}
}
void main()
{
	test3();
}
