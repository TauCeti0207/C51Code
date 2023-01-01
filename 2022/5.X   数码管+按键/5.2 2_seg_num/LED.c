//////////////////////////////////////////////////////////////////////////////////
//请保留此信息以支持作者 谢谢													//
//作者:一只程序缘																//
//哔哩哔哩账号:237304109														//
//系列视频链接:https://space.bilibili.com/237304109/channel/detail?cid=96895 	//
//本视频链接:https://www.bilibili.com/video/BV1e54y1v7Ty						//
//学习交流QQ群:650846694														//
//最后优化时间:2020/2/10														//
//////////////////////////////////////////////////////////////////////////////////

/*
字符	dp	g	f	e	d	c	b	a
2		1	0	1	0	0	1	0	0		0XA4

*/

#include <reg52.h>   //包含特殊功能寄存器定义的头文件

unsigned char code LedChar[16]={
	0XC0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,
	0X80,0X90,0X88,0X83,0XC6,0XA1,0X86,0X8E
};

sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;

void Delay500ms();

void main()
{
	unsigned char i=0;
	
    ENLED = 0;
    ADDR3 = 1;
	
    ADDR2 = 0;
    ADDR1 = 0;
    ADDR0 = 0;
	
    while (1)
	{
		Delay500ms();
		i++;
		P0=LedChar[i & 0x0f];
	}
}

/////////////////////////////////////////////////////
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