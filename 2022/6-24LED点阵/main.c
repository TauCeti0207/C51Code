#include <REGX52.H>
#include "INTRINS.H"
#include "DelayXms.h"
#include "LCD1602.H"
#include "MatrixKey.h"
#include "Timer0.h"
#include "Key.h"
#include "UART.h"
#include "MatrixLED.h"

void test1()
{
	SCK = 0;
	RCK = 0;
	MatrixLED_ShowColumn(7, 0xAA);
	while (1)
	{
		// ��ʾЦ��
		MatrixLED_ShowColumn(0, 0x3C);
		MatrixLED_ShowColumn(1, 0x42);
		MatrixLED_ShowColumn(2, 0xA9);
		MatrixLED_ShowColumn(3, 0x85);
		MatrixLED_ShowColumn(4, 0x85);
		MatrixLED_ShowColumn(5, 0xA9);
		MatrixLED_ShowColumn(6, 0x42);
		MatrixLED_ShowColumn(7, 0x3C);
	}
}


//��������
unsigned char code Animation[]={
	0x3C,0x42,0xA9,0x85,0x85,0xA9,0x42,0x3C,
	0x3C,0x42,0xA1,0x85,0x85,0xA1,0x42,0x3C,
	0x3C,0x42,0xA5,0x89,0x89,0xA5,0x42,0x3C,
};

void test2()
{
	unsigned char i, Offset = 0, Count = 0;
	MatrixLED_Init();
	while (1)
	{
		for (i = 0; i < 8; i++) //ѭ��8�Σ���ʾ8������
		{
			MatrixLED_ShowColumn(i, Animation[i + Offset]);
		}
		Count++; //�ƴ���ʱ
		if (Count > 15)
		{
			Count = 0;
			Offset += 8; //ƫ��+8���л���һ֡����
			if (Offset > 16)
			{
				Offset = 0;
			}
		}
	}
}

void main()
{
	test2();
}