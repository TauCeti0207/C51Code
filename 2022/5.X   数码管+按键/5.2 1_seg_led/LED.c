#include <reg52.h>   //包含特殊功能寄存器定义的头文件

sbit LEDA = P0^0;
sbit LEDD = P0^3;

sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;

void Delay500ms()		//@11.0592MHz
{
	unsigned char i, j, k;
	
	i = 4;
	j = 129;
	k = 119;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void main()
{
    ENLED = 0;
    ADDR3 = 1;
	
    ADDR2 = 0;
    ADDR1 = 0;
    ADDR0 = 0;
	
    while (1)
	{
		LEDA = 0;
		LEDD = 1;
		Delay500ms();
		
		LEDA = 1;
		LEDD = 0;
		Delay500ms();
	}
}
