#include <REGX52.H>
#include "INTRINS.H"
#include "DelayXms.h"
#include "LCD1602.H"
#include "MatrixKey.h"
#include "Timer0.h"
#include "Key.h"
#include "UART.h"

unsigned char Sec; // ȫ�ֱ���Ĭ����0
void test1()
{
	UART_Init(); //���ڳ�ʼ��
	while (1)
	{
		UART_SendByte(Sec); //���ڷ���һ���ֽ�
		Sec++;				// Sec����
		Delay(1000);		//��ʱ1��
	}
}

// ������Ƭ���������ݣ���Ƭ�����պ󲢻ش�������
void test2()
{
	UART_Init(); //���ڳ�ʼ��

	while (1)
	{
	}
}

void UART_Routine() interrupt 4
{
	// ����Ƿ��ǽ����ж�
	if (RI == 1)
	{
		P2 = ~SBUF;
		UART_SendByte(SBUF); // ����Է��͵�Ƭ�����յ�������
		RI = 0;				 // �����������
	}
}

void test3()
{
	UART_Init(); //���ڳ�ʼ��
	UART_SendByte('B'); // �ı����� �ȼ��� UART_SendByte(��B��);
	while (1)
	{
	}
}
void main()
{
	test3();
}