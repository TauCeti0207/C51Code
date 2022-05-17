#include <reg51.h>
#define uchar unsigned char
#define uint unsigned int

void main()
{
	// 内部RAM 初始化
	uint data y = 0;
	uchar data *p = 0x20;
	uchar data byteLength; // 转换字节数
	uchar data i;
	uchar data tmp;
	byteLength = *p;
	for(i = 0; i < byteLength; ++i)
	{
		++p;
		// BCD码219H --> 001000011001(2) 也就是219(D) -->11011011(2) -->DB(H)
		// 转换对应十进制高四位*10+低四位，存到y里面，隔着2位10进制，*100
		// 02 19
		// 1.y=02
		// 2.y=2*100 + 1*10+9 = 219
		tmp = (*p >> 4)*10 + (*p & 0x0f);
		y = y*100 + tmp;
	}
	while(1); // 动态停机
}